#include"Planet.h"
#include"Group.h"
#include"Vehicle.h"
#include"VehicleData.h"
#include"Urban.h"
#include"Route.h"
#include"Node.h"
#include"ItemData.h"

void	drawGroups()
{
	for (int i = 0; i < 2; ++i)
	{
		const auto transformer = tinyCamera.createTransformer(i);

		for (const auto& v : vehicles)
		{
			auto color = v.cargo.isEmpty() ? Color(0, 0) : v.cargo.data().color;
			Circle(v.pos(), 0.005).drawFrame(0.001, 0.000, Palette::Black);
			if (!v.cargo.isEmpty())
			{
				auto& data = v.cargo.data();
				data.icon.resize(0.01).drawAt(v.pos());
			}
		}
	}
}