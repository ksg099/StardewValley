#pragma once
#include "SpriteGo.h"

class TileMap;
class Inventory;
class BoxInven;
class UiStore;
class TileData;

class Player;
class ChangeHarvest;

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
	Player* player = nullptr;
	Inventory* inventory;
	BoxInven* boxInven;
	TileData* tileData = nullptr;

	SpriteGo* overlay = nullptr;

	//std::vector<ChangeHarvest*> crops;
	const int sellingBoxId = 1;

	int day = 1;
	int col = 30;

	float gridStartX = 0.f; //ó�� �׷����� x��ǥ
	float gridStartY = 0.f; //ó�� �׷����� y��ǥ
	float dailyTime = 6.f;
	float transitionTime = 0.f;
	float transitionDuration = 5.f; //��ȯ�� �ɸ��� �ð�
	float progress = 0.f;

	std::list<DropItem*> dropItemList;

	sf::Vector2f tileSize = { 25.f, 25.f };
	//const sf::Vector2f& tileSize = tileMap->GetCellSize();
	sf::Vector2f IndexToPos(int index);
	sf::Color dayColor = sf::Color(255, 255, 255, 0);
	sf::Color eveningColor = sf::Color(255, 100, 0, 100);
	sf::Color nightColor = sf::Color(0, 0, 0, 100);
	sf::Color currentColor;
	sf::Color targetColor;

	UiStore* uiStore = nullptr;

	bool isUiOpen = false;

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
	void Draw(sf::RenderWindow& window, Layers layer = Layers::Everything) override;

	void SetInventory();
	void SellAllItemsInBox();

	void InteractPlayerWithTileMap(const int x, const int y, const ItemType type, const int id);

	void OpenStoreUi();
};