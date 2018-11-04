#include "ModuleEditor.h"

#include <assert.h>

#include "../_Vendor/imgui-docking/imgui.h"
#include "../_Vendor/imgui-docking/imgui_impl_sdl.h"
#include "../_Vendor/imgui-docking/imgui_impl_opengl3.h"
#include "../_Vendor/MathGeoLib/Math/float4x4.h"
#include "../_Vendor/MathGeoLib/Math/float3.h"

#include "../Application.h"
#include "../Utils/Window_Utils.h"
#include "../Utils/Render_Utils.h"
#include "../Utils/MovingArray.h"
#include "../Utils/VertexBuffer.h"
#include "../Utils/VertexArray.h"
#include "../Utils/Shader.h"
#include "../Utils/IndexBuffer.h"
#include "../Utils/VertexBufferLayout.h"

#include "ModuleRenderer.h"
#include "ModuleTime.h"
#include "ModuleScene.h"
#include "ModuleTexture.h"

#include "../GameObject/GameObject.h"
#include "../GameObject/Components/Camera.h"
#include "../GameObject/Components/CameraControl.h"
#include "../GameObject/Components/MeshRenderer.h"

#define GLSL_VERSION "#version 130"
#define GRID_LENGTH 100
#define GIZMO_LENGTH 0.5F

#define DISABLEDTEXT_COLOR ImVec4(0.7F, 0.7F, 0.7F, 1.0F)
#define DISABLED_COLOR ImVec4(0.5F, 0.5F, 0.5F, 1.0F)
#define DISABLED_HIGHLIGHT_COLOR ImVec4(0.6F, 0.6F, 0.6F, 1.0F)
#define NONE_COLOR ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
#define DEBUG_COLOR ImVec4(0.0F, 1.0F, 1.0F, 1.0F)
#define INFO_COLOR ImVec4(1.0F, 1.0F, 1.0F, 1.0F)
#define WARNING_COLOR ImVec4(1.0F, 1.0F, 0.0F, 1.0F)
#define ERROR_COLOR ImVec4(1.0F, 0.0F, 0.0F, 1.0F)

void AddConsoleButton(const char* text, bool& active, MyEngine::LogLevel type)
{
	unsigned int i = 0;
	if (active)
	{
		switch (type)
		{
		case MyEngine::LogLevel::None:
			ImGui::PushStyleColor(ImGuiCol_Text, NONE_COLOR);
			break;
		case MyEngine::LogLevel::Debug:
			ImGui::PushStyleColor(ImGuiCol_Text, DEBUG_COLOR);
			break;
		case MyEngine::LogLevel::Info:
			ImGui::PushStyleColor(ImGuiCol_Text, INFO_COLOR);
			break;
		case MyEngine::LogLevel::Warning:
			ImGui::PushStyleColor(ImGuiCol_Text, WARNING_COLOR);
			break;
		case MyEngine::LogLevel::Error:
			ImGui::PushStyleColor(ImGuiCol_Text, ERROR_COLOR);
			break;
		default:
			ImGui::PushStyleColor(ImGuiCol_Text, INFO_COLOR);
			break;
		}
		++i;
	}
	else 
	{
		ImGui::PushStyleColor(ImGuiCol_Text, DISABLEDTEXT_COLOR); ++i;
		ImGui::PushStyleColor(ImGuiCol_Button, DISABLED_COLOR); ++i;
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, DISABLED_HIGHLIGHT_COLOR); ++i;
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, DISABLED_HIGHLIGHT_COLOR); ++i;
	}
	if (ImGui::Button(text)) active = !active;
	while(i-- > 0) ImGui::PopStyleColor();
}

bool ModuleEditor::Init()
{
	LOGINFO("Initializing Editor.");
	fps = new MovingArray(300, 0);
	avg_ms_array = new MovingArray(30, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	
	ImGui_ImplSDL2_InitForOpenGL(App->renderer->data->window, App->renderer->data->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();

	math::float4x4 identity = math::float4x4::identity;
	// Create Grid
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

	return true;
}

bool ModuleEditor::CleanUp()
{
	LOGINFO("Destroying Editor.");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete(fps);
	delete(avg_ms_array);

	delete(vbo_grid);	delete(vbo_gizmo);
	delete(ibo_grid);	delete(ibo_gizmo);
	delete(shader_grid);delete(shader_gizmo);
	delete(vao_grid);	delete(vao_gizmo);

	return true;
}

UpdateState ModuleEditor::PreUpdate()
{
	if (show_grid)
	{
		App->renderer->DrawLines(vao_gizmo, ibo_gizmo, shader_gizmo, 2.5F);
		App->renderer->DrawLines(vao_grid, ibo_grid, shader_grid);
	}

	return UpdateState::Update_Continue;
}

UpdateState ModuleEditor::Update()
{
	FrameStart();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit")) return UpdateState::Update_End;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Configuration", NULL, &config_window);
			ImGui::MenuItem("Console", NULL, &console_window);
			ImGui::MenuItem("Debug Tools", NULL, &debug_window);
			ImGui::MenuItem("Inspector", NULL, &inspect_window);
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

	if(config_window) 
	{
		ImGui::Begin("Configuration", &config_window);
		if (ImGui::CollapsingHeader("Editor")) PanelEditor();
		ImGui::End();
	}

	if(debug_window) 
	{
		ImGui::Begin("Debug Tools", &debug_window);
		if (ImGui::CollapsingHeader("Performance")) PanelPerformance();
		ImGui::End();
	}

	if(inspect_window)
	{
		ImGui::Begin("Inspector", &inspect_window);
		PanelObjects();
		ImGui::End();
	}

	if (console_window)
	{
		ImGui::Begin("Console", &console_window);
		PanelConsole();
		ImGui::End();
	}

	FrameEnd();
	return UpdateState::Update_Continue;
}

void ModuleEditor::FrameStart()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->renderer->data->window);
	ImGui::NewFrame();

	fps->push((unsigned int)App->time->FPS());
	avg_ms -= (*avg_ms_array)[0];
	avg_ms_array->push(App->time->DeltaTimeMS());
	avg_ms += App->time->DeltaTimeMS();
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
}

