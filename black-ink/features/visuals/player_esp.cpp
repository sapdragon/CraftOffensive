#include "../features.h"

constexpr float SPEED_FREQ = 255 / 4.0f;

void VectorTransform( const vec3_t& in1, const matrix3x4_t& in2, vec3_t& out )
{
	out[ 0 ] = in1.dot_product( in2[ 0 ] ) + in2[ 0 ][ 3 ];
	out[ 1 ] = in1.dot_product( in2[ 1 ] ) + in2[ 1 ][ 3 ];
	out[ 2 ] = in1.dot_product( in2[ 2 ] ) + in2[ 2 ][ 3 ];
}

RECT GetBBox( c_base_entity* pEntity ) 
{
	auto pCollideable = pEntity->get_collideable( );
	if ( !pCollideable )
		return { };

	vec3_t vecMins = pCollideable->obb_mins( );
	vec3_t vecMaxs = pCollideable->obb_maxs( );

	const matrix3x4_t& m_Transformation = pEntity->get_rgfl_coordinate_frame( );
	std::array < vec3_t, 8 > m_Points
		=
	{
		vec3_t( vecMins.x, vecMins.y, vecMins.z ),
		vec3_t( vecMins.x, vecMaxs.y, vecMins.z ),
		vec3_t( vecMaxs.x, vecMaxs.y, vecMins.z ),
		vec3_t( vecMaxs.x, vecMins.y, vecMins.z ),
		vec3_t( vecMaxs.x, vecMaxs.y, vecMaxs.z ),
		vec3_t( vecMins.x, vecMaxs.y, vecMaxs.z ),
		vec3_t( vecMins.x, vecMins.y, vecMaxs.z ),
		vec3_t( vecMaxs.x, vecMins.y, vecMaxs.z )
	};

	std::array < vec3_t, 8 > m_PointsTransformated;
	for ( int nVector = 0; nVector < 8; nVector++ )
		VectorTransform( m_Points[ nVector ], m_Transformation, m_PointsTransformated[ nVector ] );

	std::array < vec3_t, 8 > m_ScreenPoints;
	for ( int nVector = 0; nVector < 8; nVector++ )
	{
		if ( !render::world_to_screen( m_PointsTransformated[ nVector ], m_ScreenPoints[ nVector ] ) )
			return { };
	}

	float flLeft = m_ScreenPoints[ 0 ].x;
	float flTop = m_ScreenPoints[ 0 ].y;
	float flRight = m_ScreenPoints[ 0 ].x;
	float flBottom = m_ScreenPoints[ 0 ].y;

	for ( int nID = 1; nID < 8; nID++ )
	{
		if ( flLeft > m_ScreenPoints[ nID ].x )
			flLeft = m_ScreenPoints[ nID ].x;

		if ( flTop < m_ScreenPoints[ nID ].y )
			flTop = m_ScreenPoints[ nID ].y;

		if ( flRight < m_ScreenPoints[ nID ].x )
			flRight = m_ScreenPoints[ nID ].x;

		if ( flBottom > m_ScreenPoints[ nID ].y )
			flBottom = m_ScreenPoints[ nID ].y;
	}

	return RECT{ ( LONG ) ( flLeft ), ( LONG ) ( flBottom ), ( LONG ) ( flRight ), ( LONG ) ( flTop ) };
}

void c_player_esp::on_paint() {
	for (auto i = 1; i < interfaces::m_global_vars->m_max_clients; i++) {
		const auto player = static_cast<c_cs_player*>(interfaces::m_entity_list->get_client_entity(i));

		ESPPlayerData_t* m_Data = &m_PlayerData[ i ];
		if ( !m_Data )
			continue;

		if (!player || !player->is_alive())
			continue;

		if (!globals::m_local)
			continue;

		bool is_team = player->get_team() == globals::m_local->get_team();

		if (player->get_origin() == vec3_t(0, 0, 0))
			continue;

		m_Data->m_aBBox = GetBBox( player );
		m_Data->m_iWidth = abs( m_Data->m_aBBox.right - m_Data->m_aBBox.left );
		m_Data->m_iHeight = abs( m_Data->m_aBBox.top - m_Data->m_aBBox.bottom );

		render_player_draggable( player_esp_preview, player, m_Data );
	}
}

