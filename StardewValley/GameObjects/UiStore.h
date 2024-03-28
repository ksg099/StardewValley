#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

class SceneGame;
class Player;

struct UiItem
{
	sf::Sprite itemBackground;
	sf::RectangleShape clickBox;
	sf::Sprite itemImg;
	TextGo itemInfo;
};

class UiStore : public GameObject
{
protected:
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;

	sf::Vector2f referenceResolution = { 1920, 1080 };
	sf::Vector2f resolution = referenceResolution;

	sf::Sprite StoreBackground;
	std::vector<UiItem*> UipurchaseItems;

	int countInPage = 6;
	int currentIndex = 0;
	int countAllItem = 0;

	int clickIndex = -1;
	int purchaseIndex = -1;
	float doubleClickTimer = 0.f;
	float doubleClickDuration = 0.3f;
	bool isClick = false;

	std::vector<ItemType> itemTypes = { ItemType::Tool, ItemType::Seed };
	std::vector<DataItem> itemTable;
	
public:
	UiStore(const std::string& name = "");
	~UiStore() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SelectByDoubleClick(float dt);

	void UpdateIndex();
};