#pragma once
#include "Inventory.h"
class BoxInven : public Inventory
{
protected:
	std::vector<InvetorySlot*> secondSlots; //두번째 slot 컨테이너
	std::list<ItemData*>* secondItems;
	int boxId = 1;

public:
	BoxInven(const std::string& name = "");
	~BoxInven() override = default;

	bool SecondisAble = true;
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetIvenSlot(int x, int y, ItemData* data) override;
	void UpdateSlots() override;
	void UpdateSubSlots() override;
	//void AddItem(ItemData* currentItem) override;
	void SwapItem(int firstClickIndex, int secondClickIndex) override;
	void DisplayItemInfo(ItemData& itemData, sf::Vector2f& position) override;

};

