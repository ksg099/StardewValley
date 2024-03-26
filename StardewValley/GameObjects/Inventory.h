#pragma once
#include "SpriteGo.h"
#include "TextGo.h"

class InvetorySlot;
class ItemData;

class Inventory : public SpriteGo
{
protected:

	//::string itemInfoText;

	int countX = 10;
	int countY = 3;
	int secondcountX = 10;
	int secondcountY = 3;

	int index;

	SpriteGo smallUi;
	TextGo itemInfoText;
	//�ε��� �� �Ҵ�
	int firstClickIndex = -1;
	sf::Sprite firstClickSprite;
	bool isClick = false;
	
	std::vector<InvetorySlot*> slots; //slot �����̳�
	std::vector<InvetorySlot*> smallslots; //smallslot �����̳�
	std::list<ItemData*>* items;
	
	
	sf::Vector2f currentMousePos;
	sf::FloatRect slotBounds;

	// ���� �κ��丮
	int subIndexY = 0;

	const int inventoryBoxId = 0;

public:

	Inventory(const std::string& name = "");
	~Inventory() override = default;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	bool IsItemInBox(ItemData* item, int boxId) { return item->BoxId == boxId; }
	
	virtual void SetPosition(const sf::Vector2f& pos) override;
	virtual void SetIvenSlot(int x, int y, ItemData* data); 
	virtual void UpdateSlots();
	virtual void UpdateSubSlots();
	//virtual void AddItem(ItemData* currentItem);
	virtual void SwapItem(int firstClickIndex, int secondClickIndex);
	virtual void DisplayItemInfo(ItemData& itemData, sf::Vector2f& position);

	const int GetSubSlotIndexY() const { return subIndexY; }
	ItemData* GetItemData(const int x, const int y) const;
};