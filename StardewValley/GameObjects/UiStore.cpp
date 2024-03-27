#include "pch.h"
#include "UiStore.h"

UiStore::UiStore(const std::string& name) : GameObject(name)
{
}

void UiStore::Init()
{
	StoreBackground.setTexture(RES_MGR_TEXTURE.Get("graphics/Ui.png"));
	StoreBackground.setScale(sf::Vector2f(1.f, 1.5f));
	StoreBackground.setPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.f);
	Utils::SetOrigin(StoreBackground, Origins::MC);

	int itemCount = ITEM_TABLE->CountAll();
	for (int i = 0; i < 6; ++i)
	{
		UiItem* uiItem = new UiItem;

		uiItem->itemBackground.setTexture(RES_MGR_TEXTURE.Get("graphics/DialogBoxGreen.png"));
		uiItem->itemBackground.setPosition(StoreBackground.getPosition() - sf::Vector2f(250.f, 430.f) + sf::Vector2f(0.f, 170.f * i));
		Utils::SetOrigin(uiItem->itemBackground, Origins::MC);
		
		UipurchaseItems.push_back(uiItem);
	}
}

void UiStore::Release()
{
}

void UiStore::Reset()
{
}

void UiStore::Update(float dt)
{
}

void UiStore::Draw(sf::RenderWindow& window)
{
	window.draw(StoreBackground);

	for (auto uiItem : UipurchaseItems)
	{
		window.draw(uiItem->itemBackground);
	}
}
