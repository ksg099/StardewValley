#pragma once
#include "ObjectOnTile.h"
class CropsObjOnTile : public ObjectOnTile
{
protected:
	int currentGrowDay = 0;
	int nextGrowDay = 0;
	bool isCompleteGrowth = false;

public:
	CropsObjOnTile(const std::string& name = "");
	~CropsObjOnTile() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void InteractWithObject(const ItemType type, const int id) override;
	void Grow();
};

