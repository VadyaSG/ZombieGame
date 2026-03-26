#include"Item.h"
#include"TextureHolder.h"
#include <iostream>


Item::Item(int type) :i_type{ type }, i_sprite(Texture_Holder::get_texture("texture/health_pickup.png"))
{
	if (i_type == 1)
	{
		i_sprite = Sprite(Texture_Holder::get_texture("texture/health_pickup.png"));
		i_val = start_health_val;
	}
	else
	{
		i_sprite = Sprite(Texture_Holder::get_texture("texture/ammo_pickup.png"));
		i_val = start_ammo_val;
	}
	i_sprite.setOrigin(Vector2f(25, 25));
	i_seconds_to_live = start_sec_to_live;
	i_seconds_to_wait = start_wait_time;
}

void Item::set_arena(IntRect arena)
{
	i_arena.position.x = arena.position.x + 50;
	i_arena.size.x = arena.size.x - 50;
	i_arena.position.y = arena.position.y + 50;
	i_arena.size.y = arena.size.y - 50;
	spawn();
}

void Item::spawn()
{
	srand((int)time(0) / i_type);
	int x = (rand() % i_arena.size.x);
	srand((int)time(0) * i_type);
	int y = (rand() % i_arena.size.y);
	i_seconds_since_spawn = 0;
	i_spawned = true;
	i_sprite.setPosition(Vector2f(x, y));
}

FloatRect Item::get_pos()
{
	return i_sprite.getGlobalBounds();
}
Sprite Item::get_sprite()
{
	return i_sprite;
}
bool Item::is_spawned()
{
	return i_spawned && !i_just_picked_up;
}

int Item::got_it()
{

	i_spawned = false;
	i_seconds_since_despawn = 0;
	i_just_picked_up = true;
	i_pickup_timer = 0;
	std::cout << "Item type: " << i_type << " gives: " << i_val << std::endl;  // ÎÒËÀÄÊÀ
	return i_val;
}
void Item::upgrade()
{
	if (i_type == 1)
	{
		i_val += (start_health_val * 0.5f);
	}
	else
	{
		i_val += (start_ammo_val * 0.5f);
	}
	i_seconds_to_live += (start_sec_to_live / 10);
	i_seconds_to_wait -= (start_wait_time / 10);
}
void Item::update(float elapsed_time)
{
	
	
	if (i_just_picked_up)
	{
		i_pickup_timer += elapsed_time;
		if (i_pickup_timer >= i_pickup_cooldown)
		{
			i_just_picked_up = false;
		}
	}
	if (!i_just_picked_up)
	{


		if (i_spawned)
		{
			i_seconds_since_spawn += elapsed_time;
		}
		else
		{
			i_seconds_since_despawn += elapsed_time;
		}
		if (i_seconds_since_spawn > i_seconds_to_live && i_spawned)
		{
			i_spawned = false;
			i_seconds_since_despawn = 10;
		}
		if (i_seconds_since_despawn > i_seconds_to_wait && !i_spawned)
		{
			spawn();
		}
	}
}
