#include "pch.h"
#include "UiStore.h"
#include "SceneGame.h"
#include "Player.h"

UiStore::UiStore(const std::string& name) : GameObject(name)
{
}

void UiStore::Init()
{
	StoreBackground.setTexture(RES_MGR_TEXTURE.Get("graphics/Ui.png"));
	StoreBackground.setScale(sf::Vector2f(1.f, 1.5f));
	StoreBackground.setPosition(FRAMEWORK.GetWindowSize().x / 2.f, FRAMEWORK.GetWindowSize().y / 2.f);
	Utils::SetOrigin(StoreBackground, Origins::MC);

	countAllItem = ITEM_TABLE->CountAll();
	for (int i = 0; i < (int)ItemType::Count; ++i)
	{
		for (int j = 0; j < ITEM_TABLE->Count((ItemType)i); ++j)
		{
			DataItem itemData = ITEM_TABLE->Get((ItemType)i, j);
			itemTable.push_back(itemData);
		}
	}

	for (int i = 0; i < countInPage; ++i)
	{
		UiItem* uiItem = new UiItem;

		uiItem->itemBackground.setTexture(RES_MGR_TEXTURE.Get("graphics/DialogBoxGreen.png"));
		uiItem->itemBackground.setPosition(StoreBackground.getPosition() - sf::Vector2f(250.f, 430.f) + sf::Vector2f(0.f, 170.f * i));
		Utils::SetOrigin(uiItem->itemBackground, Origins::MC);

		uiItem->clickBox.setFillColor(sf::Color::Green);
		uiItem->clickBox.setSize({ 650.f, 150.f });
		uiItem->clickBox.setPosition(StoreBackground.getPosition() - sf::Vector2f(0.f, 430.f) + sf::Vector2f(0.f, 170.f * i));
		Utils::SetOrigin(uiItem->clickBox, Origins::MC);

		uiItem->itemImg.setTexture(RES_MGR_TEXTURE.Get(itemTable[i].textureId));
		uiItem->itemImg.setTextureRect(sf::IntRect(itemTable[i].sheetId.x, itemTable[i].sheetId.y, itemTable[i].sheetSize.x, itemTable[i].sheetSize.y));
		int scale = itemTable[i].sheetSize.x < itemTable[i].sheetSize.y ? itemTable[i].sheetSize.x : itemTable[i].sheetSize.y;
		uiItem->itemImg.setScale(sf::Vector2f(65.f / scale, 65.f / scale));
;		uiItem->itemImg.setPosition(uiItem->itemBackground.getPosition());
		Utils::SetOrigin(uiItem->itemImg, Origins::MC);

		std::string info = "Name: " + itemTable[i].name + "\nPrice: " + std::to_string(itemTable[i].purchasePrice) + " Gold";
		uiItem->itemInfo.Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), info, 30, sf::Color::Black);
		uiItem->itemInfo.SetOutline(sf::Color::White, 1.f);
		uiItem->itemInfo.SetPosition(uiItem->itemBackground.getPosition() + sf::Vector2f(100.f, -35.f));
		uiItem->itemInfo.SetOrigin(Origins::TL);
		
		UipurchaseItems.push_back(uiItem);
	}
}

void UiStore::Release()
{
}

void UiStore::Reset()
{
	doubleClickTimer = 0.f;
	isClick = false;

	int clickIndex = -1;
	int purchaseIndex = -1;

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(sceneGame->FindGo("Player"));
}

void UiStore::Update(float dt)
{
	SelectByDoubleClick(dt);

	if (InputMgr::GetMouseWheelDown(sf::Mouse::Wheel::VerticalWheel)
		&& currentIndex < itemTable.size() - countInPage)
	{
		++currentIndex;
		UpdateIndex();
	}
	if (InputMgr::GetMouseWheelUp(sf::Mouse::Wheel::VerticalWheel)
		&& currentIndex > 0)
	{
		--currentIndex;
		UpdateIndex();
	}
}

void UiStore::Draw(sf::RenderWindow& window)
{
	window.draw(StoreBackground);

	for (auto uiItem : UipurchaseItems)
	{
		window.draw(uiItem->itemBackground);
		window.draw(uiItem->itemImg);
		uiItem->itemInfo.Draw(window);
	}
}

void UiStore::SelectByDoubleClick(float dt)
{
	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);

	if (isClick)
	{
		doubleClickTimer += dt;
		if (doubleClickTimer > doubleClickDuration)
		{
			doubleClickTimer = 0.f;
			isClick = false;
		}
	}

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (!isClick) // 클릭
		{
			for (int i = 0; i < countInPage; ++i)
			{
				if (UipurchaseItems[i]->clickBox.getGlobalBounds().contains(uiPos))
				{
					clickIndex = i;
					break;
				}
			}

			if (clickIndex != -1)
			{
				isClick = true;
			}
		}
		else // 더블 클릭
		{
			for (int i = 0; i < countInPage; ++i)
			{
				if (UipurchaseItems[i]->clickBox.getGlobalBounds().contains(uiPos))
				{
					if (clickIndex == i)
					{
						purchaseIndex = currentIndex + clickIndex;
					}
					break;
				}
			}
			clickIndex = -1;
			isClick = false;
		}
	}

	if (purchaseIndex != -1)
	{
		sceneGame->CreateItem(itemTable[purchaseIndex], player->GetGridIndex().x, player->GetGridIndex().y);
		purchaseIndex = -1;
	}
}

void UiStore::UpdateIndex()
{
	for (int i = 0; i < countInPage; ++i)
	{
		UipurchaseItems[i]->itemImg.setTexture(RES_MGR_TEXTURE.Get(itemTable[i + currentIndex].textureId));
		UipurchaseItems[i]->itemImg.setTextureRect(sf::IntRect(itemTable[i + currentIndex].sheetId.x, itemTable[i + currentIndex].sheetId.y,
			itemTable[i + currentIndex].sheetSize.x, itemTable[i + currentIndex].sheetSize.y));
		int scale = itemTable[i + currentIndex].sheetSize.x < itemTable[i + currentIndex].sheetSize.y ? itemTable[i + currentIndex].sheetSize.x : itemTable[i + currentIndex].sheetSize.y;
		UipurchaseItems[i]->itemImg.setScale(sf::Vector2f(65.f / scale, 65.f / scale));
				UipurchaseItems[i]->itemImg.setPosition(UipurchaseItems[i]->itemBackground.getPosition());
		Utils::SetOrigin(UipurchaseItems[i]->itemImg, Origins::MC);

		std::string info = "Name: " + itemTable[i + currentIndex].name + "\nPrice: " + std::to_string(itemTable[i + currentIndex].purchasePrice) + " Gold";
		UipurchaseItems[i]->itemInfo.SetString(info);
		UipurchaseItems[i]->itemInfo.SetOrigin(Origins::TL);
	}
}
