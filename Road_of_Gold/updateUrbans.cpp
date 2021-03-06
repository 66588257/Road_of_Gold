#include"Planet.h"
#include"Urban.h"
#include"UI.h"
#include"CitizenData.h"
#include"ItemData.h"
#include"Sound.h"
#include<thread>

void	updateUrban(Urban& u)
{
	if (u.sandglass.update())
	{

		//s¯OÌXV
		for (auto& c : u.citizens)
		{
			c.incomeLog.push_front(c.wallet().income);
			c.incomeLog.pop_back();
			c.wallet().income = 0;	//ûüðZbg
		}

		//s¯Ì]E
		for (auto& c : u.citizens)
		{
			if (u.jobEfficiency[c.citizenType] == 0 || RandomBool(0.003))	//]E
			{
				int sum = 0;
				for (auto t : u.citizens) sum += t.avgIncome();

				sum = Random(sum);
				for (auto t : u.citizens)
				{
					sum -= t.avgIncome();
					if (sum < 0)
					{
						if (c.avgIncome() < t.avgIncome())
						{
							c.incomeLog.fill(0);
							c.citizenType = t.citizenType;
							//ÚWvðJ­ÒÌûvð³ÉÝè
							c.targetRevenue = Random(int(citizenData.front().wage*u.productivity));

							//Ì¿iÌRs[
							c.wallet().price = t.wallet().price;
						}
						break;
					}
				}
				for (int i = 0; i < int(citizenData.size()); i++)
				{
					if (u.jobEfficiency[i] != 0 && !u.citizens.any([&i](const Citizen& t) { return t.citizenType == i; }))
					{
						c.citizenType = i;
						//ÚWvðJ­ÒÌûvð³ÉÝè
						c.targetRevenue = Random(int(citizenData.front().wage*u.productivity));
					}
				}
			}
		}

		//ø¦ÌXV
		for (auto i : step(int(citizenData.size())))
		{
			auto& je = u.jobEfficiency[i];
			auto& data = citizenData[i];
			auto numCitizen = int(u.citizens.count_if([&i](const Citizen& c) { return c.citizenType == i; }));

			if (data.needEnergyType == -1) je = 1;
			else if (!u.energies.any([&data](const Energy& e) { return e.energyType == data.needEnergyType; })) je = 0;
			else
			{
				for (auto j : step(int(u.energies.size())))
				{
					auto& e = u.energies[j];
					if (e.energyType == data.needEnergyType)
					{
						if (e.numEnergy == 0) je = 0;
						else if (numCitizen == 0) je = 1.0;
						else je = sqrt((double)e.numEnergy / (double)numCitizen)*u.productivity;
					}
				}
			}
		}

		//sêÌXV
		for (auto i : step(int(u.baskets.size())))
		{
			auto& b = u.baskets[i];
			for (auto& r : b.rings) r.price = Max(1, int(r.price*0.95));	//¿iáº

																			//TradeLogÌXV
			b.tradeLog.push();
			
		}

		//SellerÌXV
		for (auto& s : u.sellers)
		{
			if (s.progress < s.period) s.progress++;
			int numSell = s.casket.numItem - int(s.target*(1 - s.progress / double(s.period)));

			if (numSell > 0)
			{
				u.sellItem(s.casket.itemType, numSell, Max(1, int(1 + s.wallet().price*Random(1.00, 1.10))), s.walletID);
				s.casket.numItem -= numSell;
				u.baskets[s.casket.itemType].tradeLog.numImport.front() += numSell;
			}
		}
		u.sellers.remove_if([](const Seller& s) { return s.progress == s.period; });

		//BuyerÌXV
		for (auto& b : u.buyers)
		{
			if (b.progress < b.period) b.progress++;

			int numBuy = Min(u.numItem(b.casket.itemType), int(b.target*(b.progress / double(b.period))) - b.casket.numItem);
			if (numBuy == 0) continue;

			u.buyItem(b.casket.itemType, b.walletID, numBuy);
			b.casket.numItem += numBuy;
			u.baskets[b.casket.itemType].tradeLog.numExport.front() += numBuy;

		}
	}

	//s¯ÌXV
	for (auto& c : u.citizens)
	{
		const auto& je = u.jobEfficiency[c.citizenType];
		const auto& data = c.data();

		if (je == 0) c.jobProgress = 0;



		//if (c.wallet().sellCount == 0)
		c.jobProgress += je*planet.timeSpeed;

		c.timer += planet.timeSpeed;
		if (c.timer >= 1.0)
		{
			c.timer -= 1.0;

			//w
			for (int i = 0; i < int(itemData.size()); i++)
			{
				const double h = Random(0.1, 5.0);
				auto itemType = Random(int(itemData.size() - 1));
				if (!u.isSoldOut(itemType))
				{
					if (itemData[itemType].value / double(u.cost(itemType)) > h && u.cost(itemType) < c.wallet().money)
						u.buyItem(itemType, c.walletID);
				}
			}
			c.wallet().pull(c.wallet().money / 10);
		}

		while (c.jobProgress >= 1.0)
		{
			c.jobProgress -= 1.0;

			c.wallet().add(int(data.wage));

			//ItemÌÌ
			if (data.product.numItem > 0)
			{
				//ÚWvðB¬Å«éêÌÝ¶Y
				if (u.isSoldOut(data.product.itemType) || u.cost(data.product.itemType) > c.targetRevenue / data.product.numItem)
				{
					u.sellItem(data.product, Max(1, int(1 + c.wallet().price*Random(1.00, 1.10))), c.walletID);
					u.baskets[data.product.itemType].tradeLog.numProduction.front() += data.product.numItem;
				}
			}
		}
	}
}
void	updateUrbans()
{
	if (KeyM.down()) useMulthThread = !useMulthThread;

	if (useMulthThread)
	{
		Array<std::thread> threads;

		for (auto& u : urbans) threads.emplace_back(updateUrban, std::ref(u));
		for (auto& t : threads)  t.join();
	}
	else for (auto& u : urbans) updateUrban(u);

	if (MouseL.down())
	{
		ui.selectedUrbanID = -1;
		for (int i = 0; i < 2; ++i) {
			const auto transformer = tinyCamera.createTransformer(i);

			for (auto j : step(int(urbans.size())))
			{
				if (urbans[j].mouseOver())
				{
					ui.selectedVehicleID = -1;
					ui.selectedUrbanID = j;
					sounds.at(0).play();
				}
			}
		}
	}
}