#pragma once

/*
2�̓s�s�Ԃ̃��[�g��\���B
pathIDs�����Ԃɂ��ǂ�ΖړI�n�ɒ����B
*/

struct Urban;

struct Route
{
	int		id;
	int		destinationUrbanID;
	int		originUrbanID;
	double	totalLength;
	bool	isSeaRoute;
	Array<int>	pathIDs;

	Route(int _id);
	Urban&	getDestinationUrban() const;
	Urban&	getOriginUrban() const;
	void	draw(const Color& _color) const;
};
extern Array<Route> routes;
void	makeRoute();