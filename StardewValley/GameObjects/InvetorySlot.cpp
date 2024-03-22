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
	SetEmpty();
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
	itemData = data;

	if (itemData->itemId == data->itemId && data->count > maxCount) //한칸에 지정한 개수(5)가 넘어가면
	{
		//if (data->count > maxCount)
		//{
			data->count = maxCount;
		/*}*/
		//else
		//{
		//	itemData->count++;
		//}
	}

	//해당 아이템의 아이콘, 아이콘의 현재 개수
	auto item = ITEM_TABLE->Get(itemData->type, itemData->itemId);
	// icon.SetTextureRect(sf::IntRect(item.sheetId.x, item.sheetId.y, item.sheetSize.x, item.sheetSize.y));
	icon.SetTexture(item.textureId);
	icon.SetScale({ 50.f / item.sheetSize.x, 50.f / item.sheetSize.y });
	icon.SetOrigin(Origins::TL);
	itemCountText.SetString(std::to_string(itemData->count));
	itemCountText.SetActive(true);
}

void InvetorySlot::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	//아이템 데이터가 있을 경우에만 그리고 개수 알려주기
	if (itemData != nullptr)
	{
		icon.Draw(window);
		if (itemCountText.GetActive())
		{
			itemCountText.Draw(window);
		}
	}
}

void InvetorySlot::SetPosition(const sf::Vector2f& pos)
{
	SpriteGo::SetPosition(pos);
	
	
	background.SetOrigin(Origins::MC);
	icon.SetOrigin(Origins::MC);
	itemCountText.SetOrigin(Origins::MC);

	background.SetPosition(pos);
	icon.SetPosition(pos);
	itemCountText.SetPosition(pos);

}

