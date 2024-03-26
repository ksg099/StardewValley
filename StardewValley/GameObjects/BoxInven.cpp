#include "pch.h"
#include "BoxInven.h"
#include "InvetorySlot.h"
#include "Inventory.h"
#include "TextGo.h"


BoxInven::BoxInven(const std::string& name) : SpriteGo(name)
{
}

void BoxInven::Init()
{
	SpriteGo::Init();
	firstItems = ITEM_SAVE->Get(firstBoxId);
	secondItems = ITEM_SAVE->Get(secondBoxId);

	//인벤토리 창 구현
	SetTexture("graphics/Ui.png");
	//smallUi.SetTexture("graphics/smallUi.png");
	//smallUi.SetPosition({ 970.f, 960.f });
	//smallUi.SetOrigin(Origins::MC);

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
	// 인벤토리 열기/닫기 토글
	if (InputMgr::GetKeyDown(sf::Keyboard::U))
	{
		isBoxInvenOpen = !isBoxInvenOpen;
	}
	if (isBoxInvenOpen)
		return;
	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);
	int firstclickSlotIndex = -1;
	int secondclickSlotIndex = -1;
	bool clickedInFirstInven = false;
	bool clickedInSecondInven = false;

	if (InputMgr::GetMouseButton(sf::Mouse::Button::Left))
	{
		//슬롯을 순회돌아서 만약
		for (int i = 0; i < firtstSlots.size(); ++i)
		{
			if (firtstSlots[i]->GetGlobalBounds().contains(uiPos))
			{
				//일때 첫번째 클릭으로 간주
				if (firstclickSlotIndex == -1)
				{
					//클릭된 슬롯을 할당후 첫번째 인벤에 있음을 확인
					firstclickSlotIndex = i;
					clickedInFirstInven = true;
				}

				//일때 두번째 클릭으로 간주
				else if (secondclickSlotIndex == -1)
				{
					//클릭된 슬롯을 할당후 두번째 인벤에 있음을 확인
					secondclickSlotIndex = i;
					clickedInSecondInven = true;
				}
				else //아무것도 클릭되지 않았을때
				{
					firstclickSlotIndex = -1;
					secondclickSlotIndex = -1;
					clickedInFirstInven = false;
					clickedInSecondInven = false;
				}
				break;
			}
		}

		for (int i = 0; i < secondSlots.size(); ++i)
		{
			if (secondSlots[i]->GetGlobalBounds().contains(uiPos))
			{
				//일때 첫번째 클릭으로 간주
				if (firstclickSlotIndex == -1) // 첫 번째 클릭이라면
				{
					firstclickSlotIndex = i;
					clickedInFirstInven = false;
				}

				//일때 두번째 클릭으로 간주
				else if (secondclickSlotIndex == -1)
				{
					secondclickSlotIndex = i;
				}
				else //아무것도 클릭되지 않았을때
				{
					firstclickSlotIndex = -1;
					secondclickSlotIndex = -1;
					clickedInFirstInven = false;
					clickedInSecondInven = false;
				}
				break;
			}
		}



		// 클릭된 슬롯 처리
		if (firstclickSlotIndex != -1 && secondclickSlotIndex != -1)
		{
			bool firstItemInFirstInven = clickedInFirstInven;
			bool secondItemInFirstInven = clickedInSecondInven;

			// 첫 번째 클릭된 슬롯과 두 번째 클릭된 슬롯이 같은 인벤토리에 속하는 경우
			// 또는 서로 다른 인벤토리에 속하는 경우를 모두 처리할 수 있음
			SwapItem(firstclickSlotIndex, secondclickSlotIndex, firstItemInFirstInven, secondItemInFirstInven);

			// 클릭 인덱스 초기화
			firstclickSlotIndex = -1;
			secondclickSlotIndex = -1;
		}
	}
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
				if (item->IndexX == indexX && item->IndexY == indexY && item != nullptr)
				{
					DisplayItemInfo(*item, uiPos);
					mouseOverSlot = true;

					break;
				}
			}
		}
	}

	//
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

	if (clickedInFirstInven == -1) //빈 공간이 눌렸을 경우 선택된 인덱스를 초기화하여 선택한 것을 초기화
	{
		firstClickIndex = -1;
	}
	UpdateSlots();
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

void BoxInven::UpdateSlots() //
{
	//auto& slote
	//for (auto& slot : slots) {
	//slot->SetEmpty();
	// }
	//메인 인벤토리내를 할당하기전에 지움
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j; //
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

void BoxInven::SwapItem(int firstclickSlotIndex, int secondclickSlotIndex, bool clickedInFirstInven, bool clickedInSecondInven)
{
	int firstX = firstclickSlotIndex % countX;
	int firstY = firstclickSlotIndex / countX;
	int secondX = secondclickSlotIndex % countX;
	int secondY = secondclickSlotIndex / countX;

}



//}

//아이템 정보 출력하기
void BoxInven::DisplayItemInfo(ItemData& itemData, sf::Vector2f& position)
{
	//itemData.type타입이 int형이 아니라 형변환함
	//아이템의 type과 id를 출력
	//인덱스 부분 생략함 + "IndexX "+ std::to_string(itemData.IndexX) + "IndexY" + std::to_string(itemData.IndexY)
	std::string info = "Box: " + std::to_string(itemData.BoxId) + ", X: " + std::to_string(itemData.IndexX) + ", Y: " + std::to_string(itemData.IndexY)
		+ ",\nType: " + std::to_string((int)itemData.type) + ", ID: " + std::to_string(itemData.itemId)
		+ ",\nName: " + ITEM_TABLE->Get(itemData.type, itemData.itemId).name;
	itemInfoText.Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), info, 20, sf::Color::Black);
}

void BoxInven::Draw(sf::RenderWindow& window)
{

	//I키 눌렀을때 메인 인벤토리, 슬롯 그려주기
	if (!isBoxInvenOpen)
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

		itemInfoText.Draw(window);
	}
}

ItemData* BoxInven::GetItemData(const int x, const int y) const
{
	if (x < 0 || y < 0 || x >= countX || y >= countY)
		return nullptr;

	return firtstSlots[y * countX + x]->GetItemData();

}