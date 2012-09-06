#include "Direction.h"

sf::Vector2i Direction::NORTH(-1, 0);
sf::Vector2i Direction::EAST(0, 1);
sf::Vector2i Direction::SOUTH(1, 0);
sf::Vector2i Direction::WEST(0, -1);

std::unordered_map<std::string, sf::Vector2i> Direction::DIRECTIONS;
std::unordered_map<std::string, std::string> Direction::OPPOSITE;