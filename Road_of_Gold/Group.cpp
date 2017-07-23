#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"
#include"JSON.h"

Vehicle::Vehicle(int _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
	, progress(0)
	, sleepTimer(0)
	, stock()
{
	chain = {
		{ int16(2), int32(0)},
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(4), 0 },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(4), 0 },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(4), 0 },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(4), 0 },
		{ int16(3), iData.choice().id },
		{ int16(0), Random(int32(urbans.size() - 1)) },
		{ int16(4), 0 },
		{ int16(3), iData.choice().id },
		{ int16(1), int32(0)}
	};
}
bool	Vehicle::inRoute() const { return routeID != -1; }
Urban&	Vehicle::getNowUrban() const { return urbans[nowUrbanID]; }
Route&	Vehicle::getRoute() const { return routes[routeID]; }
Vec2		Vehicle::getMPos() const
{
	if (inRoute())
	{
		auto& r = getRoute();
		double length = routeProgress;
		for (int i = 0; i < r.pathIDs.size(); i++)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->length) length -= p->length;
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->length);
				return pos;
			}
		}
	}
	return getNowUrban().getPos().mPos;
}
void	Vehicle::draw() const
{
	const Circle shape(0.01);
	if (inRoute())
	{
		auto& r = getRoute();
		double length = routeProgress;
		for (int i = 0; i < r.pathIDs.size(); i++)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->length)
			{
				line.draw(0.005, Color(color, 64));
				length -= p->length;
			}
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->length);
				Line(line.begin, pos).draw(0.005, Color(color, 64));
				shape.movedBy(pos).draw(color).drawFrame(0.005, Palette::Black);
				break;
			}
		}
	}
	else
	{
		Triangle(-0.02, -0.03, 0.02, -0.03, 0.0, 0.0).movedBy(getMPos()).draw(color);
	}
}

Array<Group> groups;
Group::Group()
	: id(int(groups.size()))
	, name(L"hoge")
	, money(0)
{}
void Vehicle::update()
{
}

void Group::update()
{
	for (auto& v : vehicles)
	{
		double actionTime = timeSpeed;

		for (;;)
		{
			if (v.inRoute())
			{
				if (actionTime > v.getRoute().totalLength - v.routeProgress)
				{
					actionTime -= v.getRoute().totalLength - v.routeProgress;
					v.nowUrbanID = v.getRoute().destinationUrbanID;
					v.routeProgress = 0.0;
					v.routeID = -1;
				}
				else
				{
					v.routeProgress += actionTime;
					actionTime = 0.0;
					return;
				}
			}
			else if (v.sleepTimer > 0)
			{
				if (actionTime > v.sleepTimer)
				{
					actionTime -= v.sleepTimer;
					v.sleepTimer = 0;
				}
				else
				{
					v.sleepTimer -= actionTime;
					actionTime = 0;
					break;
				}
			}
			else
			{
				//�X�N���v�g�̎��s
				for (;;)
				{
					if (v.progress >= int(v.chain.size()) || v.sleepTimer > 0) break;
					switch (v.chain[v.progress].first)
					{
					case 0:	//�s�s�ֈړ�
					{
						Urban& targetUrban = urbans[v.chain[v.progress].second];
						for (auto& r : v.getNowUrban().getRoutes())
						{
							if (r->destinationUrbanID == targetUrban.id)
							{
								v.routeProgress = 0.0;
								v.routeID = r->id;
								break;
							}
						}
						++v.progress;

						if (v.inRoute()) break;	//���[�g�����܂����ꍇ
					}
					break;

					case 1:	//�A�h���X�W�����v����
					{
						v.progress = v.chain[v.progress].second;
					}
					break;

					case 2: //�E�F�C�g����
					{
						v.sleepTimer += 1.0;
						++v.progress;
					}
					break;

					case 3: //�w������
					{
						if (v.stock.num == 0)
						{
							const int itemType = v.chain[v.progress].second;
							const int numBuy = Min(10, v.getNowUrban().baskets[v.chain[v.progress].second].getNumItem());
							if (numBuy > 0) v.getNowUrban().baskets[itemType].buyItem(numBuy);
							v.stock.num = numBuy;
							v.stock.itemType = itemType;
						}
						++v.progress;
					}
					break;

					case 4:	//�̔�����
					{
						if (v.stock.num > 0)
						{
							v.getNowUrban().baskets[v.stock.itemType].addRing(10000, v.stock.num, this);
							v.stock.num = 0;
						}
					}

					default://���݂��Ȃ�����
					{
						++v.progress;
					}
					break;

					}
				}
			}
		}

	}
}