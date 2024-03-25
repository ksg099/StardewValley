#pragma once
#include "DataTable.h"
class TileMapSave : public DataTable
{
public:
	// static DataObject Undefined;

protected:
	std::unordered_map<std::string, std::vector<TileData*>*> tileMaps;
	std::unordered_map<std::string, sf::Vector2i> tileMapsSize;
	// std::list<ItemData*> itemTable;
	// std::unordered_map < std::string, std::tuple<ObjectType, int>> nameTable;
	// std::vector<int> countTable;

public:
	TileMapSave(DataTable::Types type);
	~TileMapSave() override;

	std::vector<TileData*>* Get(const std::string& name);
	sf::Vector2i* GetTileMapSize(const std::string& name);
	//const std::pair<int, std::vector<TileData*>*>& AddITemBox();
	void Save(rapidjson::Document& doc);
	// const DataObject& Get(const std::string& name);
	// const int Count(ObjectType type);

	bool Load(rapidjson::Document& doc) override;
	void Release() override;
};