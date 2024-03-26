#include "pch.h"
#include "BoxInven.h"
#include "InvetorySlot.h"
#include "TextGo.h"


BoxInven::BoxInven(const std::string& name) : SpriteGo(name)
{
}

void BoxInven::Init()
{
	SpriteGo::Init();

	//인벤토리 창 구현
	SetTexture("graphics/Ui.png");
	smallUi.SetTexture("graphics/smallUi.png");
	smallUi.SetPosition({ 970.f, 960.f });
	smallUi.SetOrigin(Origins::MC);

	Release();

	//I키 눌렀을때 나오는 메인 인벤토리 창과 그안에 들어갈 슬롯 만들기
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//슬롯 위치 잡기
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			slot->Init();
			slot->Reset();

			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 270.f, (float)i * 60.f - 250.f }; //하드코딩
			slot->SetPosition(pos);
			slot->SetOrigin(Origins::MC);

			firtstSlots.push_back(slot);
		}
	}

	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//슬롯 위치 잡기
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			slot->Init();
			slot->Reset();

			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 270.f, (float)i * 60.f + 100.f }; //하드코딩
			slot->SetPosition(pos);
			slot->SetOrigin(Origins::MC);

			secondSlots.push_back(slot);
		}
	}
}

void BoxInven::Release()
{
	for (auto slot : firtstSlots)
	{
		if (slot != nullptr)
		{
			delete slot;
			slot = nullptr;
		}
	}
	firtstSlots.clear();

	for (auto slot : secondSlots)
	{
		if (slot != nullptr)
		{
			delete slot;
			slot = nullptr;
		}
	}
	secondSlots.clear();

	SpriteGo::Release();
}

void BoxInven::Reset()
{
	firstItems = ITEM_SAVE->Get(firstBoxId);
	secondItems = ITEM_SAVE->Get(secondBoxId);

	itemClick = nullptr;

	SpriteGo::Reset();
	SetOrigin(Origins::MC);

	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countY + j;
			firtstSlots[index]->Reset();
		}
	}

	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countY + j;
			secondSlots[index]->Reset();
		}
	}

	UpdateSlots();
}

