#pragma once
#include "ObjectOnTile.h"

class SceneGame;

class ChangeHarvest : public ObjectOnTile
{

protected:
	int maxGrowthStage = 5;
	const DataHarvest* data = nullptr;
	int growthStage = 0;

public:

	ChangeHarvest(const std::string& name = "");
	~ChangeHarvest() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetData(const DataHarvest* data);
};

