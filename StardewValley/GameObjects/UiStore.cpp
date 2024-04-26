#include "pch.h"
#include "UiStore.h"
#include "SceneGame.h"
#include "Player.h"

UiStore::UiStore(const std::string& name) : GameObject(name)
{
}

void UiStore::Init()
{
	// Portrait
	sellerPortrait.SetTexture("graphics/pierrePortrait.png");
	sellerPortrait.SetScale(sf::Vector2f(3.5f, 3.5f));
	sellerPortrait.SetOrigin(Origins::BR);
	sellerPortrait.SetPosition(sf::Vector2f(500.f, 460.f));

	// text box
	sellerTextBox.SetTexture("graphics/box1.png");
	sellerTextBox.SetOrigin(Origins::TR);
	sellerTextBox.SetPosition(sf::Vector2f(500.f, 480.f));
	sellerText.Set(RES_MGR_FONT.Get("fonts/SDMiSaeng.ttf"),
		"Welcome to Pierre!\nDo you have something\nyou want to sell?",
		35, sf::Color::Black);
	sellerText.SetOrigin(Origins::TC);
	sellerText.SetPosition(sf::Vector2f(340.f, 500.f));

	// item list box
	for (auto itemType : itemTypes)
	{
		for (int j = 0; j < ITEM_TABLE->Count(itemType); ++j)
		{
			DataItem itemData = ITEM_TABLE->Get(itemType, j);
			itemTable.push_back(itemData);
		}
	}

	itemListBox.SetTexture("graphics/box2.png");
	itemListBox.SetOrigin(Origins::TL);
	itemListBox.SetPosition(sf::Vector2f(520.f, 200.f));

	scrollBar.SetTexture("graphics/ScrollBar.png");
	scrollBar.SetOrigin(Origins::TC);
	scrollBar.SetPosition(itemListBox.GetPosition() + sf::Vector2f(itemListBox.GetGlobalBounds().width, 0.f)
	 + sf::Vector2f(40.f, 0.f));

	scroll.SetTexture("graphics/Scroll.png");
	scroll.SetOrigin(Origins::TC);
	scroll.SetPosition(scrollBar.GetPosition()); // 첫번째 인덱스

	for (int i = 0; i < slotCount; ++i)
	{
		UiShopSlot* slot = new UiShopSlot();

		slot->itemSlot.SetTexture("graphics/shopCellBox.png");
		slot->itemSlot.SetOrigin(Origins::TL);
		slot->itemSlot.SetPosition(itemListBox.GetPosition() + sf::Vector2f(18.f, 21.f + 115.f * i));

		slot->clickBox.setFillColor(sf::Color::Green);
		slot->clickBox.setSize({ slot->itemSlot.GetGlobalBounds().width, slot->itemSlot.GetGlobalBounds().height });
		slot->clickBox.setPosition(slot->itemSlot.GetPosition());
		Utils::SetOrigin(slot->clickBox, Origins::TL);

		slot->itemImgBox.SetTexture("graphics/ItemSlot.png");
		slot->itemImgBox.SetOrigin(Origins::MC);
		slot->itemImgBox.SetPosition(slot->itemSlot.GetPosition() + sf::Vector2f(70.f, slot->itemSlot.GetLocalBounds().height / 2.f));

		slot->itemImg.SetTexture(itemTable[i].textureId);
		slot->itemImg.SetTextureRect(sf::IntRect(itemTable[i].sheetId.x, itemTable[i].sheetId.y, itemTable[i].sheetSize.x, itemTable[i].sheetSize.y));
		int scale = itemTable[i].sheetSize.x < itemTable[i].sheetSize.y ? itemTable[i].sheetSize.x : itemTable[i].sheetSize.y;
		slot->itemImg.SetScale(sf::Vector2f(50.f / scale, 50.f / scale));
		slot->itemImg.SetOrigin(Origins::MC);
		slot->itemImg.SetPosition(slot->itemImgBox.GetPosition());

		std::string name = itemTable[i].name;
		slot->itemName.Set(RES_MGR_FONT.Get("fonts/SDMiSaeng.ttf"), name, 80, sf::Color::Color(86, 22, 12, 255));
		slot->itemName.SetPosition(slot->itemImgBox.GetPosition() + sf::Vector2f(50.f, -25.f));
		slot->itemName.SetOrigin(Origins::ML);

		std::string price = std::to_string(itemTable[i].purchasePrice);
		slot->itemPrice.Set(RES_MGR_FONT.Get("fonts/SDMiSaeng.ttf"), price, 50, sf::Color::Color(86, 22, 12, 255));
		slot->itemPrice.SetPosition(slot->itemImgBox.GetPosition() + sf::Vector2f(885.f, -25.f));
		slot->itemPrice.SetOrigin(Origins::MR);

		slot->coinImg.SetTexture("graphics/Coin.png");
		slot->coinImg.SetOrigin(Origins::MC);
		slot->coinImg.SetPosition(slot->itemImgBox.GetPosition() + sf::Vector2f(930.f, 0.f));

		shopSlots.push_back(slot);
	}

	GameObject::Init();
}

