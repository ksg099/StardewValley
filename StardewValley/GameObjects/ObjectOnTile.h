#pragma once
#include "SpriteGo.h"

class SceneGame;

class ObjectOnTile : public SpriteGo
{
protected:
	SceneGame* sceneGame = nullptr;
	TileData* tileData = nullptr;

	ObjectType objectType = ObjectType::NONE;
	int objectId = 0;

public:
	ObjectOnTile(const std::string& name = "");
	~ObjectOnTile() override = default;

	void SetTileData(TileData*& data) { tileData = data; }
	void SetObjectType(const ObjectType type) { objectType = type; }
	void SetObjectId(const int id) { objectId = id; }
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	virtual void InteractWithObject(const ItemType type, const int id);
};

