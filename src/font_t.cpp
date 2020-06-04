#include "font_t.h"

font_t::font_t() :
	_font_ptr(std::make_shared<sf::Font>()) {}

font_t::font_t(const font_t& other) :
	_font_ptr(other._font_ptr) {}

font_t::font_t(const char* filename) :
	_font_ptr(std::make_shared<sf::Font>())
{
	_font_ptr->loadFromFile(filename);
}

font_t::font_t(const void* buf, size_t len) :
	_font_ptr(std::make_shared<sf::Font>())
{
	_font_ptr->loadFromMemory(buf, len);
}

font_t& font_t::font_t::operator=(const font_t& other) {
	_font_ptr = other._font_ptr;
	return *this;
}

sf::Font& font_t::operator*() {
	return *_font_ptr;
}

const sf::Font& font_t::operator*() const {
	return *_font_ptr;
}

bool font_t::loadFromFile(const char* filename) {
	return _font_ptr->loadFromFile(filename);
}
