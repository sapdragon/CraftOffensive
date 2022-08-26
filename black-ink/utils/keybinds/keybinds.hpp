#pragma once

#include <any>
#include <mutex>
#include <string>
#include <vector>
#include <functional>

#include <Windows.h>

enum KeyActionType : int {
	KEY_SWAP,
	KEY_HOLD,
};

enum KeyVariableType : int {
	INTEGER_BIND,
	FLOAT_BIND,
	BOOLEAN_BIND,
};

struct keybind_data {
	int key_action;
	int key_variable;

	uint32_t hash;

	std::any oldValue;
	std::any newValue;

	int key_code;
	int status;
};

class c_key_binds {
public:
	void handle_messages( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param );

	std::vector<keybind_data> keybind_list;
private:
	void handle_booleans( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param );
	void handle_integers( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param );
	void handle_floats( HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param );
};

inline c_key_binds* keybind_manager = new c_key_binds( );
