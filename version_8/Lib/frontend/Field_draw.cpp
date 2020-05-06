//
// Created by airat on 04.05.2020.
//

#include <cmath>

#include "Field_draw.h"

Field_draw::Field_draw(const gm::Field &field,
                       const std::string &texture_file_button_disconnect,
                       const sf::IntRect &dimensions_button_disconnect,
                       const sf::Vector2f &point,
                       const hexagonal_tile &hex_tile) : width_(field.width()),
                                                         height_(field.height()),
                                                         button_disconnect(texture_file_button_disconnect,
                                                                           dimensions_button_disconnect),
														button_phase("Media/phase.png", sf::IntRect{{220, 660}, {20, 20}}),
                                                         position_(point),
                                                         hexagonal_field_(
                                                                 field.height(), //fill the vector with hex_tiles with needed size
                                                                 std::vector<hexagonal_tile>(field.width(), hex_tile)) {
    float side_len = static_cast<float>(hex_tile.getSideLen());
    float outline_thickness = static_cast<float>(hex_tile.getOutlineThickness());

    float horizontal_shift = 0;
    float horizontal_shift_step = side_len * sqrtf(3) + 2 * outline_thickness;
    float vertical_shift_step = (side_len + outline_thickness) * 1.5;
    float horizontal_shift_even_row = side_len * sqrtf(3) / 2 + outline_thickness;

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            (*this)[{x, y}].copyCell(field[{x, y}]);
            (*this)[{x, y}].setPosition(point +
                                        sf::Vector2f{horizontal_shift + horizontal_shift_step * static_cast<float>(x),
                                                     vertical_shift_step * static_cast<float>(y)});
        }

        if (y % 2 == 0) { // even row
            horizontal_shift = horizontal_shift_even_row;
        } else { //odd row
            horizontal_shift = 0;
        }

    }
}

void Field_draw::select(const sf::Vector2u &hex)
{
	(*this)[hex].select();
}

void Field_draw::deselect(const sf::Vector2u &hex)
{
	(*this)[hex].deselect();
}

void Field_draw::draw(sf::RenderTarget &render, sf::RenderStates states) const {
    for (size_t y = 0; y < height_; ++y) {
        for (size_t x = 0; x < width_; ++x) {
            render.draw(hexagonal_field_[y][x]);
        }
    }

    render.draw(button_disconnect);
	render.draw(button_phase);
}

hexagonal_tile &Field_draw::operator[](const sf::Vector2u &v) {
    return hexagonal_field_[v.y][v.x];
}

const hexagonal_tile &Field_draw::operator[](const sf::Vector2u &v) const {
    return hexagonal_field_[v.y][v.x];
}

void Field_draw::setColor(const sf::Vector2u &hex, const sf::Color &color) {
    (*this)[hex].setColor(color);
}

void Field_draw::setOutlineColor(const sf::Vector2u &hex, const sf::Color &color) {
    (*this)[hex].setOutlineColor(color);
}

float dist(const sf::Vector2f &a, const sf::Vector2f &b) {
    return sqrtf((a.x - b.x) * (a.x - b.x) +
                 (a.y - b.y) * (a.y - b.y));
}

sf::Vector2u Field_draw::hexSelected(const sf::Vector2i &point) const {
    sf::Vector2u res = {};
    sf::Vector2f center = {};
    static float sqrt3_2 = sqrtf(3) / 2;
    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            if ((*this)[{x, y}].getDimensions().contains(point.x, point.y)) {
                res = {x, y};
                center = (*this)[{x, y}].getCenter();
                float incircle_radius = (*this)[{x, y}].getSideLen() * sqrt3_2;
                float d = dist({static_cast<float >(point.x), static_cast<float >(point.y)}, center);
                if (incircle_radius >= d) {
                    return res;
                }
            }
        }
    }

    return sf::Vector2u{static_cast<unsigned int>(width_ + 1),
                        static_cast<unsigned int>(height_ + 1)};
}

void Field_draw::updateField_draw(const gm::Field &field) {
    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            (*this)[{x, y}].copyCell(field[{x, y}]);
        }
    }
}
