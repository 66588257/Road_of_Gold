#include"Urban.h"
#include"Node.h"
#include"JSON.h"
#include"Route.h"
#include"GlobalVariables.h"

Array<Urban> urbans;
Urban* selectedUrban;

Urban::Urban()
	: id(int(urbans.size()))
	, name(UrbanName.choice())
	, joinedNodeID(-1)
	, timer(0.5 + getPos().mPos.x / TwoPi)
	, day(0)
{
	resource.resize(rData.size());
	avgBhs.resize(cData.size());

	const Array<int> numCitizen = {
		100,//�J����
		10,	//�؂���
		10,	//���|�E�l
		20,	//��l
		10,	//�d���ĉ�
		5,	//���t
		5,	//���t
		5,
		5,
	};

	for (int i = 0; i < int(iData.size()); i++) baskets.emplace_back(i, id);
	for (int i = 0; i < int(Min(cData.size(), numCitizen.size())); i++)
		for (int j = 0; j < numCitizen[i]; j++) citizens.emplace_back(int(citizens.size()), i, id);
}
String	Urban::getTimeAsString() const { return  Format(int(timer * 24)).lpad(2, '0') + L":" + Format(int(timer * 24 * 60) % 60).lpad(2, '0'); }
void	Urban::update()
{
	timer += timeSpeed;
	if (timer >= 1.0)
	{
		timer -= 1.0;
		day++;
		//�ꂩ�����o��
		if (day > 100)
		{
			day = 0;

			//BHS�̍X�V
			avgBhs.fill(0);
			for (auto& c : citizens) { c.bhs = c.ths; c.ths = 0; avgBhs[c.citizenType] += c.bhs; }
			for (int i = 0; i<int(cData.size()); i++)
				avgBhs[i] = int(avgBhs[i] / citizens.count_if([&i](const Citizen& c) {return c.citizenType == i; }));
		}
		for (auto& b : baskets)
		{
			//���i�ቺ
			for (auto& r : b.rings) r.price = Max(1, int(r.price*0.95));

			//�`���[�g�̍X�V
			b.chart.push_front(b.tradeLog.isEmpty() ? b.chart.front() : int(b.tradeLog.sum() / double(b.tradeLog.size())));
			b.tradeLog.clear();
			b.chart.pop_back();
		}
	}

	//�s���̍X�V
	for (auto& c : citizens)
		c.update();
}