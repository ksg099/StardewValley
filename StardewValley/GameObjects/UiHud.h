#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

class UiHud : public GameObject
{
protected:
	sf::Vector2f referenceResolution = { 1920, 1080 };
	sf::Vector2f resolution = referenceResolution;

	std::string formatTime = " : ¿ÀÀü";
	std::string formatHighMoney = " : ÇöÀç µ·";

	SpriteGo* uiContainer;
	SpriteGo* uiEnergy;

	TextGo* timeDisplay;
	TextGo* dateDisplay;
	TextGo* moneyDisplay;

	float dailyTime = 0.f;
	float totalTime = 0.f;

	int hour = 0;
	int minute = 0;

	sf::RectangleShape gaugeHp;
	sf::Vector2f gaugeHpSize = { 14.f, 158.f };

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

	void SetTime(std::string& time);
	void SetMoney(int money);
	void SetHp(int hp, int max);
	void SetDate();
};