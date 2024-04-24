#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

class SceneGame;
class Player;

struct UiShopSlot
{
	sf::Sprite itemSlot;
	sf::RectangleShape clickBox;
	sf::Sprite itemImg;
	TextGo itemInfo;
};

class UiShop : public GameObject
{
protected:
	SpriteGo sellerPortrait;
	SpriteGo sellerTextBox;
	TextGo sellerText;

public:
	UiShop(const std::string& name = "");
	~UiShop() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

