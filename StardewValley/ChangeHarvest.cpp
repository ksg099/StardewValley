#include "pch.h"
#include "ChangeHarvest.h"

ChangeHarvest::ChangeHarvest(const std::string& name) : ObjectOnTile(name)
{
}

void ChangeHarvest::Init()
{
	ObjectOnTile::Init();

	HarvestType::CAULIFLOWER;

}

void ChangeHarvest::Release()
{
	ObjectOnTile::Release();
}

void ChangeHarvest::Reset()
{

	auto& objData = OBJECT_TABLE->Get(objectType, objectId);
	SetTexture(objData.textureId);
	SetTextureRect(sf::IntRect(objData.sheetId.x, objData.sheetId.y, objData.sheetSize.x, objData.sheetSize.y));
	SetOrigin(Origins::MC);
	ObjectOnTile::Reset();
}

void ChangeHarvest::Update(float dt)
{
	ObjectOnTile::Update(dt);
	//
}

void ChangeHarvest::Draw(sf::RenderWindow& window)
{
	ObjectOnTile::Draw(window);
}

void ChangeHarvest::SetData(const DataHarvest* data)
{
	this->data = data;
}
