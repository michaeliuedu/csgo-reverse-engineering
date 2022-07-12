#pragma once

class i_localize {
public:
	wchar_t* find(const char* token_name) {
		using original_fn = wchar_t* (__thiscall*)(i_localize*, const char*);
		return (*(original_fn**)this)[12](this, token_name);
	}

	const char* findutf8(const char* token_name) {
		using original_fn = const char* (__thiscall*)(i_localize*, const char*);
		return (*(original_fn**)this)[47](this, token_name);
	}
};