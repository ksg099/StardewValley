#pragma once
#include "ObjectOnTile.h"

class StoreObjOnTile : public ObjectOnTile
{
protected:

public:
	StoreObjOnTile(const std::string& name = "");
	~StoreObjOnTile() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void InteractWithObject(const ItemType type, const int id) override;
};