ImVec2 c_player_esp::get_position( DraggableItemCondiction pos, ESPPlayerData_t* m_Data ) {
	if ( pos == RIGHT_COND )
		return ImVec2( m_Data->m_aBBox.right + 2 + m_Data->m_iRightOffset, m_Data->m_aBBox.top + m_Data->m_iRightDownOffset );
	if ( pos == LEFT_COND )
		return ImVec2( m_Data->m_aBBox.left - 2 - m_Data->m_iLeftOffset, m_Data->m_aBBox.top + m_Data->m_iLeftDownOffset );
	if ( pos == BOT_COND )
		return ImVec2( m_Data->m_aBBox.left + ( ( m_Data->m_aBBox.right - m_Data->m_aBBox.left ) * 0.5f ), m_Data->m_aBBox.bottom + 2 + m_Data->m_iDownOffset );
	if ( pos == TOP_COND )
		return ImVec2( m_Data->m_aBBox.left + ( ( m_Data->m_aBBox.right - m_Data->m_aBBox.left ) * 0.5f ), m_Data->m_aBBox.top - m_Data->m_iUpOffset );
}

ImVec2 c_player_esp::get_position_offsetless( DraggableItemCondiction pos, ESPPlayerData_t* m_Data ) {
	if ( pos == RIGHT_COND )
		return ImVec2( m_Data->m_aBBox.right + 3 + m_Data->m_iRightOffset, m_Data->m_aBBox.top );
	if ( pos == LEFT_COND )
		return ImVec2( m_Data->m_aBBox.left - 3 - m_Data->m_iLeftOffset, m_Data->m_aBBox.top );
	if ( pos == BOT_COND )
		return ImVec2( m_Data->m_aBBox.left, m_Data->m_aBBox.bottom + 2 );
	if ( pos == TOP_COND )
		return ImVec2( m_Data->m_aBBox.left, m_Data->m_aBBox.top - 4 );
}

__forceinline ImColor toColor( col_t color )
{
	return ImColor( color.r( ), color.g( ), color.b( ), color.a( ) );
}

void c_player_esp::add_text( std::string text, DraggableItemCondiction pos, ImColor color, ESPPlayerData_t* m_Data ) {
	ImVec2 ImTextSize = ImGui::CalcTextSize( text.c_str( ) );
	ImVec2 Position = get_position( pos, m_Data );

	if ( pos == LEFT_COND )
		Position = Position - ImVec2( ImTextSize.x + 4, 0 );
	if ( pos == RIGHT_COND )
		Position = Position + ImVec2( 4, 0 );
	if ( pos == BOT_COND )
		Position = Position - ImVec2( ImTextSize.x / 2, 0 );
	if ( pos == TOP_COND )
		Position = Position - ImVec2( ImTextSize.x / 2, ImTextSize.y );

	render::text( text, vec2_t(Position.x, Position.y), col_t( color.Value.x * 255, color.Value.y * 255, color.Value.z * 255 ), fonts::m_minecraft14 );

	if ( pos == RIGHT_COND )
		m_Data->m_iRightDownOffset = m_Data->m_iRightDownOffset + ImTextSize.y;
	if ( pos == LEFT_COND )
		m_Data->m_iLeftDownOffset = m_Data->m_iLeftDownOffset + ImTextSize.y;
	if ( pos == BOT_COND )
		m_Data->m_iDownOffset = m_Data->m_iDownOffset + ImTextSize.y;
	if ( pos == TOP_COND )
		m_Data->m_iUpOffset = m_Data->m_iUpOffset + ImTextSize.y;
}

