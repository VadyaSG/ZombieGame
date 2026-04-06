#pragma once
#include"SFML/Graphics.hpp";
using namespace sf;
class Bullet
{
private:
	Vector2f position;
	RectangleShape bullet_shape;
	bool b_in_flight = false;
	float bullet_speed{ 1000 };
	float bullet_distance_x{0};
	float bullet_distance_y{0};
	float max_x{0};
	float max_y{0};
	float min_x{0};
	float min_y{0};
public:
	Bullet();
	void stop();
	bool is_in_flight();
	void shoot(float start_x, float start_y,float target_x, float target_y);
	FloatRect get_position();
	RectangleShape get_shape();
	void update(float elapsed_time);
};