void BoxInven::Update(float dt)
{
	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);

	if (itemClick != nullptr && !isClick)
	{
		auto item = ITEM_TABLE->Get(itemClick->type, itemClick->itemId);
		firstClickSprite.setTexture(RES_MGR_TEXTURE.Get(item.textureId));
		firstClickSprite.setTextureRect(sf::IntRect(item.sheetId.x, item.sheetId.y, item.sheetSize.x, item.sheetSize.y));
		firstClickSprite.setScale(sf::Vector2f(50.f / item.sheetSize.x, 50.f / item.sheetSize.y));
		Utils::SetOrigin(firstClickSprite, Origins::MC);
		isClick = true;
	}
	else if (itemClick == nullptr && isClick)
	{
		isClick = false;
	}
	firstClickSprite.setPosition(uiPos);

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Button::Left))
	{
		itemClick = SaveClickItemData(uiPos);

		if (InputMgr::GetKeyDown(sf::Keyboard::D))
		{
			if (itemClick != nullptr) // 이미 선택된 아이템이 있으면
			{
				if (itemClick->BoxId == firstBoxId)
				{
					auto it = std::find(firstItems->begin(), firstItems->end(), itemClick);
					if (it != firstItems->end())
					{
						delete* it;
						firstItems->erase(it);
					}
				}
				else
				{
					auto it = std::find(secondItems->begin(), secondItems->end(), itemClick);
					if (it != secondItems->end())
					{
						delete* it;
						secondItems->erase(it);
					}
				}
				UpdateSlots();
				itemClick = nullptr;
				itemExchange = nullptr;
			}
		}
	}

	//마우스 왼쪽 버튼을 놓았을때 첫번째 아이템이 선택된 상태라면
	//마우스를 놓은곳에 해당하는 슬롯을 다시 순회를 돌아 찾아 그위치로 아이템을 SwapItem한다.
	if (InputMgr::GetMouseButtonUp(sf::Mouse::Left) && itemClick != nullptr)
	{
		// 3. 같은 박스 교환
		// 3-2. 다른 박스 교환
		std::pair<int, bool> mouseIndex = CheckPosIndex(uiPos);
		if (mouseIndex.first == -1) // 아무것도 아닌 공간
		{

		}
		else
		{
			int indexExchangeX = mouseIndex.first % countX;
			int indexExchangeY = mouseIndex.first / countX;

			itemExchange = GetItemData(indexExchangeX, indexExchangeY, mouseIndex.second);
			if (itemExchange == nullptr) // 빈 공간
			{
				if ((itemClick->BoxId != firstBoxId && mouseIndex.second) ||
					(itemClick->BoxId == firstBoxId && !mouseIndex.second))
				{
					SwapItemDiffBox(itemClick, itemExchange, itemClick->BoxId == firstBoxId);
				}
				itemClick->IndexX = indexExchangeX;
				itemClick->IndexY = indexExchangeY;
			}
			else // 같은 박스 교환
			{
				itemExchange->IndexX = itemClick->IndexX;
				itemExchange->IndexY = itemClick->IndexY;

				itemClick->IndexX = indexExchangeX;
				itemClick->IndexY = indexExchangeY;

				if (itemClick->BoxId != itemExchange->BoxId) // 다른 박스 교환
				{
					SwapItemDiffBox(itemClick, itemExchange, itemClick->BoxId == firstBoxId);
				}
			}
		}
		UpdateSlots();
		itemClick = nullptr;
		itemExchange = nullptr;
	}

	////첫번째 클릭 된 아이템의 인덱스가 아직 아무것도 선택이 안된 경우
	////위의 조건들일 경우
	//if (clickSlotIndex != -1 && firstClickIndex == -1)
	//{
	//	int fx = clickSlotIndex % countX;
	//	int fy = clickSlotIndex / countX;
	//	auto findFirst = std::find_if(firstItems->begin(), firstItems->end(), [fx, fy](ItemData* elem)
	//		{
	//			return elem->IndexX == fx && elem->IndexY == fy;
	//		});

	//	//아이템이 있으면 클릭한 슬롯 인덱스를 firstClickIndex에 할당합니다.
	//	//첫번째로 선택한 아이템
	//	if (findFirst != firstItems->end())
	//	{
	//		firstClickIndex = clickSlotIndex;
	//	}
	//}
	itemInfoText.SetPosition(uiPos);
	//마우스 커서 위치에 있는 슬롯 아이템 정보 표시
	bool mouseOverSlot = false;
	for (int i = 0; i < firtstSlots.size(); ++i)
	{
		sf::FloatRect slotBounds = firtstSlots[i]->GetGlobalBounds();
		if (slotBounds.contains(uiPos))
		{
			for (auto& item : *firstItems)
			{
				int indexX = i % countX;
				int indexY = i / countX;
				if (item != nullptr && item->IndexX == indexX && item->IndexY == indexY)
				{
					DisplayItemInfo(*item, uiPos);
					mouseOverSlot = true;

					break;
				}
			}
		}
	}
	for (int i = 0; i < secondSlots.size(); ++i)
	{
		sf::FloatRect slotBounds = secondSlots[i]->GetGlobalBounds();
		if (slotBounds.contains(uiPos))
		{
			for (auto& item : *secondItems)
			{
				int indexX = i % countX;
				int indexY = i / countX;
				if (item->IndexX == indexX && item->IndexY == indexY && item != nullptr)
				{
					DisplayItemInfo(*item, uiPos);
					mouseOverSlot = true;

					break;
				}
			}
		}
	}

	if (!mouseOverSlot)
	{
		itemInfoText.SetString("");
	}

	//if (clickSlotIndex == -1) //빈 공간이 눌렸을 경우 선택된 인덱스를 초기화하여 선택한 것을 초기화
	//{
	//	firstClickIndex = -1;
	//}
}

void BoxInven::SetPosition(const sf::Vector2f& pos)
{
	SpriteGo::SetPosition(pos);
}

void BoxInven::SetIvenSlot(int x, int y, ItemData* data, std::vector<InvetorySlot*>& slots)
{
	int index = y * countX + x;
	slots[index]->SetItem(data);
}

void BoxInven::UpdateSlots()
{
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j;
			firtstSlots[index]->SetEmpty();
		}
	}

	//메인 인벤토리에 아이템 할당
	for (ItemData* item : *firstItems)
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item, firtstSlots);
	}

	//메인 인벤토리내를 할당하기전에 지움
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j; //
			secondSlots[index]->SetEmpty();
		}
	}

	//메인 인벤토리에 아이템 할당
	for (ItemData* item : *secondItems)
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item, secondSlots);
	}
}

