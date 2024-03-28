#include "pch.h"
#include "UiHud.h"
#include <sstream>

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

	sf::Font& font = RES_MGR_FONT.Get("fonts/Arial.ttf");
	float topY = 100.f;
	//float BottomY = referenceResolution.y - 100.f;
	float textSize = 50.f;

	uiContainer.SetTexture("graphics/UiContainer.png ");
	uiContainer.SetPosition({ 1500.f, 100.f });
	uiContainer.SetScale({ 1.2f, 1.2f });
	uiContainer.SetOrigin(Origins::TR);

	dateDisplay.SetFont(font);
	dateDisplay.Set(font, "", 20, sf::Color::Black);
	dateDisplay.SetPosition({ uiContainer.GetPosition().x - 94.f, uiContainer.GetPosition().y + 14.f});

	timeDisplay.SetFont(font);
	timeDisplay.Set(font, "", 20, sf::Color::Black);
	timeDisplay.SetPosition({ dateDisplay.GetPosition().x - 14.f, dateDisplay.GetPosition().y + 58.f });

	moneyDisplay.SetFont(font);
	moneyDisplay.Set(font, "", 20.f, sf::Color::Black);
	moneyDisplay.SetPosition({ uiContainer.GetPosition().x - 27.f, uiContainer.GetPosition().y + 120.f });
	moneyDisplay.SetOrigin(Origins::TR);

	uiArrow.SetTexture("graphics/Uiarrow.png ");
	uiArrow.SetPosition({ uiContainer.GetPosition().x - 120.f, uiContainer.GetPosition().y + 50.f });
	uiArrow.SetOrigin(Origins::TR);

	gaugeHp.setFillColor(sf::Color::Green);
	gaugeHp.setSize(gaugeHpSize);
	gaugeHp.setPosition({ 1500.f, 900 });
	Utils::SetOrigin(gaugeHp, Origins::BC);

	uiEnergy.SetTexture("graphics/UiEnergy.png");
	uiEnergy.SetPosition({ gaugeHp.getPosition().x + 8.f, gaugeHp.getPosition().y - 225.f });
	uiEnergy.SetOrigin(Origins::TR);
	Utils::SetOrigin(gaugeHp, Origins::BR);
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
	timeDisplay.Reset();
	dateDisplay.Reset();
	moneyDisplay.Reset();
	uiContainer.Reset();
	uiEnergy.Reset();
	uiArrow.Reset();
}

void UiHud::Update(float dt)
{

	totalTime += dt;

	gaugeHp.setScale({ 1.f, (float)currentHp / maxHp});
	if (currentHp / maxHp < 0)
	{
		gaugeHp.setScale({ 1.f, 0 });
	}
}

void UiHud::LateUpdate(float dt)
{
}

void UiHud::FixedUpdate(float dt)
{
}

void UiHud::SetTime(float dailyTime)
{
	int hours = static_cast<int>(dailyTime) % 24;
	int minutes = static_cast<int>((dailyTime - static_cast<float>(hours)) * 60.0f);

	//12���� Ŀ���� pm, ������ am ���� ���� ����
	std::string am_pm = hours >= 12 ? "PM" : "AM";
	hours = hours % 12;
	if (hours == 0) hours = 12;

	//hours < 10, minutes < 10�� �Ȱɸ�
	std::string hourString = std::to_string(hours);
	if (hours < 10)
	{
		hourString = "0" + hourString;
	}
	std::string minuteString = std::to_string(minutes);
	if (minutes < 10)
	{
		minuteString = "0" + minuteString;
	}

	std::string timeString = hourString + " : " + minuteString + am_pm;

	timeDisplay.SetString(std::string(timeString));
}


void UiHud::SetMoney(int money)
{
	moneyDisplay.SetString(formatMoney + std::to_string(money));
}

void UiHud::SetDate(int day)
{
	dateDisplay.SetString(formatdate + std::to_string(day));
}

void UiHud::Draw(sf::RenderWindow& window)
{
	uiContainer.Draw(window);
	uiEnergy.Draw(window);
	window.draw(gaugeHp);
	timeDisplay.Draw(window);
	moneyDisplay.Draw(window);
	dateDisplay.Draw(window);
	uiArrow.Draw(window);
}