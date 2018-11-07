#ifndef _PANEL_CONSOLE_H
#define _PANEL_CONSOLE_H

#include "_Vendor/imgui-docking/imgui.h"

#include "LogSystem.h"

#define DISABLEDTEXT_COLOR ImVec4(0.7F, 0.7F, 0.7F, 1.0F)
#define DISABLED_COLOR ImVec4(0.5F, 0.5F, 0.5F, 1.0F)
#define DISABLED_HIGHLIGHT_COLOR ImVec4(0.6F, 0.6F, 0.6F, 1.0F)
#define NONE_COLOR ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
#define DEBUG_COLOR ImVec4(0.0F, 1.0F, 1.0F, 1.0F)
#define INFO_COLOR ImVec4(1.0F, 1.0F, 1.0F, 1.0F)
#define WARNING_COLOR ImVec4(1.0F, 1.0F, 0.0F, 1.0F)
#define ERROR_COLOR ImVec4(1.0F, 0.0F, 0.0F, 1.0F)

struct PanelConsole
{
private:
	unsigned int prev_log_size = 0;
	bool show_info = true;
	bool show_debug = true;
	bool show_warning = true;
	bool show_error = true;

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
		while (i-- > 0) ImGui::PopStyleColor();
	}

public:
	void Draw(bool enabled)
	{
		ImGui::Begin("Console", &enabled);

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
			switch (it->type)
			{
			case MyEngine::LogLevel::Info:
				if (show_info) ImGui::TextColored(INFO_COLOR, it->message.c_str());
				break;
			case MyEngine::LogLevel::Debug:
				if (show_debug) ImGui::TextColored(DEBUG_COLOR, it->message.c_str());
				break;
			case MyEngine::LogLevel::Warning:
				if (show_warning) ImGui::TextColored(WARNING_COLOR, it->message.c_str());
				break;
			case MyEngine::LogLevel::Error:
				if (show_error) ImGui::TextColored(ERROR_COLOR, it->message.c_str());
				break;
			default:
				if (show_info) ImGui::TextColored(NONE_COLOR, it->message.c_str());
				break;
			}
		}

		// Set Scrolling at the end when a new line is added to the log.
		if (prev_log_size != logger->history.size())
		{
			prev_log_size = logger->history.size();
			ImGui::SetScrollHereY();
		}

		ImGui::EndChild();

		ImGui::End();
	}

};

#endif // !_PANEL_CONSOLE_H

