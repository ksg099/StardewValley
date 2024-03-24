#pragma once
#include "SpriteGo.h"
class FloorOnTile : public SpriteGo
{
private:
	TileData* tileData = nullptr;

	FloorType objectType = FloorType::NONE;
	int floorId = 0;

public:
	FloorOnTile(const std::string& name = "");
	~FloorOnTile() override = default;

	void SetTileData(TileData*& data) { tileData = data; }
	void SetFloorType(const FloorType type) { objectType = type; }
	void SetFloorId(const int id) { floorId = id; }

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	// virtual std::pair<bool, bool> InteractWithFloor(const ItemType type, const int id);
};

