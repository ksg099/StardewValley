#pragma once
#include "DataTable.h"

struct DataItem
{
	ItemType itemType;
	int itemId;

	std::string name;

	std::string textureId;
	sf::Vector2i sheetId;
	sf::Vector2i sheetSize;
};

class ItemTable : public DataTable
{
public:
	static DataItem Undefined;

protected:
	std::unordered_map<std::tuple<ItemType, int>, DataItem, hash_tuple> table;
	std::unordered_map < std::string, std::tuple<ItemType, int>> nameTable;
	std::vector<int> countTable;

public:
	ItemTable(DataTable::Types type);
	~ItemTable() override;

	const DataItem& Get(ItemType type, int id);
	const DataItem& Get(const std::string& name);
	const int Count(ItemType type);

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};

