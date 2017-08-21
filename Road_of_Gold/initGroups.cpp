#include"Group.h"
#include"Vehicle.h"
#include"Urban.h"
#include"VehicleData.h"

void	initGroups()
{
	JSONReader json(L"�N���ݒ�.json");
	int numVehicles = json[L"NumVehicles"].getOr<int>(100);
	for (int i = 0; i < numVehicles; i++)
		vehicles.emplace_back(vehicleData.choice().id(), &urbans[Random(int(urbans.size() - 1))]);
}