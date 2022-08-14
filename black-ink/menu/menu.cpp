#include "menu.h"

void c_menu::on_paint() {
	if (!(input::m_blocked = input::get_key<TOGGLE>(VK_INSERT)))
		return;

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);

	if (ImGui::Begin(_("black-ink sdk"), 0, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
		if (ImGui::BeginTabBar("cheat")) {
			if (ImGui::BeginTabItem("ragebot")) {

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("player")) {
				ImGui::Checkbox(_("visuals enable"), &cfg::get<bool>(FNV1A("visuals_enable")));
				ImGui::Checkbox(_("box esp"), &cfg::get<bool>(FNV1A("box_esp")));
				ImGui::Checkbox(_("name esp"), &cfg::get<bool>(FNV1A("name_esp")));
				ImGui::Checkbox(_("health esp"), &cfg::get<bool>(FNV1A("health_esp")));

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("visuals")) {

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("misc")) {
				ImGui::Checkbox(_("auto jump"), &cfg::get<bool>(FNV1A("auto_jump")));
				ImGui::Checkbox(_("auto strafe"), &cfg::get<bool>(FNV1A("auto_strafe")));

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("cfg")) {
				if (ImGui::Button(_("save"))) {
					cfg::save();
				}

				if (ImGui::Button(_("load"))) {
					cfg::load();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("lua")) {
				ImGui::Text(_("soon.."));

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}