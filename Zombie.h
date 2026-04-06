#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;

class Zombie
{
private:
	const float bloater_speed{ 44 };
	const float chaser_speed{ 90 };
	const float crawler_speed{ 25 };
	const float  bloater_hp{ 2 };
	const float	 chaser_hp{ 1 };
	const float	 crawler_hp{ 4 };
	const int max_varriance{ 35 };
	const int offset{ 101 - max_varriance };
	Vector2f z_position;
	Sprite z_sprite;
	float z_speed;
	float z_health;
	bool z_is_alive{true};


public:
	Zombie();
	
	bool is_hitting();
	bool is_alive();
	void spawn(float start_x, float start_y, int type, int speed);
	
	FloatRect get_position();
	Sprite get_sprite();
	void update(float elapsed_time, Vector2f player_loc);
};
