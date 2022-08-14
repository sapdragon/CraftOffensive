#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include "../common_includes.h"
#include "../sdk/definitions.h"

#include "math/datatypes.h"
#include "math/math.h"

#include "obfuscation/fnv1a.h"
#include "obfuscation/xor.h"

#include "memory/memory.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_freetype.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "render/render.h"

#include "netvars/data_map.h"
#include "netvars/netvars.h"

#include "input/input.h"

#include "cfg/cfg.h"

#include "minhook/minhook.h"

namespace utils {
	__forceinline std::string to_utf8(std::wstring_view txt) {
		if (txt.empty())
			return "";

		const auto size = WideCharToMultiByte(CP_UTF8, 0, txt.data(), txt.size(), 0, 0, 0, 0);

		auto ret = std::string(size, 0);

		WideCharToMultiByte(CP_UTF8, 0, txt.data(), txt.size(), ret.data(), size, 0, 0);

		return ret;
	}

	__forceinline std::wstring to_unicode(std::string_view txt) {
		if (txt.empty())
			return L"";

		const auto size = MultiByteToWideChar(CP_UTF8, 0, txt.data(), txt.size(), 0, 0);

		auto ret = std::wstring(size, 0);

		MultiByteToWideChar(CP_UTF8, 0, txt.data(), txt.size(), ret.data(), size);

		return ret;
	}

	__forceinline uint8_t* find_pattern_from_module(HMODULE module, std::string strByteArray) {
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(strByteArray.c_str());
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return &scanBytes[i];
			}
		}
		return nullptr;
	}
}
