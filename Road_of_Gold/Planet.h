#pragma once
#include "Pos.h"


//���R���̐����ƃ}�b�v��̎��_�ړ����s��
struct Planet
{
	Planet();
	double	getHeight(const Pos& _pos, int _octave = 8) const;
	bool	isSea(const Pos& _pos) const;
	void	updateTransform();
	void	updateViewPointSliding();
	bool	loadBiome();
	bool	loadVoronoiMap();
	void	draw() const;
	int		getMoistureLevel(const Pos& _pos) const;
	int		getTemperatureLevel(const Pos& _pos) const;
	Pos		getCursorPos() const;
	Transformer2D createTransformer(int _delta = 0) const;

	Grid<int>	voronoiMap;
	RectF	drawingRegion;
	RectF	smoothDrawingRegion;
	Texture mapTexture;
	PerlinNoise	heightNoise;
	PerlinNoise moistureNoise;
	Optional<Pos>	gazePoint;
};
extern Planet planet;