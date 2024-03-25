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

	rapidjson::Document dataDoc;
	if (Utils::LoadFromFile("data/DataTable.json", dataDoc))
	{
		GROUND_TABLE->Load(dataDoc);
		FLOOR_TABLE->Load(dataDoc);
		OBJECT_TABLE->Load(dataDoc);
		ITEM_TABLE->Load(dataDoc);
	}

	rapidjson::Document SaveDoc;
	if (Utils::LoadFromFile("data/example_save.json", SaveDoc))
	{
		TILEMAP_SAVE->Load(SaveDoc);
		ITEM_SAVE->Load(SaveDoc);
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