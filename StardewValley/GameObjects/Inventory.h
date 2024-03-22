#pragma once
#include "SpriteGo.h"

class InvetorySlot;
class ItemData;

class Inventory : public SpriteGo
{
protected:
	int countX = 10;
	int countY = 3;
	int index;

	SpriteGo smallUi;

	//�ε��� �� �Ҵ�
	int firstClickIndex = -1;

	//SpriteGo invenLine;
	std::vector<InvetorySlot*> slots; //slot �����̳�
	std::vector<InvetorySlot*> smallslots; //smallslot �����̳�
	std::list<ItemData*> items; // �ӽ�

	sf::Vector2f currentMousePos;
	sf::FloatRect slotBounds;

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
	void LoadData(rapidjson::Document& doc);
	void SetIvenSlot(int x, int y, ItemData* data);

	void UpdateSlots(const std::list<ItemData*>& list);

	void SwapItem(int firstClickIndex, int secondClixkIndex);
};

