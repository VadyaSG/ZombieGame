#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;

class Item
{
private:
	const int start_health_val{ 10 };
	const int start_ammo_val{ 12 };
	const int start_wait_time{ 40 };
	const int start_sec_to_live{ 5 };

	bool i_just_picked_up{ false };
	float i_pickup_cooldown{ 2.f };  // 0.5 секунды защиты
	float i_pickup_timer{ 0 };
	
	Sprite i_sprite;
	IntRect i_arena;
	int i_val{0};
	int i_type;
	bool i_spawned{false};
	int i_seconds_since_spawn{200};
	int i_seconds_since_despawn{100};
	int i_seconds_to_live{20};
	int i_seconds_to_wait{50};
public:
	Item(int type);
	void set_arena(IntRect arena);
	void spawn();
	FloatRect get_pos();
	Sprite get_sprite();
	void update(float elapsed_time);
	bool is_spawned();
	int got_it();
	void upgrade();
};