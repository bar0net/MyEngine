#include "ModuleEditor.h"

#include <assert.h>
#include "GL/glew.h"

#include "_Vendor/imgui-docking/imgui.h"
#include "_Vendor/imgui-docking/imgui_impl_sdl.h"
#include "_Vendor/imgui-docking/imgui_impl_opengl3.h"
#include "_Vendor/MathGeoLib/Math/float4x4.h"
#include "_Vendor/MathGeoLib/Math/float3.h"

#include "Application.h"
#include "Utils/Window_Utils.h"
#include "Utils/Render_Utils.h"
#include "Utils/MovingArray.h"

#include "GL_Buffers/VertexBuffer.h"
#include "GL_Buffers/VertexArray.h"
#include "GL_Buffers/Shader.h"
#include "GL_Buffers/IndexBuffer.h"
#include "GL_Buffers/VertexBufferLayout.h"
#include "GL_Buffers/RenderBuffer.h"
#include "GL_Buffers/FrameBuffer.h"
#include "GL_Buffers/Texture2D.h"

#include "ModuleRenderer.h"
#include "ModuleScene.h"
#include "ModuleInput.h"

#include "GameObject/GameObject.h"
#include "GameObject/Components/ComponentCamera.h"
#include "GameObject/Components/ComponentMeshRenderer.h"

#include "Editor/PanelTexture.h"

#define DELETE(x) if(x != nullptr) { delete x; } x = nullptr

#define GLSL_VERSION "#version 130"
#define GRID_LENGTH 100
#define GIZMO_LENGTH 0.5F

bool ModuleEditor::Init()
{
	LOGINFO("Initializing Editor.");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	
	ImGui_ImplSDL2_InitForOpenGL(App->renderer->data->window, App->renderer->data->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();
	
	CreateGrid();
	CreateGizmo();

	// Set Up Render to Texture
	frameBuffer = new MyEngine::FrameBuffer();
	renderTexture = new MyEngine::Texture2D(App->renderer->width, App->renderer->height);
	renderBuffer = new MyEngine::RenderBuffer();
	
	renderBuffer->SetStorage(App->renderer->width, App->renderer->height, GL_DEPTH_COMPONENT);
	frameBuffer->SetRenderBuffer(renderBuffer->ID(), GL_DEPTH_ATTACHMENT);
	frameBuffer->SetTexture(renderTexture->ID(), 0, GL_COLOR_ATTACHMENT0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLCall(glDrawBuffers(1, DrawBuffers));

	// Create Panels
	panel_performance = new PanelPerfomance();
	panel_console = new PanelConsole();
	panel_editor = new PanelEditor();
	panel_scene = new PanelScene();
	panel_texture = new PanelTexture();

	return true;
}

bool ModuleEditor::Start()
{
	math::float4x4 I = math::float4x4::identity;
	shader_grid->SetUniform4x4("model", I);

	LOG("___ EDITOR: TESTING NONE LOG LEVELS ____");
	LOGINFO("___ EDITOR: TESTING INFO LOG LEVELS ____");
	LOGDEBUG("___ EDITOR: TESTING DEBUG LOG LEVEL ____");
	LOGWARNING("___ EDITOR: TESTING WARNING LOG LEVEL ____");
	LOGERROR("___ EDITOR: TESTING ERROR LOG LEVEL ____");

	assert(editor_camera = App->scene->Find("Camera"));
	return true;
}

bool ModuleEditor::CleanUp()
{
	LOGINFO("Destroying Editor.");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	DELETE(panel_performance);
	DELETE(panel_console);
	DELETE(panel_editor);
	DELETE(panel_scene);
	DELETE(panel_texture);

	DELETE(renderTexture);
	DELETE(frameBuffer);
	DELETE(renderBuffer);
	
	DELETE(vbo_grid);		DELETE(vbo_gizmo);
	DELETE(ibo_grid);		DELETE(ibo_gizmo);
	DELETE(shader_grid);	DELETE(shader_gizmo);
	DELETE(vao_grid);		DELETE(vao_gizmo);

	return true;
}

UpdateState ModuleEditor::PreUpdate()
{
	if (panel_editor->show_grid)
	{
		App->renderer->DrawLines(vao_gizmo, ibo_gizmo, shader_gizmo, 2.5F);
		App->renderer->DrawLines(vao_grid, ibo_grid, shader_grid,2);
	}

	return UpdateState::Update_Continue;
}


void ModuleEditor::FrameStart()
{
	frameBuffer->UnBind();
	MyEngine::RenderUtils::ClearViewport();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->renderer->data->window);
	ImGui::NewFrame();
}

UpdateState ModuleEditor::Update()
{
	FrameStart();

	if (App->input->GetKeyDown(KeyCode::F))
	{
		if (inspect_object != nullptr && inspect_object->GetName() != "Camera")
		{
			MeshRenderer* mr = (MeshRenderer*)inspect_object->components["MeshRenderer"];
			if (mr != nullptr)
			{
				//TODO: Improve this.
				// Look for a better solution to position the camera!
				float ratio = mr->dimensions[0] * inspect_object->scale[0];
				if (ratio < mr->dimensions[1]) ratio = mr->dimensions[1] * inspect_object->scale[1];
				if (ratio < mr->dimensions[2]) ratio = mr->dimensions[2] * inspect_object->scale[2];
				
				float3 p =
				{
					mr->center[0] * inspect_object->scale[0],
					(mr->center[1]* inspect_object->scale[1] + 0.5F*ratio) ,
					(mr->center[2]* inspect_object->scale[2] + 0.5F*ratio) 
				};

				editor_camera->SetPosition(p.x, p.y + 2.0F, p.z + 10.0F);
				editor_camera->SetRotation(0, 0, 0);
			}
		}
		else
		{
			LOGDEBUG("No object selected to focus view. Select an object on the inspector.");
		}
	}

	//  Panels
	if (!MainMenuBar()) return UpdateState::Update_End;
	CreateDockSpace();

	if (scene_window)	panel_scene->Draw(scene_window, renderTexture->ID(), (CameraControl*)editor_camera->components["CameraControl"], scene_width, scene_height);
	if (config_window)	panel_editor->Draw(config_window, shader_grid, grid_color);
	if (debug_window)	panel_performance->Draw(debug_window, scene_width, scene_height);
	if (console_window) panel_console->Draw(console_window);
	bool enabled = true;
	panel_texture->Draw(enabled);

	if (inspect_window)
	{
		ImGui::Begin("Inspector", &inspect_window);
		PanelObjects();
		ImGui::End();
	}
	
	FrameEnd();
	return UpdateState::Update_Continue;
}

void ModuleEditor::FrameEnd() const
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	frameBuffer->Bind();
}

