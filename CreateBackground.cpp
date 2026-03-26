#pragma once
#include "ZombieGame.h"

int create_background(VertexArray& r_v_a, IntRect arena)
{
    const int tile_size{ 50 };
    const int tile_tipes{ 3 };
    const int vert_in_quad{ 4 };
    int world_width = arena.size.x / tile_size;
    int world_height = arena.size.y / tile_size;
    r_v_a.setPrimitiveType(PrimitiveType::Triangles);
    r_v_a.resize((world_width * world_height) * 6); // 2 треугольника = 6 вершин
    int current_vertex{ 0 };

    for (int w = 0; w < world_width; ++w)
    {
        for (int h = 0; h < world_height; ++h)
        {
            // Позиции вершин
            Vector2f pos0(w * tile_size, h * tile_size);                          // верхний левый
            Vector2f pos1((w * tile_size) + tile_size, h * tile_size);          // верхний правый
            Vector2f pos2((w * tile_size) + tile_size, (h * tile_size) + tile_size); // нижний правый
            Vector2f pos3(w * tile_size, (h * tile_size) + tile_size);          // нижний левый

            // Текстурные координаты
            Vector2f tex0, tex1, tex2, tex3;

            if (h == 0 || h == world_height - 1 || w == 0 || w == world_width - 1)
            {
                tex0 = Vector2f(0, tile_tipes * tile_size);
                tex1 = Vector2f(tile_size, tile_tipes * tile_size);
                tex2 = Vector2f(tile_size, tile_size + tile_tipes * tile_size);
                tex3 = Vector2f(0, tile_size + tile_tipes * tile_size);
            }
            else
            {
                srand((int)time(0) + h * world_width + w);
                int m_org = (rand() % tile_tipes);
                int vertical_offset = m_org * tile_size;

                tex0 = Vector2f(0, 0 + vertical_offset);
                tex1 = Vector2f(tile_size, 0 + vertical_offset);
                tex2 = Vector2f(tile_size, tile_size + vertical_offset);
                tex3 = Vector2f(0, tile_size + vertical_offset);
            }

            // Первый треугольник (верхний левый)
            r_v_a[current_vertex + 0].position = pos0;
            r_v_a[current_vertex + 0].texCoords = tex0;

            r_v_a[current_vertex + 1].position = pos1;
            r_v_a[current_vertex + 1].texCoords = tex1;

            r_v_a[current_vertex + 2].position = pos2;
            r_v_a[current_vertex + 2].texCoords = tex2;

            // Второй треугольник (нижний правый)
            r_v_a[current_vertex + 3].position = pos0;
            r_v_a[current_vertex + 3].texCoords = tex0;

            r_v_a[current_vertex + 4].position = pos2;
            r_v_a[current_vertex + 4].texCoords = tex2;

            r_v_a[current_vertex + 5].position = pos3;
            r_v_a[current_vertex + 5].texCoords = tex3;

            current_vertex += 6;
        }
    }
    return tile_size;
}