#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

class SceneGame;
class Player;

struct UiShopSlot
{
	SpriteGo itemSlot;
	SpriteGo itemImgBox;
	SpriteGo itemImg;
	SpriteGo coinImg;
	TextGo itemName;
	TextGo itemPrice;
	sf::RectangleShape clickBox;
};

class UiShop : public GameObject
{
protected:
	SpriteGo sellerPortrait;
	SpriteGo sellerTextBox;
	TextGo sellerText;

	SpriteGo itemListBox;
	std::vector<UiShopSlot*> shopSlots;
	const int slotCount = 4;
	int currentIndex = 0;
	int countAllItem = 0;

	std::vector<ItemType> itemTypes = { ItemType::Tool, ItemType::Seed };
	std::vector<DataItem> itemTable;

public:
	UiShop(const std::string& name = "");
	~UiShop() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void UpdateIndex();
};

