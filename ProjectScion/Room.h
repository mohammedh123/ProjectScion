#ifndef _ROOM_H
#define _ROOM_H

#include <SFML\Graphics.hpp>

struct ROOM
{    
    enum TYPE
    {
        RECTANGULAR,
        STARBURST,
        CIRCULAR,
        ROTATED
    };
    enum SIZE
    {
        //value is max-size
        SMALL = 5,
        MEDIUM = 11,
        LARGE = 11,
        HUGE = 17,
        GARGANTUAN = 23,
        COLOSSAL = 29
    };

    int x, y, w, h, doorX, doorY;

    TYPE type;
    
    inline bool CollidesWith(ROOM& r, int border = 0) const
    {
        return !(r.x - border >= x + w + border ||
            r.x + r.w + border <= x  - border ||
            r.y - border >= y + h + border ||
            r.y + r.h + border <= y - border);
    }

    inline bool CollidesWith(ROOM& r, int thisBorder, int otherBorder, ROOM* output_collision = nullptr) const
    {
        if(output_collision)
        {
            sf::IntRect r1(r.x - otherBorder, r.y - otherBorder, r.w + otherBorder*2, r.h + otherBorder*2);
            sf::IntRect r2(x - thisBorder, y - thisBorder, w + thisBorder*2, h + thisBorder*2);
            sf::IntRect result;

            bool res = r2.intersects(r1, result);

            output_collision->x = result.left;
            output_collision->y = result.top;
            output_collision->w = result.width;
            output_collision->h = result.height;

            return res;
        }
        else
            return !(r.x - otherBorder >= x + w + thisBorder ||
            r.x + r.w + otherBorder <= x  - thisBorder ||
            r.y - otherBorder >= y + h + thisBorder ||
            r.y + r.h + otherBorder <= y - thisBorder);
    }

    inline bool GetCollisionRect(ROOM& r, sf::IntRect thisBorder, sf::IntRect otherBorder, ROOM* output_collision) const
    {
        sf::IntRect r1(r.x - otherBorder.left, r.y - otherBorder.top, r.w + otherBorder.width+otherBorder.left, r.h + otherBorder.height+otherBorder.top);
        sf::IntRect r2(x - thisBorder.left, y - thisBorder.top, w + thisBorder.width+thisBorder.left, h + thisBorder.top+thisBorder.height);
        sf::IntRect result;

        bool res = r2.intersects(r1, result);

        output_collision->x = result.left;
        output_collision->y = result.top;
        output_collision->w = result.width;
        output_collision->h = result.height;

        return res;
    }

    inline void Shrink(int x, int y, int w, int h)
    {
        this->x -= x;
        this->y -= y;
        this->w -= w;
        this->h -= h;
    }
};

bool operator==(const ROOM& lhs, const ROOM& rhs);
bool operator!=(const ROOM& lhs, const ROOM& rhs);

#endif