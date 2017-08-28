#include"Planet.h"
#include"Node.h"
#include"Urban.h"
#include"BiomeData.h"
#include"Incident.h"

void	loadMap()
{
	//Map�f�[�^�̑I��
	auto items = FileSystem::DirectoryContents(L"Map/");
	for (auto& item : items)
	{
		if (FileSystem::IsDirectory(item) && FileSystem::Exists(item + L"BiomeData.bin"))
		{
			//�o�C�I�[���f�[�^�̃��[�h
			{
				BinaryReader reader(item + L"BiomeData.bin");
				for (auto& n : nodes)
				{
					reader.read(n.biomeType);

					//Node�ɐF�̓K�p
					n.color = n.data().color.lerp(RandomColor(), 0.05);
				}
			}

			//Urbans�f�[�^�̃��[�h
			if (FileSystem::Exists(item + L"Urbans.json"))
			{
				JSONReader reader(item + L"Urbans.json");
				for (auto json : reader[L"Urbans"].arrayView())
					urbans.emplace_back(json);

				for (auto& u : urbans)
					for (auto& b : u.baskets)
						b.joinedUrban = &u;
			}

			//Incidents�f�[�^�̃��[�h
			if (FileSystem::Exists(item + L"Incidents.json"))
			{
				JSONReader reader(item + L"Incidents.json");
				for (auto json : reader[L"Incidents"].arrayView())
					incidents.emplace_back(json);
			}

			return;
		}
	}
	LOG_ERROR(L"�L����Map�f�[�^��������܂���ł���");
	System::Exit();
}