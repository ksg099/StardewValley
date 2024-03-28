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

const int& HarvestTable::GetObjectID(int seedId)
{
	auto find = seedToObjTable.find(seedId);
	if (find == seedToObjTable.end())
	{
		std::cout << "Harvest Data에 해당 seed ID가 없습니다" << std::endl;
		return -1;
	}

	return find->second;
}

const int& HarvestTable::GetItemID(int objectId)
{
	auto find = seedToObjTable.find(objectId);
	if (find == seedToObjTable.end())
	{
		std::cout << "Harvest Data에 해당 object ID가 없습니다" << std::endl;
		return -1;
	}

	return find->second;
}

const DataHarvest& HarvestTable::Get(int objectId)
{
	auto find = harvestGrowingTable.find(objectId);
	if (find == harvestGrowingTable.end())
	{
		std::cout << "Harvest Data에 해당 이름이 없습니다" << std::endl;
		return Undefined;
	}

	return find->second;
}

bool HarvestTable::Load(rapidjson::Document& doc)
{
	Release();

	auto IdLinkInfoArr = doc["Harvest Info"]["Harvest ID Link Info"].GetArray();
	int harvestCount = IdLinkInfoArr.Size();
	for (int i = 0; i < harvestCount; ++i)
	{
		int seedId = IdLinkInfoArr[i]["Seed ID"].GetInt();
		int objectId = IdLinkInfoArr[i]["Object ID"].GetInt();
		
		if (seedToObjTable.find(seedId) != seedToObjTable.end() || objToHarvestTable.find(objectId) != objToHarvestTable.end())
		{
			std::cout << "harvest table에 중복된 ID가 있습니다." << std::endl;
		}
		seedToObjTable[seedId] = objectId;
		objToHarvestTable[objectId] = seedId;
	}

	auto levelInfoArr = doc["Harvest Info"]["Harvest Level Info"].GetArray();
	int harvestLevelCount = levelInfoArr.Size();
	for (int i = 0; i < harvestLevelCount; ++i)
	{
		// HarvestType type = (HarvestType)levelInfoArr[i]["Harvest ID"].GetInt();

		if (harvestGrowingTable.find(i) != harvestGrowingTable.end())
		{
			std::cout << "harvest table 중복 아이디" << std::endl;
			return false;
		}
		
		harvestGrowingTable[i] = DataHarvest();
		harvestGrowingTable[i].harvestName = levelInfoArr[i]["Harvest Name"].GetString();
		harvestGrowingTable[i].growingDay = levelInfoArr[i]["Growing Day"].GetInt();
		harvestGrowingTable[i].growingLevel = levelInfoArr[i]["Growing Level"].GetInt();
	}

	return true;
}

void HarvestTable::Release()
{
	harvestGrowingTable.clear();
}
