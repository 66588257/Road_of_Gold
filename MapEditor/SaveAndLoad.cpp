#include"Node.h"
#include"Planet.h"
#include"Pi.h"
#include"BiomeData.h"
#include"EnergyData.h"
#include"Urban.h"
#include"GlobalVariables.h"

Urban::Urban(const JSONValue _json)
	: name(_json[L"Name"].getString())
	, joinedNodeID(_json[L"JoinedNodeID"].getOr<int>(-1))
	, productivity(_json[L"Productivity"].getOr<double>(1.0))
	, numCitizens(_json[L"NumCitizens"].getOr<int>(1))
{

	//資源配置
	energies.resize(energyData.size());
	for (auto i : _json[L"Energies"].objectView())
	{
		for (int j = 0; j < int(energyData.size()); j++)
		{
			auto& data = energyData.at(j);
			if (data.name == i.name)
			{
				energies.at(j) = i.value.getOr<int>(0);
				break;
			}
		}
	} 
}

bool loadMapData(const FilePath& _path)
{
	if (!FileSystem::IsDirectory(_path)) return false;

	urbans.clear();

	//バイオームデータのロード
	if (FileSystem::Exists(_path + L"BiomeData.bin"))
	{
		Array<Node*> list;
		BinaryReader reader(_path + L"BiomeData.bin");
		for (auto& n : nodes)
		{
			reader.read(n.biomeType);
			if (n.biomeType >= int(biomeData.size())) return false;
			list.emplace_back(&n);
		}
		planet.updateImage(list);
	}
	else return false;

	//Urbansデータのロード
	if (FileSystem::Exists(_path + L"Urbans.json"))
	{
		JSONReader reader(_path + L"Urbans.json");
		for (auto json : reader[L"Urbans"].arrayView())
			urbans.emplace_back(json);
	}

	textBox.setText(FileSystem::BaseName(_path));

	return true;
}
bool saveMapData(const FilePath& _path)
{
	//バイオームデータのセーブ
	{
		BinaryWriter writer(_path + L"BiomeData.bin");
		for (auto& n : nodes) writer.write(n.biomeType);
	}

	//Urbansデータのセーブ
	if (!urbans.isEmpty())
	{
		String text(L"{\r\t\"Urbans\": [");
		for (auto i : step(int(urbans.size())))
		{
			auto& u = urbans[i];
			if (i == 0) text += L"\r\t\t{";
			else text += L",\r\t\t{";

			//都市座標の保存
			text += Format(L"\r\t\t\t\"JoinedNodeID\": ", u.joinedNodeID);

			//都市名の保存
			text += Format(L",\r\t\t\t\"Name\": \"", u.name, L"\"");

			//人口の保存
			text += Format(L",\r\t\t\t\"NumCitizens\": ", u.numCitizens);

			//生産性の保存
			text += Format(L",\r\t\t\t\"Productivity\": ", u.productivity);

			//Resourcesデータの保存
			text += L",\r\t\t\t\"Energies\": {";
			bool isFirst = true;
			for (auto j : step(energyData.size()))
			{
				if (u.energies[j] > 0)
				{
					if (isFirst) { text += L"\r"; isFirst = false; }
					else text += L",\r";
					text += Format(L"\t\t\t\t\"", energyData[j].name, L"\": ", u.energies[j]);
				}
			}
			text += L"\r\t\t\t}";

			text += L"\r\t\t}";
		}
		text += L"\r\t]\r}";
		TextWriter writer(_path + L"Urbans.json");
		writer.write(text);
		writer.close();
	}
	return true;
}