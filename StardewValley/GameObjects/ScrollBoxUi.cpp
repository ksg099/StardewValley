#include "pch.h"
#include "ScrollBoxUi.h"

ScrollBoxUi::ScrollBoxUi(const std::string& name) : GameObject(name)
{
}

void ScrollBoxUi::Init()
{
	slotBoxesBackground.SetTexture("graphics/box2.png");
	slotBoxesBackground.SetOrigin(Origins::MC);
	slotBoxesBackground.SetPosition(sf::Vector2f(0.f, 0.f));

	scrollBar.SetTexture("graphics/ScrollBar.png");
	scrollBar.SetOrigin(Origins::TC);
	scrollBar.SetPosition(slotBoxesBackground.GetPosition()
		+ sf::Vector2f(slotBoxesBackground.GetGlobalBounds().width / 2.f, -slotBoxesBackground.GetGlobalBounds().height / 2.f)
		+ sf::Vector2f(40.f, -50.f));

	scroll.SetTexture("graphics/Scroll.png");
	scroll.SetOrigin(Origins::TC);
	scroll.SetPosition(scrollBar.GetPosition()); // 첫번째 인덱스

	for (int i = 0; i < slotCount; ++i)
	{
		SlotBox* slot = new SlotBox();

		slot->slotBoxImg.SetTexture("graphics/shopCellBox.png");
		slot->slotBoxImg.SetOrigin(Origins::MC);
		slot->slotBoxImg.SetPosition(slotBoxesBackground.GetPosition() + sf::Vector2f(0.f, -slotBoxesBackground.GetGlobalBounds().height / 2.f + 79.f + 115.f * i));
		
		slot->clickBox.setFillColor(sf::Color::Green);
		slot->clickBox.setSize({ slot->slotBoxImg.GetGlobalBounds().width, slot->slotBoxImg.GetGlobalBounds().height });
		slot->clickBox.setPosition(slot->slotBoxImg.GetPosition());
		Utils::SetOrigin(slot->clickBox, Origins::MC);

		std::string slotName = "";
		slot->slotName.Set(RES_MGR_FONT.Get("fonts/SDMiSaeng.ttf"), slotName, 80, sf::Color::Color(86, 22, 12, 255));
		slot->slotName.SetPosition(slot->slotBoxImg.GetPosition() + sf::Vector2f(-500.f, -30.f));
		slot->slotName.SetOrigin(Origins::ML);

		slotBoxes.push_back(slot);
	}

	GameObject::Init();
}

void ScrollBoxUi::Release()
{
	GameObject::Release();
}

void ScrollBoxUi::Reset()
{
	GameObject::Reset();
}

void ScrollBoxUi::Update(float dt)
{
	if (fileNames == nullptr)
		return;

	GameObject::Update(dt);

	SelectByDoubleClick(dt);

	if (InputMgr::GetMouseWheelDown(sf::Mouse::Wheel::VerticalWheel)
		&& currentIndex < fileNames->size() - slotCount)
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

void ScrollBoxUi::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);

	slotBoxesBackground.Draw(window);
	scrollBar.Draw(window);
	scroll.Draw(window);

	for (int i = 0; i < slotCount; ++i)
	{
		slotBoxes[i]->slotBoxImg.Draw(window);
		// window.draw(slotBoxes[i]->clickBox);
		slotBoxes[i]->slotName.Draw(window);
	}
}

void ScrollBoxUi::SetFileNames(std::vector<TextGo*>& files)
{
	fileNames = &files;

	currentIndex = 0;
	UpdateIndex();
}

void ScrollBoxUi::UpdateIndex()
{
	for (int i = 0; i < slotCount; ++i)
	{
		std::string name = (*fileNames)[i + currentIndex]->GetString();
		slotBoxes[i]->slotName.SetString(name);
		slotBoxes[i]->slotName.SetOrigin(Origins::ML);
	}

	int totalIndex = fileNames->size() - slotCount;
	if (totalIndex > 0)
	{
		sf::Vector2f scrollPos = scrollBar.GetPosition();
		scrollPos.y += (scrollBar.GetGlobalBounds().height - scroll.GetGlobalBounds().height - 52.f) / totalIndex * currentIndex;
		scroll.SetPosition(scrollPos);
	}
}

void ScrollBoxUi::SelectByDoubleClick(float dt)
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
				if (slotBoxes[i]->clickBox.getGlobalBounds().contains(uiPos))
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
				if (slotBoxes[i]->clickBox.getGlobalBounds().contains(uiPos))
				{
					if (clickIndex == i)
					{
						doubleClickIndex = currentIndex + clickIndex;
					}
					break;
				}
			}
			clickIndex = -1;
			isClick = false;
		}
	}

	if (doubleClickIndex != -1)
	{
		// 더블 클릭 처리
	}
}