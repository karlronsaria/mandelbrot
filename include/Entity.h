#pragma once
#include <SFML/Graphics.hpp>

typedef sf::RenderWindow canvas_t;
typedef sf::RenderTarget target_t;

enum class UpdateCodes {
	IDLE,
	REMOVE,
	COUNT
};

class Entity {
public:
	virtual ~Entity() = default;
	virtual int update(canvas_t&) = 0;
	virtual void draw_to(target_t&) = 0;
};

class WindowEntity : public Entity {
public:
	virtual ~WindowEntity() = default;
	virtual void draw_to(canvas_t&) = 0;
};
