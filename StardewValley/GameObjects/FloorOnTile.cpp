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
		if (type == ItemType::Tool && id == 4)
		{
			floorType = FloorType::WET_ARABLE_LAND;
			auto floorData = FLOOR_TABLE->Get(floorType, floorId);
			SetTexture(floorData.textureId);
			SetTextureRect(sf::IntRect(floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y));
		}
		if (type == ItemType::Seed)
		{
			if (tileData->object == nullptr)
			{
				ObjectOnTile* obj = new ObjectOnTile("Object");
				obj->SetObjectType(ObjectType::CROPS);
				int objId = id; // 아이디 계산해야 함
				obj->SetObjectId(objId);
				auto& objData = OBJECT_TABLE->Get(ObjectType::CROPS, objId);
				obj->SetTexture(objData.textureId);
				obj->SetTextureRect(sf::IntRect(objData.sheetId.x, objData.sheetId.y, objData.sheetSize.x, objData.sheetSize.y));
				obj->SetOrigin(Origins::MC);
				obj->SetPosition(tileMap->GetGridPosition(tileData->indexX, tileData->indexY));
				tileData->object = obj;
				tileData->object->SetTileData(tileData);
			}
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
