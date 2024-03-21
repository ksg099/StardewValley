#pragma once
#include "SpriteGo.h"
class Item : public SpriteGo
{
protected:

	//enum class ITEMS_TYPE //도구
	//{
	//	NONE, //없어
	//	Tool, //도구
	//	Seed, //씨앗
	//	ingredient, //재료
	//	harvest, //수확물

	//};

	//ITEMS_TYPE type = ITEMS_TYPE::NONE;

public:

	Item(const std::string& name = "");
	~Item() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

