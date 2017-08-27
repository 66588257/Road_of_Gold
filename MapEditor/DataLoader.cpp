#include"JSON.h"

Array<BiomeData> biomeData;
Array<EnergyData> energyData;

bool loadJSONData()
{
	//�e��JSON�f�[�^�̓ǂݍ���
	JSONReader rjson(L"Assets/ResourceData.json");
	JSONReader bjson(L"Assets/BiomeData.json");
	for (auto r : rjson[L"ResourceData"].arrayView()) energyData.emplace_back(r);
	for (auto i : bjson[L"BiomeData"].arrayView()) biomeData.emplace_back(i);
	return true;
}