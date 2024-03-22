#pragma once

class TileMap;
class Inventory;

class SceneGame : public Scene
{
protected:
	TileMap* tileMap = nullptr;
	const sf::Vector2f tileSize = { 25.f, 25.f };

	Inventory* inventory;
	Inventory* smallinventory;
public:
	SceneGame(SceneIds id);
	virtual ~SceneGame();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};