void ModuleEditor::ProcessEvent(void* event) const
{
	assert(event);
	ImGui_ImplSDL2_ProcessEvent((SDL_Event*)event);
}

void ModuleEditor::CreateGrid()
{
	math::float4x4 identity = math::float4x4::identity;

	std::vector<float> grid;
	std::vector<unsigned int> grid_index;
	unsigned int id = 0;

	for (int i = -GRID_LENGTH; i <= GRID_LENGTH; ++i)
	{
		if (i == 0)
		{
			grid.push_back((float)-GRID_LENGTH); grid.push_back(0); grid.push_back((float)i);
			grid.push_back(0); grid.push_back(0); grid.push_back((float)i);
			grid_index.push_back(id++); grid_index.push_back(id++);

			grid.push_back((float)i); grid.push_back(0);  grid.push_back(-(float)GRID_LENGTH);
			grid.push_back((float)i); grid.push_back(0);  grid.push_back(0);
			grid_index.push_back(id++); grid_index.push_back(id++);

			grid.push_back(GIZMO_LENGTH); grid.push_back(0); grid.push_back((float)i);
			grid.push_back((float)GRID_LENGTH); grid.push_back(0); grid.push_back((float)i);
			grid_index.push_back(id++); grid_index.push_back(id++);

			grid.push_back((float)i); grid.push_back(0);  grid.push_back(GIZMO_LENGTH);
			grid.push_back((float)i); grid.push_back(0);  grid.push_back((float)GRID_LENGTH);
			grid_index.push_back(id++); grid_index.push_back(id++);

		}
		else
		{
			grid.push_back((float)-GRID_LENGTH); grid.push_back(0); grid.push_back((float)i);
			grid.push_back((float)GRID_LENGTH); grid.push_back(0); grid.push_back((float)i);
			grid_index.push_back(id++); grid_index.push_back(id++);

			grid.push_back((float)i); grid.push_back(0);  grid.push_back(-(float)GRID_LENGTH);
			grid.push_back((float)i); grid.push_back(0);  grid.push_back((float)GRID_LENGTH);
			grid_index.push_back(id++); grid_index.push_back(id++);
		}
	}
	MyEngine::VertexBufferLayout grid_layout;
	grid_layout.Push<float>(3);

	shader_grid = App->renderer->CreateShader("grid", "default.vs", "default.fs");
	shader_grid->SetUniform4("albedo", grid_color[0], grid_color[1], grid_color[2], grid_color[3]);
	shader_grid->SetUniform4x4("model", identity);
	vbo_grid = new MyEngine::VertexBuffer(&grid);
	vao_grid = new MyEngine::VertexArray();
	vao_grid->AddBuffer(*vbo_grid, grid_layout);
	ibo_grid = new MyEngine::IndexBuffer(&grid_index);
}

