#pragma once
#include "SpriteGo.h"

class InvetorySlot;
class ItemData;

class SubInventory : public SpriteGo
{
protected:
	const int countX = 10;
	const int countY = 3;

	SpriteGo smallUi;
	std::vector<InvetorySlot*> smallslots;
	sf::RectangleShape usingItemSlot;

	std::list<ItemData*>* items;
	const int inventoryBoxId = 0;

	int subIndexY = 0;
	int subIndexX = 0;

public:
	SubInventory(const std::string& name = "");
	~SubInventory() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void UpdateSlots();
	const int GetSubSlotIndexY() const { return subIndexY; }






	//// bool IsItemInBox(ItemData* item, int boxId) { return item->BoxId == boxId; }

	//virtual void SetPosition(const sf::Vector2f& pos) override;
	//virtual void SetIvenSlot(int x, int y, ItemData* data);
	//
	//virtual void UpdateSubSlots();
	////virtual void AddItem(ItemData* currentItem);
	//virtual void SwapItem(int firstClickIndex, int secondClickIndex);
	//virtual void DisplayItemInfo(ItemData& itemData, sf::Vector2f& position);

	//
	//ItemData* GetItemData(const int x, const int y) const;
};

