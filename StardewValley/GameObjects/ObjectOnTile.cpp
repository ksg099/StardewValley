#include "pch.h"
#include "ObjectOnTile.h"

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
}

void ObjectOnTile::Update(float dt)
{
	SpriteGo::Update(dt);
}

void ObjectOnTile::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
