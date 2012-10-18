#ifndef _TILE_H
#define _TILE_H

#include <SFML\Graphics.hpp>
#include <map>
#include <string>
#include <math.h>


class Tile
{
    friend bool operator==(const Tile& lhs, const Tile& rhs);
public:
    enum TYPE
    {
        UNUSED,
        BLANK,
        GROUND,
        WALL,
        CORRIDOR
    };

    Tile::TYPE type;
    bool entrance;

    sf::Color color;
    int x, y;

    bool touched;

    //pathfinding
    float G, H;
    inline float GetF() const { return G + H; }
    inline float ManhattanDistance(const Tile* tile) const
    {
        return (float)std::abs(tile->x - x) + (float)std::abs(tile->y - y);        
    }
    Tile* parent;
    //end pathfinding

    inline static int TileLookup(unsigned __int8 hash)
    {
        unsigned __int8 higherBits = hash >> 4, lowerBits = (hash << 4);
        lowerBits >>= 4;

        higherBits &= 15;
        
        if(higherBits == 0xF) //1111
        {
            switch(lowerBits)
            {
            case 0xF:
                return 12;
            case 0xE:
                return 27;
            case 0xD:
                return 7;
            case 0xC:
                return 17;
            case 0xB:
                return 9;
            case 0xA:
                return 42;
            case 0x9:
                return 8;
            case 0x8:
                return 38;
            case 0x7:
                return 29;
            case 0x6:
                return 28;
            case 0x5:
                return 43;
            case 0x4:
                return 48;
            case 0x3:
                return 19;
            case 0x2:
                return 49;
            case 0x1:
                return 39;
            case 0x0:
                return 18;
            }
        }
        else if(higherBits == 0xE) //1110
        {
            unsigned __int8 middleBits = (lowerBits << 5);
            middleBits >>= 6;

            switch(middleBits)
            {
            case 0x3: //11
                return 2;
            case 0x2:
                return 34;
            case 0x1:
                return 37;
            case 0x0:
                return 5;
            }
        }
        else if(higherBits == 0xD) //1101
        {
            unsigned __int8 leftBits = (lowerBits >> 2);

            switch(leftBits)
            {
            case 0x3:
                return 13;
            case 0x2:
                return 35;
            case 0x1:
                return 47;
            case 0x0:
                return 16;
            }
        }
        else if(higherBits == 0xC) //1100
        {
            unsigned __int8 leftMiddleBit = (lowerBits << 5);
            leftMiddleBit >>= 7;

            switch(leftMiddleBit)
            {
            case 1:
                return 3;
            case 0:
                return 6;
            }
        }
        else if(higherBits == 0xB) //1011
        {
            unsigned __int8 leftBit    = (lowerBits >> 3), 
                rightBit    = (lowerBits << 7);
            rightBit >>= 7;

            if(leftBit == rightBit)
            {
                if(leftBit == 1) // 1xx1
                    return 22;
                else //0xx0
                    return 25;
            }
            else if(leftBit == 1 && rightBit == 0)
                return 46;
            else if(leftBit == 0 && rightBit == 1)
                return 45;
        }
        else if(higherBits == 0xA) //1010
            return 32;
        else if(higherBits == 0x9) //1001
        {
            unsigned __int8 leftBit = (lowerBits << 4);
            leftBit >>= 7;

            if(leftBit == 1) 
                return 23;
            else 
                return 26;
        }
        else if(higherBits == 0x8) //1000
            return 33;
        else if(higherBits == 0x7) //0111
        {
            unsigned __int8 rightBits = (lowerBits << 6);
            rightBits >>= 6;

            switch(rightBits)
            {
            case 0x3:
                return 11;
            case 0x2:
                return 44;
            case 0x1:
                return 36;
            case 0x0:
                return 14;
            }
        }
        else if(higherBits == 0x6) //0110
        {
            unsigned __int8 rightCenterBit = (lowerBits << 6);
            rightCenterBit >>= 7;

            if(rightCenterBit == 1)
                return 1;
            else
                return 4;
        }
        else if(higherBits == 0x5) //0101
            return 10;
        else if(higherBits == 0x4) //0100
            return 0;
        else if(higherBits == 0x3) //0011
        {
            unsigned __int8 rightBit = (lowerBits << 7);
            rightBit >>= 7;

            if(rightBit == 1)
                return 21;
            else
                return 24;
        }
        else if(higherBits == 0x2) //0010
            return 31;
        else if(higherBits == 0x1) //0001
            return 20;
        else
            return 15;
    }

    static std::map<Tile::TYPE,Tile>& DefaultTiles();

    sf::Sprite baseSprite;
    bool solid;
    static const int SIZE = 32;
    
    Tile() : x(0), y(0), solid(false), type(UNUSED), entrance(false), touched(false) {}
    Tile(int x, int y) : x(x), y(y), solid(false), type(UNUSED), entrance(false), touched(false) {}
    explicit Tile(int x, int y, Tile::TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);
    explicit Tile(int x, int y, sf::Texture* image, sf::IntRect rect, Tile::TYPE type, bool solid = true, const sf::Color& color = sf::Color::White);

    ~Tile();

    void Draw(sf::RenderWindow* rw);
    void Draw(sf::RenderTexture* rt, sf::Color c=sf::Color::White, sf::RenderStates* states = nullptr);

    inline void SetRect(const sf::IntRect& rect) { baseSprite.setTextureRect(rect); }
};

bool operator==(const Tile& lhs, const Tile& rhs);
bool operator<(const Tile& lhs, const Tile& rhs);
bool operator!=(const Tile& lhs, const Tile& rhs);

#endif