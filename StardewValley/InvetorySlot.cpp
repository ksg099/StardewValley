#include "pch.h"
#include "InvetorySlot.h"

InvetorySlot::InvetorySlot(const std::string& name) : SpriteGo(name)
{
}

void InvetorySlot::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/InventorySlot.png");
}

void InvetorySlot::Release()
{
	SpriteGo::Release();
}

void InvetorySlot::Reset()
{
	SpriteGo::Reset();
}

void InvetorySlot::Update(float dt)
{
	SpriteGo::Update(dt);
}

void InvetorySlot::SetEmpty()
{
	itemData = nullptr;
	itemCountText.SetActive(false);
}

void InvetorySlot::SetItem(ItemData* data)
{
	//itemData = data;
	if (data->count > maxCount) //��ĭ�� ������ ����(5)�� �Ѿ��
	{
		data->count = maxCount;
	}

	//�ش� �������� ������, �������� ���� ����
	icon.SetTexture(itemData->itemFilePath);
	itemCountText.SetString(std::to_string(itemData->count));
	itemCountText.SetActive(true);
}

void InvetorySlot::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	//������ �����Ͱ� ���� ��쿡�� �׸��� ���� �˷��ֱ�
	if (itemData != nullptr)
	{
		icon.Draw(window);
		if (itemCountText.GetActive())
		{
			itemCountText.Draw(window);
		}
	}
}

