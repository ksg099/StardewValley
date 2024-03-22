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
	//invenLine.SetTexture("graphics/invenBoxLine.png");

	Release();
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//슬롯 위치 잡기
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 310.f, (float)i * 60.f - 300.f };
			slot->SetPosition(pos);
			slot->SetOrigin(Origins::MC);
			slot->Init();
			slots.push_back(slot);
		}
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


}

void Inventory::Update(float dt)
{
	SpriteGo::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		isAble = !isAble;
	}

	//두개 데이터 저장된 위치가 있으면 서로 전환
	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		int clickSlotIndex = -1;

		//슬롯에 저장된 사이즈만큼 순회를 하고 clickSlotIndex에 할당한다. 즉 클릭된 슬롯 찾기
		for (int i = 0; i < slots.size(); ++i)
		{
			sf::FloatRect slotBounds = slots[i]->GetGlobalBounds();
			if (slotBounds.contains(uiPos))
			{
				clickSlotIndex = i;
				break;
			}
		}
		
		if (clickSlotIndex == -1) //빈 공간이 눌렸을 경우 리셋
		{
			firstClickIndex = -1;
		}


		if (firstClickIndex == -1) //첫번째 인덱스를 클릭 했을 경우
		{
			int fx = clickSlotIndex % countX;
			int fy = clickSlotIndex / countX;
			auto findFirst = std::find_if(items.begin(), items.end(), [fx, fy](ItemData* elem) {
				return elem->IndexX == fx && elem->IndexY == fy;
				});
			if (findFirst != items.end())
			{
				firstClickIndex = clickSlotIndex;
			}
		}

		else //두번째 인덱스를 클릭 했을 경우
		{
			if (InputMgr::GetMouseButtonUp(sf::Mouse::Button::Left))
			{
				// 마우스 왼쪽 버튼이 방금 놓아졌을 때 실행할 코드
			}
			clickSlotIndex;
			SwapItem(firstClickIndex, clickSlotIndex);

			firstClickIndex = -1;
		}
	}
}

void Inventory::Draw(sf::RenderWindow& window)
{
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
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j; //
			slots[index]->SetEmpty();
		}
	}

	for (ItemData* item : items) //
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item);
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
	auto findFirst = std::find_if(items.begin(), items.end(), [fx, fy](ItemData* elem) {
		return elem->IndexX == fx && elem->IndexY == fy;
		});

	//두번째 아이템 찾기 | 람다식
	//elem 리스트의 각요소
	//찾고 싶은 아이템의 열과 행을 나타냄
	auto findSecond = std::find_if(items.begin(), items.end(), [sx, sy](ItemData* elem) {
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
}
