//
// Created by airat on 03.05.2020.
//

#include "hexagonal_tile.h"

hexagonal_tile::hexagonal_tile(const gm::Cell &cell,
                               const sf::Vector2f &position,
                               const std::string &font_file,
                               int font_size,
                               const sf::Color &font_color,
                               unsigned int side_len,
                               const sf::Color &color,
                               const sf::Color &outline_color,
                               unsigned int outline_thickness) : cell_(cell),
                                                                 hexagon_(static_cast<float >(side_len), 6),
                                                                 side_len_(side_len),
                                                                 dimensions_(sf::IntRect(position.x,
                                                                                         position.y,
                                                                                         2 * side_len,
                                                                                         2 * side_len)),
                                                                 color_(color),
                                                                 outline_color_(outline_color),
                                                                 outline_thickness_(outline_thickness) {

    hexagon_.setOutlineColor(outline_color_);
    hexagon_.setOutlineThickness(outline_thickness_);
    hexagon_.setFillColor(color_);

    if (!font_.loadFromFile(font_file)) {
        throw std::runtime_error("Cannot load font");
    }

    text_.setFillColor(font_color);
    text_.setCharacterSize(font_size);
    text_.setFont(font_);

    updateInfo(); //set initial text

    this->setPosition({static_cast<float>(this->getDimensions().left),
                       static_cast<float>(this->getDimensions().top)});
}

sf::IntRect hexagonal_tile::getDimensions() const {
    return dimensions_;
}

sf::Vector2f hexagonal_tile::getCenter() const {
    return sf::Vector2f(
            static_cast<float>(this->getDimensions().left) + static_cast<float>(this->getDimensions().width) / 2,
            static_cast<float>(this->getDimensions().top) + static_cast<float>(this->getDimensions().height) / 2);
}

void hexagonal_tile::setFont(const sf::Font &font) {
    font_ = font;
    text_.setFont(font_);
}

void hexagonal_tile::setFontSize(unsigned int size) {
    text_.setCharacterSize(size);
}

void hexagonal_tile::select() {
	hexagon_.setFillColor(sf::Color::White);
}

void hexagonal_tile::deselect() {
	hexagon_.setFillColor(color_);
}

void hexagonal_tile::setColor(const sf::Color &color) {
    color_ = color;
	if (hexagon_.getFillColor() != sf::Color::White)
		hexagon_.setFillColor(color_);
} 

void hexagonal_tile::setFontColor(const sf::Color &color) {
    text_.setFillColor(color);
}

void hexagonal_tile::setOutlineColor(const sf::Color &color) {
    outline_color_ = color;
}

void hexagonal_tile::setOutlineThickness(const unsigned int &thickness) {
    outline_thickness_ = thickness;
}

void hexagonal_tile::draw(sf::RenderTarget &render, sf::RenderStates states) const {
    render.draw(hexagon_);
    text_.setFont(font_);
    render.draw(text_);
}

void hexagonal_tile::updateInfo() {
	switch (cell_._owner) {
		case 0:
			color_ = sf::Color(87, 65, 37);
			break;
		case 1:
			color_ = sf::Color::Red;
			break;
		case 2:
			color_ = sf::Color::Blue;
			break;
		case 3:
			color_ = sf::Color::Green;
			break;
		default:
			color_ = sf::Color::Cyan;
			break;
	}
	if (hexagon_.getFillColor() != sf::Color::White)
		hexagon_.setFillColor(color_);
	
	text_.setString(" " + std::to_string(cell_._size));
//     text_.setString(std::to_string(cell_._size) + "/" +
//                     std::to_string(cell_._capacity) + " [" +
//                     std::to_string(cell_._owner) + "]");
}

// you need to align the manually if you change parameters roughly
void hexagonal_tile::setPosition(const sf::Vector2f &point) {
    hexagon_.setPosition(point);
    dimensions_.left = point.x;
    dimensions_.top = point.y;
    text_.setPosition({point.x + text_shift,   //text prints in the center of the hex with text_shift shift from the left in pixels
                       point.y +
                       static_cast<float>(this->getDimensions().height) / 2 -
                       static_cast<float>(this->text_.getCharacterSize()) / 2});
}

unsigned hexagonal_tile::getSideLen() const {
    return dimensions_.width / 2;
}

void hexagonal_tile::copyCell(const gm::Cell &cell) {
    cell_ = cell;
    this->updateInfo();
}

unsigned hexagonal_tile::getOutlineThickness() const {
    return outline_thickness_;
}

gm::Cell hexagonal_tile::getCell() const {
    return cell_;
}
