#pragma once
#include "DataTable.h"

struct DataHarvest
{
	std::string harvestName;

	int growingDay;
	int growingLevel;
};

class HarvestTable : public DataTable
{
public:
	static DataHarvest Undefined;

private:
	std::unordered_map<int, int> seedToObjTable; // seed ID to object ID
	std::unordered_map<int, int> objToHarvestTable; // object ID to harvest ID
	std::unordered_map<int, DataHarvest> harvestGrowingTable;

public:
	HarvestTable(DataTable::Types type);
	~HarvestTable() override;

	const int& GetObjectID(int seedId);
	const int& GetItemID(int objectId);
	const DataHarvest& Get(int objectId);
	// const DataItem& Get(const std::string& name);
	// const int Count(HarvestType type);

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};

