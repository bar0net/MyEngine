#include "ModuleEditor.h"

#include "../_Vendor/imgui-docking/imgui.h"
#include "../_Vendor/imgui-docking/imgui_impl_sdl.h"
#include "../_Vendor/imgui-docking/imgui_impl_opengl3.h"

#include "../Application.h"
#include "../Utils/Window_Utils.h"
#include "../Utils/MovingArray.h"

#include "ModuleRenderer.h"
#include "ModuleTime.h"

#define GLSL_VERSION "#version 130"

bool ModuleEditor::Init()
{
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
	
	return true;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete(fps);

	return true;
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