#pragma once
#include "types.h"
#include <string>

struct TextEntry {
	static const int DEFAULT_MAX_LENGTH = 15;

	std::string str;
	int pos = 0;
	int max_length = DEFAULT_MAX_LENGTH;

	void Insert(char payload);
	void Backspace();
	void Delete();
	void PositionStart();
	void PositionEnd();
	void PositionForward();
	void PositionBack();
	int_t ToInteger() const;
};
