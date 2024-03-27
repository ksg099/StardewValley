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
	std::unordered_map<HarvestType, DataHarvest> harvestTable;

public:
	HarvestTable(DataTable::Types type);
	~HarvestTable() override;

	const DataHarvest& Get(HarvestType type, int id);
	// const DataItem& Get(const std::string& name);
	// const int Count(HarvestType type);

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};

