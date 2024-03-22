#pragma once
#include "DataTable.h"

struct DataObject
{
	ObjectType objectType;
	int objectId;

	std::string textureId;
	sf::Vector2i sheetId;
	sf::Vector2i sheetSize;
};

class ObjectTable : public DataTable
{
public:
	static DataObject Undefined;

protected:
	std::string textureId;
	std::unordered_map<std::tuple<ObjectType, int>, DataObject, hash_tuple> table;
	std::unordered_map < std::string, std::tuple<ObjectType, int>> nameTable;
	std::vector<int> countTable;

public:
	ObjectTable(DataTable::Types type);
	~ObjectTable() override;

	const DataObject& Get(ObjectType type, int id);
	const DataObject& Get(const std::string& name);
	const int Count(ObjectType type);

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};