void BoxInven::SwapItem(int firstClickIndex, int secondClickIndex)
{

	int fx = firstClickIndex % countX; //가로
	int fy = firstClickIndex / countX; //세로

	int sx = secondClickIndex % countX; //가로
	int sy = secondClickIndex / countX; //세로

	//첫번째 아이템 찾기 | 람다식
	//elem 리스트의 각요소
	//찾고 싶은 아이템의 열과 행을 나타냄
	auto findFirst = std::find_if(firstItems->begin(), firstItems->end(), [fx, fy](ItemData* elem)
		{
			return elem->IndexX == fx && elem->IndexY == fy;
		});

	//두번째 아이템 찾기 | 람다식
	//elem 리스트의 각요소
	//찾고 싶은 아이템의 열과 행을 나타냄
	auto findSecond = std::find_if(firstItems->begin(), firstItems->end(), [sx, sy](ItemData* elem)
		{
			return elem->IndexX == sx && elem->IndexY == sy;
		});

	// 두번째 인덱스가 빈공간일 경우 첫번째 아이템과 빈공간 클릭시 교환
	//첫번째 아이템이 존재하고 두번째 아이템이 없을 경우
	if (findFirst != firstItems->end() && findSecond == firstItems->end())
	{
		(*findFirst)->IndexX = sx;
		(*findFirst)->IndexY = sy;
		//(*findFirst)->BoxId = secondBoxId;
	}

	// 그냥 아이템끼리 교환
	else if (findFirst != firstItems->end() && findSecond != firstItems->end())
	{
		int tempBoxId = (*findFirst)->BoxId;
		int tempX = (*findFirst)->IndexX;
		int tempY = (*findFirst)->IndexY;

		(*findFirst)->BoxId = (*findSecond)->BoxId;
		(*findFirst)->IndexX = (*findSecond)->IndexX;
		(*findFirst)->IndexY = (*findSecond)->IndexY;

		(*findSecond)->BoxId = tempBoxId;
		(*findSecond)->IndexX = tempX;
		(*findSecond)->IndexY = tempY;
	}

	UpdateSlots();
}

void BoxInven::DisplayItemInfo(ItemData& itemData, sf::Vector2f& position)
{
	std::string info = "Box: " + std::to_string(itemData.BoxId) + ", X: " + std::to_string(itemData.IndexX) + ", Y: " + std::to_string(itemData.IndexY)
		+ ",\nType: " + std::to_string((int)itemData.type) + ", ID: " + std::to_string(itemData.itemId)
		+ ",\nName: " + ITEM_TABLE->Get(itemData.type, itemData.itemId).name;
	itemInfoText.Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), info, 20, sf::Color::Black);
}

void BoxInven::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	for (auto slot : firtstSlots)
	{
		slot->Draw(window);
	}

	for (auto slot : secondSlots)
	{
		slot->Draw(window);
	}

	if (isClick)
	{
		window.draw(firstClickSprite);
	}
	else
	{
		itemInfoText.Draw(window);
	}
}

ItemData* BoxInven::GetItemData(const int x, const int y, const bool isFirstBox) const
{
	if (x < 0 || y < 0 || x >= countX || y >= countY)
		return nullptr;

	if (isFirstBox)
		return firtstSlots[y * countX + x]->GetItemData();
	else
		return secondSlots[y * countX + x]->GetItemData();
}

std::pair<int, bool> BoxInven::CheckPosIndex(sf::Vector2f uiPos)
{
	for (int i = 0; i < firtstSlots.size(); ++i)
	{
		sf::FloatRect slotBounds = firtstSlots[i]->GetGlobalBounds();
		if (slotBounds.contains(uiPos))
		{
			return std::make_pair(i, true);
		}
	}
	for (int i = 0; i < firtstSlots.size(); ++i)
	{
		sf::FloatRect slotBounds = secondSlots[i]->GetGlobalBounds();
		if (slotBounds.contains(uiPos))
		{
			return std::make_pair(i, false);
		}
	}
	return std::make_pair(-1, true);
}

ItemData* BoxInven::SaveClickItemData(sf::Vector2f uiPos)
{
	for (int i = 0; i < firtstSlots.size(); ++i)
	{
		sf::FloatRect slotBounds = firtstSlots[i]->GetGlobalBounds();
		if (slotBounds.contains(uiPos))
		{
			return GetItemData(i % countX, i / countX, true);
		}
	}
	for (int i = 0; i < firtstSlots.size(); ++i)
	{
		sf::FloatRect slotBounds = secondSlots[i]->GetGlobalBounds();
		if (slotBounds.contains(uiPos))
		{
			return GetItemData(i % countX, i / countX, false);
		}
	}

	return nullptr;
}

void BoxInven::SwapItemDiffBox(ItemData* itemClick, ItemData* itemExchange, bool isClickFirst)
{
	if (isClickFirst)
	{
		if (itemExchange != nullptr)
		{
			firstItems->push_back(itemExchange);
			auto findExchange = std::find(secondItems->begin(), secondItems->end(), itemExchange);
			secondItems->erase(findExchange);

			itemExchange->BoxId = firstBoxId;
		}

		secondItems->push_back(itemClick);
		auto findClick = std::find(firstItems->begin(), firstItems->end(), itemClick);
		firstItems->erase(findClick);

		itemClick->BoxId = secondBoxId;
	}
	else
	{
		if (itemExchange != nullptr)
		{
			secondItems->push_back(itemExchange);
			auto findExchange = std::find(firstItems->begin(), firstItems->end(), itemExchange);
			firstItems->erase(findExchange);

			itemExchange->BoxId = secondBoxId;
		}

		firstItems->push_back(itemClick);
		auto findClick = std::find(secondItems->begin(), secondItems->end(), itemClick);
		secondItems->erase(findClick);

		itemClick->BoxId = firstBoxId;
	}
}
