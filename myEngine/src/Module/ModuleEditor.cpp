#include "ModuleEditor.h"

#include <assert.h>
#include <windows.web.h>
#include <shellapi.h>
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

#include "Editor/PanelTexture.h"
#include "Editor/PanelInspector.h"
#include "Editor/PanelHierarchy.h"

#define OpenURL(x) ShellExecute(NULL, "open", x, NULL, NULL, SW_SHOWNORMAL)

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
	renderBuffer = new MyEngine::RenderBuffer();
	SetRenderTexture();

	// Create Panels
	panel_performance = new PanelPerfomance();
	panel_console = new PanelConsole();
	panel_editor = new PanelEditor();
	panel_scene = new PanelScene();

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

	RELEASE(panel_performance);
	RELEASE(panel_console);
	RELEASE(panel_editor);
	RELEASE(panel_scene);

	RELEASE(renderTexture);
	RELEASE(frameBuffer);
	RELEASE(renderBuffer);
	
	RELEASE(shader_grid);	RELEASE(shader_gizmo);
	RELEASE(grid_mesh);		RELEASE(gizmo_mesh);

	return true;
}

UpdateState ModuleEditor::PreUpdate()
{
	if (panel_editor->show_grid)
	{
		App->renderer->Draw(grid_mesh, nullptr, shader_grid, 2.0F);
		App->renderer->Draw(gizmo_mesh, nullptr, shader_gizmo, 4.0F);
	}

	return UpdateState::Update_Continue;
}


