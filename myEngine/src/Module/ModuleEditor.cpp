#include "ModuleEditor.h"

#include "../_Vendor/imgui-docking/imgui.h"
#include "../_Vendor/imgui-docking/imgui_impl_sdl.h"
#include "../_Vendor/imgui-docking/imgui_impl_opengl3.h"

#include "../Application.h"
#include "../Utils/Window_Utils.h"
#include "../Utils/MovingArray.h"
#include "../Utils/VertexBuffer.h"
#include "../Utils/Shader.h"
#include "../Utils/IndexBuffer.h"
#include "../_Vendor/MathGeoLib/Math/float4x4.h"

#include "ModuleRenderer.h"
#include "ModuleTime.h"

#define GLSL_VERSION "#version 130"

bool ModuleEditor::Init()
{
	LOGINFO("Initializing Editor.");
	fps = new MovingArray(300, 0);

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

	vbo_grid = new MyEngine::VertexBuffer(&grid);
	ibo_grid = new MyEngine::IndexBuffer(&grid_index);
	App->renderer->CreateShader("default", "default.vs", "default.fs");
	shader_grid = App->renderer->GetShader("default");

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
	delete(vbo_grid);
	delete(ibo_grid);
	delete(shader_grid);

	return true;
}

UpdateState ModuleEditor::PreUpdate()
{
	if (show_grid)
	{
		shader_grid->Bind();
		vbo_grid->Bind();
		ibo_grid->Bind();
		math::float4x4 I = math::float4x4::identity;
		shader_grid->SetUniform4x4("model", &I);
		ibo_grid->DrawLines();
	}

	return UpdateState::Update_Continue;
}

UpdateState ModuleEditor::Update()
{
	FrameStart();

	{
		fps->push((unsigned int)App->time->FPS());

		ImGui::Begin("Performance",NULL);
		ImGui::Text("Frames per second");
		ImGui::PlotHistogram("", MovingArray::Get, fps, fps->size, 0, "", 0, 120, ImVec2(0, 50));
		ImGui::Text("Curent FPS: %i", (*fps)[1]);
		ImGui::Separator();
		ImGui::Checkbox("Show Grid", &show_grid);
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