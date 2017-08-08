#include"SlideBar.h"

SlideBar::SlideBar()
	: pos(0, 0)
	, size(0, 0)
	, value(0)
	, width(0)
	, graspedPos(0)
	, isGrasped(false)
{}
SlideBar::SlideBar(Vec2 _pos, Vec2 _size, double _width)
	: pos(_pos)
	, size(_size)
	, value(0)
	, width(_width)
	, graspedPos(0)
	, isGrasped(false)
{}
SlideBar::SlideBar(double _posX, double _posY, double _sizeX, double _sizeY, double _width)
	: pos(_posX, _posY)
	, size(_sizeX, _sizeY)
	, value(0)
	, width(_width)
	, graspedPos(0)
	, isGrasped(false)
{}

void SlideBar::update()
{
	//ÉGÉâÅ[èàóù
	if (size.y <= width) return;

	const auto p = pos.lerp(pos.movedBy(0, size.y - width), value);
	const RectF frame(pos, size);
	const RectF rect(p, size.x, width);

	if (frame.leftClicked()) isGrasped = true;
	if (!MouseL.pressed()) isGrasped = false;
	if (MouseL.pressed() && isGrasped)
	{
		value += Cursor::DeltaF().y / (size.y - width);
		if (value < 0) value = 0;
		if (value > 1) value = 1;
	}
}

void SlideBar::draw() const
{
	const auto p = pos.lerp(pos.movedBy(0, size.y - width), value);
	const RectF frame(pos, size);
	const RectF rect(p, size.x, width);

	frame.drawFrame(2, Palette::Red);
	rect.draw(Color(Palette::Red, isGrasped ? 192 : 64)).drawFrame(2, Palette::Red);
}

bool SlideBar::isActive() const
{
	return isGrasped;
}