void UiStore::Release()
{
	GameObject::Release();
}

void UiStore::Reset()
{
	GameObject::Reset();

	currentIndex = 0;
	UpdateIndex();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(sceneGame->FindGo("Player"));
}

void UiStore::Update(float dt)
{
	GameObject::Update(dt);

	SelectByDoubleClick(dt);

	if (InputMgr::GetMouseWheelDown(sf::Mouse::Wheel::VerticalWheel)
		&& currentIndex < itemTable.size() - slotCount)
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
	sellerPortrait.Draw(window);
	sellerTextBox.Draw(window);
	sellerText.Draw(window);

	itemListBox.Draw(window);
	scrollBar.Draw(window);
	scroll.Draw(window);

	for (int i = 0; i < slotCount; ++i)
	{
		shopSlots[i]->itemSlot.Draw(window);
		shopSlots[i]->itemImgBox.Draw(window);
		shopSlots[i]->itemImg.Draw(window);
		shopSlots[i]->itemName.Draw(window);
		shopSlots[i]->itemPrice.Draw(window);
		shopSlots[i]->coinImg.Draw(window);
	}

	GameObject::Draw(window);
}

void UiStore::UpdateIndex()
{
	for (int i = 0; i < slotCount; ++i)
	{
		shopSlots[i]->itemImg.SetTexture(itemTable[i + currentIndex].textureId);
		shopSlots[i]->itemImg.SetTextureRect(sf::IntRect(itemTable[i + currentIndex].sheetId.x, itemTable[i + currentIndex].sheetId.y,
			itemTable[i + currentIndex].sheetSize.x, itemTable[i + currentIndex].sheetSize.y));
		int scale = itemTable[i + currentIndex].sheetSize.x < itemTable[i + currentIndex].sheetSize.y ? itemTable[i + currentIndex].sheetSize.x : itemTable[i + currentIndex].sheetSize.y;
		shopSlots[i]->itemImg.SetScale(sf::Vector2f(50.f / scale, 50.f / scale));
		shopSlots[i]->itemImg.SetPosition(shopSlots[i]->itemImgBox.GetPosition());
		shopSlots[i]->itemImg.SetOrigin(Origins::MC);

		std::string name = itemTable[i + currentIndex].name;
		shopSlots[i]->itemName.SetString(name);
		shopSlots[i]->itemName.SetOrigin(Origins::ML);

		std::string price = std::to_string(itemTable[i + currentIndex].purchasePrice);
		shopSlots[i]->itemPrice.SetString(price);
		shopSlots[i]->itemPrice.SetOrigin(Origins::MR);
	}

	int totalIndex = itemTable.size() - slotCount;
	if (totalIndex > 0)
	{
		sf::Vector2f scrollPos = scrollBar.GetPosition();
		scrollPos.y += (scrollBar.GetGlobalBounds().height - scroll.GetGlobalBounds().height - 52.f) / totalIndex * currentIndex;
		scroll.SetPosition(scrollPos);
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
			for (int i = 0; i < slotCount; ++i)
			{
				if (shopSlots[i]->clickBox.getGlobalBounds().contains(uiPos))
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
			for (int i = 0; i < slotCount; ++i)
			{
				if (shopSlots[i]->clickBox.getGlobalBounds().contains(uiPos))
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
		if (sceneGame->GetMoney() >= itemTable[purchaseIndex].purchasePrice)
		{
			sceneGame->AddMoney(-itemTable[purchaseIndex].purchasePrice);
			sceneGame->CreateItem(itemTable[purchaseIndex], player->GetGridIndex().x, player->GetGridIndex().y);
		}
		purchaseIndex = -1;
	}
}