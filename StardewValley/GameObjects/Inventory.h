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
	int index;

	SpriteGo smallUi;
	TextGo itemInfoText;
	//�ε��� �� �Ҵ�
	int firstClickIndex = -1;
	
	std::vector<InvetorySlot*> slots; //slot �����̳�
	std::vector<InvetorySlot*> smallslots; //smallslot �����̳�
	std::list<ItemData*>* items;

	sf::Vector2f currentMousePos;
	sf::FloatRect slotBounds;

	// ���� �κ��丮
	int subIndexY = 0;

	int boxId = 0;

public:

	bool isAble = true;
	Inventory(const std::string& name = "");
	~Inventory() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetIvenSlot(int x, int y, ItemData* data);

	void UpdateSlots();
	void UpdateSubSlots();

	void AddItem(ItemData* currentItem);
	void SwapItem(int firstClickIndex, int secondClixkIndex);
	void DisplayItemInfo(ItemData& itemData, sf::Vector2f& position);

	const int GetSubSlotIndexY() const { return subIndexY; }
	ItemData* GetItemData(const int x, const int y) const;
};