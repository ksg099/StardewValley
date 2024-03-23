#include "pch.h"
#include "ItemSave.h"

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
	//???
}
