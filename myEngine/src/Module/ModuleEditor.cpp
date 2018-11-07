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
#include "ModuleTime.h"
#include "ModuleScene.h"
#include "ModuleTexture.h"
#include "ModuleInput.h"

#include "GameObject/GameObject.h"
#include "GameObject/Components/Camera.h"
#include "GameObject/Components/CameraControl.h"
#include "GameObject/Components/MeshRenderer.h"

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
	/*frameBuffer = new MyEngine::FrameBuffer();

	renderTexture = new MyEngine::Texture2D(App->renderer->width, App->renderer->height);
	renderTexture->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	renderTexture->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	renderBuffer = new MyEngine::RenderBuffer();
	renderBuffer->SetStorage(App->renderer->width, App->renderer->height, GL_DEPTH_COMPONENT);

	frameBuffer->SetRenderBuffer(renderBuffer->ID(), GL_DEPTH_ATTACHMENT);
	frameBuffer->SetTexture(renderTexture->ID(), 0, GL_COLOR_ATTACHMENT0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLCall(glDrawBuffers(1, DrawBuffers));*/

	panel_performance = new PanelPerfomance();
	panel_console = new PanelConsole();
	panel_editor = new PanelEditor();

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

	delete panel_performance;
	delete panel_console;
	delete panel_editor;

	delete renderTexture;
	delete frameBuffer;
	delete renderBuffer;
	
	delete(vbo_grid);	delete(vbo_gizmo);
	delete(ibo_grid);	delete(ibo_gizmo);
	delete(shader_grid);delete(shader_gizmo);
	delete(vao_grid);	delete(vao_gizmo);

	return true;
}

UpdateState ModuleEditor::PreUpdate()
{
	if (panel_editor->show_grid)
	{
		App->renderer->DrawLines(vao_gizmo, ibo_gizmo, shader_gizmo, 2.5F);
		App->renderer->DrawLines(vao_grid, ibo_grid, shader_grid);
	}

	return UpdateState::Update_Continue;
}

UpdateState ModuleEditor::Update()
{
	FrameStart();
	if (App->input->GetKeyDown(KeyCode::F))
	{
		if (inspect_object != nullptr && inspect_object->GetName() != "Camera")
		{
			float3 p = inspect_object->position - 10 * editor_camera->Front() + 2 * editor_camera->Up();
			editor_camera->SetPosition(p.x, p.y, p.z);
		}
		else
		{
			LOGDEBUG("No object selected to focus view. Select an object on the inspector.");
		}
	}

	// Drawing
	if (!MainMenuBar()) return UpdateState::Update_End;

	if (scene_window)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Begin("Scene", &scene_window, ImGuiWindowFlags_NoScrollbar);
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImVec2 imgSize = size; 
		float space = 0;

		if (size.x * App->renderer->height < size.y * App->renderer->width)
		{
			imgSize.y = (size.x * App->renderer->height / App->renderer->width);
			space = (size.y - imgSize.y) / 2.0F;
			ImGui::Dummy(ImVec2(0, space));
		}
		else
		{
			imgSize.x = (size.y * App->renderer->width / App->renderer->height);
			space = (size.x - imgSize.x) / 2.0F;
			ImGui::Dummy(ImVec2(space, 0)); ImGui::SameLine();
		}

		ImGui::Image((ImTextureID)renderTexture, imgSize, ImVec2(0,1), ImVec2(1,0));

		scene_width = imgSize.x;
		scene_height = imgSize.y;

		ImGui::End();
		ImGui::PopStyleVar();
	}

	if(inspect_window)
	{
		ImGui::Begin("Inspector", &inspect_window);
		PanelObjects();
		ImGui::End();
	}

	if (config_window)	panel_editor->Draw(config_window, shader_grid, grid_color);
	if (debug_window)	panel_performance->Draw(debug_window, scene_width, scene_height);
	if (console_window) panel_console->Draw(console_window);

	FrameEnd();
	return UpdateState::Update_Continue;
}

void ModuleEditor::FrameStart()
{
	//MyEngine::RenderUtils::UnBindRenderBuffer(App->renderer->width, App->renderer->height);
	//MyEngine::RenderUtils::ClearViewport();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->renderer->data->window);
	ImGui::NewFrame();
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

	//MyEngine::RenderUtils::BindRenderBuffer(frameBuffer->ID(), App->renderer->width, App->renderer->height);
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

bool ModuleEditor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
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
	}

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
		if (ImGui::InputFloat3("Position", pos, 2)) this->inspect_object->SetPosition(pos[0], pos[1], pos[2]);			

		float rot[3] = { this->inspect_object->rotation.x, this->inspect_object->rotation.y, this->inspect_object->rotation.z };
		if (ImGui::InputFloat3("Rotation", rot, 2)) this->inspect_object->SetRotation(rot[0], rot[1], rot[2]);
			
		float scale[3] = { this->inspect_object->scale.x, this->inspect_object->scale.y, this->inspect_object->scale.z };
		if (ImGui::InputFloat3("Scale", scale, 2)) this->inspect_object->SetScale(scale[0], scale[1], scale[2]);
			

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
		for (unsigned int i = 0; i < component->meshes.size(); ++i)
		{
			ImGui::Text("Mesh (%i)", i);
			ImGui::Text("Numer of Triangles: %d", component->meshes[i].num_triangles);
			std::string s = "Display Texture " + std::to_string(i);
			ImGui::Checkbox(s.c_str(), &component->meshes[i].display_texture);
			if (component->meshes[i].display_texture)
				ImGui::Image((ImTextureID)component->meshes[i].textureID, ImVec2(75, 75),ImVec2(0,0), ImVec2(1,1), ImVec4(1,1,1,1));
			else
				ImGui::Image((ImTextureID)component->meshes[i].textureID, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.6F, 0.6F, 0.6F, 1));
			
			ImGui::SameLine();

			std::string popupID = std::string("MeshAlbedo") + std::to_string(i);
			std::string albedoID = std::string("Albedo (") + std::to_string(i) + std::string(")");

			bool open_albedo = ImGui::ColorButton(albedoID.c_str(), *(ImVec4*)&component->meshes[i].albedo, 0, ImVec2(10,75));
			ImGui::SameLine(); ImGui::Text("Albedo");

			if (open_albedo) ImGui::OpenPopup(popupID.c_str());
			if (ImGui::BeginPopup(popupID.c_str()))
			{
				ImGui::ColorPicker4(albedoID.c_str(), component->meshes[i].albedo);
				ImGui::EndPopup();
			}

			ImGui::Separator();
		}
	}
}
