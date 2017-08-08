#pragma once
#include"Pos.h"

#include"Urban.h"
#include"Group.h"
#include"Node.h"
#include"Route.h"
#include"TinyCamera.h"


//���R���̐����ƃ}�b�v��̎��_�ړ����s��
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