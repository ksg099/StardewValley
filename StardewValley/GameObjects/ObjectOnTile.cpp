#include "pch.h"
#include "ObjectOnTile.h"
#include "SceneGame.h"
#include "FloorOnTile.h"

ObjectOnTile::ObjectOnTile(const std::string& name) : SpriteGo(name)
{
}

void ObjectOnTile::Init()
{
	SpriteGo::Init();
}

void ObjectOnTile::Release()
{
	SpriteGo::Release();
}

void ObjectOnTile::Reset()
{
	SpriteGo::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
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
			//여러번 도끼질해야 나무를 베는건데 이걸 어떻게 처리하지?
			//도끼질 하다가 다른 나무로 가서 도끼질하면 처음부터 도끼질해야하는건데 그냥 count ++ 하면 안된다.
		}
		break;
	case ObjectType::WEED:
		//낫 상호작용
		if (type == ItemType::Tool && id == 2)
		{
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
		break;
	case ObjectType::FURNITURE:
		//곡괭이 상호작용
		break;
	case ObjectType::BOX:
		break;
	case ObjectType::WALL:
		break;
	case ObjectType::BUILDING:
		break;
	default:
		break;
	}
}
