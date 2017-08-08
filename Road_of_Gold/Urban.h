﻿#pragma once

struct Group;
struct Node;
struct Pos;
struct Route;
struct Citizen;

struct Ring
{
	int		price;
	int		num;
	int		ownerGroupID;
	int		ownerCitizenID;

	Ring(int _price, int _num, const Group* _owner);
	Ring(int _price, int _num, const Citizen* _owner);
};
bool operator<(const Ring& _left, const Ring& _right);
bool operator>(const Ring& _left, const Ring& _right);

struct Basket
{
	int		joinedUrbanID;
	int		itemType;
	Array<int>	tradeLog;	//取引ログ
	Array<int>	chart;		//最安値ログ(日付単位)
	Array<int>	productionLog;	//生産ログ（日付単位）
	Array<int>	consumptionLog;	//消費ログ（日付単位）
	Array<Ring> rings;

	Basket(int _itemType, int _joinedUrbanID);
	String&	getItemName() const;
	void	addRing(int _price, int _num, const Group* _owner);
	void	addRing(int _price, int _num, const Citizen* _owner);
	int		getCost(int _num) const;
	int		getNumItem() const;
	void	buyItem(int _num);
	int		getPrice() const { return rings.front().price; }
	bool	isEmpty() const { return rings.isEmpty(); }
};

struct Citizen
{
	int		id;
	int		joinedUrbanID;
	int		citizenType;
	int		money;
	double	timer;
	double	progress;
	int		price;
	int		hapiness;
	double	personality;
	Array<int>	incomeLog;	//100日分の収入記録

	Citizen(int _id, int _citizenType, int _joinedUrbanID);
	int		avgIncome() const;
	void	addMoney(int _amount);	//外部からの収入
	void	update();
	void	goToShopping();
};

struct Urban
{
	int		id;
	String	name;
	int		joinedNodeID;
	double	timer;
	int		day;
	int		numCitizens;
	Array<Basket>	baskets;
	Array<Citizen>	citizens;
	Array<int>		avgIncome;	//各職業の収入平均
	Array<int>		resource;
	Array<double>	jobEfficiency;
	Array<int>		routeIDs;


	Urban(const JSONValue _json);
	Circle	getShape() const;
	void	update();
	void	draw() const;
	String	getTimeAsString() const;
	Array<Route*>	getRoutesToUrban(int _urbanID, double _maxRange, bool _isSeaRoute) const;

};
extern Urban*			selectedUrban;
extern Array<Urban>		urbans;