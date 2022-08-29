#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "../../../globals.h"
#include "../../../utils/imgui/imgui.h"
#include "../../../utils/imgui/imgui_internal.h"

enum draggable_item_condiction : int {
	LEFT_COND = 0,
	RIGHT_COND = 1,
	TOP_COND = 2,
	BOT_COND = 3,
	CENTER_COND = 4,
};

struct movable_item {
	bool m_enabled = false;

	const char* m_name = "";
	ImVec2 m_size = {60, 20};
	ImVec2 m_draw_position = {};

	int m_draw = 0;
	int m_condition = LEFT_COND;
	int m_type = 0;
	int m_index = 0;

	movable_item(int drw, const char* name = "Default", int cond = 4, int type = 0 ) :
		m_draw(drw), m_name(name ), m_condition(cond ), m_type(type )
	{}
};

class c_esp_preview
{
public:
	c_esp_preview( int type, std::vector<movable_item> vec ) : m_draw_type( type )
	{
		for ( int i = 0; i < vec.size( ); i++ )
			items[ vec[ i ].m_condition ].push_back( vec[i] );
	};

public:
	void instance( );
	void swap_enable_item( const char* name );
	bool get_value( const char* name );

	std::map<int, std::vector<movable_item>> items;

	int calc_items( int cond );

private:
	bool handle( movable_item& Item );
	void recalculate( float animation );
	void move( int pos, int index, int destination );
	void move_first( int pos, int index, int destination );
	bool mouse_intersect_rect( ImVec2 pos1, ImVec2 pos2 );

	int m_draw_type;
	bool m_is_dragging;
	bool m_anim_flag;
	float m_animation;
};

inline c_esp_preview* player_esp_preview = new c_esp_preview( 0,
	std::vector<movable_item>{
		movable_item( 0, "Weapon", BOT_COND, 0 ),
		movable_item( 1, "Username", TOP_COND, 0 ),
		movable_item( 2, "Health", LEFT_COND, 1 ),
		movable_item( 3, "Armor", LEFT_COND, 1 ),
		movable_item( 4, "Box", CENTER_COND, 2 ),
	}
);

inline c_esp_preview* player_team_esp_preview = new c_esp_preview( 1,
	std::vector<movable_item>{
		movable_item( 0, "Weapon", BOT_COND, 0 ),
		movable_item( 1, "Username", TOP_COND, 0 ),
		movable_item( 2, "Health", LEFT_COND, 1 ),
		movable_item( 3, "Armor", LEFT_COND, 1 ),
		movable_item( 4, "Box", CENTER_COND, 2 ),
	}
);