#include"BiomeData.h"
#include"EnergyData.h"
#include"VehicleData.h"
Array<EnergyData>	energyData;
Array<BiomeData>	biomeData;
Array<VehicleData>	vehicleData;

int		BiomeData::id() const { return int(this - &biomeData.front()); }
int		EnergyData::id() const { return int(this - &energyData.front()); }
int		VehicleData::id() const { return int(this - &vehicleData.front()); }
void	loadBiomeData()
{
	JSONReader biomeDataJson(L"Assets/BiomeData.json");
	for (auto json : biomeDataJson[L"BiomeData"].arrayView())
		biomeData.emplace_back(json);
	Log(L"BiomeData�̓ǂݍ��݊��� size = ", biomeData.size());
}
void	loadEnergyData()
{
	JSONReader energyDataJson(L"Assets/EnergyData.json");
	for (auto json : energyDataJson[L"EnergyData"].arrayView())
		energyData.emplace_back(json);
	Log(L"EnergyData�̓ǂݍ��݊��� size = ", energyData.size());
}
void	loadVehicleData()
{
	JSONReader vehicleDataJson(L"Assets/VehicleData.json");
	for (auto json : vehicleDataJson[L"VehicleData"].arrayView())
		vehicleData.emplace_back(json);
	Log(L"VehicleData�̓ǂݍ��݊��� size = ", vehicleData.size());
}