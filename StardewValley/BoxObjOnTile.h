#pragma once
#include "ObjectOnTile.h"
class BoxObjOnTile : public ObjectOnTile
{
protected:

public:
	BoxObjOnTile(const std::string& name = "");
	~BoxObjOnTile() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void InteractWithObject(const ItemType type, const int id) override;
};