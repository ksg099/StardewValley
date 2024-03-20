#pragma once
#include "SpriteGo.h"
class ObjectOnTile : public SpriteGo
{
private:

public:
	ObjectOnTile(const std::string& name = "");
	~ObjectOnTile() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

