//
// Created by airat on 05.05.2020.
//

#ifndef TBS_LIST_H
#define TBS_LIST_H

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "../primitives/Prim_game.hpp"

class List : public sf::Drawable {
public:
    List(const std::string &list_texture_file,
         const sf::IntRect &list_dimensions,
         const std::string &font_file,
         int font_size,
         const sf::Color &font_color,
         const sf::Vector2f &text_pos,
         unsigned max_displayed_str_num,
         unsigned max_str_char_num,
         const std::string &button_up_texture_file,
         const sf::IntRect &button_up_dimensions,
         const std::string &button_down_texture_file,
         const sf::IntRect &button_down_dimensions);

    //set the list of players if you want to write it for 1st time or update
    void setPlayers(const std::vector<gm::player_data> &player_data);

    //set string - in chat. In chat you should select first_displayed - so the list will start here
    //you better to synchronize first_displayed_str and max_displayed_str_num, like this: first_displayed_str = [last message] - max_displayed_str_num
    void setString(const std::vector<std::string> &msg, bool display_from_end = false);

    void shiftDisplayedStrUp();

    void shiftDisplayedStrDown();

public:
    virtual void draw(sf::RenderTarget &render, sf::RenderStates states) const override;

public:
    Button button_up;
    Button button_down;

private:
    void makeStrToDraw();

private:

    sf::Text text_;
    sf::IntRect dimensions_;

    sf::Texture texture_;
    sf::Sprite sprite_;

    sf::Font font_;

    std::vector<std::string> text_str_;
    std::string str_to_draw_;

    const unsigned max_displayed_str_num_;
    unsigned total_str_num_;
    unsigned first_displayed_str_;
    const unsigned max_str_char_num_;
};


#endif //TBS_LIST_H
