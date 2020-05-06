//
// Created by airat on 04.05.2020.
//

#ifndef TBS_FIELD_DRAW_H
#define TBS_FIELD_DRAW_H

#include <vector>

#include "../primitives/Prim_game.hpp"
#include "hexagonal_tile.h"
#include "Button.h"

/*
 * you must update field_draw state in update() using updateField_draw() (!)
 */

/*
 * you may change color/outline color in game's update() function if you want
 */

class Field_draw : public sf::Drawable {
public:
    Field_draw(const gm::Field &field,
               const std::string &texture_file_button_disconnect,
               const sf::IntRect &dimensions_button_disconnect,
               const sf::Vector2f &point = {10, 10},
               const hexagonal_tile &hex_tile = { //default hexagon parameters for 10x10 field
                       {}, //cell init param is also redundant for field
                       sf::Vector2f(0, 0), //for field position of hex is redundant
                       "Media/Chilanka-Regular.ttf",
                       16,
                       sf::Color::Black,
                       45,
                       sf::Color::Red,
                       sf::Color::White,
                       1 // don't use more than thickness = 2
               });

public:

	void select(const sf::Vector2u &hex);
	void deselect(const sf::Vector2u &hex);
    void setColor(const sf::Vector2u &hex, const sf::Color &color);
    void setOutlineColor(const sf::Vector2u &hex, const sf::Color &color);

    //returns selected hex on the window, if click is not on the field, returns {width_ + 1, height_ + 1}
    //implementation is extremely naive: it checks if the point is in incircle
    sf::Vector2u hexSelected(const sf::Vector2i &point) const;

    void updateField_draw(const gm::Field &field);

    hexagonal_tile &operator[](const sf::Vector2u &v);
    const hexagonal_tile &operator[](const sf::Vector2u &v) const;

public:
    virtual void draw(sf::RenderTarget &render, sf::RenderStates states) const override;

public:
    Button button_disconnect;
	Button button_phase;

private:
    sf::Texture texture_;
    sf::Sprite sprite_;

    std::vector<std::vector<hexagonal_tile>> hexagonal_field_;
    sf::Vector2f position_;
    size_t width_;
    size_t height_;
};


#endif //TBS_FIELD_DRAW_H
