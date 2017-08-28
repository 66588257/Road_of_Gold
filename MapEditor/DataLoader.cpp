#include"JSON.h"

Array<BiomeData> biomeData;
Array<EnergyData> energyData;

bool loadJSONData()
{
	//�e��JSON�f�[�^�̓ǂݍ���
	JSONReader rjson(L"Assets/EnergyData.json");
	JSONReader bjson(L"Assets/BiomeData.json");
	for (auto r : rjson[L"EnergyData"].arrayView()) energyData.emplace_back(r);
	for (auto i : bjson[L"BiomeData"].arrayView()) biomeData.emplace_back(i);
	return true;
}