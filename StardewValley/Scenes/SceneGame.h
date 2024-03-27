#pragma once
#include "SpriteGo.h"

class TileMap;
class Inventory;
class BoxInven;
class TileData;
class Player;

class SceneGame : public Scene
{
public:
	struct DropItem
	{
		sf::Sprite itemSprite;
		int count = 1;
		ItemType itemType = ItemType::Ingredient;
		int itemID = 0;

	};

protected:
	TileMap* tileMap = nullptr;
	// const sf::Vector2f tileSize = { 25.f, 25.f };
	TileData* tileData = nullptr;
	Player* player = nullptr;


	float dailyTime = 6.f;
	int day = 1;

	std::list<DropItem*> dropItemList;

	SpriteGo* layer = nullptr;

	sf::Color dayColor = sf::Color(255, 255, 255, 0);
	sf::Color eveningColor = sf::Color(255, 100, 0, 100);
	sf::Color nightColor = sf::Color(0, 0, 0, 100);
	sf::Color currentColor;
	sf::Color targetColor;
	float transitionTime = 0.f;
	float transitionDuration = 5.f; //전환에 걸리는 시간
	float progress = 0.f;

	Inventory* inventory;
	BoxInven* boxInven;
	const int sellingBoxId = 1;

public:
	SceneGame(SceneIds id);
	virtual ~SceneGame();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	sf::Color LerpColor(const sf::Color& start, const sf::Color& end, float t);
	void CreateItem(DataItem data, int indexX, int indexY);
	
	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;

	void SetInventory();
	void SellAllItemsInBox();
};