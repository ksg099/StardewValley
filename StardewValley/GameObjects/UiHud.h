#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

class UiHud : public GameObject
{
protected:
	//sf::Vector2f referenceResolution = { 1920, 1080 };
	//sf::Vector2f resolution = referenceResolution;

	std::string formatdate = "Day : ";
	std::string formatMoney = " ";

	SpriteGo uiContainer;
	SpriteGo uiEnergy;
	SpriteGo uiArrow;

	TextGo timeDisplay;
	TextGo dateDisplay;
	TextGo moneyDisplay;

	float dailyTime = 0.f;
	float totalTime = 0.f;

	int hour = 0;
	int minute = 0;
	int currentHeight = 0;
	sf::RectangleShape gaugeHp;
	sf::Vector2f gaugeHpSize = { 14.f, 220.f };

	// 임시 - 나중에 게임씬에서 받아오는 것
	int currentHp = 50;
	int maxHp = 100;

public:
	UiHud(const std::string& name = "");
	~UiHud() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetTime(float dailyTime);
	void SetMoney(int money);
	void SetHp(int hp, int max);
	void SetDate(int day);
};