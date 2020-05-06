//
// Created by airat on 05.05.2020.
//

#ifndef TBS_CHAT_H
#define TBS_CHAT_H

#include "SFML/Graphics.hpp"

#include "textbox.h"
#include "List.h"

class Chat : public sf::Drawable {
public:
    Chat(const std::string &chat_texture_file,
         const sf::IntRect &chat_dimensions,
         //List
         const std::string &list_texture_file,
         const sf::IntRect &list_dimensions,
         const std::string &list_font_file,
         int list_font_size,
         const sf::Color &list_font_color,
         const sf::Vector2f &list_text_pos,
         unsigned max_displayed_str_num,
         unsigned max_str_char_num,
         const std::string &button_up_texture_file,
         const sf::IntRect &button_up_dimensions,
         const std::string &button_down_texture_file,
         const sf::IntRect &button_down_dimensions,
         //textbox
         const std::string &textbox_texture_file,
         const sf::IntRect &textbox_dimensions,
         const std::string &textbox_font_file,
         int textbox_font_size,
         const sf::Color &textbox_color,
         bool textbox_sel,
         const std::string &textbox_default_str,
         int textbox_max_string_length = max_string_length_default,
         bool textbox_moving_window = false,
         unsigned max_displayed_char = max_string_length_default);

    //use only this method to load messages, not msg_list.setPlayers
    //you can also add nickname at the beginning of the string if you want (manually)
    void loadMessages(const std::vector<std::string> &msg,  bool display_from_end = false);
    //read written text
    //after using the function the text will be deleted
    std::string getText();

public:
    virtual void draw(sf::RenderTarget &render, sf::RenderStates states) const override;

public:
    Textbox textbox;
    List  msg_list;

private:
    sf::IntRect dimensions_;
    sf::Texture texture_;
    sf::Sprite sprite_;
};


#endif //TBS_CHAT_H
