//
// Created by airat on 28.04.2020.
//

#ifndef TBS_BUTTON_H
#define TBS_BUTTON_H


#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {
public:
    Button(const std::string &texture_file,
           const sf::IntRect &dimensions);

    sf::IntRect getDimensions() const;

    void drawTo(sf::RenderWindow &window) const;

    virtual void draw(sf::RenderTarget &render, sf::RenderStates states) const override;

private:
    sf::Texture texture_;
    sf::Sprite sprite_;
    sf::IntRect dimensions_;
};


#endif //TBS_BUTTON_H
