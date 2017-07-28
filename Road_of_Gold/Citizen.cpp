#include"Urban.h"
#include"GlobalVariables.h"
#include"JSON.h"


Citizen::Citizen(int _id, int _citizenType, int _joinedUrbanID)
	: citizenType(_citizenType)
	, money(00)
	, timer(Random(0.0, 1.0))
	, joinedUrbanID(_joinedUrbanID)
	, id(_id)
	, price(100)
	, hapiness(0)
	, progress(0)
	, ths(0)
	, bhs(0)
	, tmr(Random(99) + 100)
{}
void	Citizen::update()
{
	auto& u = urbans[joinedUrbanID];

	timer += timeSpeed;
	if (timer >= 1.0)
	{
		timer -= 1.0;
		
		//�]�E�̔���
		tmr--;
		if (tmr == 0)
		{
			tmr = 100;
			for (int i = 0; i < int(cData.size()); i++)
			{
				if (RandomBool(Max(0.0, double(u.avgBhs[i] - bhs) / 10000.0)))
				{
					citizenType = Random(int(cData.size() - 1));
					money = 0;
				}
			}
		}

		money -= 50;	//������̎x����

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
		if (RandomBool(u.getEfficiency(citizenType)) && totalCost < money)
		{
			if (flag)	//�����A�K�v�ȍޗ����s��ɏo�Ă����
			{
				for (auto& rID : cJob.needResouceID) u.cRT[rID]++;

				//�ޗ��̍w��
				for (auto& p : cJob.consume)
					u.baskets[p.itemID].buyItem(p.numConsume);

				//���i�̔̔�
				for (auto& p : cJob.product)
					u.baskets[p.itemID].addRing(1 + int(price*Random(1.1, 1.2)), p.numProduct, this);

				//��p�̎x����
				money -= totalCost;
			}

			//��y�Ƃ��Ă̍w��
			Array<Basket*> buyList;	//�w������
			for (;;)
			{
				Basket* best = nullptr;	//�L�͌��
				double	earn = 0.0;		//�R�X�p
				for (int i = 0; i < int(iData.size()); i++)
				{
					auto& b = u.baskets[i];
					if (!b.rings.isEmpty() && !buyList.any([&b](const Basket* t) {return t == &b; }) && (best == nullptr || iData[i].value / double(b.rings.front().price) > earn))
					{
						earn = iData[i].value / double(b.rings.front().price);
						best = &b;
					}
				}
				if (best != nullptr && best->rings.front().price <= money) {
					money -= best->rings.front().price;
					buyList.emplace_back(best);
					best->buyItem(1);
				}
				else break;
			}
			hapiness = 0;
			for (auto& b : buyList)
			{
				hapiness += iData[b->itemType].value;
				ths += iData[b->itemType].value;
			}
		}
		else money += 60;	//�J���҂Ƃ��ē���
	}
}
