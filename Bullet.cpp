#include"Bullet.h"
#include "Item.h"

Bullet::Bullet()
{
	bullet_shape.setSize(sf::Vector2f(10, 10));
	
}

void Bullet::stop()
{
	b_in_flight = false;
}

bool Bullet::is_in_flight()
{
	return b_in_flight;
}
FloatRect Bullet::get_position()
{
	return bullet_shape.getGlobalBounds();
}

RectangleShape Bullet::get_shape()
{
	return bullet_shape;
}


void Bullet::shoot(float start_x, float start_y,float target_x,float target_y)
{
	b_in_flight = true;
	position.x = start_x;
	position.y = start_y;

	float dx = target_x - start_x;
	float dy = target_y - start_y;

	float length = sqrt(dx * dx + dy * dy);
	if (length > 0)
	{
		bullet_distance_x = (dx / length) * bullet_speed;
		bullet_distance_y = (dy / length) * bullet_speed;
	}
	else
	{
		// ≈сли цель в той же точке, стрел€ем вверх (или любое другое направление по умолчанию)
		bullet_distance_x = 0;
		bullet_distance_y = -bullet_speed;
	}
	

	float range{ 1000 };
	min_x = start_x - range;
	max_x = start_x + range;
	min_y = start_y - range;
	max_y = start_y + range;

	bullet_shape.setPosition(position);
}
void Bullet::update(float elapsed_time)
{
	position.x += bullet_distance_x * elapsed_time;
	position.y += bullet_distance_y * elapsed_time;

	bullet_shape.setPosition(position);

	if (position.x<min_x || position.x>max_x || position.y<min_y || position.y>max_y)
	{
		b_in_flight = false;
	}
}


