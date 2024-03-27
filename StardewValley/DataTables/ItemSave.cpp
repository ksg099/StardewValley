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

void ItemSave::Save(rapidjson::Document& doc)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	
	rapidjson::Value boxDataArr(rapidjson::kArrayType);
	for (auto itemBox : itemBoxTable)
	{
		if (itemBox.second == nullptr)
			continue;

		rapidjson::Value boxData;
		boxData.SetObject();
		boxData.AddMember("Box ID", itemBox.first, allocator);

		rapidjson::Value itemDataArr(rapidjson::kArrayType);
		for (auto item : *itemBox.second)
		{
			if (item == nullptr)
				continue;

			rapidjson::Value itemData;
			itemData.SetObject();
			itemData.AddMember("Index X", item->IndexX, allocator);
			itemData.AddMember("Index Y", item->IndexY, allocator);
			itemData.AddMember("Item Type", (int)item->type, allocator);
			itemData.AddMember("Item ID", item->itemId, allocator);
			itemData.AddMember("Instance ID", item->instanceId, allocator);
			itemData.AddMember("Count", item->count, allocator);
			itemData.AddMember("CanOverlap", item->canOverLap, allocator);
			itemDataArr.PushBack(itemData, allocator);
		}
		boxData.AddMember("Item Data", itemDataArr, allocator);
		boxDataArr.PushBack(boxData, allocator);
	}
	doc.AddMember("Box Data", boxDataArr, allocator);
}

bool ItemSave::Load(rapidjson::Document& doc)
{
	Release();

	auto boxArr = doc["Box Data"].GetArray();
	int boxCount = boxArr.Size();
	for (int i = 0; i < boxCount; ++i)
	{
		int boxId = boxArr[i]["Box ID"].GetInt();
		auto itemArr = boxArr[i]["Item Data"].GetArray();
		
		std::list<ItemData*>* items = new std::list<ItemData*>;
		itemBoxTable[boxId] = items;

		int itemCount = itemArr.Size();
		for (int j = 0; j < itemCount; ++j)
		{
			
			int indexX = itemArr[j]["Index X"].GetInt();
			int indexY = itemArr[j]["Index Y"].GetInt();

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
			itemData->type = (ItemType)itemArr[j]["Item Type"].GetInt();
			itemData->itemId = itemArr[j]["Item ID"].GetInt();
			itemData->instanceId = itemArr[j]["Instance ID"].GetInt();
			itemData->count = itemArr[j]["Count"].GetInt();
			itemData->canOverLap = itemArr[j]["CanOverlap"].GetBool();
			
			items->push_back(itemData);
		}
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