void ModuleEditor::CreateGizmo()
{
	math::float4x4 identity = math::float4x4::identity;

	std::vector<float> gizmo =
	{
		0.0F,	0.0F,		0.0F,	0.0F, 0.0F, 0.0F,
		GIZMO_LENGTH, 0.0F, 0.0F,	1.0F, 0.0F, 0.0F,
		0.0F, GIZMO_LENGTH, 0.0F,	0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, GIZMO_LENGTH,	0.0F, 0.0F, 1.0F
	};

	std::vector<unsigned int> gizmo_index = { 0, 1, 0, 2, 0, 3 };
	MyEngine::VertexBufferLayout gizmo_layout;
	gizmo_layout.Push<float>(3);
	gizmo_layout.Push<float>(3);

	shader_gizmo = App->renderer->CreateShader("gizmo", "gizmo.vs", "gizmo.fs");
	shader_gizmo->SetUniform4x4("model", identity);
	vbo_gizmo = new MyEngine::VertexBuffer(&gizmo);
	vao_gizmo = new MyEngine::VertexArray();
	vao_gizmo->AddBuffer(*vbo_gizmo, gizmo_layout);
	ibo_gizmo = new MyEngine::IndexBuffer(&gizmo_index);
}

// ====================================
//				PANELS
// ====================================

void ModuleEditor::CreateDockSpace()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0F);
	bool p_open = true;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruDockspace;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0F, 0.0F), dockspace_flags);
	ImGui::End();
}

bool ModuleEditor::MainMenuBar()
{

	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Quit")) return false;
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Windows"))
	{
		ImGui::MenuItem("Configuration", NULL, &config_window);
		ImGui::MenuItem("Console", NULL, &console_window);
		ImGui::MenuItem("Debug Tools", NULL, &debug_window);
		ImGui::MenuItem("Inspector", NULL, &inspect_window);
		ImGui::MenuItem("Scene", NULL, &scene_window);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"))
	{
		ImGui::MenuItem("Github");
		ImGui::MenuItem("About");
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();


	return true;
}

void ModuleEditor::PanelObjects() 
{
	if (App->scene->gameObjects.size() == 0) return;

	ImVec2 panel_size = ImGui::GetWindowSize();

	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
	for (std::unordered_map<std::string, GameObject*>::iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		if (inspect_object == it->second) 
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2F, 0.3F, 0.7F, 1.0F));
		else 
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0F, 0.0F, 0.0F, 0.0F));

		if (ImGui::Button(it->first.c_str(), ImVec2(panel_size.x - 17, 0))) inspect_object = it->second; 

		ImGui::PopStyleColor();
	}
	ImGui::PopStyleVar();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	if (this->inspect_object != nullptr)
	{
		ImGui::TextColored(ImVec4(0.3F, 0.5F, 0.8F, 1.0F), inspect_object->GetName());
		float pos[3] = { this->inspect_object->position.x, this->inspect_object->position.y, this->inspect_object->position.z };
		if (ImGui::DragFloat3("Position", pos, 2)) this->inspect_object->SetPosition(pos[0], pos[1], pos[2]);			

		float rot[3] = { this->inspect_object->rotation.x, this->inspect_object->rotation.y, this->inspect_object->rotation.z };
		if (ImGui::DragFloat3("Rotation", rot, 2)) this->inspect_object->SetRotation(rot[0], rot[1], rot[2]);
			
		float scale[3] = { this->inspect_object->scale.x, this->inspect_object->scale.y, this->inspect_object->scale.z };
		if (ImGui::DragFloat3("Scale", scale, 2)) this->inspect_object->SetScale(scale[0], scale[1], scale[2]);
			

		ImGui::Separator();
		for (std::unordered_map<const char*, Component*>::iterator it = inspect_object->components.begin(); it != inspect_object->components.end(); ++it)
		{
			if (it->first == "Camera") PanelCamera((Camera*)it->second);
			else if (it->first == "CameraControl") PanelCameraControl((CameraControl*)it->second);
			else if (it->first == "MeshRenderer") PanelMeshRenderer((MeshRenderer*)it->second);
			else ImGui::Text(it->first);
		}
	}

}

