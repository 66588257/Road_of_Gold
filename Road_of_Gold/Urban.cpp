#include"Urban.h"
#include"Node.h"
#include"JSON.h"
#include"Route.h"
#include"GlobalVariables.h"

Array<Urban> urbans;
Urban* selectedUrban;

Circle	Urban::getShape() const { return Circle(nodes[joinedNodeID].pos.mPos, 0.010*(1 + numCitizens / 500)); }
void	Urban::draw() const
{
	const Circle circle = getShape();
	const Color color = selectedUrban == this ? Palette::Yellow : (circle.mouseOver() ? Palette::Orange : Palette::Red);
	circle.draw(color).drawFrame(0.005, Palette::Black);
}
String	Urban::getTimeAsString() const { return  Format(int(timer * 24)).lpad(2, '0') + L":" + Format(int(timer * 24 * 60) % 60).lpad(2, '0'); }
void	Urban::update()
{
	timer += timeSpeed;
	if (timer >= 1.0)
	{
		timer -= 1.0;
		day++;

		//Efficiency�̍X�V
		{
			Array<int>	usedResource(rData.size());
			for (auto c : citizens)
				for (auto rID : cData[c.citizenType].job.needResourceID) usedResource[rID]++;

			for (auto i : step(int(cData.size())))
			{
				double efficiency = 1.0;
				for (auto j : cData[i].job.needResourceID)
				{
					if (resource[j] == 0) efficiency = 0.0;
					else if (resource[j] < usedResource[j]) efficiency *= (double(resource[j]) / double(usedResource[j]));
				}
				jobEfficiency[i] = efficiency;
			}
		}

		//AvgIncome�̏C��
		for (auto i : step(int(cData.size())))
		{
			int num = 0;
			int sum = 0;
			for (auto& c : citizens)
			{
				if (c.citizenType == i)
				{
					num++;
					sum += c.avgIncome();
				}
			}
			if (num > 0) avgIncome[i] = sum / num;
		}
		for (auto& c : citizens)
		{
			c.incomeLog.push_front(0);
			c.incomeLog.pop_back();
		}

		//�s�ꑀ��
		for (auto& b : baskets)
		{
			//���i�ቺ
			for (auto& r : b.rings) r.price = Max(1, int(r.price*0.95));

			//�`���[�g�̍X�V
			b.chart.push_front(b.tradeLog.isEmpty() ? b.chart.front() : int(b.tradeLog.sum() / double(b.tradeLog.size())));
			b.tradeLog.clear();
			b.chart.pop_back();
		}
		for (auto i : step(int(cData.size())))
		{
			int sum = 0;
			int num = 0;
			for (auto& c : citizens)
			{
				if (c.citizenType == i)
				{
					num++;
					sum += c.money / 10;
					c.money -= c.money / 10;
				}
			}
			if (num > 0)
			{
				for (auto& c : citizens)
					if (c.citizenType == i)
						c.money += sum / num;
			}
		}
	}

	//�s���̍X�V
	for (auto& c : citizens)
		c.update();
}