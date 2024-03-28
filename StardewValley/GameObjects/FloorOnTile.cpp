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

	if (sceneGame->IsNextDay() && floorType == FloorType::WET_ARABLE_LAND && tileData->object == nullptr)
	{
		tileData->floorType = floorType = FloorType::DRIED_ARABLE_LAND;
		auto floorData = FLOOR_TABLE->Get(floorType, floorId);
		SetTexture(floorData.textureId);
		SetTextureRect(sf::IntRect(floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y));
	}
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
		// 물뿌리개 상호작용
		if (type == ItemType::Tool && id == 4)
		{
			tileData->floorType = floorType = FloorType::WET_ARABLE_LAND;
			auto floorData = FLOOR_TABLE->Get(floorType, floorId);
			SetTexture(floorData.textureId);
			SetTextureRect(sf::IntRect(floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y));
		}
		// 씨앗 상호작용
		if (type == ItemType::Seed && tileData->object == nullptr)
		{
			tileData->objectType = ObjectType::CROPS;
			tileData->objectId = HARVEST_TABLE->GetObjectID(id);
			tileData->object = tileMap->CreateObject(tileData->objectType, tileData->objectId);
			tileData->object->SetPosition(tileMap->GetGridPosition(tileData->indexX, tileData->indexY));
			//tileData->object = obj;
			tileData->object->SetTileData(tileData);
			sceneGame->AddGo(tileData->object);

			auto invenBox = ITEM_SAVE->Get(0);
			for (auto item : *invenBox)
			{
				if (item != nullptr && item->type == type && item->itemId == id)
				{
					--item->count;
					if (item->count <= 0)
					{
						auto it = std::find(invenBox->begin(), invenBox->end(), item);

						if (it != invenBox->end())
						{
							// 선택된 아이템을 items 리스트에서 삭제
							delete* it;
							invenBox->erase(it);
							sceneGame->UpdateInvenSlot();
						}
					}
					break;
				}
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
