#pragma once
#include "SpriteGo.h"
#include "TextGo.h"

class InvetorySlot;
class ItemData;

class BoxInven : public SpriteGo
{
protected:
	const int countX = 10;
	const int countY = 3;

	SpriteGo smallUi;
	TextGo itemInfoText;

	//int firstClickBoxId = -1;
	//int firstClickIndex = -1;
	ItemData* itemClick = nullptr;
	ItemData* itemExchange = nullptr;

	sf::Sprite firstClickSprite;
	bool isClick = false;

	std::vector<InvetorySlot*> firtstSlots;
	std::list<ItemData*>* firstItems;
	int firstBoxId = 0;

	std::vector<InvetorySlot*> secondSlots;
	std::list<ItemData*>* secondItems;
	int secondBoxId = 1;

	sf::Vector2f currentMousePos;
	sf::FloatRect slotBounds;
	
public:
	BoxInven(const std::string& name = "");
	~BoxInven() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	bool IsItemInBox(ItemData* item, int boxId) { return item->BoxId == boxId; }

	void SetPosition(const sf::Vector2f& pos) override;
	void SetIvenSlot(int x, int y, ItemData* data, std::vector<InvetorySlot*>& slots);
	void UpdateSlots();
	void SwapItem(int firstClickIndex, int secondClickIndex);
	void DisplayItemInfo(ItemData& itemData, sf::Vector2f& position);

	ItemData* GetItemData(const int x, const int y, const bool isFirstBox) const;
	std::pair<int, bool> CheckPosIndex(sf::Vector2f uiPos);
	ItemData* SaveClickItemData(sf::Vector2f uiPos);
	void SwapItemDiffBox(ItemData* itemClick, ItemData* itemExchange, bool isClickFirst);
};

