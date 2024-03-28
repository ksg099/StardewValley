#include "pch.h"
#include "CropsObjOnTile.h"
#include "FloorOnTile.h"
#include "SceneGame.h"

CropsObjOnTile::CropsObjOnTile(const std::string& name) : ObjectOnTile(name)
{
}

void CropsObjOnTile::Init()
{
	ObjectOnTile::Init();
}

void CropsObjOnTile::Release()
{
	ObjectOnTile::Release();
}

void CropsObjOnTile::Reset()
{
	ObjectOnTile::Reset();

	auto harvestInfo = HARVEST_TABLE->Get(objectId);
	nextGrowDay = harvestInfo.growingDay;
	currentGrowDay = 0;
	if (nextGrowDay == 0)
	{
		isCompleteGrowth = true;
	}
}

void CropsObjOnTile::Update(float dt)
{
	ObjectOnTile::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::G))
	{
		Grow();
	}
}

void CropsObjOnTile::Draw(sf::RenderWindow& window)
{
	ObjectOnTile::Draw(window);
}

void CropsObjOnTile::InteractWithObject(const ItemType type, const int id)
{
	if (type == ItemType::Tool && id == 4)
	{
		tileData->floor->InteractWithFloor(type, id);
	}
	else if (isCompleteGrowth)
	{
		sceneGame->CreateItem(ITEM_TABLE->Get(ItemType::Harvest, HARVEST_TABLE->GetItemID(objectId)), tileData->indexX, tileData->indexY);
		SetActive(false);
		sceneGame->RemoveGo(this);
		tileData->object = nullptr;
		tileData->objectType = ObjectType::NONE;
		tileData->objectId = 0;
	}
}

void CropsObjOnTile::Grow()
{
	if (!isCompleteGrowth)
	{
		if (currentGrowDay == nextGrowDay)
		{
			auto harvestInfo = HARVEST_TABLE->Get(objectId);
			nextGrowDay = harvestInfo.growingDay;
			currentGrowDay = 0;
			if (nextGrowDay == 0)
			{
				isCompleteGrowth = true;
				return;
			}

			auto objectInfo = OBJECT_TABLE->Get(objectType, ++objectId);
			++tileData->objectId;
			SetTexture(objectInfo.textureId);
			SetTextureRect(sf::IntRect(objectInfo.sheetId.x, objectInfo.sheetId.y, objectInfo.sheetSize.x, objectInfo.sheetSize.y));
			SetOrigin(Origins::MC);
		}
		else
		{
			++currentGrowDay;
		}
	}
}
