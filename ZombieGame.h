#pragma once
#include<SFML/Graphics.hpp>
#include "Zombie.h"
using namespace sf;
int create_background(VertexArray& r_v_a, IntRect arena);
Zombie* create_hord(int nums_of_z, IntRect arena);