void c_player_esp::add_bar( DraggableItemCondiction pos, float& percentage, float max, ImColor color, ImColor color1, ImColor color2, ESPPlayerData_t* m_Data ) {

	ImVec2 Position = get_position_offsetless( pos, m_Data );
	int XOffset, X2Offset;
	int YOffset, Y2Offset;

	auto widthSides = abs( m_Data->m_aBBox.bottom - m_Data->m_aBBox.top ) - ( ( ( abs( m_Data->m_aBBox.bottom - m_Data->m_aBBox.top ) * percentage ) / max ) );
	auto widthUpDown = abs( m_Data->m_aBBox.left - m_Data->m_aBBox.right ) - ( ( ( abs( m_Data->m_aBBox.left - m_Data->m_aBBox.right ) * percentage ) / max ) );

	if ( pos == RIGHT_COND ) {
		XOffset = 1 + m_Data->m_iRightOffset;
		X2Offset = 3 + m_Data->m_iRightOffset;
		YOffset = 1;
		Y2Offset = -1 + m_Data->m_iHeight;
	}

	if ( pos == LEFT_COND ) {
		XOffset = -1 - m_Data->m_iLeftOffset;
		X2Offset = -3 - m_Data->m_iLeftOffset;
		YOffset = 1;
		Y2Offset = -1 + m_Data->m_iHeight;
	}

	if ( pos == BOT_COND || pos == TOP_COND ) {
		XOffset = 1;
		X2Offset = m_Data->m_iWidth - 1;
		YOffset = m_Data->m_iDownOffset;
		Y2Offset = 2 + m_Data->m_iDownOffset;
	}

	render::rect_angle(
		( float ) Position.x + XOffset - 1 + ( pos == LEFT_COND ? 1 : 0 ), ( float ) Position.y + YOffset - 1,
		( float ) Position.x + X2Offset + 1 - ( pos == LEFT_COND ? 1 : 0 ), ( float ) Position.y + Y2Offset + 1,
		col_t( 
			( int ) ( color2.Value.x * 255 ),
			( int ) ( color2.Value.y * 255 ), 
			( int ) ( color2.Value.z * 255 ),
			255
		), 0, 0
	);

	render::rect_filled_int(
		( float ) Position.x + XOffset - 1, ( float ) Position.y + YOffset - 1,
		( float ) Position.x + X2Offset + 1, ( float ) Position.y + Y2Offset + 1,
		col_t(
			( int ) ( color1.Value.x * 255 ),
			( int ) ( color1.Value.y * 255 ),
			( int ) ( color1.Value.z * 255 ),
			255
		)
	);

	render::rect_filled_int(
		( float ) Position.x + XOffset, ( float ) Position.y + YOffset + ( pos == LEFT_COND || pos == RIGHT_COND ? widthSides : 0 ),
		( float ) Position.x + X2Offset - ( pos == BOT_COND || pos == TOP_COND ? widthUpDown : 0 ), ( float ) Position.y + Y2Offset,
		col_t(
			( int ) ( color.Value.x * 255 ),
			( int ) ( color.Value.y * 255 ),
			( int ) ( color.Value.z * 255 ),
			255
		)
	);

	if ( pos == RIGHT_COND )
		m_Data->m_iRightOffset = m_Data->m_iRightOffset + 3;
	if ( pos == LEFT_COND )
		m_Data->m_iLeftOffset = m_Data->m_iLeftOffset + 3;
	if ( pos == BOT_COND )
		m_Data->m_iDownOffset = m_Data->m_iDownOffset + 5;
	if ( pos == TOP_COND )
		m_Data->m_iUpOffset = m_Data->m_iUpOffset + 5;
}

