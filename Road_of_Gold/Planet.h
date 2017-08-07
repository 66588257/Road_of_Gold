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

	void	updateUrbans();
	void	updateGroups();
	void	updateCamera();
	void	drawUrbans() const;
	void	drawGroups() const;
	void	drawCamera() const;
};
extern Planet planet;