#include "TextEntry.h"

void TextEntry::Insert(char payload) {
	if (str.length() < max_length) {
		if (pos == str.length())
			str.append(1, payload);
		else
			str.insert(pos, 1, payload);

		pos = pos + 1;
	}
}

void TextEntry::Backspace() {
	if (str.length() > 0)
		if (pos > 0)
			str.erase(--pos, 1);
}

void TextEntry::Delete() {
	if (str.length() > 0) {
		if (pos >= 0)
			str.erase(pos, 1);

		if (pos > str.length())
			pos = pos - 1;
	}
}

void TextEntry::PositionStart() {
	pos = 0;
}

void TextEntry::PositionEnd() {
	pos = str.length() > 0 ? str.length() : 0;
}

void TextEntry::PositionForward() {
	if (str.length() > 0 && pos < str.length())
		pos = pos + 1;
}

void TextEntry::PositionBack() {
	if (pos > 0)
		pos = pos - 1;
}

int_t TextEntry::ToInteger() const {
	return str.length() > 0 ? std::stoll(str) : 0LL;
}
