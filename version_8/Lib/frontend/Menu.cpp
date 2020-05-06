//
// Created by airat on 28.04.2020.
//

#include "Menu.h"

#define MENU_BUTTON_CTR(num) texture_file[(num)], dimensions[(num)]
#define MENU_TEXTBOX_CTR(num) texture_file[(num) + BUTTON_NUM], dimensions[(num) + BUTTON_NUM], font_file[(num)] ,char_size[(num)], color[(num)], sel[(num)], default_str[(num)]

Menu::Menu(const std::string &texture_file_background,
           const std::array<std::string, TOTAL_NUM> &texture_file,
           const std::array<sf::IntRect, TOTAL_NUM> &dimensions,
           const std::array<std::string , TEXTBOX_NUM> &font_file,
           const std::array<int, TEXTBOX_NUM> &char_size,
           const std::array<sf::Color, TEXTBOX_NUM> &color,
           const std::array<bool, TEXTBOX_NUM> &sel,
           const std::array<std::string, TEXTBOX_NUM> &default_str) : button_connect(MENU_BUTTON_CTR(0)),
                                                                      button_create(MENU_BUTTON_CTR(1)),
                                                                      button_exit(MENU_BUTTON_CTR(2)),
                                                                      textbox_nickname(MENU_TEXTBOX_CTR(0)),
                                                                      textbox_ip(MENU_TEXTBOX_CTR(1)) {
    if (!texture_.loadFromFile(texture_file_background)) {
        throw std::runtime_error("Cannot load texture");
    }

    sprite_.setTexture(texture_);
    sprite_.setPosition(0, 0);
}

void Menu::drawTo(sf::RenderWindow &window) const {
    button_connect.drawTo(window);
    button_create.drawTo(window);
    button_exit.drawTo(window);
    textbox_nickname.drawTo(window);
    textbox_ip.drawTo(window);
}

void Menu::draw(sf::RenderTarget &render, sf::RenderStates states) const {
    render.draw(sprite_);
    render.draw(button_connect);
    render.draw(button_create);
    render.draw(button_exit);
    render.draw(textbox_nickname);
    render.draw(textbox_ip);
}
