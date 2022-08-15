#include "../events.h"


void events::round_start( i_game_event* event )
{
	dormant->reset_data( );
}
