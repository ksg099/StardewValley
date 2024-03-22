#include "pch.h"
#include "FloorTable.h"

FloorTable::FloorTable(DataTable::Types type) : DataTable(type)
{
}

FloorTable::~FloorTable()
{
	Release();
}

const DataFloor& FloorTable::Get(FloorType type, int id)
{
	auto find = table.find(std::make_tuple(type, id));
	if (find == table.end())
	{
		std::cout << "Floor Data에 해당 Type과 ID가 없습니다" << std::endl;
		return Undefined;
	}
	return find->second;
}

const DataFloor& FloorTable::Get(const std::string& name)
{
	auto find = nameTable.find(name);
	if (find == nameTable.end())
	{
		std::cout << "Floor Data에 해당 이름이 없습니다" << std::endl;
		return Undefined;
	}

	return Get(std::get<0>(find->second), std::get<1>(find->second));
}

const int FloorTable::Count(FloorType type)
{
	int typeNum = (int)type;

	if (typeNum < 0 || typeNum >= countTable.size())
		return 0;

	return countTable[(int)type];
}

bool FloorTable::Load(rapidjson::Document& doc)
{
	Release();

	auto infoArr = doc["Tile Info"]["Floor"]["Type Info"].GetArray();
	int floorTypeCount = infoArr.Size();
	for (int i = 0; i < floorTypeCount; ++i)
	{
		int floorIdCount = infoArr[i]["Sheet Info"].Size();
		countTable.push_back(floorIdCount);
		for (int j = 0; j < floorIdCount; ++j)
		{
			std::tuple<FloorType, int> key = std::make_tuple((FloorType)i, j);

			if (table.find(key) != table.end())
			{
				std::cout << "object table 중복 아이디" << std::endl;
				return false;
			}

			table[key] = DataFloor();
			table[key].floorType = (FloorType)i;
			table[key].floorId = j;
			table[key].textureId = infoArr[i]["Sheet Info"][j]["Resource"].GetString();
			table[key].sheetId = { infoArr[i]["Sheet Info"][j]["Sheet ID X"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID Y"].GetInt() };
			table[key].sheetSize = { infoArr[i]["Sheet Info"][j]["Sheet ID W"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID H"].GetInt() };

			nameTable[infoArr[i]["Sheet Info"][j]["Floor Name"].GetString()] = key;
		}
	}

	return true;
}

void FloorTable::Release()
{
	table.clear();
	nameTable.clear();
	countTable.clear();
}
