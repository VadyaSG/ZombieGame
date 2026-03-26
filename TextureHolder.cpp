#include"TextureHolder.h"
#include<assert.h>


Texture_Holder* Texture_Holder::m_s_Instance = nullptr;
Texture_Holder::Texture_Holder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& Texture_Holder::get_texture(std::string const& filename)
{
	auto& m = m_s_Instance->m_Textures;//получить ссылку на m_Textures,использу€ m_s_Instance

	auto key_val_pair = m.find(filename);//найти нужную пару,использу€ переданное им€ файла

	if (key_val_pair != m.end())//если найдено совпадение вернуть 2ючасть пары(текстуру)
	{
		return key_val_pair->second;

	}
	else//в противном случае создаем пару использу€ им€ фаайла, загружаем текстуы из файла 
	{
		auto& texture = m[filename];
		texture.loadFromFile(filename);
		return texture;
	}
}
