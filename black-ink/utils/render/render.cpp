#include "../utils.h"
#include "../../sdk/interfaces.h"
#include "../../features/features.h"

#include "assets/background.h"
#include "assets/minecraftFont.h"
#include "assets/items.h"

namespace render {
	void init() {
		ImGui::CreateContext();

		ImGui_ImplWin32_Init(input::m_hwnd);
		ImGui_ImplDX9_Init(interfaces::m_d3d_device);

		m_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());
		m_data_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());
		m_replace_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());

		auto& io = ImGui::GetIO();
		auto& style = ImGui::GetStyle();

		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &LogotypeRaw, sizeof( LogotypeRaw ), &assets::logotype );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &BackgroundRaw, sizeof( BackgroundRaw ), &assets::background );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &DirtRaw, sizeof( DirtRaw ), &assets::dirt );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &DiamondRaw, sizeof( DiamondRaw ), &assets::diamond );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &CreeperRaw, sizeof( CreeperRaw ), &assets::creeper );

		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &Aimbot, sizeof( Aimbot ), &assets::icons[ 0 ] );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &Visuals, sizeof( Visuals ), &assets::icons[ 1 ] );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &Misc, sizeof( Misc ), &assets::icons[ 2 ] );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &Skins, sizeof( Skins ), &assets::icons[ 3 ] );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &Files, sizeof( Files ), &assets::icons[ 4 ] );
		D3DXCreateTextureFromFileInMemory( interfaces::m_d3d_device, &Dashboard, sizeof( Dashboard ), &assets::icons[ 5 ] );

		ImFontConfig minecraft14;
		minecraft14.RasterizerFlags = ImGuiFreeType::ForceAutoHint;

		ImFontConfig minecraft16;
		minecraft16.RasterizerFlags = ImGuiFreeType::ForceAutoHint;

		fonts::m_minecraft14 = io.Fonts->AddFontFromMemoryCompressedTTF( minecraft_compressed_data, minecraft_compressed_size, 14, &minecraft14 );
		fonts::m_minecraft16 = io.Fonts->AddFontFromMemoryCompressedTTF( minecraft_compressed_data, minecraft_compressed_size, 16, &minecraft16 );


		style.WindowPadding = {};
		style.WindowBorderSize = {};
		style.ItemSpacing = {};

		ImGuiFreeType::BuildFontAtlas(io.Fonts);
	}

	vec2_t get_text_size(std::string_view txt, ImFont* font) {
		if (!font
			|| txt.empty()
			|| !font->IsLoaded())
			return vec2_t();

		const auto size = font->CalcTextSizeA(font->FontSize, std::numeric_limits<float>::max(), 0.f, txt.data());

		return vec2_t(IM_FLOOR(size.x + 0.95f), size.y);
	}

	void text(std::string_view txt, vec2_t pos, const col_t& clr, ImFont* font, bit_flag_t<uint8_t> flags) {
		if (!font
			|| txt.empty()
			|| clr.a() <= 0
			|| !font->IsLoaded())
			return;

		const auto centered_x = flags.has(FONT_CENTERED_X);
		const auto centered_y = flags.has(FONT_CENTERED_Y);

		if (centered_x
			|| centered_y) {
			const auto text_size = get_text_size(txt, font);

			if (centered_x) {
				pos.x -= text_size.x / 2.f;
			}

			if (centered_y) {
				pos.y -= text_size.y / 2.f;
			}
		}

		m_draw_list->PushTextureID(font->ContainerAtlas->TexID);

		if (flags.has(FONT_DROP_SHADOW)) {
			m_draw_list->AddTextSoftShadow(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), clr.hex(), txt.data());
		}
		else if (flags.has(FONT_OUTLINE)) {
			m_draw_list->AddTextOutline(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), clr.hex(), txt.data());
		}
		else {
			m_draw_list->AddText(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), clr.hex(), txt.data());
		}

		m_draw_list->PopTextureID();
	}

	void line(const vec2_t& from, const vec2_t& to, const col_t& clr) {
		m_draw_list->AddLine(*reinterpret_cast<const ImVec2*>(&from), *reinterpret_cast<const ImVec2*>(&to), clr.hex());
	}

	void rect(const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding) {
		m_draw_list->AddRect(*reinterpret_cast<const ImVec2*>(&pos), ImVec2(pos.x + size.x, pos.y + size.y), clr.hex(), rounding);
	}

	void rect_filled(const vec2_t& pos, const vec2_t& size, const col_t& clr, float rounding) {
		m_draw_list->AddRectFilled(*reinterpret_cast<const ImVec2*>(&pos), ImVec2(pos.x + size.x, pos.y + size.y), clr.hex(), rounding);
	}

	void rect_filled_multi_clr(const vec2_t& pos, const vec2_t& size, const col_t& clr_upr_left, const col_t& clr_upr_right, const col_t& clr_bot_left, const col_t& clr_bot_right) {
		m_draw_list->AddRectFilledMultiColor(*reinterpret_cast<const ImVec2*>(&pos), ImVec2(pos.x + size.x, pos.y + size.y), clr_upr_left.hex(), clr_upr_right.hex(), clr_bot_right.hex(), clr_bot_left.hex());
	}

	void rect_angle(int x1, int y1, int x2, int y2, col_t color, float thickness, float rounding) {
		m_draw_list->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), color.hex(), rounding, 15, thickness);
	}

	void rect_filled_int(int x1, int y1, int x2, int y2, const col_t& clr) {
		m_draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), clr.hex());
	}

	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		auto screen_transform = [](const vec3_t& in, vec3_t& out) -> bool {
			static const auto& matrix = *reinterpret_cast<v_matrix*>(*SIG("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9").self_offset(0x3).cast<uintptr_t*>() + 0xB0);

			out.x = matrix[0][0] * in.x + matrix[0][1] * in.y + matrix[0][2] * in.z + matrix[0][3];
			out.y = matrix[1][0] * in.x + matrix[1][1] * in.y + matrix[1][2] * in.z + matrix[1][3];

			const auto w = matrix[3][0] * in.x + matrix[3][1] * in.y + matrix[3][2] * in.z + matrix[3][3];

			if (w < 0.001f) {
				out.x *= 100000.f;
				out.y *= 100000.f;
				return true;
			}

			const auto inv_w = 1.f / w;

			out.x *= inv_w;
			out.y *= inv_w;

			return false;
		};

		if (!screen_transform(in, out)) {
			out.x = (m_screen_size.x * 0.5f) + (out.x * m_screen_size.x) * 0.5f;
			out.y = (m_screen_size.y * 0.5f) - (out.y * m_screen_size.y) * 0.5f;
			return true;
		}

		return false;
	}

	void text_string(float x, float y, col_t color, int flags, ImFont* font, const char* message, ...) {
		char output[1024] = {};
		va_list args;
		va_start(args, message);
		vsprintf_s(output, message, args);
		va_end(args);

		if (!font->ContainerAtlas)
			return;

		m_draw_list->PushTextureID(font->ContainerAtlas->TexID);

		auto coord = ImVec2(x, y);
		auto size = ImGui::CalcTextSize(output);
		auto coord_out = ImVec2{ coord.x + 1.f, coord.y + 1.f };
		col_t outline_clr = col_t(0, 0, 0, 170);

		int width = 0, height = 0;

		if (!(flags & e_font_flags::FONT_CENTERED_X))
			size.x = 0;
		if (!(flags & e_font_flags::FONT_CENTERED_Y))
			size.y = 0;
		ImVec2 pos = ImVec2(coord.x - (size.x * .5), coord.y - (size.y * .5));

		if (flags & e_font_flags::FONT_DROP_SHADOW) {
			m_draw_list->AddTextSoftShadow(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), color.hex(), output);
		}
		else if (flags & e_font_flags::FONT_OUTLINE) {
			m_draw_list->AddTextOutline(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), color.hex(), output);
		}
		else {
			m_draw_list->AddText(font, font->FontSize, *reinterpret_cast<ImVec2*>(&pos), color.hex(), output);
		}

		m_draw_list->PopTextureID();

	}

	void add_to_draw_list() {
		const auto lock = std::unique_lock<std::mutex>(m_mutex, std::try_to_lock);
		if (lock.owns_lock()) {
			*m_replace_draw_list = *m_data_draw_list;
		}

		*ImGui::GetBackgroundDrawList() = *m_replace_draw_list;
	}

	void begin() {
		m_draw_list->Clear();
		m_draw_list->PushClipRectFullScreen();

		m_screen_size = *reinterpret_cast<vec2_t*>(&ImGui::GetIO().DisplaySize);

		player_esp->on_paint();
		{
			const auto lock = std::unique_lock<std::mutex>(m_mutex);

			*m_data_draw_list = *m_draw_list;
		}
	}

	void polygon(const std::vector<vec2_t>& points, const col_t& clr) {
		if (clr.a() <= 0)
			return;

		m_draw_list->_Path.reserve(m_draw_list->_Path.Size + points.size() + 1);

		for (auto& point : points) {
			m_draw_list->_Path.push_back(*reinterpret_cast<const ImVec2*>(&point));
		}

		m_draw_list->PathStroke(clr.hex(), true, 1.f);
	}

	void polygon_filled(const std::vector<vec2_t>& points, const col_t& clr) {
		if (clr.a() <= 0)
			return;

		m_draw_list->_Path.reserve(m_draw_list->_Path.Size + points.size() + 1);

		for (auto& point : points) {
			m_draw_list->_Path.push_back(*reinterpret_cast<const ImVec2*>(&point));
		}

		m_draw_list->PathFillConvex(clr.hex());
	}

	std::mutex m_mutex;

	vec2_t m_screen_size;

	std::shared_ptr<ImDrawList> m_draw_list;
	std::shared_ptr<ImDrawList> m_data_draw_list;
	std::shared_ptr<ImDrawList> m_replace_draw_list;
}