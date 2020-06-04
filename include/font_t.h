#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

class font_t {
private:
	std::shared_ptr<sf::Font> _font_ptr;
public:
	font_t();
	font_t(const font_t& other);
	font_t(const char* filename);
	font_t(const void* buf, size_t len);

	virtual ~font_t() = default;

	font_t& operator=(const font_t& other);
	sf::Font& operator*();
	const sf::Font& operator*() const;

	bool loadFromFile(const char* filename);
};
