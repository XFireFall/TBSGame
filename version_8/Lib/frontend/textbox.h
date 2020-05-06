//
// Created by airat on 29.04.2020.
//

#ifndef TBS_TEXTBOX_H
#define TBS_TEXTBOX_H

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

// Define keys:
static const int DELETE_KEY = 8;
static const int ENTER_KEY = 13;
static const int ESCAPE_KEY = 27;

static const int max_string_length_default = 15;

class Textbox : public sf::Drawable {
public:
    Textbox(const std::string &texture_file,
            const sf::IntRect &dimensions,
            const std::string &font_file,
            int size,
            const sf::Color &color,
            bool sel,
            const std::string &default_str,
            int max_string_length = max_string_length_default,
            bool moving_window = false,
            unsigned max_displayed_char = max_string_length_default);

    // Make sure font is passed by reference:
    void setFont(const sf::Font &fonts);

    void setPosition(const sf::Vector2f &point);

    // Set char limits:
    void setLimit(bool ToF);

    void setLimit(bool ToF, int lim);

    // Change selected state:
    void setSelected(bool sel);

    bool getSelected() const;

    sf::IntRect getDimensions() const;

    //read written text in textbox
    //bool: true if you want to clear text after reading
    std::string getText(bool clear_text = false);

    void clear();

public:

    virtual void draw(sf::RenderTarget &render, sf::RenderStates states) const override;

    void drawTo(sf::RenderWindow &window) const;

public:

    // Function for event loop:
    void typedOn(sf::Event input);

    const unsigned max_displayed_char_;

private:
    mutable sf::Text textbox_;
    sf::IntRect dimensions_;

    sf::Texture texture_;
    sf::Sprite sprite_;

    sf::Font font_;
    std::string text_;
    std::string display_text_;

    int max_string_length;
    bool moving_window_;

    unsigned first_displayed_char = 0;

    bool isSelected_ = false;
    bool hasLimit_ = false;
    int limit_ = 0;

    // Delete the last character of the text_:
    void deleteLastChar();

    // Get user input:
    void inputLogic(int charTyped);
};


#endif //TBS_TEXTBOX_H
