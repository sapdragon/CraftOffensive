#include "keybinds.hpp"
#include "../../globals.h"

void c_key_binds::handle_messages( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param )
{
	handle_booleans( hwnd, msg, w_param, l_param );
	handle_integers( hwnd, msg, w_param, l_param );
	handle_floats( hwnd, msg, w_param, l_param );
}

void c_key_binds::handle_booleans( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param )
{
	DWORD dwParam = w_param;

	if ( GET_XBUTTON_WPARAM( w_param ) == XBUTTON1 )
		dwParam = VK_XBUTTON1;

	else if ( GET_XBUTTON_WPARAM( w_param ) == XBUTTON2 )
		dwParam = VK_XBUTTON2;

	for ( auto &keybind : keybind_list ) 
	{
		if ( keybind.key_variable != BOOLEAN_BIND )
			continue;

		if ( keybind.key_code == w_param || keybind.key_code == dwParam ) 
		{
			if ( keybind.key_action == KEY_SWAP ) 
			{
				if ( msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_XBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_LBUTTONDOWN )
				{
					if ( keybind.status == 0 ) {
						cfg::set<bool>( keybind.hash, std::any_cast< bool >( keybind.newValue ) );
						keybind.status = 1;
					}
					else {
						cfg::set<bool>( keybind.hash, std::any_cast< bool >( keybind.oldValue ) );
						keybind.status = 0;
					}

				}
			}
			else if ( keybind.key_action == KEY_HOLD ) 
			{
				if ( msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_XBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_LBUTTONDOWN )
				{
					cfg::set<bool>( keybind.hash, std::any_cast< bool >( keybind.newValue ) );
					keybind.status = 1;
				}

				if ( msg == WM_KEYUP || msg == WM_SYSKEYUP || msg == WM_XBUTTONUP || msg == WM_RBUTTONUP || msg == WM_LBUTTONUP )
				{
					cfg::set<bool>( keybind.hash, std::any_cast< bool >( keybind.oldValue ) );
					keybind.status = 0;
				}
			}
		}
	}
}

void c_key_binds::handle_integers( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param )
{
	DWORD dwParam = w_param;

	if ( GET_XBUTTON_WPARAM( w_param ) == XBUTTON1 )
		dwParam = VK_XBUTTON1;

	else if ( GET_XBUTTON_WPARAM( w_param ) == XBUTTON2 )
		dwParam = VK_XBUTTON2;

	for ( auto& keybind : keybind_list )
	{
		if ( keybind.key_variable != INTEGER_BIND )
			continue;

		if ( keybind.key_code == w_param || keybind.key_code == dwParam )
		{
			if ( keybind.key_action == KEY_SWAP )
			{
				if ( msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_XBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_LBUTTONDOWN )
				{
					if ( keybind.status == 0 ) {
						cfg::set<int>( keybind.hash, std::any_cast< int >( keybind.newValue ) );
						keybind.status = 1;
					}
					else {
						cfg::set<int>( keybind.hash, std::any_cast< int >( keybind.oldValue ) );
						keybind.status = 0;
					}

				}
			}
			else if ( keybind.key_action == KEY_HOLD )
			{
				if ( msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_XBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_LBUTTONDOWN )
				{
					cfg::set<int>( keybind.hash, std::any_cast< int >( keybind.newValue ) );
					keybind.status = 1;
				}

				if ( msg == WM_KEYUP || msg == WM_SYSKEYUP || msg == WM_XBUTTONUP || msg == WM_RBUTTONUP || msg == WM_LBUTTONUP )
				{
					cfg::set<int>( keybind.hash, std::any_cast< int >( keybind.oldValue ) );
					keybind.status = 0;
				}
			}
		}
	}
}

void c_key_binds::handle_floats( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param )
{
	DWORD dwParam = w_param;

	if ( GET_XBUTTON_WPARAM( w_param ) == XBUTTON1 )
		dwParam = VK_XBUTTON1;

	else if ( GET_XBUTTON_WPARAM( w_param ) == XBUTTON2 )
		dwParam = VK_XBUTTON2;

	for ( auto& keybind : keybind_list )
	{
		if ( keybind.key_variable != FLOAT_BIND )
			continue;

		if ( keybind.key_code == w_param || keybind.key_code == dwParam )
		{
			if ( keybind.key_action == KEY_SWAP )
			{
				if ( msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_XBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_LBUTTONDOWN )
				{
					if ( keybind.status == 0 ) {
						cfg::set<float>( keybind.hash, std::any_cast< float >( keybind.newValue ) );
						keybind.status = 1;
					}
					else {
						cfg::set<float>( keybind.hash, std::any_cast< float >( keybind.oldValue ) );
						keybind.status = 0;
					}

				}
			}
			else if ( keybind.key_action == KEY_HOLD )
			{
				if ( msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_XBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_LBUTTONDOWN )
				{
					cfg::set<float>( keybind.hash, std::any_cast< float >( keybind.newValue ) );
					keybind.status = 1;
				}

				if ( msg == WM_KEYUP || msg == WM_SYSKEYUP || msg == WM_XBUTTONUP || msg == WM_RBUTTONUP || msg == WM_LBUTTONUP )
				{
					cfg::set<float>( keybind.hash, std::any_cast< float >( keybind.oldValue ) );
					keybind.status = 0;
				}
			}
		}
	}
}
