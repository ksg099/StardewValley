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
	hasHitBox = true;
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

//슬롯에 아이템 그리기만 해준다.
void InvetorySlot::SetItem(ItemData* data)
{
	itemData = data;

	//maxCount초과시 maxCount로 할당
	if (itemData->count > maxCount)
	{
		itemData->count = maxCount;
	}


	//해당 아이템의 아이콘, 아이콘의 현재 개수
	auto item = ITEM_TABLE->Get(itemData->type, itemData->itemId);
	// icon.SetTextureRect(sf::IntRect(item.sheetId.x, item.sheetId.y, item.sheetSize.x, item.sheetSize.y));
	icon.SetTexture(item.textureId);
	icon.SetScale({ 50.f / item.sheetSize.x, 50.f / item.sheetSize.y }); //이부분 슬롯의 가운데에 오도록 다시 정리필요할듯
	icon.SetOrigin(Origins::TL);

	//
	itemCountText.SetString(std::to_string(itemData->count));
	itemCountText.SetActive(true);
}

void InvetorySlot::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	//아이템 데이터가 있을 경우에만 그리고 개수 알려주기
	if (itemData != nullptr && itemData->count > 0)
	{
		icon.Draw(window);
		itemCountText.Draw(window);
		//if (itemCountText.GetActive())
		//{
		//	itemCountText.Draw(window);
		//}
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

	sf::Vector2f itemCountPosition = icon.GetPosition();
	itemCountText.SetPosition(itemCountPosition);

}

