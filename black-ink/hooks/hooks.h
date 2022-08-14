#pragma once
#include "../globals.h"
#include "../features/features.h"

namespace hooks {
	void init();

	void undo();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	namespace d3d_device {
		namespace reset {
			long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params);
			using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		}

		namespace present {
			long __stdcall hook(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region);
			using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
		}
	}

	namespace client_dll {
		namespace frame_stage_notify {
			void __stdcall hook(e_client_frame_stage stage);
			using fn = void(__stdcall*)(e_client_frame_stage);
		}

		namespace create_move {
			void __stdcall hook(int sequence_number, float sample_frametime, bool active, bool& packet);
			void __stdcall gate(int sequence_number, float sample_frametime, bool active);
			using fn = void(__stdcall*)(int, float, bool);
		}
	}

	namespace client_mode {
		namespace override_view {
			void __stdcall hook(view_setup_t* view);
			using fn = void(__stdcall*)(view_setup_t*);
		}
	}

	namespace model_render {
		namespace draw_model_execute {
			void __fastcall hook(i_model_render* ecx, void* edx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones);
			using fn = void(__thiscall*)(i_model_render*, void*, const draw_model_state_t&, const model_render_info_t&, matrix3x4_t*);
		}
	}

	namespace panel {
		namespace paint_traverse {
			void __fastcall hook(void* ecx, void* edx, uint32_t id, bool force_repaint, bool allow_force);
			using fn = void(__thiscall*)(void*, uint32_t, bool, bool);
		}
	}

	namespace surface {
		namespace lock_cursor {
			void __fastcall hook(i_surface* ecx, void* edx);
			using fn = void(__fastcall*)(i_surface*, void*);
		}
	}

	namespace player {
		namespace eye_angles {
			qangle_t* __fastcall hook(void* ecx, void* edx);
			using fn = qangle_t* (__fastcall*)(void*, void*);
		}
	}

	inline hooks::d3d_device::present::fn present_original = nullptr;
	inline hooks::d3d_device::reset::fn reset_original = nullptr;
	inline hooks::client_dll::frame_stage_notify::fn frame_stage_notify_original = nullptr;
	inline hooks::client_dll::create_move::fn create_move_original = nullptr;
	inline hooks::client_mode::override_view::fn override_view_original = nullptr;
	inline hooks::model_render::draw_model_execute::fn draw_model_execute_original = nullptr;
	inline hooks::panel::paint_traverse::fn paint_traverse_original = nullptr;
	inline hooks::player::eye_angles::fn eye_angles_original = nullptr;
	inline hooks::surface::lock_cursor::fn lock_cursor_original = nullptr;
}
