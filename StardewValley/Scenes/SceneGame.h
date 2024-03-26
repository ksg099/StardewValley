#pragma once
#include "SpriteGo.h"

class TileMap;
class Inventory;
class BoxInven;
class TestMapTool;
class Player;

class SceneGame : public Scene
{
protected:
	TileMap* tileMap = nullptr;
	TestMapTool* testMapTool;
	Player* player = nullptr;

	// const sf::Vector2f tileSize = { 25.f, 25.f };

	float dailyTime = 6.f;
	int day = 1;
	int col = 30;

	float gridStartX = 0.f; //처음 그려지는 x좌표
	float gridStartY = 0.f; //처음 그려지는 y좌표

	//int growUpday = 3;

	//sf::RectangleShape overlayer;

	SpriteGo* layer = nullptr;
	SpriteGo* Seed = nullptr;
	SpriteGo* Plants = nullptr;
	SpriteGo* Plant1 = nullptr;
	SpriteGo* Plant2 = nullptr;
	SpriteGo* Plant3 = nullptr;
	SpriteGo* Plant4 = nullptr;
	SpriteGo* Plant5 = nullptr;
	SpriteGo* Plant6 = nullptr;

	std::vector<SpriteGo*> plants; //식물 모음

	sf::Vector2f tileSize = { 25.f, 25.f };
	//const sf::Vector2f& tileSize = tileMap->GetCellSize();
	sf::Vector2f IndexToPos(int index);
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
public:
	SceneGame(SceneIds id);
	virtual ~SceneGame();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	sf::Color LerpColor(const sf::Color& start, const sf::Color& end, float t);
	
	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};