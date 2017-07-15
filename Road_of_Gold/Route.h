#pragma once

/*
2�̓s�s�Ԃ̃��[�g��\���B
pathIDs�����Ԃɂ��ǂ�ΖړI�n�ɒ����B
*/

struct Urban;

struct Route
{
	Route(int _id);
	Urban&	getDestinationUrban() const;
	Urban&	getOriginUrban() const;
	void	draw(const Color& _color) const;

	int		id;
	int		destinationUrbanID;
	int		originUrbanID;
	Array<int>	pathIDs;
	double	totalLength;
	bool	isSeaRoute;
};
extern Array<Route> routes;
void	makeRoute();