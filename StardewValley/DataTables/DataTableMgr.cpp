#include "pch.h"
#include "DataTableMgr.h"
#include "GroundTable.h"

#include "ObjectTable.h"
#include "ItemTable.h"

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

	DataTable* objectTable = new ObjectTable(DataTable::Types::OBJECT);
	tables.insert({ DataTable::Types::OBJECT, objectTable });

	DataTable* itemTable = new ItemTable(DataTable::Types::ITEM);
	tables.insert({ DataTable::Types::ITEM, itemTable });
}

void DataTableMgr::Release()
{
	for (auto pair : tables)
	{
		delete pair.second;
	}
	tables.clear();
}