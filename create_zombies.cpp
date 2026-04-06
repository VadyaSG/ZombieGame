#include"ZombieGame.h"
#include"Zombie.h"

Zombie* create_hord(int nums_of_z, IntRect arena)
{
	Zombie* zombies = new Zombie[nums_of_z];
	int max_y = arena.size.y - 20;
	int min_y = arena.position.y + 20;
	int max_x = arena.size.x - 20;
	int min_x = arena.position.x + 20;

	for (int i{ 0 }; i < nums_of_z; ++i)
	{
		
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x{}, y{};
		switch (side)
		{
		case 0:                                    //left
			x = min_x;
			y = (rand() % max_y) + min_y;
			break;
		case 1:                                  //right
			x = max_x;
			y = (rand() % max_y) + min_y;
			break;
		case 2:                                   //up
			x = (rand() % max_x) + min_x;
			y = min_y;
			break;
		case 3:                                  //down
			x = (rand() % max_x) + min_x;
			y = max_y;
			break;
		}

		//zompie type
		srand((int)time(0) * i * 2); 
		int type = (rand() % 3);
		zombies[i].spawn(x, y, type, i);
	}
	return zombies;
}
