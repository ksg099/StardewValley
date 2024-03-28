#include "pch.h"
#include "BoxObjOnTile.h"
#include "SceneGame.h"

BoxObjOnTile::BoxObjOnTile(const std::string& name) : ObjectOnTile(name)
{
}

void BoxObjOnTile::Init()
{
	ObjectOnTile::Init();
}

void BoxObjOnTile::Release()
{
	ObjectOnTile::Release();
}

void BoxObjOnTile::Reset()
{
	ObjectOnTile::Reset();
}

void BoxObjOnTile::Update(float dt)
{
	ObjectOnTile::Update(dt);
}

void BoxObjOnTile::Draw(sf::RenderWindow& window)
{
	ObjectOnTile::Draw(window);
}

void BoxObjOnTile::InteractWithObject(const ItemType type, const int id)
{
	//ObjectOnTile::InteractWithObject(type, id);

	sceneGame->OpenBox(objectId + 1);
}
