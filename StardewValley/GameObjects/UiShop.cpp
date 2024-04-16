#include "pch.h"
#include "UiShop.h"

UiShop::UiShop(const std::string& name) : GameObject(name)
{
}

void UiShop::Init()
{
	sellerPortrait.SetTexture("graphics/pierrePortrait.png");
	sellerPortrait.SetScale(sf::Vector2f(3.5f, 3.5f));
	sellerPortrait.SetOrigin(Origins::BR);
	sellerPortrait.SetPosition(sf::Vector2f(500.f, 460.f));

	sellerTextBox.SetTexture("graphics/box1.png");
	sellerTextBox.SetOrigin(Origins::TR);
	sellerTextBox.SetPosition(sf::Vector2f(500.f, 480.f));
	sellerText.Set(RES_MGR_FONT.Get("fonts/Galmuri7.ttf"),
		"Welcome to Pierre!\nDo you have something\nyou want to sell?",
		35, sf::Color::Black);
	sellerText.SetOrigin(Origins::TC);
	sellerText.SetPosition(sf::Vector2f(340.f, 500.f));

	GameObject::Init();
}

void UiShop::Release()
{
	GameObject::Release();
}

void UiShop::Reset()
{
	GameObject::Reset();
}

void UiShop::Update(float dt)
{
	GameObject::Update(dt);
}

void UiShop::Draw(sf::RenderWindow& window)
{
	sellerPortrait.Draw(window);
	sellerTextBox.Draw(window);
	sellerText.Draw(window);

	GameObject::Draw(window);
}
