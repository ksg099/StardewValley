#include "pch.h"
#include "Inventory.h"
#include "InvetorySlot.h"

Inventory::Inventory(const std::string& name) : SpriteGo(name)
{
}

void Inventory::Init()
{
	SpriteGo::Init();

	//�κ��丮 â ����
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
	//���� ��ȸ �������� �̸� �ʱ�ȭ
	for (auto slot : slots)
	{
		if (slot != nullptr)
		{
			delete slot;
			slot = nullptr;
		}
	}
	slots.clear();

	//JSON ���Ͽ� ���ǵ� �� �����ۿ� ���� InventorySlot ��ü�� �����ϰ� �ʱ�ȭ�Ͽ�, ���� �� �κ��丮 �ý��ۿ� �������� �߰�
	for (int i = 0; i < countY; i++) // 3��
	{
		for (int j = 0; j < countX; j++) //10��
		{
			//rapidjson::Document�� ��ü doc�� �̿��ؼ� �迭ItemData�� i��° ������ �д´�.
			//���� ������ itemData�� �Ҵ��Ѵ�.
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
			
			//slot��ü�� �����Ҵ��Ͽ� ������ �Ҵ��� itemData�� slot �����̳ʿ� �߰��Ͽ� �����Ѵ�.
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
