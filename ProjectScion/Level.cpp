#include <vector>
#include <math.h>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string>
#include "Level.h"
#include "Tile.h"
#include "StateManager.h"
#include "ScionEngine.h"

#include <noise/noise.h>
#include "noiseutils.h"
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <functional>
#include <algorithm>
#include <deque>
#include <array>

using namespace std;
using namespace noise;

namespace std
{
	template<>
	struct hash<Tile*>
	{
		std::size_t operator()(const Tile* tile) const
		{
			return std::hash<int>()(tile->x) ^ std::hash<int>()(tile->y);
		}
	};

	template<>
	struct hash<Tile>
	{
		std::size_t operator()(const Tile& tile) const
		{
			return std::hash<int>()(tile.x) ^ std::hash<int>()(tile.y);
		}
	};
}

struct PTileComp
{
	bool operator()(const Tile* lhs, const Tile* rhs) const
	{ return lhs->GetF() < rhs->GetF(); }
};

Level::Level(int w, int h) : w(w), h(h), camera(800, 600, 1.0f)
{
	SetDimensions(w, h);

	//camera = unique_ptr<Camera>(new Camera(800, 600, 0.2f));

}

Level::~Level()
{
}

void Level::SetDimensions(int w, int h)
{
	map.resize(h);

	for(int i = 0; i < w; i++)
		map.at(i).resize(w);

	for(int i = 0; i < w; i++)
		for(int j = 0; j < h; j++)
			map[j][i] = Tile(i, j);
}

void Level::LoadLevel()
{
	//tbd
}

void Level::Draw(sf::RenderWindow* window)
{  
	auto bounds = camera.GetTileBounds();
	auto camOffsetX = camera.GetTileOffset().x;
	auto camOffsetY = camera.GetTileOffset().y;

	for(int y = 0, tileY = bounds.top; y < bounds.height && tileY < h; y++, tileY++)
	{
		if(tileY < 0) continue;

		for(int x = 0, tileX = bounds.left; x < bounds.width && tileX < w; x++, tileX++)
		{
			if(tileX < 0) continue;

			GetTile(tileX, tileY).Draw(window);
		}
	}
}

//path finding stuff

float Level::HeuristicForNode(const Tile& start, const Tile& end)
{
	//used euclidean distance
	return (float)std::sqrtf(Tile::SIZE*(end.x - start.x) * Tile::SIZE*(end.x - start.x) + Tile::SIZE*(end.y - start.y) * Tile::SIZE*(end.y - start.y));
}

float Level::GetDistance(const Tile& s, const Tile& e) const
{
	//returns the distance between 2 nodes
	int xD = abs(e.x - s.x);
	int yD = abs(e.y - s.y);

	if (xD > yD || yD > xD) //1,0 or 0,1
		return 1.0f;
	else if (xD == yD) //1,1
	{
		return 9999.0f;
		//if (!GetTile(s.x + (e.x - s.x), s.y).solid && !GetTile(s.x, s.y + (e.y - s.y)).solid)
		//	return 0.9f;
		//else
		//	return 140.0f;
	}
	else
		return 0;
}

void Level::GetNeighbors(const Tile& n, deque<Tile>& neighbors)
{
	//returns the neighbors of a node

	if (n.x > 0)
	{
		neighbors.push_back(GetTile(n.x - 1, n.y));

		if (n.y > 0)
			neighbors.push_back(GetTile(n.x - 1, n.y - 1));
		if (n.y < h - 1)
			neighbors.push_back(GetTile(n.x - 1, n.y + 1));
	}

	if (n.y > 0)
		neighbors.push_back(GetTile(n.x, n.y - 1));

	if (n.x < w - 1)
	{
		neighbors.push_back(GetTile(n.x + 1, n.y));

		if (n.y > 0)
			neighbors.push_back(GetTile(n.x + 1, n.y - 1));
		if (n.y < h - 1)
			neighbors.push_back(GetTile(n.x + 1, n.y + 1));
	}

	if (n.y < h - 1)
		neighbors.push_back(GetTile(n.x, n.y + 1));
}

void Level::GetNeighbors(const Tile* n, std::deque<Tile*>& neighbors)
{
	//returns the neighbors of a node

	neighbors.clear();

	if (n->x > 0)
	{
		neighbors.push_back(&GetTile(n->x - 1, n->y));

		if (n->y > 0)
			neighbors.push_back(&GetTile(n->x - 1, n->y - 1));
		if (n->y < h - 1)
			neighbors.push_back(&GetTile(n->x - 1, n->y + 1));
	}

	if (n->y > 0)
		neighbors.push_back(&GetTile(n->x, n->y - 1));

	if (n->x < w - 1)
	{
		neighbors.push_back(&GetTile(n->x + 1, n->y));

		if (n->y > 0)
			neighbors.push_back(&GetTile(n->x + 1, n->y - 1));
		if (n->y < h - 1)
			neighbors.push_back(&GetTile(n->x + 1, n->y + 1));
	}

	if (n->y < h - 1)
		neighbors.push_back(&GetTile(n->x, n->y + 1));
}

