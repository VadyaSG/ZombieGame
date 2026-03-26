#include <sstream>
#include <fstream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Zombie.h"
#include "ZombieGame.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include"Item.h"
#include <iostream>


using namespace sf;
Vector2f scaling(RenderWindow& window, float pos_x, float pos_y)
{
    Vector2u window_size = window.getSize();
    float scale_x = static_cast<float>(window_size.x) / 1920.0f;
    float scale_y = static_cast<float>(window_size.y) / 1080.0f;

    return { pos_x * scale_x, pos_y * scale_y };
}
int main()
{
    bool wave_in_progress = true;
    bool need_new_wave = false;

    Texture_Holder holder_txt;
    //game always in one of 4 condition
    enum class State { PAUSED, LEVELIN_UP, GAME_OVER, PLAYING };
    //start with game over condition
    State state = State::GAME_OVER;

    // get a resolution screen and creating sfml window
    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().size.x;
    resolution.y = VideoMode::getDesktopMode().size.y;

    RenderWindow window(VideoMode(Vector2u(resolution.x, resolution.y)), "Zombie Game", Style::None);

    //create object View for main action
    View main_view(FloatRect(Vector2f(0, 0), Vector2f(resolution.x, resolution.y)));

    Clock clock;

    Time game_time_total;
    Vector2f mouse_world_pos;
    Vector2i mouse_screen_pos;

    Player player;

    // ИНИЦИАЛИЗАЦИЯ АРЕНЫ ДО ИГРОВОГО ЦИКЛА
    IntRect arena;
    arena.size.x = 1500;
    arena.size.y = 1500;
    arena.position.x = 0;
    arena.position.y = 0;

    int tile_size = 50; // значение по умолчанию

    Texture background_texture = Texture_Holder::get_texture("texture/background_sheet.png");
    VertexArray background;
    
    int nums_of_z;
    int nums_of_alive_z;
    Zombie* zombies = nullptr;

    Bullet bullets[100];
    int current_bullet = 0;
    int bullets_spare = 24;
    int bullets_in_clip = 6;
    int clip_size = 6;
    float fire_rate = 1;
    Time last_presed;

    window.setMouseCursorVisible(false);
    Texture crosshair_txt = Texture_Holder::get_texture("texture/crosshair.png");
    Sprite crosshair_sprite(crosshair_txt);
    crosshair_sprite.setOrigin(Vector2f(25, 25));

    Item helath_item(1);
    Item ammo(3);

    int score{ 0 };
    int hi_score{ 0 };

    Texture game_over_txt = Texture_Holder::get_texture("texture/background.png");
    Sprite game_over_sprite(game_over_txt);
    game_over_sprite.setPosition(Vector2f(0,0));
    View hudWiew(FloatRect(Vector2f(0, 0), Vector2f(resolution.x, resolution.y)));
    Texture ammo_icon= Texture_Holder::get_texture("texture/ammo_icon.png");
    Sprite ammo_icon_sprite(ammo_icon);
    ammo_icon_sprite.setPosition(scaling(window, 30, 980));

    Font font;
    font.openFromFile("fonts/zombiecontrol.ttf");

    Text pause_text(font);
    pause_text.setCharacterSize(155);
    pause_text.setFillColor(Color::White);
    pause_text.setPosition(scaling(window, 400, 400));
    pause_text.setString("Press Enter \nto continue");

    Text game_over_text(font);
   game_over_text.setCharacterSize(125);
   game_over_text.setFillColor(Color::White);
   game_over_text.setPosition(scaling(window, 600, 540));
   game_over_text.setString("Press Enter \nto play");

   Text level_up_text(font);
  level_up_text.setCharacterSize(80);
  level_up_text.setFillColor(Color::White);
  level_up_text.setPosition(scaling(window, 150, 250));
  level_up_text.setString("Press Enter \nto play");
  std::stringstream level_up_stream;
  level_up_stream <<
      "1-povishenie skorostrelnosty" <<
      "\n2-povishenie oboymi" <<
      "\n3-povishenie HP" <<
      "\n4-povishenie speed" <<
      "\n5-povishenie aptechka" <<
      "\n6-povishenie ammo";
  level_up_text.setString(level_up_stream.str());

  Text ammo_text(font);
  ammo_text.setCharacterSize(55);
  ammo_text.setFillColor(Color::White);
  ammo_text.setPosition(scaling(window, 110, 980));

  Text score_text(font);
 score_text.setCharacterSize(55);
 score_text.setFillColor(Color::White);
 score_text.setPosition(scaling(window, 20, 0));

 std::ifstream input_file("gamedata/score.txt");

 if (input_file.is_open())
 {
     input_file >> hi_score;
     input_file.close();
 }

  Text hi_score_text(font);
  hi_score_text.setCharacterSize(55);
  hi_score_text.setFillColor(Color::White);
  hi_score_text.setPosition(scaling(window, 1400, 0));
  std::stringstream s;
  s << "Hight score: " << hi_score;
  hi_score_text.setString(s.str());

  Text zombie_remaining_text(font);
 zombie_remaining_text.setCharacterSize(55);
 zombie_remaining_text.setFillColor(Color::White);
 zombie_remaining_text.setPosition(scaling(window, 1500, 980));
 zombie_remaining_text.setString("Zombies: 100");

 //wave text
 int wave{ 1 };
 Text wave_num_text(font);
  wave_num_text.setCharacterSize(55);
  wave_num_text.setFillColor(Color::White);
  wave_num_text.setPosition(scaling(window, 1250, 980));
  wave_num_text.setString("Wave: 0");

  //HP bar
  RectangleShape health_bar;
  health_bar.setFillColor(Color::Red);
  

  health_bar.setPosition(scaling(window, 450, 980));
  

  //update hud
  int frames_since_last_hud_update{ 0 };
  int fps_mfi{ 1000 };
  
  //sounds
  SoundBuffer hit_bufer;
  hit_bufer.loadFromFile("sounds/hit.wav");
  Sound hit(hit_bufer);

  SoundBuffer splat_buffer;
  splat_buffer.loadFromFile("sounds/splat.wav");
  Sound splat(splat_buffer);

  SoundBuffer shoot_buffer;
  shoot_buffer.loadFromFile("sounds/shoot.wav");
  Sound shoot(shoot_buffer);

  SoundBuffer reload_buffer;
  reload_buffer.loadFromFile("sounds/reload.wav");
  Sound reload(reload_buffer);

  SoundBuffer reload_failed_buffer;
  reload_failed_buffer.loadFromFile("sounds/reload_failed.wav");
  Sound reload_fail(reload_failed_buffer);

  SoundBuffer level_up_buffer;
  level_up_buffer.loadFromFile("sounds/powerup.wav");
  Sound level_up(level_up_buffer);

  SoundBuffer pickup_buffer;
  pickup_buffer.loadFromFile("sounds/pickup.wav");
  Sound pickup(pickup_buffer);




    // Создаем фон один раз перед игровым циклом
    if (background_texture.getSize().x > 0)
    {
        tile_size = create_background(background, arena);
    }

    // Спавним игрока один раз перед игровым циклом
    player.spawn(arena, resolution, tile_size);

    // spawn zombie
    nums_of_z = 5;
    zombies = create_hord(nums_of_z, arena);
    nums_of_alive_z = nums_of_z;

    // Для отслеживания нажатых клавиш
    bool w_pressed = false, s_pressed = false, a_pressed = false, d_pressed = false;

    while (window.isOpen())
    {
        // ОБРАБОТКА СОБЫТИЙ
        while (const std::optional event = window.pollEvent())
        {
            //close game
            if (event->is<Event::Closed>())
            {
                window.close();
            }

            else if (const auto* key_pressed = event->getIf<Event::KeyPressed>())
            {
                if (key_pressed->scancode == Keyboard::Scancode::Escape)
                {
                    window.close();
                }

                // Обработка нажатий WASD
                if (key_pressed->scancode == Keyboard::Scancode::W)
                {
                    w_pressed = true;
                    player.move_up();
                    
                }
                if (key_pressed->scancode == Keyboard::Scancode::S)
                {
                    s_pressed = true;
                    player.move_down();
                }
                if (key_pressed->scancode == Keyboard::Scancode::A)
                {
                    a_pressed = true;
                    player.move_left();
                }
                if (key_pressed->scancode == Keyboard::Scancode::D)
                {
                    d_pressed = true;
                    player.move_right();
                }
                //reload on keyb
                if (key_pressed->scancode == Keyboard::Scancode::R)
                {
                    if (bullets_spare >= clip_size)
                    {
                        bullets_in_clip = clip_size;
                        bullets_spare -= clip_size;
                        reload.play();
                    }
                    else if (bullets_spare > 0)
                    {
                        bullets_in_clip = bullets_spare;
                        bullets_spare = 0;
                        reload.play();
                    }
                    else
                    {
                        reload_fail.play();
                    }
                }
               
            }

            else if (const auto* key_released = event->getIf<Event::KeyReleased>())
            {
                // Обработка отпускания клавиш
                if (key_released->scancode == Keyboard::Scancode::W)
                {
                    w_pressed = false;
                    player.stop_up();
                }
                if (key_released->scancode == Keyboard::Scancode::S)
                {
                    s_pressed = false;
                    player.stop_down();
                }
                if (key_released->scancode == Keyboard::Scancode::A)
                {
                    a_pressed = false;
                    player.stop_left();
                }
                if (key_released->scancode == Keyboard::Scancode::D)
                {
                    d_pressed = false;
                    player.stop_right();
                }
            }

            if (Mouse::isButtonPressed(Mouse::Button::Left))
            {
                if (game_time_total.asMilliseconds() - last_presed.asMilliseconds() > 100 / fire_rate && bullets_in_clip > 0)
                {
                    bullets[current_bullet].shoot(player.get_center().x, player.get_center().y, mouse_world_pos.x, mouse_world_pos.y);
                    current_bullet++;
                    if (current_bullet > 99)
                    {
                        current_bullet = 0;
                    }
                    last_presed = game_time_total;
                    shoot.play();
                    --bullets_in_clip;
                }
            }

            if (const auto* key_pressed = event->getIf<Event::KeyPressed>())
            {
                if (state == State::GAME_OVER&& key_pressed->scancode == Keyboard::Scancode::Enter)
                {
                   
                    state = State::PLAYING;
                }
                else if (state == State::PLAYING&& key_pressed->scancode == Keyboard::Scancode::Enter)
                {
                    state = State::PAUSED;
                }
                else if (state == State::PAUSED && key_pressed->scancode == Keyboard::Scancode::Enter)
                {
                    state = State::PLAYING;
                }
               
            }
        }
        //menu
        if (state == State::LEVELIN_UP)
        {
            window.clear();
            window.draw(game_over_sprite);
            window.draw(level_up_text);
            window.display();

            bool upgraded = false; // Флаг, что был выбран апгрейд

            if (Keyboard::isKeyPressed(Keyboard::Key::Num1))
            {
                fire_rate++;
                upgraded = true;
                level_up.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Num2))
            {
                clip_size += clip_size;
                upgraded = true;
                level_up.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Num3))
            {
                player.upgrade_hp();
                upgraded = true;
                level_up.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Num4))
            {
                player.upgrade_speed();
                upgraded = true;
                level_up.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Num5))
            {
                helath_item.upgrade();
                upgraded = true;
                level_up.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Num6))
            {
                ammo.upgrade();
                upgraded = true;
                level_up.play();
            }

            // Если был выбран апгрейд, переходим в игру и создаем новую волну
            if (upgraded)
            {
                wave++; // Увеличиваем волну только один раз
                need_new_wave = true; // Устанавливаем флаг для создания новой волны
                state = State::PLAYING;
            }
        }
        if (state == State::PAUSED)
        {
           
            window.draw(pause_text);
            window.display();
        }

        if (state == State::GAME_OVER)
        {
            player.reset_player_stats();
            window.clear();
            window.draw(game_over_sprite);
            window.draw(game_over_text);
            window.draw(score_text);
            window.draw(hi_score_text);
            wave = 1;
            score = 0;
            bullets_spare = 24;
            bullets_in_clip = 6;
            clip_size = 6;
            fire_rate = 1;
            window.display();
        }



        // ОБНОВЛЕНИЕ ИГРЫ
        if (state == State::PLAYING)
        {
            if (need_new_wave)
            {
                std::cout << "Creating new wave: " << wave << ", zombies count: " << (5 + (wave - 1) * 5) << std::endl;

                if (zombies != nullptr)
                {
                    delete[] zombies;
                    zombies = nullptr;
                }

                nums_of_z = 5 + (wave - 1) * 2;
                zombies = create_hord(nums_of_z, arena);
                nums_of_alive_z = nums_of_z;
                need_new_wave = false;
            }

            // Получаем время с прошлого кадра
            Time dt = clock.restart();
            game_time_total += dt;
            float dt_as_seconds = dt.asSeconds();

            // Получаем позицию мыши
            mouse_screen_pos = Mouse::getPosition(window);
            mouse_world_pos = window.mapPixelToCoords(mouse_screen_pos, main_view);

            // Обновляем игрока
            player.update(dt_as_seconds, mouse_screen_pos);

            // Сохраняем новую позицию игрока и центрируем камеру
            Vector2f player_position = player.get_center();
            main_view.setCenter(player_position);

            crosshair_sprite.setPosition(mouse_world_pos);

            helath_item.set_arena(arena);
            ammo.set_arena(arena);

            // Обновляем зомби
            if (zombies != nullptr)
            {
                for (int i{ 0 }; i < nums_of_z; ++i)
                {
                    zombies[i].update(dt_as_seconds, player_position);
                }
            }

            // Обновляем пули
            for (int i = 0; i < 100; ++i)
            {
                if (bullets[i].is_in_flight())
                {
                    bullets[i].update(dt_as_seconds);
                }
            }

            helath_item.update(dt_as_seconds);
            ammo.update(dt_as_seconds);

            // Коллизии пуль с зомби
            if (zombies != nullptr)
            {
                for (int i{ 0 }; i < 100; ++i)
                {
                    for (int j = 0; j < nums_of_z; ++j)
                    {
                        if (bullets[i].is_in_flight() && zombies[j].is_alive())
                        {
                            if (bullets[i].get_position().findIntersection(zombies[j].get_position()))
                            {
                                bullets[i].stop();
                                if (zombies[j].is_hitting())
                                {
                                    score += 10;
                                    if (score >= hi_score)
                                    {
                                        hi_score = score;
                                    }
                                    nums_of_alive_z--;
                                    if (nums_of_alive_z <= 0)
                                    {
                                        state = State::LEVELIN_UP;
                                    }
                                }
                                splat.play();
                            }
                        }
                    }
                }
            }

            // Проверка урона игрока
            if (zombies != nullptr)
            {
                for (int i{ 0 }; i < nums_of_z; ++i)
                {
                    if (player.get_position().findIntersection(zombies[i].get_position()) && zombies[i].is_alive())
                    {
                        if (player.hit(game_time_total))
                        {
                            hit.play();
                        }
                        if (player.get_hp() <= 0)
                        {
                            state = State::GAME_OVER;
                            std::ofstream output_file("gamedata/score.txt");
                            output_file << hi_score;
                            output_file.close();
                        }
                    }
                }
            }

            // Подбор предметов
            if (player.get_position().findIntersection(helath_item.get_pos()) && helath_item.is_spawned())
            {
                player.increase_health_lvl(helath_item.got_it());
                pickup.play();
            }
            if (player.get_position().findIntersection(ammo.get_pos()) && ammo.is_spawned())
            {
                bullets_spare += ammo.got_it();
                pickup.play();
            }

            // Обновление HUD текста
            health_bar.setSize(Vector2f(player.get_hp() * 3, 50));
            frames_since_last_hud_update++;
            if (frames_since_last_hud_update > fps_mfi)
            {
                std::stringstream ss_ammo, ss_score, ss_hiScore, ss_wave, ss_zomb_alive;
                ss_ammo << bullets_in_clip << "/" << bullets_spare;
                ammo_text.setString(ss_ammo.str());
                ss_score << "Score: " << score;
                score_text.setString(ss_score.str());
                ss_hiScore << "High score: " << hi_score;
                hi_score_text.setString(ss_hiScore.str());
                ss_wave << "Wave: " << wave;
                wave_num_text.setString(ss_wave.str());
                ss_zomb_alive << "Zombies: " << nums_of_alive_z;
                zombie_remaining_text.setString(ss_zomb_alive.str());
                frames_since_last_hud_update = 0;
            }

            
            window.clear();
            window.setView(main_view);
            window.draw(background, &background_texture);
            window.draw(player.get_sprite());

            if (ammo.is_spawned()) window.draw(ammo.get_sprite());
            if (helath_item.is_spawned()) window.draw(helath_item.get_sprite());

            window.draw(crosshair_sprite);

            if (zombies != nullptr)
            {
                for (int i{ 0 }; i < nums_of_z; ++i)
                {
                    window.draw(zombies[i].get_sprite());
                }
            }

            for (int i = 0; i < 100; ++i)
            {
                if (bullets[i].is_in_flight())
                {
                    window.draw(bullets[i].get_shape());
                }
            }

            window.setView(hudWiew);
            window.draw(ammo_icon_sprite);
            window.draw(score_text);
            window.draw(ammo_text);
            window.draw(health_bar);
            window.draw(hi_score_text);
            window.draw(wave_num_text);
            window.draw(zombie_remaining_text);

            window.display();
        }
        
    }
    delete[] zombies;
    return 0;
}
