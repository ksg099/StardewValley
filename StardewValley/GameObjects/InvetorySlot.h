#pragma once
#include "SpriteGo.h"
#include "TextGo.h"

class InvetorySlot : public SpriteGo
{
protected:

	ItemData* itemData = nullptr;

	SpriteGo background; //�⺻ ����ִ� ���� �̹���
	SpriteGo icon; //������ ������ �̹���
	TextGo itemCountText; //������ ���� �����

	int maxCount = 5; //��ĭ �� �ִ� ������ ����

	bool isDraw = true;

public:

	InvetorySlot(const std::string& name = "");
	~InvetorySlot() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetIsDraw(const bool isDraw) { this->isDraw = isDraw; }

	void SetEmpty(); //��� �ִ� ����
	void SetItem(ItemData* data); // ItemData�� �����͸� �޾� �������� ����

	void SetItemData(ItemData*& data) { itemData = data; }
	ItemData* GetItemData() const { return itemData; }
};