#include "Tile.h"
#include <SFML\Graphics.hpp>
#include "ScionEngine.h"

using namespace std;

const map<TILE_TYPE,Tile>& Tile::DefaultTiles()
{
	static map<TILE_TYPE, Tile> m;

	if(m.empty())
	{
		//m[UNUSED] = Tile();
		//m[BLANK] = Tile(BLANK, false);
		m[BLANK] = Tile(ScionEngine::GetTexture("tiles.png"), sf::IntRect(0, 32, 32, 32), BLANK, false);
		m[GROUND] = Tile(ScionEngine::GetTexture("tiles.png"), sf::IntRect(0, 0, 32, 32), GROUND, false);
		m[WALL] = Tile(ScionEngine::GetTexture("tiles.png"), sf::IntRect(32, 0, 32, 32), WALL);
	}

	return m;
}

Tile::Tile(TILE_TYPE type, bool solid, const sf::Color& color) : type(type), solid(solid), color(color)
{
}

Tile::Tile(sf::Texture* image, sf::IntRect rect, TILE_TYPE type, bool solid, const sf::Color& color) : baseSprite(*image, rect), type(type), solid(solid), color(color)
{
	//baseSprite.setOrigin(Tile::SIZE/2, Tile::SIZE/2);
}

Tile::~Tile()
{

}

void Tile::Draw(int x, int y, sf::RenderTexture* rt)
{
	baseSprite.setPosition((float)x, (float)y);
	rt->draw(baseSprite);
}


void Tile::Draw(int x, int y, sf::RenderWindow* rw)
{
	baseSprite.setColor(color);
	baseSprite.setPosition((float)x, (float)y);
	rw->draw(baseSprite);
}

