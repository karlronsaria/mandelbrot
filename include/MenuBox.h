#pragma once
#include "Entity.h"
#include "font_t.h"

class MenuBox: public Entity {
public:
	static const char* FONT_FILE_PATH;
protected:
	static float INIT_X_POS;
	static float INIT_Y_POS;

	static float ITEM_HEIGHT;
	static float ITEM_WIDTH;
	static int DEFAULT_POINT;
	static sf::Color DEFAULT_FILL_COLOR;

	font_t _font;
	std::vector<sf::Text> _labels;

	void add_text_item(font_t& myFont, int point, const sf::Color& fill, float x, float y);
	void add_text_item(float x, float y);
	void add_text_item(size_t count);
	static float next_x_pos(int count);
	static float next_y_pos(int count);
public:
	MenuBox(const font_t&);
	MenuBox(const MenuBox&) = default; // = delete;
	MenuBox& operator=(const MenuBox&) = default; // = delete;
	virtual ~MenuBox();
	virtual int update(canvas_t&) = 0;
	virtual void draw_to(target_t&);

	static sf::Text NewTextItem(font_t& myFont, int point, const sf::Color& fill, float x, float y);
	static float NextPosition(float init, float len, int count);
};