void ModuleEditor::FrameStart()
{
	App->renderer->Render();
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
			if (inspect_object->components.find(ComponentType::MESH_RENDERER) != inspect_object->components.end())
			{
				MeshRenderer* mr = (MeshRenderer*)inspect_object->components[ComponentType::MESH_RENDERER][0];
				if (mr != nullptr)
				{
					//TODO: Improve this.
					// Look for a better solution to position the camera!
					float3 scale = inspect_object->Scale();
					float3 position = inspect_object->Position();

					float ratio = mr->dimensions[0] * scale[0];
					if (ratio < mr->dimensions[1]) ratio = mr->dimensions[1] * scale[1];
					if (ratio < mr->dimensions[2]) ratio = mr->dimensions[2] * scale[2];

					float3 p =
					{
						 mr->center[0] * scale[0] + position[0],
						(mr->center[1]  + 0.5F * mr->dimensions[1]) * scale[1] + position[1],
						(mr->center[2]  + 0.5F * mr->dimensions[2]) * scale[2] + position[2]
					};

					editor_camera->SetPosition(p.x, p.y + 2.0F, p.z + 10.0F);
					editor_camera->SetRotation(0, 0, 0);
				}
			}
			else
			{
				editor_camera->SetPosition(inspect_object->Position());
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

	if (scene_window)	panel_scene->Draw(scene_window, renderTexture->ID(), (CameraControl*)editor_camera->components[ComponentType::CAMERA_CONTROL][0], scene_width, scene_height);
	if (config_window)	panel_editor->Draw(config_window, grid_mesh, grid_color);
	if (debug_window)	panel_performance->Draw(debug_window, scene_width, scene_height);
	if (console_window) panel_console->Draw(console_window);
	if (hierarchy_window) PanelHierarchy::Draw(&hierarchy_window, inspect_object);
	if (inspect_window) PanelInspector::Draw(&inspect_window, inspect_object, editor_camera);
	if (texture_window) PanelTexture::Draw(texture_window);
	
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

GameObject* ModuleEditor::GetInspectedObject()
{
	return inspect_object;
}

void ModuleEditor::SetRenderTexture()
{
	RELEASE(renderTexture);
	//RELEASE(renderBuffer);

	renderTexture = new MyEngine::Texture2D(App->renderer->width, App->renderer->height);

	renderBuffer->SetStorage(App->renderer->width, App->renderer->height, GL_DEPTH_COMPONENT);
	frameBuffer->SetRenderBuffer(renderBuffer->ID(), GL_DEPTH_ATTACHMENT);
	frameBuffer->SetTexture(renderTexture->ID(), 0, GL_COLOR_ATTACHMENT0);

	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	GLCall(glDrawBuffers(1, DrawBuffers));
}

void ModuleEditor::CreateGrid()
{
	math::float4x4 identity = math::float4x4::identity;

	std::vector<float> grid;
	std::vector<unsigned int> grid_index;
	unsigned int id = 0U;

	grid.reserve(2 * (GRID_LENGTH + 1) * 3 * 4);
	grid_index.reserve(2 * (GRID_LENGTH + 1) * 4);
	for (int i = -GRID_LENGTH; i <= GRID_LENGTH; ++i)
	{
		if (i == 0)
		{
			grid.emplace_back((float)-GRID_LENGTH); grid.emplace_back(0); grid.emplace_back((float)i);
			grid.emplace_back(0);					grid.emplace_back(0); grid.emplace_back((float)i);
			grid_index.emplace_back(id++); grid_index.emplace_back(id++);

			grid.emplace_back((float)i); grid.emplace_back(0);  grid.emplace_back(-(float)GRID_LENGTH);
			grid.emplace_back((float)i); grid.emplace_back(0);  grid.emplace_back(0);
			grid_index.emplace_back(id++); grid_index.emplace_back(id++);

			grid.emplace_back(GIZMO_LENGTH);		grid.emplace_back(0); grid.emplace_back((float)i);
			grid.emplace_back((float)GRID_LENGTH);	grid.emplace_back(0); grid.emplace_back((float)i);
			grid_index.emplace_back(id++); grid_index.emplace_back(id++);

			grid.emplace_back((float)i); grid.emplace_back(0);  grid.emplace_back(GIZMO_LENGTH);
			grid.emplace_back((float)i); grid.emplace_back(0);  grid.emplace_back((float)GRID_LENGTH);
			grid_index.emplace_back(id++); grid_index.emplace_back(id++);

		}
		else
		{
			grid.emplace_back((float)-GRID_LENGTH);		grid.emplace_back(0); grid.emplace_back((float)i);
			grid.emplace_back((float)GRID_LENGTH);		grid.emplace_back(0); grid.emplace_back((float)i);
			grid_index.emplace_back(id++); grid_index.emplace_back(id++);

			grid.emplace_back((float)i); grid.emplace_back(0);  grid.emplace_back(-(float)GRID_LENGTH);
			grid.emplace_back((float)i); grid.emplace_back(0);  grid.emplace_back((float)GRID_LENGTH);
			grid_index.emplace_back(id++); grid_index.emplace_back(id++);
		}
	}

	MyEngine::VertexBufferLayout grid_layout;
	grid_layout.Push<float>(3);

	shader_grid = App->renderer->CreateShader("grid", "default.vs", "default.fs");
	shader_grid->SetUniform4("albedo", grid_color[0], grid_color[1], grid_color[2], grid_color[3]);
	shader_grid->SetUniform4x4("model", identity);
	
	grid_mesh = new Mesh();
	grid_mesh->vbo = new MyEngine::VertexBuffer(&grid);
	grid_mesh->vao = new MyEngine::VertexArray();
	grid_mesh->vao->AddBuffer(*grid_mesh->vbo, grid_layout);
	grid_mesh->ibo = new MyEngine::IndexBuffer(&grid_index);
	grid_mesh->polygon = 2;
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

	gizmo_mesh = new Mesh();
	gizmo_mesh->vbo = new MyEngine::VertexBuffer(&gizmo);
	gizmo_mesh->vao = new MyEngine::VertexArray();
	gizmo_mesh->vao->AddBuffer(*gizmo_mesh->vbo, gizmo_layout);
	gizmo_mesh->ibo = new MyEngine::IndexBuffer(&gizmo_index);
	gizmo_mesh->polygon = 2;
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
		ImGui::MenuItem("Console", NULL, &console_window);
		ImGui::MenuItem("Hierarchy", NULL, &hierarchy_window);
		ImGui::MenuItem("Inspector", NULL, &inspect_window);
		ImGui::MenuItem("Scene", NULL, &scene_window);
		ImGui::Separator();
		ImGui::MenuItem("Configuration", NULL, &config_window);
		ImGui::MenuItem("Debug Tools", NULL, &debug_window);
		ImGui::MenuItem("Textures", NULL, &texture_window);

		ImGui::EndMenu();
	}

	bool show_about = false;
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("Github")) { OpenURL("https://github.com/bar0net/MyEngine"); }
		if (ImGui::MenuItem("Report bug")) { OpenURL("https://github.com/bar0net/MyEngine/issues"); }

		if (ImGui::MenuItem("About")) 
		{
			show_about = true;
		}
		ImGui::EndMenu();
	}
	
	ImGui::EndMainMenuBar();

	if (show_about) ImGui::OpenPopup("AboutPopup");
	if (ImGui::BeginPopup("AboutPopup"))
	{
		ImGui::Text("MyEngine (Provisional name)");
		ImGui::Text("Developed by Jordi Tudela Alcacer");
		ImGui::Text("Under MIT License, 2018.");
		ImGui::Separator();
		std::string s = std::string("version ") + std::string(MYENGINE_VERSION);
		ImGui::Text(s.c_str());
		ImGui::EndPopup();
	}
	

	return true;
}

