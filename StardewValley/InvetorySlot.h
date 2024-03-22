#pragma once
#include "SpriteGo.h"
#include "TextGo.h"
#include "Item.h"

class InvetorySlot : public SpriteGo
{
protected:

	enum class state
	{
		empty,
		full,
	};

	ItemData* itemData;

	SpriteGo background; //기본 비어있는 상태 이미지
	SpriteGo icon; //아이템 아이콘 이미지
	TextGo itemCountText; //아이템 개수 몇개인지
	
	int maxCount = 5; //한칸 당 최대 아이템 개수

public:

	InvetorySlot(const std::string& name = "");
	~InvetorySlot() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	
	void SetPosition(const sf::Vector2f& pos) override;


	void SetEmpty(); //비어 있는 상태
	void SetItem(ItemData* data); // ItemData의 데이터를 받아 아이템을 세팅

	void SetItemData(ItemData*& data) { itemData = data; }
};

