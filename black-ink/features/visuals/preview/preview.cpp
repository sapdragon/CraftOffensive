#include "preview.hpp"
#include "preview_draggables.hpp"

bool c_esp_preview::handle( movable_item& Item ) {
	ImGuiWindow* window = ImGui::GetCurrentWindow( );
	ImGuiID id = window->GetID( Item.m_name ); // use the pointer address as identifier
	ImGuiButtonFlags flags = 0;

	ImGuiContext& g = *GImGui;
	ImGuiStyle& style = g.Style;

	ImRect rect = ImRect( ImGui::GetWindowPos( ).x + Item.m_draw_position.x,
		ImGui::GetWindowPos( ).y + Item.m_draw_position.y,
		ImGui::GetWindowPos( ).x + Item.m_draw_position.x + Item.m_size.x,
		ImGui::GetWindowPos( ).y + Item.m_draw_position.y + Item.m_size.y );

	ImGui::ItemSize( rect, style.FramePadding.y );
	if ( !ImGui::ItemAdd( rect, id ) )
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior( rect, id, &hovered, &held, flags );
	bool is_active = ImGui::IsItemActive( );

	ImGuiCol handle_color = ImColor( ImGui::GetStyle( ).Colors[ ImGuiCol_ButtonHovered ] );
	ImDrawList* draw_list = ImGui::GetWindowDrawList( );

	if ( this->m_draw_type == 0 )
		EnemyPreview::GetDrawableEnemies( ImGui::IsMouseClicked( ImGuiMouseButton_Right ) && ImGui::IsMouseHoveringRect( rect.Min, rect.Max ), draw_list, ImGui::GetWindowPos( ) + Item.m_draw_position, Item.m_size, Item.m_condition, Item.m_draw );
		if ( this->m_draw_type == 1 )
		TeamPreview::GetDrawableTeammates( ImGui::IsMouseClicked( ImGuiMouseButton_Right ) && ImGui::IsMouseHoveringRect( rect.Min, rect.Max ), draw_list, ImGui::GetWindowPos( ) + Item.m_draw_position, Item.m_size, Item.m_condition, Item.m_draw );
	
	if ( is_active && ImGui::IsMouseDragging( 0 ) ) {
		Item.m_draw_position[ 0 ] += ImGui::GetIO( ).MouseDelta.x;
		Item.m_draw_position[ 1 ] += ImGui::GetIO( ).MouseDelta.y;
	}

	return held;
}

bool c_esp_preview::mouse_intersect_rect( ImVec2 pos1, ImVec2 pos2 ) {
	if ( ImGui::GetIO( ).MousePos.x >= ImGui::GetWindowPos( ).x + pos1.x &&
		ImGui::GetIO( ).MousePos.y >= ImGui::GetWindowPos( ).y + pos1.y &&
		ImGui::GetIO( ).MousePos.x <= ImGui::GetWindowPos( ).x + pos2.x &&
		ImGui::GetIO( ).MousePos.y <= ImGui::GetWindowPos( ).y + pos2.y )
		return true;

	return false;
}

void c_esp_preview::swap_enable_item( const char* name ) {
	for ( auto a = 0; a < 5; a++ ) {
		for ( size_t i = 0; i < items[ a ].size( ); i++ )
		{
			if ( items[ a ][ i ].m_name == name ) {
				items[ a ][ i ].m_enabled = !items[ a ][ i ].m_enabled;
			}
		}
	}
}

bool c_esp_preview::get_value( const char* name ) {
	for ( auto a = 0; a < 5; a++ ) {
		for ( size_t i = 0; i < items[ a ].size( ); i++ )
		{
			if ( items[ a ][ i ].m_name == name ) {
				return items[ a ][ i ].m_enabled;
			}
		}
	}
}

