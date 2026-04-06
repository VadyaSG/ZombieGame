#include"Player.h"
#include "Zombie.h"

Player::Player() :speed(start_speed), hp(start_hp), sprite(texture)
{
	//10
	texture.loadFromFile("texture/player.png");
	sprite.setTexture(texture,true);
	sprite.setOrigin(Vector2f(25, 25));
}

void Player::spawn(IntRect l_area, Vector2f l_resolution, int l_tile_size)
{
	//copy details of arena in arena_member
	arena.position.x = l_area.position.x;
	arena.size.x = l_area.size.x;
	arena.position.y = l_area.position.y;
	arena.size.y = l_area.size.y;
	
	// spawn player in centr of arena
	position.x = arena.size.x / 2;
	position.y = arena.size.y / 2;

	//copy size of tile on this arena
	tile_size = l_tile_size;
	//save resolution for next using
	resolution.x = l_resolution.x;
	resolution.y = l_resolution.y;
}

void Player::reset_player_stats()
{
	speed = start_speed;
	hp = start_hp;
	max_hp = start_hp;
}

Time Player::get_last_hit_time()
{
	return last_hit;
}

bool Player::hit(Time l_time_hit)
{
	if (l_time_hit.asMilliseconds() - last_hit.asMilliseconds() > 200)
	{
		last_hit = l_time_hit;
		hp -= 10;
		return true;
	}
	else
	{
		return false;
	}
}



FloatRect Player::get_position()
{
	return sprite.getGlobalBounds();
}

Vector2f Player::get_center()
{
	return position;
}

float Player::get_rotation()
{
	return sprite.getRotation().asDegrees();
}

Sprite& Player::get_sprite()
{
	return sprite;
}

int Player::get_hp()
{
	return hp;
}

void Player::move_left()
{
	is_left_pressed = true;
}

void Player::move_right()
{
	is_right_pressed = true;
}

void Player::move_up()
{
	is_up_pressed = true;
}

void Player::move_down()
{
	is_down_pressed = true;
}

void Player::stop_left()
{
	is_left_pressed = false;
}

void Player::stop_right()
{
	is_right_pressed = false;
}

void Player::stop_up()
{
	is_up_pressed = false;
}

void Player::stop_down()
{
	is_down_pressed = false;
}

void Player::update(float l_elapsed_time, Vector2i l_mouse_pos)
{
	if (is_up_pressed)
	{
		position.y -= speed * l_elapsed_time;
	}
	if (is_down_pressed)
	{
		position.y += speed * l_elapsed_time;
	}
	if (is_left_pressed)
	{
		position.x -= speed * l_elapsed_time;
	}
	if (is_right_pressed)
	{
		position.x += speed * l_elapsed_time;
	}
	sprite.setPosition(position);

	//удержание преса на экране

	if (position.x < arena.position.x + tile_size)
	{
		position.x = arena.position.x + tile_size;
	}
	else if (position.x > arena.position.x + arena.size.x - tile_size)
	{
		position.x = arena.position.x + arena.size.x - tile_size;
	}

	if (position.y < arena.position.y + tile_size)
	{
		position.y = arena.position.y + tile_size;
	}
	else if (position.y > arena.position.y + arena.size.y - tile_size)
	{
		position.y = arena.position.y + arena.size.y - tile_size;
	}
	sprite.setPosition(position);
	
	float angle = (atan2(l_mouse_pos.y - resolution.y / 2, l_mouse_pos.x - resolution.x / 2) * 180 )/ 3.141;
	
	sprite.setRotation(degrees(angle));
}

void Player::upgrade_speed()
{
	//boost speed in 20%
	speed += (start_speed * .2);
}

void Player::upgrade_hp()
{
	max_hp += (start_hp * .2);
}

void Player::increase_health_lvl(int l_amount)
{
	hp += l_amount;
	if (hp > max_hp)
	{
		hp = max_hp;
	}
}