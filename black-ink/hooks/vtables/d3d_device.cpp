#include "../hooks.h"
#include "../../menu/menu.h"

long __stdcall hooks::d3d_device::present::hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region) {
	IDirect3DVertexDeclaration9* vert_dec;
	if (device->GetVertexDeclaration(&vert_dec))
		return present_original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);

	IDirect3DVertexShader9* vert_shader;
	if (device->GetVertexShader(&vert_shader))
		return present_original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	menu->on_paint();


	render::add_to_draw_list();

	ImGui::GetBackgroundDrawList( )->AddCircle( ImVec2( 1280 / 2, 720 / 2 ), cfg::get < float >( FNV1A( "legitbot.aimbot.fov" ) ) * 5.f, ImColor(255,0,0 ) , 100);

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetVertexShader(vert_shader);
	device->SetVertexDeclaration(vert_dec);

	return present_original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
}

long __stdcall hooks::d3d_device::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params) {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto ret = reset_original(device, present_params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}