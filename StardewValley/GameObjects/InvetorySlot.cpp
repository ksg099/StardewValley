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

	isDraw = true;

	itemCountText.Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), "", 20, sf::Color::Black);
	itemCountText.SetOutline(sf::Color::White, 1.f);

	itemCountText.SetPosition(position + sf::Vector2f(20.f, 20.f));
	itemCountText.SetOrigin(Origins::BR);
}

void InvetorySlot::Update(float dt)
{
	SpriteGo::Update(dt);
}

void InvetorySlot::SetEmpty()
{
	itemData = nullptr;
	itemCountText.SetActive(false);
	itemCountText.SetString("");
}

//���Կ� ������ �׸��⸸ ���ش�.
void InvetorySlot::SetItem(ItemData* data)
{
	itemData = data;

	//maxCount�ʰ��� maxCount�� �Ҵ�
	//if (itemData->count > maxCount)
	//{
	//	itemData->count = maxCount;
	//}


	//�ش� �������� ������, �������� ���� ����
	auto item = ITEM_TABLE->Get(itemData->type, itemData->itemId);
	icon.SetTextureRect(sf::IntRect(item.sheetId.x, item.sheetId.y, item.sheetSize.x, item.sheetSize.y));
	icon.SetTexture(item.textureId);
	float scale = item.sheetSize.x > item.sheetSize.y ? item.sheetSize.x : item.sheetSize.y;
	icon.SetScale(sf::Vector2f(50.f / scale, 50.f / scale));
	// icon.SetScale({ 50.f / item.sheetSize.x, 50.f / item.sheetSize.y }); //�̺κ� ������ ����� ������ �ٽ� �����ʿ��ҵ�
	icon.SetOrigin(Origins::MC);
	//itemCountText.SetString(std::to_string(itemData->count));
	//itemCountText.SetActive(true);
	isDraw = true;

	itemCountText.SetString(std::to_string(itemData->count));
	itemCountText.SetPosition(position + sf::Vector2f(20.f, 20.f));
	itemCountText.SetOrigin(Origins::BR);
}

void InvetorySlot::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	//������ �����Ͱ� ���� ��쿡�� �׸��� ���� �˷��ֱ�
	if (itemData != nullptr && isDraw)
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
	//itemCountText.SetPosition(pos);

	//sf::Vector2f itemCountPosition = icon.GetPosition();
	//itemCountText.SetPosition(itemCountPosition);

}

