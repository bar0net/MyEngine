#include "ModuleEditor.h"

#include "../_Vendor/imgui-docking/imgui.h"
#include "../_Vendor/imgui-docking/imgui_impl_sdl.h"
#include "../_Vendor/imgui-docking/imgui_impl_opengl3.h"

#include "../Application.h"
#include "../Utils/Window_Utils.h"
#include "../Utils/MovingArray.h"
#include "../Utils/VertexBuffer.h"
#include "../Utils/VertexArray.h"
#include "../Utils/Shader.h"
#include "../Utils/IndexBuffer.h"
#include "../_Vendor/MathGeoLib/Math/float4x4.h"
#include "../Utils/VertexBufferLayout.h"

#include "ModuleRenderer.h"
#include "ModuleTime.h"
#include "ModuleScene.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Components/Camera.h"
#include "../_Vendor/MathGeoLib/Math/float3.h"

#define GLSL_VERSION "#version 130"

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
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	
	ImGui_ImplSDL2_InitForOpenGL(App->renderer->data->window, App->renderer->data->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();
	
	// GRID
	std::vector<float> grid;
	std::vector<unsigned int> grid_index;
	unsigned int id = 0;

	for (float i = -10.0f; i <= 10.0f; i += 1)
	{
		grid.push_back(-100.0f); grid.push_back(0); grid.push_back(abs(i)*i);
		grid.push_back(100.0f); grid.push_back(0); grid.push_back(abs(i)*i);
		grid_index.push_back(id++); grid_index.push_back(id++);

		grid.push_back(abs(i)*i); grid.push_back(0);  grid.push_back(-100.0f);
		grid.push_back(abs(i)*i); grid.push_back(0);  grid.push_back(100.0f);
		grid_index.push_back(id++); grid_index.push_back(id++);
	}
	MyEngine::VertexBufferLayout vbl;
	vbl.Push<float>(3);

	vbo_grid = new MyEngine::VertexBuffer(&grid);
	ibo_grid = new MyEngine::IndexBuffer(&grid_index);
	
	vao_grid = new MyEngine::VertexArray();
	vao_grid->AddBuffer(*vbo_grid, vbl);
	App->renderer->CreateShader("grid", "default.vs", "default.fs");
	shader_grid = App->renderer->GetShader("grid");
	shader_grid->SetUniform4("albedo", grid_color[0], grid_color[1], grid_color[2], grid_color[3]);

	return true;
}

bool ModuleEditor::Start()
{
	math::float4x4 I = math::float4x4::identity;
	shader_grid->SetUniform4x4("model", &I);

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
	delete(vbo_grid);
	delete(ibo_grid);
	delete(shader_grid);
	delete(vao_grid);

	return true;
}

UpdateState ModuleEditor::PreUpdate()
{
	if (show_grid)
	{
		shader_grid->Bind();
		//vbo_grid->Bind();
		vao_grid->Bind();
		ibo_grid->Bind();
		ibo_grid->DrawLines();
	}

	return UpdateState::Update_Continue;
}

UpdateState ModuleEditor::Update()
{
	FrameStart();

	{
		fps->push((unsigned int)App->time->FPS());
		avg_ms -= (*avg_ms_array)[0];
		avg_ms_array->push(App->time->DeltaTimeMS());
		avg_ms += App->time->DeltaTimeMS();

		ImGui::Begin("Menu",NULL);
		if (ImGui::CollapsingHeader("Performance"))
		{
			ImGui::Text("Frames per second");
			ImGui::PlotHistogram("", MovingArray::Get, fps, fps->size, 0, "", 0, 120, ImVec2(0, 50));
			ImGui::Text("Average ms/frame: %ims - FPS: %i", avg_ms / (avg_ms_array->size), (int)(1000.0f * avg_ms_array->size / avg_ms));
		}

		if (ImGui::CollapsingHeader("Editor"))
		{
			// Background Color
			ImVec4 bgcolor( App->renderer->clearColor[0], App->renderer->clearColor[1], App->renderer->clearColor[2], App->renderer->clearColor[3] );
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
			ImGui::Separator();
		}

		if (ImGui::CollapsingHeader("Camera"))
		{
			GameObject* go = App->scene->gameObjects["Camera"];
			if (go != nullptr)
			{
				Camera* c = (Camera*)go->components["Camera"];
				if (c != nullptr)
				{
					float pos[3] = { go->position.x, go->position.y, go->position.z };
					ImGui::InputFloat3("Position", pos, 2);
					if (pos[0] != go->position.x || pos[1] != go->position.y || pos[2] != go->position.z)
						go->SetPosition(pos[0], pos[1], pos[2]);

					float rot[3] = { go->rotation.x, go->rotation.y, go->rotation.z };
					ImGui::InputFloat3("Rotation", rot, 2);
					if (pos[0] != go->rotation.x || pos[1] != go->rotation.y || pos[2] != go->rotation.z)
						go->SetRotation(rot[0], rot[1], rot[2]);

					float fov = c->fov;
					ImGui::SliderFloat("FOV", &fov, 60, 120);
					if (fov != c->fov)
					{
						c->fov = fov;
						c->UpdateFrustum();
					}
				}
			}
			ImGui::Text("Width: %ipx - Height: %ipx", App->renderer->width, App->renderer->height);
		}
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
}

void ModuleEditor::FrameEnd()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ModuleEditor::ProcessEvent(void* event)
{
	ImGui_ImplSDL2_ProcessEvent((SDL_Event*)event);
}