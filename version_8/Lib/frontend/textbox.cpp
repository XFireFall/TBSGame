//
// Created by airat on 29.04.2020.
//

#include "textbox.h"

Textbox::Textbox(const std::string &texture_file,
                 const sf::IntRect &dimensions,
                 const std::string &font_file,
                 int size,
                 const sf::Color &color,
                 bool sel,
                 const std::string &default_str,
                 int max_string_length,
                 bool moving_window,
                 unsigned max_displayed_char) : isSelected_(sel),
                                                dimensions_(dimensions),
                                                max_string_length(max_string_length),
                                                moving_window_(moving_window),
                                                max_displayed_char_(--max_displayed_char) {

    if (!texture_.loadFromFile(texture_file)) {
        throw std::runtime_error("Cannot load texture");
    }

    sprite_.setTexture(texture_);

    if (!font_.loadFromFile(font_file)) {
        throw std::runtime_error("Cannot load font");
    }

    textbox_.setFillColor(color);
    textbox_.setCharacterSize(size);
    textbox_.setFont(font_);

    this->setLimit(true, max_string_length);
    this->setPosition({static_cast<float>(this->getDimensions().left),
                       static_cast<float>(this->getDimensions().top)});

    if (isSelected_) {
        textbox_.setString("_");
    } else {
        if (!hasLimit_) {
            textbox_.setString(default_str);
            text_ = default_str;
        } else {
            if (default_str.length() > limit_) {
                throw std::invalid_argument("String is too long");
            }

            textbox_.setString(default_str);
            text_ = default_str;
        }
    }
}

void Textbox::setFont(const sf::Font &fonts) {
    font_ = fonts;
    textbox_.setFont(font_);
}

void Textbox::setPosition(const sf::Vector2f &point) {
    textbox_.setPosition(point + sf::Vector2f(10, 3)); //
    sprite_.setPosition(point);
}

void Textbox::setLimit(bool ToF) {
    hasLimit_ = ToF;
}

void Textbox::setLimit(bool ToF, int lim) {
    hasLimit_ = ToF;
    limit_ = lim - 1;
}

void Textbox::setSelected(bool sel) {
    isSelected_ = sel;

    if (!sel) {
        std::string newT;
        for (char i : text_) {
            newT += i;
        }
        textbox_.setString(newT);
    }
}

std::string Textbox::getText(bool clear_text) {
    if (!clear_text) {
        return text_;
    } else {
        std::string tmp = text_;
        this->clear();
        return tmp;
    }
}

void Textbox::drawTo(sf::RenderWindow &window) const {
    window.draw(sprite_);
    window.draw(textbox_);
}

void Textbox::draw(sf::RenderTarget &render, sf::RenderStates states) const {
    render.draw(sprite_);
    render.draw(textbox_);
}

void Textbox::typedOn(sf::Event input) {
    if (isSelected_) {
        int charTyped = input.text.unicode;

        if (charTyped < 128) {
            if (!moving_window_) {

                if (hasLimit_) {
                    if (text_.length() <= limit_) {
                        inputLogic(charTyped);
                    } else if (text_.length() > limit_ && charTyped == DELETE_KEY) {
                        deleteLastChar();
                    }
                } else {
                    inputLogic(charTyped);
                }

            } else {
                if (hasLimit_) {
                    if (text_.length() <= limit_) {
                        inputLogic(charTyped);
                    } else if (text_.length() > limit_ && charTyped == DELETE_KEY) {
                        deleteLastChar();
                    } else if (text_.length() > limit_) {
                        inputLogic(charTyped);
                    }
                } else {
                    ++first_displayed_char;
                    inputLogic(charTyped);
                }

            }
        }
    }
}

void Textbox::deleteLastChar() {
    (first_displayed_char > 0 ? --first_displayed_char : first_displayed_char);
    text_ = text_.substr(0, text_.length() - 1);
    textbox_.setString(text_ + "_");
}

void Textbox::inputLogic(int charTyped) {
    if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
        if (text_.length() >= max_displayed_char_) {
            ++first_displayed_char;
        }

        text_ += static_cast<char>(charTyped);
    } else if (charTyped == DELETE_KEY) {
        if (text_.length() > 0) {
            deleteLastChar();
        }
    }

    if (!moving_window_) {
        textbox_.setString(text_ + "_");
    } else {
        display_text_ = text_.substr(first_displayed_char, text_.length() - first_displayed_char);
        textbox_.setString(display_text_ + "_");
    }
}

sf::IntRect Textbox::getDimensions() const {
    return dimensions_;
}

bool Textbox::getSelected() const {
    return isSelected_;
}

void Textbox::clear() {
    text_ = "";
    display_text_ = "";
    first_displayed_char = 0;
    textbox_.setString(text_);
}

