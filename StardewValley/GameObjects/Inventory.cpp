#include "pch.h"
#include "Inventory.h"
#include "InvetorySlot.h"

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

	//sf::Vector2f smallUiPosition = smallUi.GetPosition();

	Release();

	//I키 눌렀을때 나오는 메인 인벤토리 창과 그안에 들어갈 슬롯 만들기 3*10
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//슬롯 위치 잡기
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 310.f, (float)i * 60.f - 300.f }; //하드코딩
			slot->SetPosition(pos);
			slot->SetOrigin(Origins::MC);
			slot->Init();
			slots.push_back(slot);
		}
	}

	//메인 화면 하단에 나오는 작은 인벤토리의 슬롯 만들기
	for (int j = 0; j < countX; j++)
	{
		//슬롯 위치 잡기
		InvetorySlot* smallslot = new InvetorySlot("Inventory Slot");
		sf::Vector2f pos = smallUi.GetPosition();
		smallslot->SetPosition(pos - sf::Vector2f(350.f - 70.f * j, 35.f)); //하드코딩
		smallslot->SetOrigin(Origins::MC);
		smallslot->Init();
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
	
	UpdateSlots(items);
	UpdateSubSlots();
}

void Inventory::Update(float dt)
{
	SpriteGo::Update(dt);

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
		auto findFirst = std::find_if(items.begin(), items.end(), [fx, fy](ItemData* elem)
			{
				return elem->IndexX == fx && elem->IndexY == fy;
			});

		//아이템이 있으면 클릭한 슬롯 인덱스를 firstClickIndex에 할당합니다.
		//첫번째로 선택한 아이템
		if (findFirst != items.end())
		{
			firstClickIndex = clickSlotIndex;
		}
	}

	if (clickSlotIndex == -1) //빈 공간이 눌렸을 경우 선택된 인덱스를 초기화하여 선택한 것을 초기화
	{
		firstClickIndex = -1;
	}

	// tab을 누르면 서브 인벤토리 변경
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
	{
		++subIndexY;
		if (subIndexY >= countY)
		{
			subIndexY = 0;
		}
		UpdateSubSlots();
	}
}


void Inventory::Draw(sf::RenderWindow& window)
{
	//메인 화면에 보일 인벤토리랑 슬롯 그려주기
	smallUi.Draw(window);
	for (auto smallslot : smallslots)
	{
		smallslot->Draw(window);
	}

	//I키 눌렀을때 메인 인벤토리, 슬롯 그려주기
	if (!isAble)
	{
		
		SpriteGo::Draw(window);

		for (auto slot : slots)
		{
			slot->Draw(window);
		}
	}
}

void Inventory::SetPosition(const sf::Vector2f& pos)
{
	SpriteGo::SetPosition(pos);
}

void Inventory::LoadData(rapidjson::Document& doc)
{
	//인덱스 사이즈만큼 순회
	for (int i = 0; i < doc["ItemData"].Size(); ++i)
	{
		//rapidjson::Document의 객체 doc를 이용해서 배열ItemData의 i번째 정보를 읽는다.
		//읽은 정보를 itemData에 할당한다.
		//[i * countX + j] 특정 칸 찾기
		ItemData* itemData = new ItemData;
		itemData->BoxId = doc["ItemData"][i]["Box ID"].GetInt();
		itemData->IndexX = doc["ItemData"][i]["Index X"].GetInt();
		itemData->IndexY = doc["ItemData"][i]["Index Y"].GetInt();
		itemData->itemId = doc["ItemData"][i]["ItemId"].GetInt();
		itemData->instanceId = doc["ItemData"][i]["InstanceId"].GetInt();
		itemData->count = doc["ItemData"][i]["Count"].GetInt();
		itemData->canOverLap = doc["ItemData"][i]["CanOverlap"].GetBool();
		itemData->type = (ItemType)doc["ItemData"][i]["ItemType"].GetInt();
		itemData->itemFilePath = doc["ItemData"][i]["Resource"].GetString();

		items.push_back(itemData);
	}
}

void Inventory::SetIvenSlot(int x, int y, ItemData* data)
{
	int index = y * countX + x;
	slots[index]->SetItem(data);
}

void Inventory::UpdateSlots(const std::list<ItemData*>& list) //
{
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
	for (ItemData* item : items)
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item);
	}
}

void Inventory::UpdateSubSlots()
{
	//작은 인벤토리 슬롯 먼저 비우고
	for (auto& smallslot : smallslots)
	{
		smallslot->SetEmpty();
	}

	// 작은 인벤토리 슬롯에 아이템을 할당합니다.
	for (ItemData* item : items)
	{
		int indexX = item->IndexX;
		int indexY = item->IndexY;

		if (indexY == subIndexY)
		{
			smallslots[indexX]->SetItem(item);
		}
	}
}

void Inventory::SwapItem(int firstClickIndex, int secondClixkIndex)
{
	int fx = firstClickIndex % countX;
	int fy = firstClickIndex / countX;
	int sx = secondClixkIndex % countX;
	int sy = secondClixkIndex / countX;

	//첫번째 아이템 찾기 | 람다식
	//elem 리스트의 각요소
	//찾고 싶은 아이템의 열과 행을 나타냄
	auto findFirst = std::find_if(items.begin(), items.end(), [fx, fy](ItemData* elem)
	{
	return elem->IndexX == fx && elem->IndexY == fy;
	});

	//두번째 아이템 찾기 | 람다식
	//elem 리스트의 각요소
	//찾고 싶은 아이템의 열과 행을 나타냄
	auto findSecond = std::find_if(items.begin(), items.end(), [sx, sy](ItemData* elem)
	{
	return elem->IndexX == sx && elem->IndexY == sy;
	});

	// 두번째 인덱스가 빈공간일 경우 첫번째 아이템과 빈공간 클릭시 교환
	//첫번째 아이템이 존재하고 두번째 아이템이 없을 경우
	if (findFirst != items.end() && findSecond == items.end())
	{
		(*findFirst)->IndexX = sx;
		(*findFirst)->IndexY = sy;
		//전의 인덱스 위치를 초기화 해야하나?
	}

	// 그냥 아이템끼리 교환
	else if (findFirst != items.end() && findSecond != items.end())
	{
		int tempX = (*findFirst)->IndexX;
		int tempY = (*findFirst)->IndexY;

		(*findFirst)->IndexX = (*findSecond)->IndexX;
		(*findFirst)->IndexY = (*findSecond)->IndexY;

		(*findSecond)->IndexX = tempX;
		(*findSecond)->IndexY = tempY;
	}

	UpdateSlots(items); // 슬롯 상태 업데이트
	UpdateSubSlots();
}
