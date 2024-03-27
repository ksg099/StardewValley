#include "pch.h"
#include "HarvestTable.h"

DataHarvest HarvestTable::Undefined;

HarvestTable::HarvestTable(DataTable::Types type) : DataTable(type)
{
}

HarvestTable::~HarvestTable()
{
	Release();
}

const DataHarvest& HarvestTable::Get(HarvestType type, int id)
{
	auto find = harvestTable.find(type);
	if (find == harvestTable.end())
	{
		std::cout << "Harvest Data에 해당 이름이 없습니다" << std::endl;
		return Undefined;
	}

	return find->second;
}

bool HarvestTable::Load(rapidjson::Document& doc)
{
	Release();

	auto infoArr = doc["Harvest Info"].GetArray();
	int harvestCount = infoArr.Size();
	for (int i = 0; i < harvestCount; ++i)
	{
		HarvestType type = (HarvestType)infoArr[i]["Harvest ID"].GetInt();

		if (harvestTable.find(type) != harvestTable.end())
		{
			std::cout << "harvest table 중복 아이디" << std::endl;
			return false;
		}
		
		harvestTable[type] = DataHarvest();
		harvestTable[type].harvestName = infoArr[i]["Harvest Name"].GetString();
		harvestTable[type].growingDay = infoArr[i]["Growing Day"].GetInt();
		harvestTable[type].growingLevel = infoArr[i]["Growing Level"].GetInt();
	}

	return true;
}

void HarvestTable::Release()
{
	harvestTable.clear();
}
