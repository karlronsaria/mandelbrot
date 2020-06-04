#include "MenuBox.h"

float MenuBox::INIT_X_POS = 15.f;
float MenuBox::INIT_Y_POS = 15.f;
float MenuBox::ITEM_HEIGHT = 20.f;
float MenuBox::ITEM_WIDTH = 220.f;
int MenuBox::DEFAULT_POINT = 15;
sf::Color MenuBox::DEFAULT_FILL_COLOR(255, 255, 255, 192);

void MenuBox::add_text_item(font_t& myFont, int point, const sf::Color& fill, float x, float y) {
	_labels.push_back(NewTextItem(myFont, point, fill, x, y));
}

void MenuBox::add_text_item(float x, float y) {
	add_text_item(
		_font,
		MenuBox::DEFAULT_POINT,
		MenuBox::DEFAULT_FILL_COLOR,
		x, y
	);
}

void MenuBox::add_text_item(size_t count) {
	add_text_item(MenuBox::INIT_X_POS, MenuBox::next_y_pos(count));
}

float MenuBox::next_x_pos(int count) {
	return NextPosition(MenuBox::INIT_X_POS, MenuBox::ITEM_WIDTH, count);
}

float MenuBox::next_y_pos(int count) {
	return NextPosition(MenuBox::INIT_Y_POS, MenuBox::ITEM_HEIGHT, count);
}

MenuBox::MenuBox(const font_t& font) :
	_font(font) {}

MenuBox::~MenuBox() {}

void MenuBox::draw_to(target_t& window) {
	for (auto item : _labels) {
		window.draw(item);
	}
}

sf::Text MenuBox::NewTextItem(font_t& myFont, int point, const sf::Color& fill, float x, float y) {
	sf::Text item;
	item.setFont(*myFont);
	item.setCharacterSize(point);
	item.setFillColor(fill);
	item.setPosition({ x, y });
	return std::move(item);
}

float MenuBox::NextPosition(float init, float len, int count) {
	return init + count * len;
}
