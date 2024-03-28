#pragma once
#include "SpriteGo.h"

class SceneGame;
class TileMap;

class ObjectOnTile : public SpriteGo
{
protected:
	SceneGame* sceneGame = nullptr;
	TileData* tileData = nullptr;
	TileMap* tileMap = nullptr;

	ObjectType objectType = ObjectType::NONE;
	int objectId = 0;

	int value = 0;

public:
	ObjectOnTile(const std::string& name = "");
	~ObjectOnTile() override = default;

	void SetTileData(TileData*& data) { tileData = data; }
	void SetObjectType(const ObjectType type) { objectType = type; }
	void SetObjectId(const int id) { objectId = id; }
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void InteractWithObject(const ItemType type, const int id);
};

