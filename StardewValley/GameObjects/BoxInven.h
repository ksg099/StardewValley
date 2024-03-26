#pragma once
#include "SpriteGo.h"
#include "TextGo.h"

class InvetorySlot;
class ItemData;

class BoxInven : public SpriteGo
{
protected:
	

	//::string itemInfoText;

	int countX = 10;
	int countY = 3;

	SpriteGo smallUi;
	TextGo itemInfoText;
	int firstClickBoxId = -1;
	int firstClickIndex = -1;

	
	
	std::vector<InvetorySlot*> firtstSlots;
	std::list<ItemData*>* firstItems;
	int firstBoxId = 0;

	std::vector<InvetorySlot*> secondSlots;
	std::list<ItemData*>* secondItems;
	int secondBoxId = 1;

	sf::Vector2f currentMousePos;
	sf::FloatRect slotBounds;
	bool isBoxInvenOpen;
	bool clickedInFirstInven = false;
public:
	BoxInven(const std::string& name = "");
	~BoxInven() override = default;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetBoxInvenOpen(const bool isBoxInvenOpen) { this->isBoxInvenOpen = isBoxInvenOpen; }
	bool IsBoxInvenOpen() const { return isBoxInvenOpen; }

	bool IsItemInBox(ItemData* item, int boxId) { return item->BoxId == boxId; }

	virtual void SetPosition(const sf::Vector2f& pos) override;
	virtual void SetIvenSlot(int x, int y, ItemData* data, std::vector<InvetorySlot*>& slots);
	virtual void UpdateSlots();
	virtual void SwapItem(int firstclickSlotIndex, int secondclickSlotIndex, bool clickedInFirstInven, bool clickedInSecondInven);
	virtual void DisplayItemInfo(ItemData& itemData, sf::Vector2f& position);

	ItemData* GetItemData(const int x, const int y) const;
};

