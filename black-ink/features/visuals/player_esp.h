#pragma once
#include "../../globals.h"
#include "preview/preview.hpp"

struct ESPPlayerData_t
{
	RECT m_aBBox;

	// anims
	float m_flPrevHealth = 0.0f;
	float m_flPrevArmor = 0.0f;
	float m_flPrevAmmo = 0.0f;
	float m_flPlayerPercentage = 1.0f;

	// box width && height
	int m_iWidth = 0;
	int m_iHeight = 0;

	int m_iDownOffset = 0;
	int m_iUpOffset = 0;

	int m_iLeftDownOffset = 0;
	int m_iLeftOffset = 0;

	int m_iRightDownOffset = 0;
	int m_iRightOffset = 0;
};

class c_player_esp : public c_singleton<c_player_esp> {
public:
	bool force_dormant(c_base_player* pPlayer);
	void on_paint();

	ImVec2 get_position(DraggableItemCondiction pos, ESPPlayerData_t* m_Data);
	ImVec2 get_position_offsetless(DraggableItemCondiction pos, ESPPlayerData_t* m_Data);

	void render_enemy_draggable(c_esp_preview* preview, c_cs_player* player, ESPPlayerData_t* m_Data );
	void render_team_draggable( c_esp_preview* preview, c_cs_player* player, ESPPlayerData_t* m_Data );

	void add_bar( DraggableItemCondiction pos, float& percentage, float max, ImColor color, ImColor color1, ImColor color2, ESPPlayerData_t* m_Data );
	void add_text( std::string text, DraggableItemCondiction pos, ImColor color, ESPPlayerData_t* m_Data );
	void add_box( ESPPlayerData_t* m_Data, ImColor outer, ImColor inner, ImColor out );

	std::array < ESPPlayerData_t, 64 > m_PlayerData;
};

#define player_esp c_player_esp::instance()
