#pragma once
#include "MenuBox.h"

class WindowDebug: public MenuBox {
private:
	float _left;
	float _right;
	float _top;
	float _bottom;

	int _mouse_x;
	int _mouse_y;

	enum class LabelIndex {
		WINDOW_LEFT = 1,
		WINDOW_RIGHT = 3,
		WINDOW_TOP = 5,
		WINDOW_BOTTOM = 7,
		MOUSE_X = 8,
		MOUSE_Y = 9,
		COUNT
	};
public:
	WindowDebug(const font_t& font_ptr);
	virtual ~WindowDebug();
	int update(canvas_t& window);
};
