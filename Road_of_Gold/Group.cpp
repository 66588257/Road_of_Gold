#include"Group.h"
#include"Node.h"
#include"Urban.h"
#include"Route.h"
#include"GlobalVariables.h"
#include"JSON.h"

Array<Vehicle> vehicles;

Vehicle::Vehicle(int _nowUrbanID)
	: nowUrbanID(_nowUrbanID)
	, routeID(-1)
	, routeProgress(0.0)
	, progress(0)
	, sleepTimer(0)
	, stock()
	, joinedGroupID(-1)
{}
Vec2	Vehicle::getMPos() const
{
	if (routeID != -1)
	{
		auto& r = routes[routeID];
		double length = routeProgress;
		for (int i = 0; i < r.pathIDs.size(); i++)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->cost) length -= p->cost;
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->cost);
				return pos;
			}
		}
	}
	return urbans[nowUrbanID].getPos().mPos;
}
void	Vehicle::draw() const
{
	const Circle shape(0.01);
	if (routeID != -1)
	{
		auto& r = routes[routeID];
		double length = routeProgress;
		for (int i = 0; i < r.pathIDs.size(); i++)
		{
			auto& p = paths[r.pathIDs[i]];
			const auto line = p->getLine();

			if (length > p->cost)
			{
				line.draw(0.005, Color(groups[joinedGroupID].color, 64));
				length -= p->cost;
			}
			else
			{
				const auto pos = line.begin.lerp(line.end, length / p->cost);
				Line(line.begin, pos).draw(0.005, Color(groups[joinedGroupID].color, 64));
				shape.movedBy(pos).draw(groups[joinedGroupID].color).drawFrame(0.005, Palette::Black);
				break;
			}
		}
	}
	else
	{
		Triangle(-0.02, -0.03, 0.02, -0.03, 0.0, 0.0).movedBy(getMPos()).draw(groups[joinedGroupID].color);
	}
}

Array<Group> groups;
Group::Group()
	: id(int(groups.size()))
	, name(Format(L"��", id, L"����������"))
	, money(0)
	, color(RandomColor())
{}
void	Vehicle::update()
{
	auto& g = groups[joinedGroupID];
	if (chain.isEmpty())
	{
		auto& u1 = urbans[nowUrbanID];
		auto& u2 = urbans.choice();
		if (&u1 != &u2)
		{
			for (auto& r : u1.getRoutesToUrban(u2.id))
				chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });

			chain.push_back({ int16(Command::SELL), int32(1000) });
			chain.push_back({ int16(Command::BUY), iData.choice().id });
			chain.push_back({ int16(Command::WAIT), int32(0) });

			for (auto& r : u2.getRoutesToUrban(u1.id))
				chain.push_back({ int16(Command::MOVE), r->destinationUrbanID });

			chain.push_back({ int16(Command::SELL), int32(1000) });
			chain.push_back({ int16(Command::BUY), iData.choice().id });
			chain.push_back({ int16(Command::WAIT), int32(0) });

			chain.push_back({ int16(Command::JUMP), int32(0) });
		}
	}
	double actionTime = timeSpeed;

	for (;;)
	{
		if (routeID != -1)
		{
			if (actionTime >= routes[routeID].totalLength - routeProgress)
			{
				actionTime -= routes[routeID].totalLength - routeProgress;
				nowUrbanID = routes[routeID].destinationUrbanID;
				routeProgress = 0.0;
				routeID = -1;
				++progress;
			}
			else
			{
				routeProgress += actionTime;
				break;
			}
		}
		else if (sleepTimer > 0)
		{
			if (actionTime > sleepTimer)
			{
				actionTime -= sleepTimer;
				sleepTimer = 0;
				++progress;
			}
			else
			{
				sleepTimer -= actionTime;
				break;
			}
		}
		else if (progress < int(chain.size()) && progress >= 0)
		{
			//�X�N���v�g�̎��s
			const int command = chain[progress].first;
			const int data = chain[progress].second;
			switch (Command(command))
			{
			case Command::MOVE:	//�s�s�ֈړ�
				for (auto& rID : urbans[nowUrbanID].routeIDs)
				{
					if (routes[rID].destinationUrbanID == data)
					{
						routeProgress = 0.0;
						routeID = routes[rID].id;
						break;
					}
				}
				break;

			case Command::JUMP:	//�A�h���X�W�����v����
				progress = data;
				break;

			case Command::WAIT: //�E�F�C�g����
				sleepTimer = 1;
				break;

			case Command::BUY: //�w������
				if (stock.num == 0)
				{
					Basket& b = urbans[nowUrbanID].baskets[data];
					const int numBuy = Min(10, b.getNumItem());
					if (numBuy > 0)
					{
						g.money -= b.getCost(numBuy);
						b.buyItem(numBuy);
						stock.num = numBuy;
						stock.itemType = data;
					}
					sleepTimer = 1 / 24.0;	//1hour
				}
				else ++progress;
				break;

			case Command::SELL:	//�̔�����
				if (stock.num > 0)
				{
					Basket& b = urbans[nowUrbanID].baskets[stock.itemType];
					b.addRing(data, stock.num, &g);
					stock.num = 0;
					sleepTimer = 1 / 24.0;	//1hour
				}
				else ++progress;
				break;

			default:	//���݂��Ȃ�����
				++progress;
				break;
			}
		}
		else break;	//���݂��Ȃ��n�_�̎Q��
	}
}

void Group::update()
{
}