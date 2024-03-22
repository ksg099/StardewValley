#pragma once
#include "SpriteGo.h"
class Item : public SpriteGo
{
protected:

public:

	Item(const std::string& name = "");
	~Item() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

