#pragma once
#include "DataTable.h"

struct DataGround
{
	GroundType groundType;
	int groundId;

	sf::Vector2i sheetId;
	sf::Vector2i sheetSize;
};

class GroundTable : public DataTable
{
public:
	static DataGround Undefined;

protected:
	std::string textureId;
	std::unordered_map<std::tuple<GroundType, int>, DataGround, hash_tuple> table;

public:
	GroundTable(DataTable::Types type);
	~GroundTable() override;

	const DataGround& Get(GroundType type, int id);
	const std::string& GetTextureId() const { return textureId; }

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};

