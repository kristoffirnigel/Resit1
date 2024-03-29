#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "ResitMouseGame.h"

int createBackground(VertexArray& rVA, IntRect arena)
{
	const int TILE_SIZE = 180; //the types and sizes of different background tiles

	const int TILE_TYPES = 3;

	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;

	int worldHeight = arena.height / TILE_SIZE;

	rVA.setPrimitiveType(Quads); //working with quads !!

	rVA.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0; //start the vertex array from here

	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE); //positioning verteces
			rVA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f((w * TILE_SIZE), (h * TILE_SIZE) + TILE_SIZE);

			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1) //setting up the borders of the map using the wall texture
			{
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else //the floor types are selected randomly
			{
				srand((int)time(0) + h * w - h); 
				int randTile = (rand() % TILE_TYPES);
				int verticalOffset = randTile * TILE_SIZE;
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
			}

			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}

	return TILE_SIZE;
}