deque<Tile>& Level::ConstructPath(std::unordered_map<Tile, Tile>& q, deque<Tile>& path, const Tile& t)
{
	//traverses the map and constructs a path given a node
	if (q.find(t) != q.end())
	{
		path = ConstructPath(q, path, q[t]);
		path.push_back(t);
		return path;
	}
	else
		return path;
}

deque<Tile> Level::FindPath(const Tile& start, const Tile& end)
{
	bool newGScoreBetter = false;
	unordered_set<Tile> closedSet;
	unordered_set<Tile> openSet;
	openSet.insert(start);

	std::unordered_map<Tile, Tile> navigated;
	std::unordered_map<Tile, float> gScore;
	std::unordered_map<Tile, float> hScore;
	std::map<float, list<Tile>> fScore; //easy way to use a structure to auto-sort for you

	gScore[start] = 0;
	hScore[start] = HeuristicForNode(start, end);
	fScore[gScore[start] + hScore[start]].push_back(start);

	deque<Tile> neighbors;

	while (openSet.size() > 0)
	{
		//take the lowest f-score node and dequeue it
		auto& lowestScore = fScore.begin()->second;
		Tile current = lowestScore.front();
		lowestScore.pop_front();

		if (lowestScore.size() == 0)
			fScore.erase(fScore.begin()); //just some maintenance to make sure you keep the lowest fscore with .First()

		if (current == end) //reached the end
		{
			deque<Tile> path;
			path.push_back(start);
			return ConstructPath(navigated, path, current);
		}

		openSet.erase(current); //remove current from open
		closedSet.insert(current); //put into closed

		GetNeighbors(current, neighbors);
		for(auto neighbor1 = std::begin(neighbors); neighbor1 != std::end(neighbors); neighbor1++)
		{
			auto& neighbor = *neighbor1;
			if (closedSet.find(neighbor) != closedSet.end() || neighbor.solid)
				continue;

			//newgscore is gscore + 1*distance
			float newGScore = gScore[current] + 1*GetDistance(current, neighbor);

			if (openSet.find(neighbor) == openSet.end())
			{
				openSet.insert(neighbor);
				hScore[neighbor] = HeuristicForNode(neighbor, end);
				newGScoreBetter = true;
			}
			else if (newGScore < gScore[neighbor])
				newGScoreBetter = true;
			else
				newGScoreBetter = false;

			if (newGScoreBetter)
			{
				navigated[neighbor] = current;
				gScore[neighbor] = newGScore;

				if (fScore.find(gScore[neighbor] + hScore[neighbor]) == fScore.end())
					fScore[gScore[neighbor] + hScore[neighbor]];

				fScore[gScore[neighbor] + hScore[neighbor]].push_back(neighbor);
			}
		}
	}

	//failed
	return deque<Tile>();
}

deque<Tile*> Level::ConstructPath(Tile* end)
{
	deque<Tile*> path;

	while(end)
	{
		path.push_front(end);
		end = end->parent;
	}

	return path;
}

deque<Tile*> Level::FindPath(Tile* start, Tile* end)
{
	for(int i = 0; i < w; i++)
		for(int j = 0; j < h; j++)
			map[j][i].G = map[j][i].H = 0; 
	
	start->H = start->ManhattanDistance(end);
	start->parent = nullptr;

	bool newGScoreBetter = false;
	std::unordered_set<Tile*> closedSet;
	deque<Tile*> openSet;
	openSet.push_back(start);
	end->parent = end;
	std::deque<Tile*> neighbors;

	while (openSet.size() > 0)
	{
		Tile* current = *openSet.begin();

		if(*current == *end)
			return ConstructPath(end);
		
		openSet.erase(openSet.begin());
		closedSet.insert(current);
				
		GetNeighbors(current, neighbors);
		for(auto neighbor1 = std::begin(neighbors); neighbor1 != std::end(neighbors); neighbor1++)
		{
			auto& neighbor = *neighbor1;
			if (closedSet.find(neighbor) != closedSet.end() || neighbor->solid)
				continue;

			//newgscore is gscore + 1*distance
			float newGScore = current->G + GetDistance(*current, *neighbor);

			if (find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
			{
				openSet.push_back(neighbor);
				newGScoreBetter = true;
			}
			else if (newGScore < neighbor->G)
				newGScoreBetter = true;
			else
				newGScoreBetter = false;

			if (newGScoreBetter)
			{
				neighbor->parent = current;
				neighbor->G = newGScore;
				double cross = abs((neighbor->x - end->x)*(start->y - end->y) - (start->x - end->x)*(neighbor->y - end->y));
				neighbor->H = neighbor->ManhattanDistance(end) + cross*0.001;
			}
		}

		sort(openSet.begin(), openSet.end(), PTileComp());
	}

	//failed
	return deque<Tile*>();
}
