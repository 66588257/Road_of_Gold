#pragma once
#include "Pos.h"

//���R���̐����ƃ}�b�v��̎��_�ړ����s��
struct Planet
{
	Planet();
	double	getHeight(const Pos& _pos, const int& _octave = 8) const;
	bool	isSea(const Pos& _pos) const;
	Mat3x2	getTransform(const int& _delta = 0) const;
	void	updateTransform();
	void	updateViewPointSliding();
	void	makeNewWorld(const int& _sizeX = 2048);
	void	draw() const;

	RectF	drawingRegion;
	RectF	smoothDrawingRegion;
	Texture mapTexture;
	PerlinNoise	heightNoise;
	Optional<Pos>	gazePoint;
};
extern Planet planet;