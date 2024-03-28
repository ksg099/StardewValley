#pragma once
#include "Singleton.h"
#include "DataTable.h"

class DataTableMgr : public Singleton<DataTableMgr>
{
	friend Singleton<DataTableMgr>;
private:
	DataTableMgr();
	~DataTableMgr();

	std::unordered_map<DataTable::Types, DataTable*> tables;
	std::vector<std::string> tileMapData;
	std::vector<std::string> gameSaveData;

	std::string gameSaveSelect = "data/default_save.json";
	bool isSaveSelect = false;
	std::string tileMapSelect = "";
	bool isTileMapSelect = false;

	std::string recentSaveFilePath = "";

public:
	void Init();
	void Release();

	template<typename T>
	T* Get(DataTable::Types type);

	const std::vector<std::string>& GetTileMapData() { return tileMapData; }
	const std::vector<std::string>& GetGameSaveData() { return gameSaveData; }

	void SetTileMapSelect(const std::string& name);
	const std::string& GetTileMapSelect() { return tileMapSelect; }
	void SetGameSaveSelect(const std::string& name);
	const std::string& GetGameSaveSelect() { return gameSaveSelect; }
	void SetRecentFileSelect() { SetGameSaveSelect(recentSaveFilePath); }
	void SaveRecentFilePath();
	void SetSelectReset();

	bool LoadSaveData();
};

template<typename T>
inline T* DataTableMgr::Get(DataTable::Types type)
{
	auto find = tables.find(type);
	if (find == tables.end())
	{
		return nullptr;
	}
	return dynamic_cast<T*>(find->second);
}

#define DT_MGR (DataTableMgr::Instance())
#define GROUND_TABLE (DT_MGR.Get<GroundTable>(DataTable::Types::GROUND))
#define FLOOR_TABLE (DT_MGR.Get<FloorTable>(DataTable::Types::FLOOR))
#define OBJECT_TABLE (DT_MGR.Get<ObjectTable>(DataTable::Types::OBJECT))
#define ITEM_TABLE (DT_MGR.Get<ItemTable>(DataTable::Types::ITEM))
#define HARVEST_TABLE (DT_MGR.Get<HarvestTable>(DataTable::Types::HARVEST))
#define TILEMAP_SAVE (DT_MGR.Get<TileMapSave>(DataTable::Types::TILEMAP_SAVE_DATA))
#define ITEM_SAVE (DT_MGR.Get<ItemSave>(DataTable::Types::ITEM_SAVE_DATA))