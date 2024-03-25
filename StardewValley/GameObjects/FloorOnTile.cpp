#include "pch.h"
#include "FloorOnTile.h"
#include "SceneGame.h"

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
		if (type == ItemType::Tool && id == 4)
		{
			floorType = FloorType::WET_ARABLE_LAND;
			auto floorData = FLOOR_TABLE->Get(floorType, floorId);
			SetTexture(floorData.textureId);
			SetTextureRect(sf::IntRect(floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y));
		}
		break;
	case FloorType::WET_ARABLE_LAND:
		break;
	case FloorType::WOOD_PATH:
		break;
	case FloorType::STONE_PATH:
		break;
	default:
		break;
	}
}
