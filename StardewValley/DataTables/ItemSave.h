#pragma once
#include "DataTable.h"
class ItemSave : public DataTable
{
public:
	// static DataObject Undefined;

protected:
	std::unordered_map<int, std::list<ItemData*>*> itemBoxTable;
	// std::list<ItemData*> itemTable;
	// std::unordered_map < std::string, std::tuple<ObjectType, int>> nameTable;
	// std::vector<int> countTable;

public:
	ItemSave(DataTable::Types type);
	~ItemSave() override;

	const DataObject& Get(ObjectType type, int id);
	const DataObject& Get(const std::string& name);
	const int Count(ObjectType type);

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};

