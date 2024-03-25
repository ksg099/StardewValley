#pragma once
#include "SpriteGo.h"

class SceneGame;
class TileMap;

class FloorOnTile : public SpriteGo
{
private:
	SceneGame* sceneGame = nullptr;
	TileMap* tileMap = nullptr;
	TileData* tileData = nullptr;

	FloorType floorType = FloorType::NONE;
	int floorId = 0;

public:
	FloorOnTile(const std::string& name = "");
	~FloorOnTile() override = default;

	void SetTileData(TileData*& data) { tileData = data; }
	void SetFloorType(const FloorType type) { floorType = type; }
	void SetFloorId(const int id) { floorId = id; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	virtual void InteractWithFloor(const ItemType type, const int id);
};

