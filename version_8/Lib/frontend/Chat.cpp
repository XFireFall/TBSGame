//
// Created by airat on 05.05.2020.
//

#include "Chat.h"

Chat::Chat(const std::string &chat_texture_file,
           const sf::IntRect &chat_dimensions,
        //List
           const std::string &list_texture_file,
           const sf::IntRect &list_dimensions,
           const std::string &list_font_file,
           int list_font_size,
           const sf::Color &list_font_color,
           const sf::Vector2f &list_text_pos,
           unsigned int max_displayed_str_num,
           unsigned int max_str_char_num,
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
           int textbox_max_string_length,
           bool textbox_moving_window,
           unsigned max_displayed_char) : dimensions_(chat_dimensions),
                                                     msg_list(list_texture_file,
                                                              list_dimensions,
                                                              list_font_file,
                                                              list_font_size,
                                                              list_font_color,
                                                              list_text_pos,
                                                              max_displayed_str_num,
                                                              max_str_char_num,
                                                              button_up_texture_file,
                                                              button_up_dimensions,
                                                              button_down_texture_file,
                                                              button_down_dimensions),
                                                     textbox(textbox_texture_file,
                                                             textbox_dimensions,
                                                             textbox_font_file,
                                                             textbox_font_size,
                                                             textbox_color,
                                                             textbox_sel,
                                                             textbox_default_str,
                                                             textbox_max_string_length,
                                                             textbox_moving_window,
                                                             max_displayed_char) {
    if (!texture_.loadFromFile(list_texture_file)) {
        throw std::runtime_error("Cannot load texture");
    }

    sprite_.setTexture(texture_);
    sprite_.setPosition(dimensions_.left, dimensions_.top);
}

void Chat::draw(sf::RenderTarget &render, sf::RenderStates states) const {
    render.draw(sprite_);
    render.draw(msg_list);
    render.draw(textbox);
}

void Chat::loadMessages(const std::vector<std::string> &msg, bool display_from_end) {
    msg_list.setString(msg, display_from_end);
}

std::string Chat::getText() {
    return textbox.getText(true);
}
