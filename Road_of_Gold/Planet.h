#pragma once
#include"Pos.h"


//���R���̐����ƃ}�b�v��̎��_�ړ����s��
struct Planet
{
	Grid<int>	voronoiMap;
	RectF		drawingRegion;
	RectF		smoothDrawingRegion;
	Texture		mapTexture;
	Optional<Pos>	gazePoint;

	Planet();
	void	setRegions();
	void	updateTransform();
	void	updateViewPointSliding();
	void	makeGroupsRandom();
	bool	loadNodeMap();
	bool	loadBiome();
	bool	loadVoronoiMap();
	Pos		getCursorPos() const;
	Transformer2D createTransformer(int _delta = 0) const;
};
extern Planet planet;