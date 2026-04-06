#pragma once

#include<SFML/Graphics.hpp>
#include<map>
using namespace sf;

class Texture_Holder
{
private:
	std::map<std::string, Texture> m_Textures;
	static Texture_Holder* m_s_Instance;
public:
	Texture_Holder();
	static Texture& get_texture(std::string const& filename);
};