void ModuleEditor::ProcessEvent(void* event) const
{
	assert(event);
	ImGui_ImplSDL2_ProcessEvent((SDL_Event*)event);
}

void ModuleEditor::PanelPerformance() const
{
	ImGui::Text("Frames per second");
	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 25);
	ImGui::PlotHistogram("", MovingArray::Get, fps, fps->size, 1, "", 0, 120, ImVec2(0, 50));
	ImGui::Text("Average ms/frame: %ims - FPS: %i", avg_ms / (avg_ms_array->size), (int)(1000.0F * avg_ms_array->size / avg_ms));
	ImGui::Text("Width: %ipx - Height: %ipx", App->renderer->width, App->renderer->height);

	bool vsyncEnabled = App->renderer->vsyncEnabled;
	ImGui::Checkbox("VSync", &vsyncEnabled);
	if (vsyncEnabled != App->renderer->vsyncEnabled) App->renderer->EnableVSync(vsyncEnabled);

	ImGui::Separator();
	int major = 0; 
	int minor = 0; 
	int patch = 0;
	MyEngine::WindowUtils::Version(major, minor, patch);

	ImGui::Text("GPU");
	ImGui::Text("%s", MyEngine::RenderUtils::Vendor());
	ImGui::Text("%s", MyEngine::RenderUtils::Renderer());

	ImGui::Separator();
	ImGui::Text("Libraries");
	ImGui::Text("Glew %s", MyEngine::RenderUtils::Version());
	ImGui::Text("SDL %d.%d.%d", major, minor, patch);
	ImGui::Text("ImGui %s", ImGui::GetVersion());
	ImGui::Text("MathGeoLib 1.5"); 
}

void ModuleEditor::PanelEditor()
{
	// Background Color
	ImVec4 bgcolor(App->renderer->clearColor[0], App->renderer->clearColor[1], App->renderer->clearColor[2], App->renderer->clearColor[3]);
	bool open_bgcolor = ImGui::ColorButton("BGButton", bgcolor);
	ImGui::SameLine(); ImGui::Text("Background Color");
	if (open_bgcolor) ImGui::OpenPopup("BackgroundPicker");
	if (ImGui::BeginPopup("BackgroundPicker"))
	{
		if (ImGui::ColorPicker4("Background Color", App->renderer->clearColor)) App->renderer->UpdateClearColor();
		ImGui::EndPopup();
	}

	// Grid Color
	bool open_gridcolor = ImGui::ColorButton("GridColorButton", *(ImVec4*)&grid_color);
	ImGui::SameLine(); ImGui::Text("Grid Color");
	if (open_gridcolor) ImGui::OpenPopup("GridColorPicker");
	if (ImGui::BeginPopup("GridColorPicker"))
	{
		if (ImGui::ColorPicker4("Grid Color", grid_color))
			shader_grid->SetUniform4("albedo", grid_color[0], grid_color[1], grid_color[2], grid_color[3]);
		ImGui::EndPopup();
	}

	ImGui::Checkbox("Show Grid", &show_grid);
		
}

void ModuleEditor::PanelObjects() 
{
	if (App->scene->gameObjects.size() == 0) return;

	ImVec2 panel_size = ImGui::GetWindowSize();

	for (std::unordered_map<const char*, GameObject*>::iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
	{
		if (ImGui::Button(it->first, ImVec2(panel_size.x - 17, 0))) 
		{
			inspect_object = it->second; 
		}
	}

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

void ModuleEditor::PanelConsole()
{
	// Clear Button
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0F, 0.55F, 0.55F, 1.0F));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0F, 0.6F, 0.6F, 1.0F));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0F, 0.62F, 0.62F, 1.0F));
	if (ImGui::Button("Clear")) logger->history.clear();
	ImGui::PopStyleColor(); 
	ImGui::PopStyleColor(); 
	ImGui::PopStyleColor();
	ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

	// Log Level Buttons
	AddConsoleButton("Info", show_info, MyEngine::LogLevel::Info);			ImGui::SameLine();
	AddConsoleButton("Debug", show_debug, MyEngine::LogLevel::Debug);		ImGui::SameLine();
	AddConsoleButton("Warning", show_warning, MyEngine::LogLevel::Warning);	ImGui::SameLine();
	AddConsoleButton("Error", show_error, MyEngine::LogLevel::Error);		ImGui::Separator();

	// Console Text
	ImGui::BeginChild("Scrolling");
	for (std::list<MyEngine::LogData>::iterator it = logger->history.begin(); it != logger->history.end(); ++it)
	{
		switch(it->type)
		{
		case MyEngine::LogLevel::Info:
			if(show_info) ImGui::TextColored(INFO_COLOR, it->message.c_str());
			break;
		case MyEngine::LogLevel::Debug:
			if(show_debug) ImGui::TextColored(DEBUG_COLOR, it->message.c_str());
			break;
		case MyEngine::LogLevel::Warning:
			if(show_warning) ImGui::TextColored(WARNING_COLOR, it->message.c_str());
			break;
		case MyEngine::LogLevel::Error:
			if (show_error) ImGui::TextColored(ERROR_COLOR, it->message.c_str());
			break;
		default:
			if (show_info) ImGui::TextColored(NONE_COLOR, it->message.c_str());
			break;
		}
	}
	ImGui::EndChild();
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
			ImGui::Separator();
		}
	}
}
