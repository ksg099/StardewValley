#include "pch.h"
#include "UiHud.h"

UiHud::UiHud(const std::string& name) : GameObject(name)
{
}

void UiHud::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	gaugeHp.setSize({ gaugeHpSize.x * value, gaugeHpSize.y });
}
void UiHud::Init()
{

	sf::Font& font = RES_MGR_FONT.Get("fonts/zombiecontrol.ttf");
	float topY = 100.f;
	float BottomY = referenceResolution.y - 100.f;
	float textSize = 50.f;

	dateDisplay = new TextGo("dateDisplay");
	dateDisplay->SetFont(font);
	dateDisplay->SetString("Date: --/--");
	dateDisplay->Set(font, "", textSize, sf::Color::White);
	dateDisplay->SetPosition({ referenceResolution.x - 400.f, 50.f });

	timeDisplay = new TextGo("timeDisplay");
	timeDisplay->SetFont(font);
	timeDisplay->SetString("00:00");
	timeDisplay->Set(font, "", textSize, sf::Color::White);
	timeDisplay->SetPosition({ referenceResolution.x - 200.f, 50.f });

	// 소지금을 위한 텍스트 초기화
	moneyDisplay = new TextGo("moneyDisplay");
	moneyDisplay->SetFont(font);
	moneyDisplay->Set(font, "", 60.f, sf::Color::White);
	moneyDisplay->SetPosition({ referenceResolution.x - 200.f, 100.f });


	uiContainer = new SpriteGo("uiContainer");
	uiContainer->SetTexture("graphics/UiContainer.png ");
	uiContainer->SetPosition({ 1500.f, 100.f });
	uiContainer->SetScale({ 1.2f, 1.2f });
	uiContainer->SetOrigin(Origins::TR);

	gaugeHp.setFillColor(sf::Color::Green);
	gaugeHp.setSize(gaugeHpSize);
	gaugeHp.setPosition({ 1500.f, 900 });

	uiEnergy = new SpriteGo("uiEnergy");
	uiEnergy->SetTexture("graphics/UiEnergy.png");
	uiEnergy->SetPosition({ gaugeHp.getPosition().x + 8.f, gaugeHp.getPosition().y - 225.f });
	uiEnergy->SetOrigin(Origins::TR);
	Utils::SetOrigin(gaugeHp, Origins::BR);

	SetDate();
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
	timeDisplay->Reset();
	dateDisplay->Reset();

	moneyDisplay->Reset();
	uiContainer->Reset();
	uiEnergy->Reset();
}

void UiHud::Update(float dt)
{

	totalTime += dt;

	gaugeHp.setSize({ gaugeHp.getSize().x, gaugeHp.getSize().y - 5.f * dt });
	if (gaugeHp.getSize().y < 0)
	{
		gaugeHp.setSize({ gaugeHp.getSize().x, 0 });
	}
	SetDate();
}

void UiHud::LateUpdate(float dt)
{
}

void UiHud::FixedUpdate(float dt)
{
}

void UiHud::SetTime(std::string& time)
{
	timeDisplay->SetString(hour + ":" + minute);
}


void UiHud::SetMoney(int money)
{
	moneyDisplay->SetString("Money: " + std::to_string(money));
}

void UiHud::SetDate()
{
}

void UiHud::Draw(sf::RenderWindow& window)
{
	uiContainer->Draw(window);
	uiEnergy->Draw(window);
	window.draw(gaugeHp);
	timeDisplay->Draw(window);
	moneyDisplay->Draw(window);
	dateDisplay->Draw(window);
}