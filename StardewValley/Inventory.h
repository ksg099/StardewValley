#pragma once
#include "SpriteGo.h"

class InvetorySlot;

class Inventory : public SpriteGo
{
protected:
	int countX = 2;
	int countY = 2;
	std::vector<InvetorySlot*> slots; //slot 컨테이너

public:

	bool isAble = true;
	Inventory(const std::string& name = "");
	~Inventory() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void LoadData(rapidjson::Document& doc);

};

