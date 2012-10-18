#ifndef _DIRECTION_H
#define _DIRECTION_H

#include <unordered_map>
#include <deque>
#include <string>
#include <algorithm>

#include <SFML\Graphics.hpp>

class Direction
{
public:
    static sf::Vector2i NORTH, EAST, SOUTH, WEST;

    static std::unordered_map<std::string, sf::Vector2i> DIRECTIONS;
    static std::unordered_map<std::string, std::string> OPPOSITE;

    static void InitializeDirections()
    {
        DIRECTIONS.clear();
        DIRECTIONS["north"] = NORTH;
        DIRECTIONS["east"]    = EAST;
        DIRECTIONS["south"] = SOUTH;
        DIRECTIONS["west"]    = WEST;
        
        OPPOSITE.clear();
        OPPOSITE["north"]    = "south";
        OPPOSITE["east"]    = "west";
        OPPOSITE["south"]    = "north";
        OPPOSITE["west"]    = "east";
    }

    static std::deque<std::string> GetKeys()
    {
        std::deque<std::string> dirKeys;

        std::for_each(begin(DIRECTIONS), end(DIRECTIONS), [&](std::pair<std::string, sf::Vector2i> val)
        {
            dirKeys.push_back(val.first);
        });

        return dirKeys;
    }

    static std::deque<std::string> ShuffleKeys()
    {
        auto&& dirKeys = GetKeys();

        std::random_shuffle(begin(dirKeys), end(dirKeys));
        return dirKeys;
    }
};

#endif