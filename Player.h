#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
using namespace sf;
class Player
{
private:
	const float start_speed{ 200 };
	const float start_hp{ 100 };
	Vector2f position;// положение персонажа
	
	Sprite sprite;
	Texture texture;
	
	Vector2f resolution;// разрешение экрана
	IntRect arena;//размер текущей арены
	int tile_size{50};//размер кажэой плитки арены
	bool is_up_pressed{false};
	bool is_down_pressed{false};
	bool is_left_pressed{false};
	bool is_right_pressed{false};
	int hp{0};
	int max_hp{100};
	Time last_hit;//когда герой был ранен последний раз
	float speed;//скорость в пикселях в секунду
public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tile_size);
	void reset_player_stats();
	bool hit(Time time_hit);// обработка удара зомби по игроку
	Time get_last_hit_time();// когда игрока последний раз ранили
	FloatRect get_position();// положение персонажа
	Vector2f get_center();// где центр персонажа
	float get_rotation();//направление взгляда
	Sprite& get_sprite();//отправка копии спрайта в основную функцию
	//управление
	void move_left();
	void move_right();
	void move_up();
	void move_down();
	void stop_left();
	void stop_right();
	void stop_up();
	void stop_down();

	void update(float elapsed_time, Vector2i mouse_pos);
	void upgrade_speed();
	void upgrade_hp();
	void increase_health_lvl(int amount);//увеличевает макс хп персонажа
	int get_hp();//сколько хп сейчас
};
