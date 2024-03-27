#include "pch.h"
#include "ItemTable.h"

DataItem ItemTable::Undefined;

ItemTable::ItemTable(DataTable::Types type) : DataTable(type)
{
}

ItemTable::~ItemTable()
{
	Release();
}

const DataItem& ItemTable::Get(ItemType type, int id)
{
	auto find = table.find(std::make_tuple(type, id));
	if (find == table.end())
	{
		std::cout << "Item Data에 해당 Type과 ID가 없습니다" << std::endl;
		return Undefined;
	}
	return find->second;
}

const DataItem& ItemTable::Get(const std::string& name)
{
	auto find = nameTable.find(name);
	if (find == nameTable.end())
	{
		std::cout << "Item Data에 해당 이름이 없습니다" << std::endl;
		return Undefined;
	}

	return Get(std::get<0>(find->second), std::get<1>(find->second));
}

const int ItemTable::Count(ItemType type)
{
	int typeNum = (int)type;

	if (typeNum < 0 || typeNum >= countTable.size())
		return 0;

	return countTable[(int)type];
}

bool ItemTable::Load(rapidjson::Document& doc)
{
	Release();

	auto infoArr = doc["Item Info"].GetArray();
	int itemTypeCount = infoArr.Size();
	for (int i = 0; i < itemTypeCount; ++i)
	{
		int itemIdCount = infoArr[i]["Sheet Info"].Size();
		countTable.push_back(itemIdCount);
		for (int j = 0; j < itemIdCount; ++j)
		{
			std::tuple<ItemType, int> key = std::make_tuple((ItemType)i, j);

			if (table.find(key) != table.end())
			{
				std::cout << "object table 중복 아이디" << std::endl;
				return false;
			}

			table[key] = DataItem();
			table[key].itemType = (ItemType)i;
			table[key].itemId = j;
			table[key].name = infoArr[i]["Sheet Info"][j]["Item Name"].GetString();
			table[key].sellingPrice = infoArr[i]["Sheet Info"][j]["Selling Price"].GetInt();
			table[key].purchasePrice = infoArr[i]["Sheet Info"][j]["Purchase Price"].GetInt();
			table[key].textureId = infoArr[i]["Sheet Info"][j]["Resource"].GetString();
			table[key].sheetId = { infoArr[i]["Sheet Info"][j]["Sheet ID X"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID Y"].GetInt() };
			table[key].sheetSize = { infoArr[i]["Sheet Info"][j]["Sheet ID W"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID H"].GetInt() };

			nameTable[infoArr[i]["Sheet Info"][j]["Item Name"].GetString()] = key;
		}
	}

	return true;
}

void ItemTable::Release()
{
	table.clear();
	nameTable.clear();
	countTable.clear();
}
