#include "pch.h"
#include "ObjectOnTile.h"
#include "SceneGame.h"
#include "FloorOnTile.h"
#include "TileMap.h"

ObjectOnTile::ObjectOnTile(const std::string& name) : SpriteGo(name)
{
}

void ObjectOnTile::Init()
{
	SpriteGo::Init();
	value = 0;
}

void ObjectOnTile::Release()
{
	SpriteGo::Release();
}

void ObjectOnTile::Reset()
{
	SpriteGo::Reset();
	value = 0;
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMap = dynamic_cast<TileMap*>(sceneGame->FindGo("Background"));
}

void ObjectOnTile::Update(float dt)
{
	SpriteGo::Update(dt);
}

void ObjectOnTile::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void ObjectOnTile::InteractWithObject(const ItemType type, const int id)
{
	switch (objectType)
	{
	case ObjectType::STONE:
		// 곡괭이 상호작용
		if (type == ItemType::Tool && id == 0)
		{
			DataItem itemData = ITEM_TABLE->Get("stoneItem");
			sceneGame->CreateItem(itemData, tileData->indexX, tileData->indexY);
			SetActive(false);
			sceneGame->RemoveGo(this);
			tileData->object = nullptr;
			tileData->objectType = ObjectType::NONE;
			tileData->objectId = 0;

			tileData->isPassable = true;
			tileData->isPossiblePlace = true;
		}
		break;
	case ObjectType::TREE:
		//도끼 상호작용
		if (type == ItemType::Tool && id == 1)
		{
			++value;
			if (value == 5)
			{
				DataItem itemData = ITEM_TABLE->Get("branchItem");
				sceneGame->CreateItem(itemData, tileData->indexX, tileData->indexY);
				value = 0;
				SetActive(false);
				sceneGame->RemoveGo(this);
				tileData->object = nullptr;
				tileData->objectType = ObjectType::NONE;
				tileData->objectId = 0;

				tileData->isPassable = true;
				tileData->isPossiblePlace = true;
			}
		}
		break;
	case ObjectType::BRANCH:
		//도끼 상호작용
		if (type == ItemType::Tool && id == 1)
		{
			++value;
			if (value == 1)
			{
				DataItem itemData = ITEM_TABLE->Get("branchItem");
				sceneGame->CreateItem(itemData, tileData->indexX, tileData->indexY);
				value = 0;
				SetActive(false);
				sceneGame->RemoveGo(this);
				tileData->object = nullptr;
				tileData->objectType = ObjectType::NONE;
				tileData->objectId = 0;

				tileData->isPassable = true;
				tileData->isPossiblePlace = true;
			}
		}
		break;
	case ObjectType::STUMP:
		//도끼 상호작용
		if (type == ItemType::Tool && id == 1)
		{
			++value;
			if (value == 2)
			{
				DataItem itemData = ITEM_TABLE->Get("branchItem");
				sceneGame->CreateItem(itemData, tileData->indexX, tileData->indexY);
				value = 0;
				SetActive(false);
				sceneGame->RemoveGo(this);
				tileData->object = nullptr;
				tileData->objectType = ObjectType::NONE;
				tileData->objectId = 0;

				tileData->isPassable = true;
				tileData->isPossiblePlace = true;
			}
		}
		break;
	case ObjectType::WEED:
		//도끼, 낫 상호작용
		if ((type == ItemType::Tool && id == 1) ||
			(type == ItemType::Tool && id == 2))
		{
			DataItem itemData = ITEM_TABLE->Get("weedItem");
			sceneGame->CreateItem(itemData, tileData->indexX, tileData->indexY);
			SetActive(false);
			sceneGame->RemoveGo(this);
			tileData->object = nullptr;
			tileData->objectType = ObjectType::NONE;
			tileData->objectId = 0;

			tileData->isPassable = true;
			tileData->isPossiblePlace = true;
		}
		break;
	case ObjectType::CROPS:
		// 물뿌리개 상호작용
		if (type == ItemType::Tool && id == 4)
		{
			tileData->floor->InteractWithFloor(type, id);
		}
		//TO-DO : 다 자란 작물만 수확 상호작용할 수 있게 짜기
		break;
	case ObjectType::FURNITURE:
		//곡괭이 상호작용
		if (type == ItemType::Tool && id == 0)
		{
			//TO-DO : 해당 가구 아이템 드롭
			SetActive(false);
			sceneGame->RemoveGo(this);
			tileData->object = nullptr;
			tileData->objectType = ObjectType::NONE;
			tileData->objectId = 0;

			tileData->isPassable = true;
			tileData->isPossiblePlace = true;
		}
		break;
	case ObjectType::BOX:
		//곡괭이 상호작용
		if (type == ItemType::Tool && id == 0)
		{
			//TO-DO : 상자 아이템 드롭
			SetActive(false);
			sceneGame->RemoveGo(this);
			tileData->object = nullptr;
			tileData->objectType = ObjectType::NONE;
			tileData->objectId = 0;

			tileData->isPassable = true;
			tileData->isPossiblePlace = true;
		}
		break;
	case ObjectType::WALL:
		break;
	case ObjectType::BUILDING:
		//건물 들어가기
		break;
	default:
		break;
	}
}
