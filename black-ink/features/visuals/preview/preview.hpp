#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "../../../globals.h"
#include "../../../utils/imgui/imgui.h"
#include "../../../utils/imgui/imgui_internal.h"

enum DraggableItemCondiction : int {
	LEFT_COND = 0,
	RIGHT_COND = 1,
	TOP_COND = 2,
	BOT_COND = 3,
	CENTER_COND = 4,
	POOL_COND = 5,
	IN_MOVE_COND = 6,
};

struct MovableItems {
	std::string	ItemName;
	ImVec2 TemporaryPos = ImVec2( );
	ImVec2 BasicPositions = ImVec2( );
	ImVec2 WidgetSize = ImVec2( );

	int Draw;
	int VectorCond = 4;

	int Type = 0; // 0 - text, 1 - bar, 2 - box

	MovableItems(
		int drw,
		std::string name = "Default",
		int cond = 4,
		ImVec2 temp1 = ImVec2{ },
		ImVec2 temp2 = ImVec2{ }, int type = 0 )
	{
		Draw = drw;
		ItemName = name;
		VectorCond = cond;
		TemporaryPos = temp1;
		BasicPositions = temp2;
		Type = type;
	}
};

class c_esp_preview
{
public:
	c_esp_preview( int type, std::vector<MovableItems> items ) : draw_type(type) {
		draggable_items[ 5 ].insert( draggable_items[5].end( ), items.begin( ), items.end( ) );
	};

public:
	void instance( );

	std::vector<std::vector<MovableItems>> draggable_items = {
		std::vector<MovableItems>{}, // Left 0 
		std::vector<MovableItems>{}, // Right 1
		std::vector<MovableItems>{}, // Top 2
		std::vector<MovableItems>{}, // Bot 3
		std::vector<MovableItems>{}, // Center 4
		std::vector<MovableItems>{}, // Pool 5
		std::vector<MovableItems>{}, // InMove 6
	};

private:
	bool item_in_move( MovableItems Item );
	void vector_to_vector( MovableItems Item, int Destination );
	void vector_to_vector_indexed( MovableItems Item, int Destination, int Index );
	bool mouse_intersect_rect( ImVec2 pos1, ImVec2 pos2 );

	void recalculate_pool( float animation );
	void recalculate_sides( float animation );
	void recalculate_top_bot( float animation );

	std::tuple<int, int> get_movable_item_position( MovableItems Item );
	std::tuple<int, int> get_movable_item_position( std::string name );

	bool handle( MovableItems& Item );

	int draw_type;

	bool isMouseInAction = false;
	float RecalculateAnimation = 0.f;
	bool RecalculateAnimationFlag = false;
};

inline c_esp_preview* player_esp_preview = new c_esp_preview( 0, std::vector<MovableItems>{
		MovableItems( 0, "Weapon", POOL_COND, { 10, 440}, { 10, 440 }, 0 ),
		MovableItems( 1, "Username" , POOL_COND, { 70, 440 }, { 70, 440 }, 0 ),
		MovableItems( 2, "Health" , POOL_COND, { 150, 440 }, { 150, 440 }, 1 ),
		MovableItems( 3, "Armor" , POOL_COND, { 210, 440 }, { 210, 440 }, 1 ),
		MovableItems( 4, "Box" , POOL_COND, { 260, 440 }, { 260, 440 }, 2 ),
	}
);