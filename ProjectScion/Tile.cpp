#include "Tile.h"
#include <SFML\Graphics.hpp>
#include "ScionEngine.h"

using namespace std;

const map<Tile::TYPE,Tile>& Tile::DefaultTiles()
{
	static map<Tile::TYPE, Tile> m;

	if(m.empty())
	{
		m[UNUSED] = Tile();
		//m[BLANK] = Tile(BLANK, false);
		m[BLANK] = Tile(0, 0, ScionEngine::GetTexture("newtiles.png"), sf::IntRect(0, 32, 32, 32), BLANK, false);
		m[GROUND] = Tile(0, 0, ScionEngine::GetTexture("newtiles.png"), sf::IntRect(0, 0, 32, 32), GROUND, false);
		m[WALL] = Tile(0, 0, ScionEngine::GetTexture("newtiles.png"), sf::IntRect(32, 0, 32, 32), WALL);
		m[CORRIDOR] = Tile(0, 0, ScionEngine::GetTexture("newtiles.png"), sf::IntRect(0, 32, 32, 32), CORRIDOR, false);
	}

	return m;
}

Tile::Tile(int x, int y, Tile::TYPE type, bool solid, const sf::Color& color) : x(x), y(y), type(type), solid(solid), color(color), touched(false)
{
}

Tile::Tile(int x, int y, sf::Texture* image, sf::IntRect rect, Tile::TYPE type, bool solid, const sf::Color& color) : x(x), y(y), baseSprite(*image, rect), type(type), solid(solid), color(color), touched(false)
{
	//baseSprite.setOrigin(Tile::SIZE/2, Tile::SIZE/2);
}

Tile::~Tile()
{

}

void Tile::Draw(sf::RenderTexture* rt)
{
	baseSprite.setPosition((float)x*SIZE, (float)y*SIZE);
	rt->draw(baseSprite);
}


void Tile::Draw(sf::RenderWindow* rw)
{
	baseSprite.setColor(color);
	baseSprite.setPosition((float)x*SIZE, (float)y*SIZE);
	rw->draw(baseSprite);
}

bool operator==(const Tile& lhs, const Tile& rhs)
{
	return lhs.type == rhs.type && lhs.solid == rhs.solid && lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Tile& lhs, const Tile& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const Tile& lhs, const Tile& rhs)
{
	return lhs.GetF() < rhs.GetF();
}