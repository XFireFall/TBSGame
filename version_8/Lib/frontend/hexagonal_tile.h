//
// Created by airat on 03.05.2020.
//

#ifndef TBS_HEXAGONAL_TILE_H
#define TBS_HEXAGONAL_TILE_H

#include <cstdint>
#include "SFML/Graphics.hpp"
#include "../primitives/Prim_game.hpp"

static const float text_shift = 20; //shift of the text in the hexagon in pixels

/*field cell_ is the reference to real cell which changes will be updated in update() with updateInfo() - but the
 *you also can store copy of cell (by rewriting the code), but in this case you have to update the cells' parameters in update() in due course
 */

class hexagonal_tile : public sf::Drawable {
public:
    hexagonal_tile(const gm::Cell &cell,
                   const sf::Vector2f &position,
                   const std::string &font_file,
                   int font_size,
                   const sf::Color &font_color,
                   unsigned int side_len,
                   const sf::Color &color,
                   const sf::Color &outline_color,
                   unsigned int outline_thickness);

    void setFont(const sf::Font &font);

    void setFontSize(unsigned int size);

    void setPosition(const sf::Vector2f &point);

	
	void select(void);
	void deselect(void);
    void setColor(const sf::Color &color);

    void setFontColor(const sf::Color &color);

    void setOutlineColor(const sf::Color &color);

    void setOutlineThickness(const unsigned int &thickness);

    sf::IntRect getDimensions() const;

    sf::Vector2f getCenter() const;

    unsigned getSideLen() const;

    unsigned getOutlineThickness() const;

    gm::Cell getCell() const;

    void copyCell(const gm::Cell &cell);

    void updateInfo();
public:
    virtual void draw(sf::RenderTarget &render, sf::RenderStates states) const override;

private:
    sf::CircleShape hexagon_;

    unsigned int side_len_;
    sf::Color color_;
    sf::Color outline_color_;
    unsigned int outline_thickness_;

    gm::Cell cell_;

    mutable sf::Text text_;
    sf::IntRect dimensions_;

    sf::Font font_;
};


#endif //TBS_HEXAGONAL_TILE_H
