//
// Created by airat on 28.04.2020.
//

#include "Button.h"

Button::Button(const std::string &texture_file,
               const sf::IntRect &dimensions) : dimensions_(dimensions) {

    if (!texture_.loadFromFile(texture_file)) {
        throw std::runtime_error("Cannot load texture");
    }

    sprite_.setTexture(texture_);
    sprite_.setPosition(dimensions.left, dimensions.top);
    //sprite_.setTextureRect(dimensions_);
}

void Button::drawTo(sf::RenderWindow &window) const {
    window.draw(sprite_);
}

sf::IntRect Button::getDimensions() const {
    return dimensions_;
}

void Button::draw(sf::RenderTarget &render, sf::RenderStates states) const {
    render.draw(sprite_);
}
