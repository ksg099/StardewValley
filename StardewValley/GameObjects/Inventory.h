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

	//인덱스 값 할당
	int firstClickIndex = -1;

	//SpriteGo invenLine;
	std::vector<InvetorySlot*> slots; //slot 컨테이너
	std::vector<InvetorySlot*> smallslots; //smallslot 컨테이너
	std::list<ItemData*>* items; // 임시

	sf::Vector2f currentMousePos;
	sf::FloatRect slotBounds;

	// 서브 인벤토리
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
	// void LoadData(rapidjson::Document& doc);
	void SetIvenSlot(int x, int y, ItemData* data);

	void UpdateSlots();
	void UpdateSubSlots();

	void SwapItem(int firstClickIndex, int secondClixkIndex);
};

