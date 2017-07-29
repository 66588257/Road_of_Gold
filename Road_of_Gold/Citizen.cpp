#include"Urban.h"
#include"GlobalVariables.h"
#include"JSON.h"


void	Citizen::goToShopping()
{
	auto& u = urbans[joinedUrbanID];

	int target = 0;	//�ڕW
	int maxEarn = 0;
	for (int i = 0; i < (1 << iData.size()); ++i)
	{
		int cost = 0;
		int earn = 0;
		for (int j = 0; j < iData.size(); ++j)
		{
			if ((i & (1 << j)) != 0 && !u.baskets[j].rings.isEmpty())
			{
				cost += u.baskets[j].rings.front().price;
				earn += iData[j].value;
			}
		}
		if (money < cost) continue;

		for (int k = 0;; ++k)
			if ((1 << k) > money - cost) { earn += k * 10; break; }

		if (maxEarn < earn)
		{
			target = i;
			maxEarn = earn;
		}
	}
	hapiness = 0;
	for (int j = 0; j < iData.size(); ++j)
	{
		if ((target & (1 << j)) != 0 && !u.baskets[j].rings.isEmpty())
		{
			money -= u.baskets[j].rings.front().price;
			u.baskets[j].buyItem(1);
			hapiness += iData[j].value;
		}
	}
	for (int k = 0;; ++k)
		if ((1 << k) > money) { hapiness += k * 10; break; }
	money = 0;
}
Citizen::Citizen(int _id, int _citizenType, int _joinedUrbanID)
	: citizenType(_citizenType)
	, money(0)
	, timer(Random(0.0, 1.0))
	, joinedUrbanID(_joinedUrbanID)
	, id(_id)
	, price(100)
	, hapiness(0)
	, progress(0)
{
	incomeLog.resize(100);
}
int		Citizen::avgIncome() const { return int(incomeLog.sum() / double(incomeLog.size())); }
void	Citizen::update()
{
	auto& u = urbans[joinedUrbanID];

	timer += timeSpeed;
	if (timer >= 1.0)
	{
		timer -= 1.0;

		//�]�E�̔���
		if (RandomBool(0.001) || u.jobEfficiency[citizenType] == 0.0)
		{
			auto max = u.avgIncome[citizenType] * 2;
			for (auto i : step(int(cData.size())))
			{
				if (max < u.avgIncome[i])
				{
					max = u.avgIncome[i];
					citizenType = i;

					//�̔����i�̍Đݒ�
					int num = 0;
					int sum = 0;
					for (auto& c : u.citizens)
						if (c.citizenType == i) { num++; sum += c.price; }
					if (num > 0) price = sum / num;
				}
			}
		}
		auto& cJob = cData[citizenType].job;

		//�d�����B���\���ǂ�������
		int totalCost = cJob.cost - cJob.wage;
		bool flag = true;
		for (auto& p : cJob.consume)
		{
			if (u.baskets[p.itemID].getNumItem() < p.numConsume) { flag = false; break; }
			totalCost += u.baskets[p.itemID].getCost(p.numConsume);
		}

		//�d���̎��s
		progress += u.jobEfficiency[citizenType];
		if (progress >= 1.0)
		{
			progress -= 1.0;
			if (totalCost < money)
			{
				if (flag)	//�����A�K�v�ȍޗ����s��ɏo�Ă����
				{
					//�ޗ��̍w��
					for (auto& p : cJob.consume)
						u.baskets[p.itemID].buyItem(p.numConsume);

					//���i�̔̔�
					for (auto& p : cJob.product)
						u.baskets[p.itemID].addRing(1 + int(price*Random(1.05, 1.1)), p.numProduct, this);

					//��p�̎x����
					addMoney(-totalCost);
				}

				//������������
				goToShopping();
			}
			else addMoney(50);	//�J���҂Ƃ��ē���
		}
	}
}

void	Citizen::addMoney(int _amount)
{
	incomeLog.front() += _amount;
	money += _amount;
}