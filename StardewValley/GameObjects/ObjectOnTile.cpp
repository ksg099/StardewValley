#include "pch.h"
#include "ObjectOnTile.h"
#include "SceneGame.h"

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

std::pair<bool, bool> ObjectOnTile::InteractWithObject(const ItemType type, const int id)
{
	switch (objectType)
	{
	case ObjectType::STONE:
		if (type == ItemType::Tool && id == 0)
		{
			SetActive(false);
			sceneGame->RemoveGo(this);
			tileData->object = nullptr;
			return std::make_pair(true, true);
		}
		break;
	case ObjectType::TREE:
		break;
	case ObjectType::WEED:
		break;
	case ObjectType::CROPS:
		break;
	case ObjectType::FURNITURE:
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

	return std::make_pair(false, false);
}
