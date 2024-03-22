#pragma once
#include "DataTable.h"

struct DataFloor
{
	FloorType floorType;
	int floorId;

	std::string textureId;
	sf::Vector2i sheetId;
	sf::Vector2i sheetSize;
};

class FloorTable : public DataTable
{
public:
	static DataFloor Undefined;

protected:
	std::unordered_map<std::tuple<FloorType, int>, DataFloor, hash_tuple> table;
	std::unordered_map < std::string, std::tuple<FloorType, int>> nameTable;
	std::vector<int> countTable;

public:
	FloorTable(DataTable::Types type);
	~FloorTable() override;

	const DataFloor& Get(FloorType type, int id);
	const DataFloor& Get(const std::string& name);
	const int Count(FloorType type);

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};

