#include "pch.h"
#include "ItemSave.h"

ItemSave::ItemSave(DataTable::Types type) : DataTable(type)
{
}

ItemSave::~ItemSave()
{
	Release();
}

std::list<ItemData*>* ItemSave::Get(int boxId)
{
	auto find = itemBoxTable.find(boxId);
	if (find == itemBoxTable.end())
	{
		std::cout << "Item Data에 해당 Type과 ID가 없습니다" << std::endl;
		return nullptr;
	}
	return find->second;
}

const std::pair<int, std::list<ItemData*>*>& ItemSave::AddITemBox()
{
	int itemBoxCount = itemBoxTable.size();
	std::pair<int, std::list<ItemData*>*> boxPair = std::make_pair(-1, nullptr);
	for (int i = 2; i <= itemBoxCount; ++i)
	{
		auto find = itemBoxTable.find(i);
		if (find == itemBoxTable.end())
		{
			std::list<ItemData*>* items = new std::list<ItemData*>;
			boxPair.first = i;
			boxPair.second = items;
			return boxPair;
		}
	}
	std::cout << "아이템 박스를 추가할 수 없습니다!" << std::endl;
	return boxPair;
}

bool ItemSave::Load(rapidjson::Document& doc)
{
	Release();

	auto infoArr = doc["ItemData"].GetArray();
	int itemCount = infoArr.Size();
	for (int i = 0; i < itemCount; ++i)
	{
		int boxId = infoArr[i]["Box ID"].GetInt();
		std::list<ItemData*>* items = nullptr;
		auto find = itemBoxTable.find(boxId);
		if (find == itemBoxTable.end())
		{
			items = new std::list<ItemData*>;
			itemBoxTable[boxId] = items;
		}
		else
		{
			items = find->second;
		}

		int indexX = infoArr[i]["Index X"].GetInt();
		int indexY = infoArr[i]["Index Y"].GetInt();
		for (auto item : *items)
		{
			if (indexX == item->IndexX && indexY == item->IndexY)
			{
				std::cout << "아이디가 중복된 아이템이 있습니다!" << std::endl;
				std::cout << "Box ID: " << boxId << ", X: " << indexX << ", Y: " << indexY << std::endl;
				return false;
			}
		}

		ItemData* itemData = new ItemData;
		itemData->BoxId = boxId;
		itemData->IndexX = indexX;
		itemData->IndexY = indexY;
		itemData->itemId = infoArr[i]["ItemId"].GetInt();
		itemData->instanceId = infoArr[i]["InstanceId"].GetInt();
		itemData->count = infoArr[i]["Count"].GetInt();
		itemData->canOverLap = infoArr[i]["CanOverlap"].GetBool();
		itemData->type = (ItemType)infoArr[i]["ItemType"].GetInt();
		// itemData->itemFilePath = infoArr[i]["Resource"].GetString();

		items->push_back(itemData);
	}
	return true;
}

void ItemSave::Release()
{
	for (auto itemBox : itemBoxTable)
	{
		if (itemBox.second != nullptr)
		{
			for (auto item : *itemBox.second)
			{
				if (item != nullptr)
				{
					delete item;
					item = nullptr;
				}
			}

			delete itemBox.second;
			itemBox.second = nullptr;
		}
	}

	itemBoxTable.clear();
}