void ModuleEditor::PanelCamera(Camera* component) const
{
	assert(component);
	if (ImGui::CollapsingHeader("Camera Properties"))
	{
		if (ImGui::InputFloat("Near Plane", &component->nearPlane)) component->UpdateFrustum();
		if (ImGui::InputFloat("Far Plane", &component->farPlane)) component->UpdateFrustum();
		if (ImGui::SliderFloat("FOV", &component->fov, 60, 120)) component->UpdateFrustum();
	}
}

void ModuleEditor::PanelCameraControl(CameraControl* component) const
{
	assert(component);
	if (ImGui::CollapsingHeader(component->GetName()))
	{
		ImGui::InputFloat("Velocity", &component->velocity);
		ImGui::InputFloat("Angular Velocity", &component->angularVelocity);
	}
}

void ModuleEditor::PanelMeshRenderer(MeshRenderer * component) const
{
	if (ImGui::CollapsingHeader(component->GetName()))
	{
		ImGui::Text("Bounding Box");
		ImGui::Text("Local Center: (%f, %f, %f)", 
			component->center[0] * component->GetGameObject()->scale[0],
			component->center[1] * component->GetGameObject()->scale[1],
			component->center[2] * component->GetGameObject()->scale[2]);
		ImGui::Text("Width: %f, Height: %f, Depth: %f", 
			component->dimensions[0] * component->GetGameObject()->scale[0],
			component->dimensions[1] * component->GetGameObject()->scale[1],
			component->dimensions[2] * component->GetGameObject()->scale[2]);
		ImGui::Separator();

		for (unsigned int i = 0; i < component->meshes.size(); ++i)
		{
			ImGui::Text("SubMesh (%i)", i);
			ImGui::Text("Numer of Triangles: %d", component->meshes[i]->num_triangles);
			std::string s = "Display Texture " + std::to_string(i);
			ImGui::Checkbox(s.c_str(), &component->meshes[i]->display_texture);
			if (component->meshes[i]->display_texture)
				ImGui::Image((ImTextureID)component->meshes[i]->textureID, ImVec2(75, 75),ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1));
			else
				ImGui::Image((ImTextureID)component->meshes[i]->textureID, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.6F, 0.6F, 0.6F, 1));
			
			ImGui::SameLine();

			std::string popupID = std::string("MeshAlbedo") + std::to_string(i);
			std::string albedoID = std::string("Albedo (") + std::to_string(i) + std::string(")");

			bool open_albedo = ImGui::ColorButton(albedoID.c_str(), *(ImVec4*)&component->meshes[i]->albedo, 0, ImVec2(10,75));
			ImGui::SameLine(); ImGui::Text("Albedo");

			if (open_albedo) ImGui::OpenPopup(popupID.c_str());
			if (ImGui::BeginPopup(popupID.c_str()))
			{
				ImGui::ColorPicker4(albedoID.c_str(), component->meshes[i]->albedo);
				ImGui::EndPopup();
			}

			ImGui::Separator();
		}
	}
}
