#ifndef _PANEL_PERFORMANCE_H
#define _PANEL_PERFORMANCE_H

#include "Utils/MovingArray.h"

#include "_Vendor/imgui-docking/imgui.h"
#include "IL/il.h"

#include "Globals.h"
#include "Application.h"
#include "Module/ModuleRenderer.h"
#include "Module/ModuleTime.h"
#include "Utils/Render_Utils.h"
#include "Utils/Window_Utils.h"

struct PanelPerfomance
{
private:
	MovingArray * fps = nullptr;
	MovingArray* avg_ms_array = nullptr;
	unsigned int avg_ms = 0;

public:
	PanelPerfomance()
	{
		fps = new MovingArray(300, 0);
		avg_ms_array = new MovingArray(30, 0);
	}

	~PanelPerfomance()
	{
		delete fps;
		delete avg_ms_array;
	}

	void Draw(bool& enabled, unsigned int scene_width, unsigned int scene_height)
	{
		ImGui::Begin("Debug Tools", &enabled);

		fps->push((unsigned int)App->time->FPS());
		avg_ms -= (*avg_ms_array)[0];
		avg_ms_array->push(App->time->DeltaTimeMS());
		avg_ms += App->time->DeltaTimeMS();

		if (ImGui::CollapsingHeader("Performance"))
		{
			ImGui::Text("=== PERFORMANCE ===");
			ImGui::Text("Frames per second");
			ImGui::PushItemWidth(ImGui::GetWindowWidth() - 25);
			ImGui::PlotHistogram("", MovingArray::Get, fps, fps->size, 1, "", 0, 120, ImVec2(0, 50));
			ImGui::Text("Average ms/frame: %ims - FPS: %i", avg_ms / (avg_ms_array->size), (int)(1000.0F * avg_ms_array->size / avg_ms));
			ImGui::Text("Window: %ipx x %ipx", App->renderer->width, App->renderer->height);
			ImGui::Text("Scene View: %ipx x %ipx", scene_width, scene_height);

			bool vsyncEnabled = App->renderer->vsyncEnabled;
			ImGui::Checkbox("VSync", &vsyncEnabled);
			if (vsyncEnabled != App->renderer->vsyncEnabled) App->renderer->EnableVSync(vsyncEnabled);
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("GPU");
			ImGui::Text("%s", MyEngine::RenderUtils::Vendor());
			ImGui::Text("%s", MyEngine::RenderUtils::Renderer());
		}

		if (ImGui::CollapsingHeader("Libraries"))
		{
			ImGui::Separator();
			int major = 0;
			int minor = 0;
			int patch = 0;
			MyEngine::WindowUtils::Version(major, minor, patch);

			ImGui::Text("MyEngine %s", MYENGINE_VERSION);
			ImGui::Text("Glew %s", MyEngine::RenderUtils::Version());
			ImGui::Text("SDL %d.%d.%d", major, minor, patch);
			ImGui::Text("ImGui %s", ImGui::GetVersion());
			ImGui::Text("MathGeoLib 1.5");
			ImGui::Text("DevIL %i", ilGetInteger(IL_VERSION_NUM));
		}

		ImGui::End();
	}

};

#endif // !_PANEL_PERFORMANCE_H

