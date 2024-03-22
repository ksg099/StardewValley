#pragma once
#include "SpriteGo.h"

class TileMap;

class Player : public SpriteGo
{
private:
	TileMap* tileMap = nullptr;

	sf::Vector2i gridIndex = { 0, 0 };

	bool isMove = false;
	sf::Vector2f currentGridPosition = { 0.f, 0.f };
	sf::Vector2f nextGridPosition = { 0.f, 0.f };
	float speed = 300.f;
	sf::Vector2i tileMoveDirection = { 0, 0 };
	float moveTimer = 0.f;
	float moveDuration = 0.f;

	sf::Vector2i lookDir = { 0, 0 };

	// free move
	sf::Vector2f freeMoveDirection = { 0.f, 0.f };
	float playerHalfWidth = 0.f;

public:
	Player(const std::string& name = "");
	~Player() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void MoveTileUnit(float dt);
	void CheckCollision(sf::Vector2f& nextPos, sf::Vector2f& prevPos);
	void ChangeGridIndex(sf::Vector2f& nextPos);
};

