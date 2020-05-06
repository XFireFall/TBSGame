//
// Created by airat on 28.04.2020.
//

#ifndef TBS_MENU_H
#define TBS_MENU_H

#include "SFML/Graphics.hpp"

#include "Button.h"
#include "textbox.h"

static const int BUTTON_NUM = 3;
static const int TEXTBOX_NUM = 2;
constexpr static const int TOTAL_NUM = BUTTON_NUM + TEXTBOX_NUM;

class Menu : public sf::Drawable {
public:
    Menu(const std::string &texture_file_background,
         const std::array<std::string, TOTAL_NUM> &texture_file,
         const std::array<sf::IntRect, TOTAL_NUM> &dimensions,
         const std::array<std::string , TEXTBOX_NUM> &font_file,
         const std::array<int, TEXTBOX_NUM> &char_size,
         const std::array<sf::Color, TEXTBOX_NUM> &color,
         const std::array<bool, TEXTBOX_NUM> &sel,
         const std::array<std::string, TEXTBOX_NUM> &default_str);

public:

    void drawTo(sf::RenderWindow &window) const;
    virtual void draw(sf::RenderTarget &render, sf::RenderStates states) const override;

public:
    Button button_connect;
    Button button_create;
    Button button_exit;

    Textbox textbox_nickname;
    Textbox textbox_ip;

private:
    sf::Texture texture_;
    sf::Sprite sprite_;
};


#endif //TBS_MENU_H
