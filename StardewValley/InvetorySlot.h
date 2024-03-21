#pragma once
#include "SpriteGo.h"
#include "TextGo.h"
#include "Item.h"

class InvetorySlot : public SpriteGo
{
protected:
	ItemData* itemData;

	SpriteGo background; //�⺻ ����ִ� ���� �̹���
	SpriteGo icon; //������ ������ �̹���
	TextGo itemCountText; //������ ���� �����
	
	int maxCount = 5; //��ĭ �� �ִ� ������ ����

public:

	InvetorySlot(const std::string& name = "");
	~InvetorySlot() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	
	void SetEmpty(); //��� �ִ� ����
	void SetItem(ItemData* data); // ItemData�� �����͸� �޾� �������� ����

	void SetItemData(ItemData*& data) { itemData = data; }
};

