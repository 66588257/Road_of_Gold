#pragma once
#include"Pos.h"

#include"Urban.h"
#include"Group.h"
#include"Node.h"
#include"Route.h"
#include"TinyCamera.h"


//自然環境の生成とマップ上の視点移動を行う
struct Planet
{
	Grid<int>	voronoiMap;
	Texture		mapTexture;

	Planet();
	void	makeGroupsRandom();
	bool	loadNodeMap();
	bool	loadBiome();
	bool	loadVoronoiMap();
};
extern Planet planet;