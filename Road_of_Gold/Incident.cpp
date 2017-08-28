#include"Incident.h"
#include"Scuttle.h"
#include"EnergyData.h"
#include"Urban.h"

double	Incident::time() const
{
	double result = 0;
	result += json[L"Year"].getOr<int>(0) * 360;
	result += (json[L"Month"].getOr<int>(1) - 1) * 30;	//1月から12月まで
	result += (json[L"Day"].getOr<int>(1) - 1);	//1日から30日まで
	return result;
}
void	Incident::addScuttle()
{
	String	s;
	s += json[L"Description"].getString() + L"\n";
	s += L"\n効果\n";
	for (auto a : json[L"Action"].arrayView())
	{
		s += L"\t";
		auto t = a[L"Type"].getString();
		if (t == L"PullEnergy")
		{
			s += a[L"Urban"].getString();
			s += L"が";
			s += Format(a[L"Num"].get<int>());
			s += a[L"EnergyName"].getString();
			s += L"消失\n";
		}
		if (t == L"AddEnergy")
		{
			s += a[L"Urban"].getString();
			s += L"に";
			s += Format(a[L"Num"].get<int>());
			s += a[L"EnergyName"].getString();
			s += L"追加\n";
		}
		if (t == L"CorrectProductivity")
		{
			s += a[L"Urban"].getString();
			s += L"の労働効率にx";
			s += Format(a[L"Value"].get<double>());
			s += L"の補正\n";
		}

	}
	scuttles.emplace_back(json[L"Name"].getString(), s, json[L"Button"].getString());
	if (FileSystem::Exists(json[L"Sound"].getString()))
	{
		scuttles.back().audio = Audio(json[L"Sound"].getString());
		scuttles.back().audio.play();
	}
}
void	Incident::action()
{
	addScuttle();
	for (auto a : json[L"Action"].arrayView())
	{
		auto t = a[L"Type"].getString();
		if (t == L"PullEnergy")
		{
			auto* u = getUrban(a[L"Urban"].getString());
			u->pullEnergy(a[L"EnergyName"].getString(), a[L"Num"].get<int>());
		}
		if (t == L"AddEnergy")
		{
			auto* u = getUrban(a[L"Urban"].getString());
			u->addEnergy(a[L"EnergyName"].getString(), a[L"Num"].get<int>());
		}
		if (t == L"CorrectProductivity")
		{
			auto* u = getUrban(a[L"Urban"].getString());
			u->productivity *= a[L"Value"].get<double>();
		}
	}
}