void c_player_esp::add_box( ESPPlayerData_t* m_Data )
{
	render::rect_angle(
		m_Data->m_aBBox.left, m_Data->m_aBBox.top,
		m_Data->m_aBBox.right, m_Data->m_aBBox.bottom,
		col_t(
			64, 64, 64, 255
		), 1 , 0
	);

	render::rect_angle(
		m_Data->m_aBBox.left + 1,  m_Data->m_aBBox.top + 1,
		m_Data->m_aBBox.right - 1, m_Data->m_aBBox.bottom - 1,
		col_t(
			255, 255, 255, 255
		), 1, 0
	);

	render::rect_angle(
		m_Data->m_aBBox.left + 2, m_Data->m_aBBox.top + 2,
		m_Data->m_aBBox.right - 2, m_Data->m_aBBox.bottom - 2,
		col_t(
			64, 64, 64, 255
		), 1, 0
	);
}

void c_player_esp::render_player_draggable( c_esp_preview* preview, c_cs_player* player, ESPPlayerData_t* m_Data )
{
	for ( auto a = 0; a < POOL_COND; a++ )
	{
		m_Data->m_iDownOffset = 0;
		m_Data->m_iUpOffset = 0;

		m_Data->m_iLeftDownOffset = 0;
		m_Data->m_iLeftOffset = 0;

		m_Data->m_iRightDownOffset = 0;
		m_Data->m_iRightOffset = 0;

		for ( auto b = 0; b < preview->draggable_items[ a ].size( ); b++ )
		{
			if ( preview->draggable_items[ a ][ b ].Type == 0 ) {
				std::string Text;
				ImColor Color;

				if ( preview->draggable_items[ a ][ b ].ItemName == "Username" ) {
					player_info_t info;
					interfaces::m_engine->get_player_info( player->get_index( ), &info );

					Text = info.m_name;
					if ( Text.length( ) > 32 )
					{
						Text.erase( 32, Text.length( ) - 32 );
						Text.append( "..." );
					}
		
					Color = ImColor(255, 255, 255);
				}

				add_text( Text, ( DraggableItemCondiction ) a, Color, m_Data );
			}
			if ( preview->draggable_items[ a ][ b ].Type == 1 ) {
				ImColor Main;
				ImColor Inner;
				ImColor Outer;

				if ( preview->draggable_items[ a ][ b ].ItemName == "Health" ) {
					auto iHealthValue = std::clamp( player->get_health( ), 0, 100 );
					auto percentage = iHealthValue / 100.f;
					if ( m_Data->m_flPrevHealth > iHealthValue )
						m_Data->m_flPrevHealth -= SPEED_FREQ * interfaces::m_global_vars->m_frame_time;
					else
						m_Data->m_flPrevHealth = iHealthValue;

					Main = toColor(cfg::get<col_t>( FNV1A( "esp.enemies.health.color" )));
					Outer = toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.border.outside.color" ) ) );
					Inner = toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.border.inside.color" ) ) );

					add_bar( ( DraggableItemCondiction ) a, m_Data->m_flPrevHealth, 100, Main, Inner, Outer, m_Data );
				}

				if ( preview->draggable_items[ a ][ b ].ItemName == "Armor" ) {
					auto iArmorValue = std::clamp( player->get_armor_value( ), 0, 100 );
					if ( m_Data->m_flPrevArmor > iArmorValue )
						m_Data->m_flPrevArmor -= SPEED_FREQ * interfaces::m_global_vars->m_frame_time;
					else
						m_Data->m_flPrevArmor = iArmorValue;

					Main = toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.color" ) ) );
					Outer = toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.border.outside.color" ) ) );
					Inner = toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.border.inside.color" ) ) );

					add_bar( ( DraggableItemCondiction ) a, m_Data->m_flPrevArmor, 100, Main, Inner, Outer, m_Data );
				}
			}
			if ( preview->draggable_items[ a ][ b ].Type == 2 ) {
					add_box( m_Data );
			}
		}
	}
}