void c_esp_preview::recalculate( float animation ) {
	ImVec2 position_left = { 110, 70 }, position_right = { 215, 70 };
	ImVec2 position_top = { 110, 88 }, position_bot = { 110, 310 };

	for ( auto a = 0; a < 5; a++ ) 
	{
		for ( size_t i = 0; i < items[ a ].size( ); i++ )
		{
			if ( !items[ a ][ i ].m_enabled ) {
				continue;
			}

			if ( items[ a ][ i ].m_condition == CENTER_COND ) {
				items[ a ][ i ].m_draw_position = ImLerp(
					items[ a ][ i ].m_draw_position,
					ImVec2( 110, 90 ),
					animation
				);
			}
			if ( items[ a ][ i ].m_condition == LEFT_COND ) {
				if ( items[ a ][ i ].m_name == "Health" || items[ a ][ i ].m_name == "Armor" ) {
					items[ a ][ i ].m_draw_position = ImLerp(
						items[ a ][ i ].m_draw_position,
						position_left + ImVec2( -6, 20 ),
						animation
					);
				}
				else {
					items[ a ][ i ].m_draw_position = ImLerp(
						items[ a ][ i ].m_draw_position,
						position_left + ImVec2( -items[ a ][ i ].m_size.x, items[ a ][ i ].m_size.y ),
						animation
					);
				}

				if ( items[ a ][ i ].m_name == "Health" || items[ a ][ i ].m_name == "Armor" ) {
					position_left += ImVec2( -6, 0 );
				}
				else {
					position_left += ImVec2( 0, items[ a ][ i ].m_size.y );
				}
			}
			if ( items[ a ][ i ].m_condition == RIGHT_COND ) {
				if ( items[ a ][ i ].m_name == "Health" || items[ a ][ i ].m_name == "Armor" ) {
					items[ a ][ i ].m_draw_position = ImLerp(
						items[ a ][ i ].m_draw_position,
						position_right + ImVec2( 6, 20 ),
						animation
					);
				}
				else {
					items[ a ][ i ].m_draw_position = ImLerp(
						items[ a ][ i ].m_draw_position,
						position_right + ImVec2( 0, items[ a ][ i ].m_size.y ),
						animation
					);
				}

				if ( items[ a ][ i ].m_name == "Health" || items[ a ][ i ].m_name == "Armor" ) {
					position_right += ImVec2( 6, 0 );
				}
				else {
					position_right += ImVec2( 0, items[ a ][ i ].m_size.y );
				}
			}
			if ( items[ a ][ i ].m_condition == TOP_COND ) {
				items[ a ][ i ].m_draw_position = ImLerp(
					items[ a ][ i ].m_draw_position,
					position_top + ImVec2( 0, -items[ a ][ i ].m_size.y ),
					animation
				);

				position_top += ImVec2( 0, -items[ a ][ i ].m_size.y );
			}
			if ( items[ a ][ i ].m_condition == BOT_COND ) {
				items[ a ][ i ].m_draw_position = ImLerp(
					items[ a ][ i ].m_draw_position,
					position_bot + ImVec2( 0, +items[ a ][ i ].m_size.y ),
					animation
				);

				position_bot += ImVec2( 0, items[ a ][ i ].m_size.y );
			}
		}
	}
}

int c_esp_preview::calc_items( int cond ) {
	return items[ cond ].size( );
}

void c_esp_preview::move( int pos, int index, int destination ) {
	movable_item temporaty = items[ pos ][ index ];
	items[ pos ].erase( items[ pos ].begin( ) + index );
	items[ destination ].emplace_back( temporaty );
}

void c_esp_preview::move_first( int pos, int index, int destination ) {
	movable_item temporaty = items[ pos ][ index ];
	items[ pos ].erase( items[ pos ].begin( ) + index );
	items[ destination ].insert( items[ destination ].begin(), temporaty );
}

void c_esp_preview::instance( ) {
	for ( size_t i = 0; i < 5; i++ )
	{
		for ( auto a = 0; a < items[ i ].size( ); a++ )
		{
			if ( !items[ i ][ a ].m_enabled ) {
				continue;
			}

			
			if ( handle( items[ i ][ a ] ) ) {
				m_is_dragging = true;
				m_anim_flag = true;

				if ( mouse_intersect_rect( ImVec2( 0, 90 ), ImVec2( 110, 490 - 80 ) ) ) {
					if ( items[ i ][ a ].m_name != "Box" && calc_items( LEFT_COND ) < 6 ) {
						items[ i ][ a ].m_condition = LEFT_COND;

						if ( items[ i ][ a ].m_name == "Health" || items[ i ][ a ].m_name == "Armor" )
							move_first( i, a, LEFT_COND );
						else
							move( i, a, LEFT_COND );
					}
				}
				else if ( mouse_intersect_rect( ImVec2( 220, 90 ), ImVec2( 330, 490 - 80 ) ) ) {
					if ( items[ i ][ a ].m_name != "Box" && calc_items( RIGHT_COND ) < 6 ) {
						items[ i ][ a ].m_condition = RIGHT_COND;

						if ( items[ i ][ a ].m_name == "Health" || items[ i ][ a ].m_name == "Armor" )
							move_first( i, a, RIGHT_COND );
						else 
							move( i, a, RIGHT_COND );
					}
				}
				else if ( mouse_intersect_rect( ImVec2( 110, 5 ), ImVec2( 220, 5 + 85 ) ) ) {
					if ( items[ i ][ a ].m_name != "Box" && calc_items( TOP_COND ) < 3 ) {
						items[ i ][ a ].m_condition = TOP_COND;
						move( i, a, TOP_COND );
					}
				}
				else if ( mouse_intersect_rect( ImVec2( 110, 400 - 80 ), ImVec2( 220, 400 ) ) ) {
					if ( items[ i ][ a ].m_name != "Box" && calc_items( BOT_COND ) < 3 ) {
						items[ i ][ a ].m_condition = BOT_COND;
						move( i, a, BOT_COND );
					}
				}
				else if ( mouse_intersect_rect( ImVec2( 110, 120 ), ImVec2( 220, 430 - 80 ) ) ) {
					if ( items[ i ][ a ].m_name == "Box" ) {
						items[ i ][ a ].m_condition = CENTER_COND;
						move( i, a, CENTER_COND );
					}
				}
			}
		}
	}

	if ( !ImGui::GetIO( ).MouseDown[ 0 ] )
		m_is_dragging = false;

	if ( !m_is_dragging ) {
		if ( m_anim_flag == true ) {
			m_animation = 0.f;
			m_anim_flag = false;
		}

		if ( m_animation < 1.f )
			m_animation += 0.005f * ( 1.f - ImGui::GetIO( ).DeltaTime );

		m_animation = std::clamp( m_animation, 0.f, 1.f );

		recalculate( m_animation );
	}
}
