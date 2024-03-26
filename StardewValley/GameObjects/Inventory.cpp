#include "pch.h"
#include "Inventory.h"
#include "InvetorySlot.h"
#include "TextGo.h"


Inventory::Inventory(const std::string& name) : SpriteGo(name)
{
}

void Inventory::Init()
{
	SpriteGo::Init();

	//인벤토리 창 구현
	SetTexture("graphics/Ui.png");
	smallUi.SetTexture("graphics/smallUi.png");
	smallUi.SetPosition({970.f, 960.f});
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
			
			slots.push_back(slot);
		}
	}

	


	//메인 화면 하단에 나오는 작은 인벤토리의 슬롯 만들기
	for (int j = 0; j < countX; j++)
	{
		//슬롯 위치 잡기
		InvetorySlot* smallslot = new InvetorySlot("Inventory Slot");
		sf::Vector2f pos = smallUi.GetPosition();
		smallslot->Init();

		smallslot->SetPosition(pos - sf::Vector2f(320.f - 70.f * j, 0.f)); //하드코딩
		smallslot->SetOrigin(Origins::MC);

		smallslots.push_back(smallslot);
	}
	
}

void Inventory::Release()
{
	for (auto slot : slots)
	{
		if (slot != nullptr)
		{
			delete slot;
			slot = nullptr;
		}
	}
	slots.clear();

	SpriteGo::Release();
}

void Inventory::Reset()
{
	items = ITEM_SAVE->Get(inventoryBoxId);

	SpriteGo::Reset();
	SetOrigin(Origins::MC);
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countY + j;
			slots[index]->Reset();
		}
	}
	
	UpdateSlots();
	UpdateSubSlots();
}

void Inventory::Update(float dt)
{
	//인벤토리가 안보였다면 I키를 눌렀을때 보이게 하기
	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		isAble = !isAble;
	}

	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);
	int clickSlotIndex = -1;

	//마우스 왼쪽 버튼이 눌렸 있을때 마우스 위치에 해당하는 슬롯을
	//순회를 돌아 찾아 clickSlotIndex에 할당
	if (InputMgr::GetMouseButton(sf::Mouse::Button::Left))
	{
		for (int i = 0; i < slots.size(); ++i)
		{
			sf::FloatRect slotBounds = slots[i]->GetGlobalBounds();
			if (slotBounds.contains(uiPos))
			{
				clickSlotIndex = i;
				break;
			}
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::D))
		{
			if (firstClickIndex != -1) // 이미 선택된 아이템이 있으면
			{
				int fx = firstClickIndex % countX;
				int fy = firstClickIndex / countX;

				auto it = std::find_if(items->begin(), items->end(), [fx, fy](ItemData* elem) {
					return elem->IndexX == fx && elem->IndexY == fy;
					});

				if (it != items->end())
				{
					// 선택된 아이템을 items 리스트에서 삭제
					delete *it;
					items->erase(it);

					// 인벤토리 UI 갱신
					UpdateSlots();
					UpdateSubSlots();

					// 선택 초기화
					firstClickIndex = -1;
				}
			}
		}
	}

	//마우스 왼쪽 버튼을 놓았을때 첫번째 아이템이 선택된 상태라면
	//마우스를 놓은곳에 해당하는 슬롯을 다시 순회를 돌아 찾아 그위치로 아이템을 SwapItem한다.
	if (InputMgr::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (firstClickIndex != -1) //이미 첫번째 인덱스가 선택되어 있을때
		{
			for (int i = 0; i < slots.size(); ++i)
			{
				sf::FloatRect slotBounds = slots[i]->GetGlobalBounds();
				if (slotBounds.contains(uiPos))
				{
					clickSlotIndex = i;
					break;
				}
			}

			if (clickSlotIndex != -1)
			{
				clickSlotIndex;
				SwapItem(firstClickIndex, clickSlotIndex);

				firstClickIndex = -1;
			}
		}
	}

	//첫번째 클릭 된 아이템의 인덱스가 아직 아무것도 선택이 안된 경우
	//위의 조건들일 경우
	if (clickSlotIndex != -1 && firstClickIndex == -1)
	{
		int fx = clickSlotIndex % countX;
		int fy = clickSlotIndex / countX;
		auto findFirst = std::find_if(items->begin(), items->end(), [fx, fy](ItemData* elem)
			{
				return elem->IndexX == fx && elem->IndexY == fy;
			});

		//아이템이 있으면 클릭한 슬롯 인덱스를 firstClickIndex에 할당합니다.
		//첫번째로 선택한 아이템
		if (findFirst != items->end())
		{
			firstClickIndex = clickSlotIndex;
		}
	}
	itemInfoText.SetPosition(uiPos);
	//마우스 커서 위치에 있는 슬롯 아이템 정보 표시
	bool mouseOverSlot = false;
	for (int i = 0; i < slots.size(); ++i)
	{
		sf::FloatRect slotBounds = slots[i]->GetGlobalBounds();
		if (slotBounds.contains(uiPos))
		{
			for (auto& item : *items)
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

	if (clickSlotIndex == -1) //빈 공간이 눌렸을 경우 선택된 인덱스를 초기화하여 선택한 것을 초기화
	{
		firstClickIndex = -1;
	}

	// tab을 누르면 서브 인벤토리 변경
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
	{
		subIndexY++;
		if (subIndexY >= countY)
		{
			subIndexY = 0;
		}
		UpdateSubSlots();
	}
}

void Inventory::SetPosition(const sf::Vector2f& pos)
{
	SpriteGo::SetPosition(pos);
}

void Inventory::SetIvenSlot(int x, int y, ItemData* data)
{
	int index = y * countX + x;
	slots[index]->SetItem(data);
}

void Inventory::UpdateSlots() //
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
			slots[index]->SetEmpty();
		}
	}

	//메인 인벤토리에 아이템 할당
	for (ItemData* item : *items)
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item);
	}
	
	//
	//for (auto& currentItem : *items) {
	//	bool placed = false;

	//	// 이미 할당된 슬롯에서 동일한 id를 가진 아이템을 찾습니다.
	//	for (int i = 0; i < slots.size() && !placed; i++)
	//	{
	//		if (item->IndexX == indexX && item->IndexY == indexY && item != nullptr && item == boxId)
	//		{
	//			//찾으면 아이템 할당하고 할당된 장소라고 선언
	//			slots[i]->item->count += currentItem->count;
	//			placed = true;
	//		}
	//	}

	//	// 아직 아이템을 배치하지 않았다면, 슬롯에 접근하여 아이템을 할당
	//	if (!placed)
	//	{
	//		for (int i = 0; i < slots.size() && !placed; i++)
	//		{
	//			if (slots[i]->item == nullptr)
	//			{
	//				slots[i]->item = currentItem;
	//				placed = true;
	//			}
	//		}
	//	}
	//}
}

