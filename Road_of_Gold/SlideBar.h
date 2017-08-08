#pragma once

struct SlideBar
{
	Vec2	pos;
	Vec2	size;
	double	value;	//0.0to1.0
	double	width;
	bool	isGrasped;
	double	graspedPos;

	SlideBar();
	SlideBar(Vec2 _pos, Vec2 _size, double _width);
	SlideBar(double _posX, double _posY, double _sizeX, double _sizeY, double _width = 16);
	void	update();
	void	draw() const;
	bool	isActive() const;
};