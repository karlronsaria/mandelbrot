#include "DebugWindow.h"

WindowDebug::WindowDebug(const font_t& font):
	MenuBox(font)
{
	_left = 0;
	_right = 0;
	_top = 0;
	_bottom = 0;

	_mouse_x = 0;
	_mouse_y = 0;

	add_text_item(0);
	_labels.back().setString("Window Left:");
	add_text_item(WindowDebug::next_x_pos(1), WindowDebug::next_y_pos(0));

	add_text_item(1);
	_labels.back().setString("Window Right:");
	add_text_item(WindowDebug::next_x_pos(1), WindowDebug::next_y_pos(1));

	add_text_item(2);
	_labels.back().setString("Window Top:");
	add_text_item(WindowDebug::next_x_pos(1), WindowDebug::next_y_pos(2));

	add_text_item(3);
	_labels.back().setString("Window Bottom:");
	add_text_item(WindowDebug::next_x_pos(1), WindowDebug::next_y_pos(3));

	add_text_item(WindowDebug::INIT_X_POS, WindowDebug::next_y_pos(4) + 15);
	add_text_item(WindowDebug::INIT_X_POS, WindowDebug::next_y_pos(5) + 15);
}

WindowDebug::~WindowDebug() {}

int WindowDebug::update(canvas_t& window) {
	_left = (float)window.getPosition().x;
	_right = (float)window.getPosition().x + window.getSize().x;
	_top = (float)window.getPosition().y;
	_bottom = (float)window.getPosition().y + window.getSize().y;

	_mouse_x = sf::Mouse::getPosition().x;
	_mouse_y = sf::Mouse::getPosition().y;

	_labels[(int)LabelIndex::WINDOW_LEFT].setString(std::to_string(_left));
	_labels[(int)LabelIndex::WINDOW_RIGHT].setString(std::to_string(_right));
	_labels[(int)LabelIndex::WINDOW_TOP].setString(std::to_string(_top));
	_labels[(int)LabelIndex::WINDOW_BOTTOM].setString(std::to_string(_bottom));

	_labels[(int)LabelIndex::MOUSE_X].setString("Mouse X:  " + std::to_string(_mouse_x));
	_labels[(int)LabelIndex::MOUSE_Y].setString("Mouse Y:  " + std::to_string(_mouse_y));

	return (int)UpdateCodes::IDLE;
}
