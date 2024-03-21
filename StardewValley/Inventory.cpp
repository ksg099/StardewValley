#include "pch.h"
#include "Inventory.h"
#include "InvetorySlot.h"

Inventory::Inventory(const std::string& name) : SpriteGo(name)
{
}

void Inventory::Init()
{
	SpriteGo::Init();

	//인벤토리 창 구현
	SetTexture("graphics/Inventory.png");

	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			InvetorySlot* slot = new InvetorySlot;
			slots.push_back(slot);
		}
	}
}

void Inventory::Release()
{
	for (auto slot : slots)
	{
		if (slot != nullptr)
		{
			delete slot;
			slot = nullptr;
		}
	}
	slots.clear();

	SpriteGo::Release();
}

void Inventory::Reset()
{
	SetPosition({ 0.f, 0.f });
	SetScale({ 10.f, 5.f });
	SetOrigin(Origins::MC);
	SpriteGo::Reset();
}

void Inventory::Update(float dt)
{
	SpriteGo::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		isAble = !isAble;
	}
}

void Inventory::Draw(sf::RenderWindow& window)
{
	if (!isAble)
	{
		SpriteGo::Draw(window);

		for (auto slot : slots)
		{
			slot->Draw(window);
		}
	}
}

void Inventory::LoadData(rapidjson::Document& doc)
{
	//슬롯 순회 돌기전에 미리 초기화
	for (auto slot : slots)
	{
		if (slot != nullptr)
		{
			delete slot;
			slot = nullptr;
		}
	}
	slots.clear();

	//JSON 파일에 정의된 각 아이템에 대한 InventorySlot 객체를 생성하고 초기화하여, 게임 내 인벤토리 시스템에 아이템을 추가
	for (int i = 0; i < countY; i++) // 3행
	{
		for (int j = 0; j < countX; j++) //10열
		{
			//rapidjson::Document의 객체 doc를 이용해서 배열ItemData의 i번째 정보를 읽는다.
			//읽은 정보를 itemData에 할당한다.
			ItemData* itemData = new ItemData;
			itemData->BoxId = doc["ItemData"][i * countX + j]["Box ID"].GetInt();
			itemData->IndexX = doc["ItemData"][i * countX + j]["Index X"].GetInt();
			itemData->IndexY = doc["ItemData"][i * countX + j]["Index Y"].GetInt();
			itemData->itemId = doc["ItemData"][i * countX + j]["ItemId"].GetInt();
			itemData->instanceId = doc["ItemData"][i * countX + j]["InstanceId"].GetInt();
			itemData->count = doc["ItemData"][i * countX + j]["Count"].GetInt();
			itemData->canOverLap = doc["ItemData"][i * countX + j]["CanOverlap"].GetBool();
			itemData->type = (ItemType)doc["ItemData"][i * countX + j]["ItemType"].GetInt();
			itemData->itemFilePath = doc["ItemData"][i * countX + j]["Resource"].GetString();
			
			//slot객체를 동적할당하여 위에서 할당한 itemData를 slot 컨테이너에 추가하여 관리한다.
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			slot->Init();
			slot->Reset();
			slot->SetItemData(itemData);

			slot->SetPosition({ (float)i * 60.f, (float)j * 60.f });
			slot->SetOrigin(Origins::MC);

			slots.push_back(slot);
		}
	}
}
