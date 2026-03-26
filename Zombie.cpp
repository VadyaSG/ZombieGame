#include"Zombie.h"
#include"TextureHolder.h"
#include<cstdlib>
#include<ctime>

Zombie::Zombie()
	: z_sprite(Texture_Holder::get_texture("textule/bloater.png")) // Временная текстура
	, z_speed(0)
	, z_health(0)
	, z_is_alive(true) 
{
	z_sprite.setOrigin(Vector2f(25, 25));
}

bool Zombie::is_hitting()
{
	--z_health;
	if (z_health <= 0)
	{
		z_is_alive = false;
		z_sprite.setTexture(Texture_Holder::get_texture("texture/blood.png"));
		return true;
	}
	return false;
	
}

bool Zombie::is_alive()
{
	return z_is_alive;
}

void Zombie::spawn(float start_x, float start_y, int type, int speed)
{
	switch (type)
	{
	case 0:
		z_sprite = Sprite(Texture_Holder::get_texture("texture/bloater.png"));
		z_speed = bloater_speed;
		z_health = bloater_hp;
		break;
	case 1:
		z_sprite = Sprite(Texture_Holder::get_texture("texture/chaser.png"));
		z_speed = chaser_speed;
		z_health = chaser_hp;
		break;
	case 2:
		z_sprite = Sprite(Texture_Holder::get_texture("texture/crawler.png"));
		z_speed = crawler_speed;
		z_health = crawler_hp;
		break;
	}
	//уникальность каждого зомби
	srand((int)time(0) * speed);
	float z_mod = (rand() % max_varriance) + offset;
	z_mod /= 100;
	z_speed *= z_mod;

	//инициализация местоположения
	z_position.x = start_x;
	z_position.y = start_y;
	z_sprite.setOrigin(Vector2f(25, 25));
	z_sprite.setPosition(Vector2f(z_position));
}

FloatRect Zombie::get_position()
{
	return z_sprite.getGlobalBounds();
}
Sprite Zombie::get_sprite()
{
	return z_sprite;
}
void Zombie::update(float elapsed_time, Vector2f player_loc)
{
	if (z_is_alive)
	{


		float player_x = player_loc.x;
		float player_y = player_loc.y;

		if (player_x > z_position.x)
		{
			z_position.x = z_position.x + z_speed * elapsed_time;
		}
		if (player_y > z_position.y)
		{
			z_position.y = z_position.y + z_speed * elapsed_time;
		}
		if (player_x < z_position.x)
		{
			z_position.x = z_position.x - z_speed * elapsed_time;
		}
		if (player_y < z_position.y)
		{
			z_position.y = z_position.y - z_speed * elapsed_time;
		}

		z_sprite.setPosition(z_position);
		float angle = (atan2(player_y - z_position.y, player_x - z_position.x) * 180) / 3.141;
		z_sprite.setRotation(degrees(angle));
	}
}