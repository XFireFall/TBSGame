//
// Created by airat on 05.05.2020.
//

#include "List.h"

List::List(const std::string &list_texture_file,
           const sf::IntRect &list_dimensions,
           const std::string &font_file,
           int font_size,
           const sf::Color &font_color,
           const sf::Vector2f &text_pos,
           unsigned int max_displayed_str_num,
           unsigned max_str_char_num,
           const std::string &button_up_texture_file,
           const sf::IntRect &button_up_dimensions,
           const std::string &button_down_texture_file,
           const sf::IntRect &button_down_dimensions) : dimensions_(list_dimensions),
                                                        max_displayed_str_num_(max_displayed_str_num),
                                                        max_str_char_num_(max_str_char_num),
                                                        button_up("Media/button_up.png",
                                                                  button_up_dimensions),
                                                        button_down("Media/button_down.png",
                                                                    button_down_dimensions),
                                                        text_str_(0),
                                                        first_displayed_str_(0),
                                                        total_str_num_(0) {
    if (!texture_.loadFromFile(list_texture_file)) {
        throw std::runtime_error("Cannot load texture");
    }

    sprite_.setTexture(texture_);
    sprite_.setPosition(dimensions_.left, dimensions_.top);

    if (!font_.loadFromFile(font_file)) {
        throw std::runtime_error("Cannot load font");
    }

    text_.setFillColor(font_color);
    text_.setCharacterSize(font_size);
    text_.setFont(font_);
    text_.setPosition(text_pos);


}

void List::setPlayers(const std::vector<gm::player_data> &player_data) {
    size_t str_len = 0;
    size_t substr_num = 0;

    text_str_.clear();
    for (const auto &str : player_data) {
        if ((str_len = str.nickname.size()) > max_str_char_num_) {
            substr_num = str_len / max_str_char_num_;

            for (size_t i = 0; i < substr_num; ++i) {
                text_str_.push_back(">" + str.nickname.substr(max_str_char_num_ * i, max_str_char_num_));
            }

            text_str_.push_back(str.nickname.substr(max_str_char_num_ * substr_num, str_len % max_str_char_num_));
        } else {
            text_str_.push_back(">" + str.nickname);
        }
    }

    total_str_num_ = text_str_.size();
    first_displayed_str_ = 0;

    makeStrToDraw();
}

void List::setString(const std::vector<std::string> &msg, bool display_from_end) {
    size_t str_len = 0;
    size_t substr_num = 0;

    text_str_.clear();
    unsigned str_num = 0;
    for (const auto &str : msg) {
        if ((str_len = str.size()) > max_str_char_num_) {
            substr_num = str_len / max_str_char_num_;

            for (size_t i = 0; i < substr_num; ++i) {
                text_str_.push_back(">" + str.substr(max_str_char_num_ * i, max_str_char_num_));
            }

            text_str_.push_back(str.substr(max_str_char_num_ * substr_num, str_len % max_str_char_num_));
        } else {
            text_str_.push_back(">" + str);
        }

        ++str_num;
    }

    //TODO WORKS?
    total_str_num_ = text_str_.size();
    first_displayed_str_ = (text_str_.size() >= max_displayed_str_num_ - 1 ?
                            text_str_.size() - max_displayed_str_num_ + 1 : 0);

    makeStrToDraw();
}

void List::draw(sf::RenderTarget &render, sf::RenderStates states) const {
    render.draw(sprite_);
    render.draw(button_up);
    render.draw(button_down);
    render.draw(text_);
}

void List::shiftDisplayedStrUp() {
    if (first_displayed_str_ > 0) {
        --first_displayed_str_;
    }

    makeStrToDraw();
}

void List::shiftDisplayedStrDown() {
    if (first_displayed_str_ + max_displayed_str_num_ < total_str_num_) {
        ++first_displayed_str_;
    }

    makeStrToDraw();
}

void List::makeStrToDraw() {
    size_t last_displayed_str = first_displayed_str_ + (total_str_num_ - first_displayed_str_ > max_displayed_str_num_ ?
                                                        max_displayed_str_num_ :
                                                        total_str_num_ - first_displayed_str_);
    str_to_draw_.clear();
    for (size_t i = first_displayed_str_; i < last_displayed_str; ++i) {
        str_to_draw_ += (text_str_[i] + "\n");
    }
    //str_to_draw_ += text_str_[last_displayed_str - 1];

    text_.setString(str_to_draw_);
}

