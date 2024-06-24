#pragma once

#include<Windows.h>
#include "gameMenu.h"
#include <C:/Users/Sopha/Downloads/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/include/SFML/Graphics.hpp>
#include<C:/Users/Sopha/Downloads/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/include/SFML/Audio.hpp>
#include "Animator.h"

using namespace sf;

void InitText(Text& mtext, float xpos, float ypos, String str, int size_font=60,Color menu_text_color=Color::White, int bord=0, Color border_color = Color::Black );

int GamaStart(sf::RenderWindow& window);

//Настройки
int Options(sf::RenderWindow& window);

// Об Игре
int About_Game(sf::RenderWindow& window);

int Menu_pik_Dam(sf::RenderWindow& window); 