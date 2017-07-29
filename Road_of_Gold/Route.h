#pragma once

/*
2�̓s�s�Ԃ̃��[�g��\���B
pathIDs�����Ԃɂ��ǂ�ΖړI�n�ɒ����B
*/

struct Urban;

struct Route
{
	int		id;
	int		destinationUrbanID;	//�ړI�n�̓s�s
	int		originUrbanID;		//�o���n�̓s�s
	double	totalCost;			//�ړ��R�X�g
	bool	isSeaRoute;			//�C�ニ�[�g���ۂ�
	Array<int>	pathIDs;		//�o�R����p�X

	Route(int _id);
	Urban&	getDestinationUrban() const;
	Urban&	getOriginUrban() const;
	void	draw(const Color& _color) const;
};
extern Array<Route> routes;
void	makeRoute();