#pragma once
#include "GameObject.h"

class SceneGame;
class Player;

class TileMap : public GameObject
{
protected:
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;

	std::vector<TileData*>* tiles;
	sf::Vector2f tileSize = { 25.f, 25.f };

	sf::VertexArray va;
	std::string spriteSheetId;
	sf::Texture* texture;

	sf::Vector2i cellCount; // 셀의 가로줄, 세로줄 개수
	sf::Vector2f cellSize; // 단위 셀의 가로, 세로 길이
	const sf::Vector2f tileTextureSize = { 16.f, 16.f };

	sf::Transform transform;

public:
	TileMap(const std::string& name = "");

	virtual sf::FloatRect GetLocalBounds() override;
	virtual sf::FloatRect GetGlobalBounds() override;

	const sf::Vector2f& GetGridPosition(int x, int y) const;
	const TileData* GetTileData(int x, int y)const;
	TileData* GetTileData(int x, int y);

	const sf::Vector2i& GetCellCount() const { return cellCount; }
	const sf::Vector2f& GetCellSize() const { return cellSize; }

	void Set(const sf::Vector2i& count, const sf::Vector2f& size);
	void SetSpriteSheetId(const std::string& id);
	void UpdateTransform();

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void Translate(const sf::Vector2f& delta) override;
	void SetRotation(const float r) override;

	void SetScale(const sf::Vector2f& scale) override;

	void SetFlipX(bool flip) override;
	void SetFlipY(bool flip) override;

	void Init() override;
	void Release() override;

	void Reset() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;



	void LoadTileMap(const std::string& name);
	FloorOnTile* CreateFloor(const FloorType type, const int id);
	ObjectOnTile* CreateObject(const ObjectType type, const int id);

	void InteractWithTile(const int x, const int y, const ItemType type, const int id);

	const bool IsOutOfRange(const int x, const int y) const { return (x < 0 || y < 0 || x >= cellCount.x || y >= cellCount.y); }

	void SetPlayerPassable(int x, int y, bool isPassable);
	bool IsPassable(int x, int y);
};

