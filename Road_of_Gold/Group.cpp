#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"
#include"JSON.h"

enum struct Command {
	MOVE,	//�s�s�Ɉړ�
	JUMP,	//���s�ʒu�̕ύX
	WAIT,	//���s�x�~
	BUY,	//�w��
	SELL,	//�̔�
};

Vehicle::Vehicle(int _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
	, progress(0)
	, sleepTimer(0)
	, stock()
{
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
	, color(RandomColor())
{}
void Vehicle::update()
{
}

void Group::update()
{
	for (auto& v : vehicles)
	{
		if (v.chain.isEmpty())
		{
			auto& u1 = v.getNowUrban();
			auto& u2 = urbans.choice();
			if (&u1 != &u2)
			{
				v.chain.push_back({ int16(Command::WAIT), int32(0) });
				for (auto& r : u1.getRoutesToUrban(u2.id))
				{
					Log(r->destinationUrbanID);
					v.chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });
				}
				for (auto& r : u2.getRoutesToUrban(u1.id))
				{
					v.chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });
				}
				v.chain.push_back({ int16(Command::JUMP), int32(0) });
			}
			auto r = v.getNowUrban().getRoutes().choice();
			/*
			v.chain = {
				//{ int16(Command::WAIT), int32(0) },
				{ int16(Command::MOVE), r->destinationUrbanID },
				{ int16(Command::BUY), iData.choice().id },
				{ int16(Command::MOVE), r->originUrbanID },
				{ int16(Command::SELL), 10 },
				{ int16(Command::JUMP), int32(0) },
			};
			v.chain = {
				{ int16(Command::WAIT), int32(0) },
				{ int16(Command::MOVE), urbans.choice().id },
				{ int16(Command::SELL), 100 },
				{ int16(Command::BUY), iData.choice().id },
				{ int16(Command::MOVE), urbans.choice().id },
				{ int16(Command::SELL), 100 },
				{ int16(Command::BUY), iData.choice().id },
				{ int16(Command::MOVE), urbans.choice().id },
				{ int16(Command::SELL), 100 },
				{ int16(Command::BUY), iData.choice().id },
				{ int16(Command::MOVE), urbans.choice().id },
				{ int16(Command::SELL), 100 },
				{ int16(Command::BUY), iData.choice().id },
				{ int16(Command::MOVE), urbans.choice().id },
				{ int16(Command::SELL), 100 },
				{ int16(Command::BUY), iData.choice().id },
				{ int16(Command::JUMP), int32(0) }
			};*/
		}
		double actionTime = timeSpeed;

		for (;;)
		{
			if (actionTime == 0) break;
			else if (v.inRoute())
			{
				if (actionTime >= v.getRoute().totalLength - v.routeProgress)
				{
					actionTime -= v.getRoute().totalLength - v.routeProgress;
					v.nowUrbanID = v.getRoute().destinationUrbanID;
					v.routeProgress = 0.0;
					v.routeID = -1;
				}
				else
				{
					v.routeProgress += actionTime;
					actionTime = 0;
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
				}
			}
			else
			{
				//�X�N���v�g�̎��s
				if (v.progress >= int(v.chain.size())) break;
				for (;;)
				{
					if (v.inRoute() || v.sleepTimer > 0) break;
					switch (Command(v.chain[v.progress].first))
					{
					case Command::MOVE:	//�s�s�ֈړ�
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
					}
					break;

					case Command::JUMP:	//�A�h���X�W�����v����
					{
						v.progress = v.chain[v.progress].second;
					}
					break;

					case Command::WAIT: //�E�F�C�g����
					{
						v.sleepTimer += 1;
						++v.progress;
					}
					break;

					case Command::BUY: //�w������
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

					case Command::SELL:	//�̔�����
					{
						if (v.stock.num > 0)
						{
							v.getNowUrban().baskets[v.stock.itemType].addRing(v.chain[v.progress].second, v.stock.num, this);
							v.stock.num = 0;
						}
						++v.progress;
					}
					break;

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