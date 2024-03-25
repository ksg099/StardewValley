#include "pch.h"
#include "DataTableMgr.h"
#include "GroundTable.h"
#include "FloorTable.h"
#include "ObjectTable.h"
#include "ItemTable.h"
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

	DataTable* itemSaveTable = new ItemSave(DataTable::Types::SAVE);
	tables.insert({ DataTable::Types::SAVE, itemSaveTable });

	// 불러오기: TO-DO: 게임 초기에 수행되도록 위치 변경해야 함
	rapidjson::Document dataDoc;
	if (Utils::LoadFromFile("data/DataTable.json", dataDoc))
	{
		GROUND_TABLE->Load(dataDoc);
		FLOOR_TABLE->Load(dataDoc);
		OBJECT_TABLE->Load(dataDoc);
		ITEM_TABLE->Load(dataDoc);
	}

	 // rapidjson::Document doc파일을 load하고 위에서 동적할당한 inven의 LoadData에 doc를 적용한다.
	rapidjson::Document SaveDoc;
	if (Utils::LoadFromFile("data/example.json", SaveDoc))
	{
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