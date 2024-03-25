#include "pch.h"
#include "FloorOnTile.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "ObjectOnTile.h"

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
	{;
	case FloorType::DRIED_ARABLE_LAND:
	case FloorType::WET_ARABLE_LAND:
		// 물뿌리개 상호작용
		if (type == ItemType::Tool && id == 4)
		{
			floorType = FloorType::WET_ARABLE_LAND;
			auto floorData = FLOOR_TABLE->Get(floorType, floorId);
			SetTexture(floorData.textureId);
			SetTextureRect(sf::IntRect(floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y));
		}
		// 씨앗 상호작용
		if (type == ItemType::Seed && tileData->object == nullptr)
		{
			int objId = id; // 아이디 계산해야 함
			ObjectOnTile* obj = tileMap->CreateObject(ObjectType::CROPS, objId);
			obj->SetPosition(tileMap->GetGridPosition(tileData->indexX, tileData->indexY));
			tileData->object = obj;
			tileData->object->SetTileData(tileData);
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
