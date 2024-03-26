#include "pch.h"
#include "DataTableMgr.h"
#include "GroundTable.h"
#include "FloorTable.h"
#include "ObjectTable.h"
#include "ItemTable.h"
#include "TileMapSave.h"
#include "ItemSave.h"

DataTableMgr::DataTableMgr()
{
}

DataTableMgr::~DataTableMgr()
{
	Release();
}

void DataTableMgr::Init()
{
	Release();

	DataTable* groundTable = new GroundTable(DataTable::Types::GROUND);
	tables.insert({ DataTable::Types::GROUND, groundTable });

	DataTable* floorTable = new FloorTable(DataTable::Types::FLOOR);
	tables.insert({ DataTable::Types::FLOOR, floorTable });

	DataTable* objectTable = new ObjectTable(DataTable::Types::OBJECT);
	tables.insert({ DataTable::Types::OBJECT, objectTable });

	DataTable* itemTable = new ItemTable(DataTable::Types::ITEM);
	tables.insert({ DataTable::Types::ITEM, itemTable });

	DataTable* tileMapSave = new TileMapSave(DataTable::Types::TILEMAP_SAVE_DATA);
	tables.insert({ DataTable::Types::TILEMAP_SAVE_DATA, tileMapSave });

	DataTable* itemSaveTable = new ItemSave(DataTable::Types::ITEM_SAVE_DATA);
	tables.insert({ DataTable::Types::ITEM_SAVE_DATA, itemSaveTable });

	Utils::ListFiles("data/Tile Map", tileMapData);
	Utils::ListFiles("data/Save Data", gameSaveData);

	rapidjson::Document dataDoc;
	if (Utils::LoadFromFile("data/DataTable.json", dataDoc))
	{
		GROUND_TABLE->Load(dataDoc);
		FLOOR_TABLE->Load(dataDoc);
		OBJECT_TABLE->Load(dataDoc);
		ITEM_TABLE->Load(dataDoc);
	}
}

void DataTableMgr::Release()
{
	for (auto pair : tables)
	{
		delete pair.second;
	}
	tables.clear();
}

void DataTableMgr::SetTileMapSelect(const std::string& name)
{
	tileMapSelect = name;
	isTileMapSelect = true;
}

void DataTableMgr::SetGameSaveSelect(const std::string& name)
{
	gameSaveSelect = name;
	isSaveSelect = true;
}

bool DataTableMgr::LoadSaveData()
{
	if (!isSaveSelect)
	{
		return false;
	}

	if (!isTileMapSelect)
	{
		tileMapSelect = gameSaveSelect;
	}

	rapidjson::Document tileMapDoc;
	if (!Utils::LoadFromFile(tileMapSelect, tileMapDoc))
	{
		return false;
	}
	TILEMAP_SAVE->Load(tileMapDoc);

	rapidjson::Document SaveDoc;
	if (!Utils::LoadFromFile(gameSaveSelect, SaveDoc))
	{
		return false;
	}
	ITEM_SAVE->Load(SaveDoc);

	return true;
}