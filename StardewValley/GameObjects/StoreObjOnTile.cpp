#include "pch.h"
#include "StoreObjOnTile.h"
#include "SceneGame.h"

StoreObjOnTile::StoreObjOnTile(const std::string& name) : ObjectOnTile(name)
{
}

void StoreObjOnTile::Init()
{
	ObjectOnTile::Init();
}

void StoreObjOnTile::Release()
{
	ObjectOnTile::Release();
}

void StoreObjOnTile::Reset()
{
	ObjectOnTile::Reset();
}

void StoreObjOnTile::Update(float dt)
{
	ObjectOnTile::Update(dt);
}

void StoreObjOnTile::Draw(sf::RenderWindow& window)
{
	ObjectOnTile::Draw(window);
}

void StoreObjOnTile::InteractWithObject(const ItemType type, const int id)
{
	sceneGame->OpenStoreUi();
}
