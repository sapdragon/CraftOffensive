#include "../events.h"


void events::item_equip( i_game_event* event )
{
	dormant->on_item_equip( event );
}
