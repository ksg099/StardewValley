#pragma once
#include "SpriteGo.h"

class SceneGame;
class TileMap;
class Inventory;
class SubInventory;

class Player : public SpriteGo
{
private:
	SceneGame* sceneGame = nullptr;
	TileMap* tileMap = nullptr;
	Inventory* inventory = nullptr;
	SubInventory* subInven = nullptr;
	ItemData* itemInUse = nullptr;


	sf::Vector2i gridIndex = { 0, 0 };

	bool isMove = false;
	sf::Vector2f currentGridPosition = { 0.f, 0.f };
	sf::Vector2f nextGridPosition = { 0.f, 0.f };
	float speed = 100.f;
	sf::Vector2i tileMoveDirection = { 0, 0 };
	float moveTimer = 0.f;
	float moveDuration = 0.f;

	Sides side = Sides::None;
	sf::Vector2i lookDir = { 0, 0 };

	// free move
	sf::Vector2f freeMoveDirection = { 0.f, 0.f };
	float playerHalfWidth = 0.f;

	Animator animator;

public:
	Player(const std::string& name = "");
	~Player() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	//sf::Vector2f GetPosition() const { return position; }
	float GetSpeed() { return speed; }
	const sf::Vector2i GetGridIndex() const { return gridIndex; }
	// void MoveTileUnit(float dt);
	void PlayMoveAnimation(sf::Vector2f posDIff);
	void CheckCollision(sf::Vector2f& nextPos, sf::Vector2f& prevPos);
	void ChangeGridIndex(sf::Vector2f& nextPos);

	// void SetPlant();
};

