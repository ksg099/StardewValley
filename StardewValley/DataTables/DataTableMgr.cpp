#include "pch.h"
#include "DataTableMgr.h"
#include "GroundTable.h"

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

	DataTable* table = new GroundTable(DataTable::Types::GROUND);
	tables.insert({ DataTable::Types::GROUND, table });
}

void DataTableMgr::Release()
{
	for (auto pair : tables)
	{
		delete pair.second;
	}
	tables.clear();
}