void Inventory::UpdateSubSlots()
{
	//작은 인벤토리 슬롯 먼저 비우고
	for (auto& smallslot : smallslots)
	{
		smallslot->SetEmpty();
	}

	// 작은 인벤토리 슬롯에 아이템을 할당합니다.
	for (ItemData* item : *items)
	{
		int indexX = item->IndexX;
		int indexY = item->IndexY;

		if (indexY == subIndexY)
		{
			smallslots[indexX]->SetItem(item);
		}
	}
}

//아이템 검사 및 수령 조절
//void Inventory::AddItem(ItemData* currentItem)
//{
//	//for (auto& item : *items) {
//	//	if (item->type == currentItem->type && item->itemId == currentItem->itemId) {
//	//		// 이미 존재하는 아이템이면 개수만 증가시킴
//	//		item->count += currentItem->count;
//	//		UpdateSlots(); // UI 갱신
//	//		return;
//	//	}
//	//}
//
//	//// 새 아이템이면 다음 빈 슬롯에 추가
//
//
//	//UpdateSlots(); // UI 갱신
//}

void Inventory::SwapItem(int firstClickIndex, int secondClickIndex)
{

	int fx = firstClickIndex % countX; //가로
	int fy = firstClickIndex / countX; //세로

	int sx = secondClickIndex % countX; //가로
	int sy = secondClickIndex / countX; //세로

	//첫번째 아이템 찾기 | 람다식
	//elem 리스트의 각요소
	//찾고 싶은 아이템의 열과 행을 나타냄
	auto findFirst = std::find_if(items->begin(), items->end(), [fx, fy](ItemData* elem)
	{
	return elem->IndexX == fx && elem->IndexY == fy;
	});

	//두번째 아이템 찾기 | 람다식
	//elem 리스트의 각요소
	//찾고 싶은 아이템의 열과 행을 나타냄
	auto findSecond = std::find_if(items->begin(), items->end(), [sx, sy](ItemData* elem)
	{
	return elem->IndexX == sx && elem->IndexY == sy;
	});

	// 두번째 인덱스가 빈공간일 경우 첫번째 아이템과 빈공간 클릭시 교환
	//첫번째 아이템이 존재하고 두번째 아이템이 없을 경우
	if (findFirst != items->end() && findSecond == items->end())
	{
		(*findFirst)->IndexX = sx;
		(*findFirst)->IndexY = sy;
		//(*findFirst)->BoxId = secondBoxId;
	}

	// 그냥 아이템끼리 교환
	else if (findFirst != items->end() && findSecond != items->end())
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

	UpdateSlots(); // 슬롯 상태 업데이트
	UpdateSubSlots();
}

void Inventory::DisplayItemInfo(ItemData& itemData, sf::Vector2f& position)
{
	//itemData.type타입이 int형이 아니라 형변환함
	//아이템의 type과 id를 출력
	//인덱스 부분 생략함 + "IndexX "+ std::to_string(itemData.IndexX) + "IndexY" + std::to_string(itemData.IndexY)
	std::string info = "Box: " + std::to_string(itemData.BoxId) + ", X: " + std::to_string(itemData.IndexX) + ", Y: " + std::to_string(itemData.IndexY)
		+ ",\nType: " + std::to_string((int)itemData.type) + ", ID: " + std::to_string(itemData.itemId)
		+ ",\nName: " + ITEM_TABLE->Get(itemData.type, itemData.itemId).name;
	itemInfoText.Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), info, 20, sf::Color::Black);
}

void Inventory::Draw(sf::RenderWindow& window)
{

	//I키 눌렀을때 메인 인벤토리, 슬롯 그려주기
	if (!isAble)
	{
		SpriteGo::Draw(window);
		for (auto slot : slots)
		{
			slot->Draw(window);
		}
		itemInfoText.Draw(window);
	}
	//메인 화면에 보일 인벤토리랑 슬롯 그려주기
	else
	{
		smallUi.Draw(window);
		for (auto smallslot : smallslots)
		{
			smallslot->Draw(window);
		}
	}
}

ItemData* Inventory::GetItemData(const int x, const int y) const
{
	if (x < 0 || y < 0 || x >= countX || y >= countY)
		return nullptr;

	return slots[y * countX + x]->GetItemData();
	
}