#include "pch.h"
#include "FloorOnTile.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "ObjectOnTile.h"
#include "CropsObjOnTile.h"

FloorOnTile::FloorOnTile(const std::string& name) : SpriteGo(name)
{
}

void FloorOnTile::Init()
{
	SpriteGo::Init();
}

void FloorOnTile::Release()
{
	SpriteGo::Release();
}

void FloorOnTile::Reset()
{
	SpriteGo::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));
}

void FloorOnTile::Update(float dt)
{
	SpriteGo::Update(dt);
}

void FloorOnTile::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void FloorOnTile::InteractWithFloor(const ItemType type, const int id)
{
	switch (floorType)
	{
	case FloorType::DRIED_ARABLE_LAND:
	case FloorType::WET_ARABLE_LAND:
		// ¹°»Ñ¸®°³ »óÈ£ÀÛ¿ë
		if (type == ItemType::Tool && id == 4)
		{
			tileData->floorType = floorType = FloorType::WET_ARABLE_LAND;
			auto floorData = FLOOR_TABLE->Get(floorType, floorId);
			SetTexture(floorData.textureId);
			SetTextureRect(sf::IntRect(floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y));
		}
		// ¾¾¾Ñ »óÈ£ÀÛ¿ë
		if (type == ItemType::Seed && tileData->object == nullptr)
		{
			tileData->objectType = ObjectType::CROPS;
			tileData->objectId = HARVEST_TABLE->GetObjectID(id);
			tileData->object = tileMap->CreateObject(tileData->objectType, tileData->objectId);
			tileData->object->SetPosition(tileMap->GetGridPosition(tileData->indexX, tileData->indexY));
			//tileData->object = obj;
			tileData->object->SetTileData(tileData);
			sceneGame->AddGo(tileData->object);
		}
		break;
	case FloorType::WOOD_PATH:
		break;
	case FloorType::STONE_PATH:
		break;
	default:
		break;
	}
}
