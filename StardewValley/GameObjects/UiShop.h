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
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;

	SpriteGo sellerPortrait;
	SpriteGo sellerTextBox;
	TextGo sellerText;

	SpriteGo itemListBox;
	std::vector<UiShopSlot*> shopSlots;
	const int slotCount = 4;
	int currentIndex = 0;
	int countAllItem = 0;

	SpriteGo scrollBar;
	SpriteGo scroll;

	int clickIndex = -1;
	int purchaseIndex = -1;
	float doubleClickTimer = 0.f;
	float doubleClickDuration = 0.3f;
	bool isClick = false;

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
	void SelectByDoubleClick(float dt);
};

