#include "pch.h"
#include "GroundTable.h"

DataGround GroundTable::Undefined;

GroundTable::GroundTable(DataTable::Types type) : DataTable(type)
{
}

GroundTable::~GroundTable()
{
	Release();
}

const DataGround& GroundTable::Get(GroundType type, int id)
{
	auto find = table.find(std::make_tuple(type, id));
	if (find == table.end())
	{
		std::cout << "Ground Data에 해당 Type과 ID가 없습니다" << std::endl;
		return Undefined;
	}
	return find->second;
}

bool GroundTable::Load(rapidjson::Document& doc)
{
	Release();

	textureId = doc["Tile Info"]["Ground"]["Resource"].GetString();

	auto infoArr = doc["Tile Info"]["Ground"]["Type Info"].GetArray();
	int groundTypeCount = infoArr.Size();
	for (int i = 0; i < groundTypeCount; ++i)
	{
		int groundIdCount = infoArr[i]["Sheet Info"].GetArray().Size();
		countTable.push_back(groundIdCount);
		for (int j = 0; j < groundIdCount; ++j)
		{
			std::tuple<GroundType, int> key = std::make_tuple((GroundType)i, j);

			if (table.find(key) != table.end())
			{
				std::cout << "ground table 중복 아이디" << std::endl;
				return false;
			}

			table[key] = DataGround();
			table[key].groundType = (GroundType)i;
			table[key].groundId = j;
			table[key].sheetId = { infoArr[i]["Sheet Info"][j]["Sheet ID X"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID Y"].GetInt() };
			table[key].sheetSize = { infoArr[i]["Sheet Info"][j]["Sheet ID W"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID H"].GetInt() };
		}
	}

	return true;
}

void GroundTable::Release()
{
	table.clear();
	countTable.clear();
}

const int GroundTable::Count(GroundType type)
{
		int typeNum = (int)type;

		if (typeNum < 0 || typeNum >= countTable.size())
			return 0;

		return countTable[(int)type];
}
