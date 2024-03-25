#include "pch.h"
#include "BoxInven.h"
#include "InvetorySlot.h"

BoxInven::BoxInven(const std::string& name) : Inventory(name)
{
}

void BoxInven::Init()
{
	Inventory::Init();

	secondItems = ITEM_SAVE->Get(boxId);
	//SetTexture("graphics/Ui.png");

	//U키 눌렀을때 나오는 메인 인벤토리 창과 그안에 들어갈 슬롯 만들기
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//슬롯 위치 잡기
			InvetorySlot* secondSlot = new InvetorySlot("Inventory Slot");
			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 310.f, (float)i * 60.f - 50.f }; //하드코딩
			secondSlot->SetPosition(pos);
			secondSlot->SetOrigin(Origins::MC);
			secondSlot->Init();
			secondSlots.push_back(secondSlot);
		}}
}

void BoxInven::Release()
{

	Inventory::Release();

	for (auto secondSlot : secondSlots)
	{
		if (secondSlot != nullptr)
		{
			delete secondSlot;
			secondSlot = nullptr;
		}
	}
	secondSlots.clear();
}

void BoxInven::Reset()
{
	Inventory::Reset();
}

void BoxInven::Update(float dt)
{
	Inventory::Update(dt);
	if (InputMgr::GetKeyDown(sf::Keyboard::U))
	{
		SecondisAble = !SecondisAble;
	}
}

void BoxInven::SwapItem(int firstClickIndex, int secondClickIndex)
{
	Inventory::SwapItem(firstClickIndex, secondClickIndex);
	//items->boxId

	//	if (findFirst != items->end() && findSecond != items->end())
	//	{
	//		int tempBoxId = (*findFirst)->BoxId;


	//		(*findFirst)->BoxId = (*findSecond)->BoxId;

	//		(*findSecond)->BoxId = tempBoxId;
	//}

}

void BoxInven::DisplayItemInfo(ItemData& itemData, sf::Vector2f& position)
{
	Inventory::DisplayItemInfo(itemData, position);
}

void BoxInven::SetPosition(const sf::Vector2f& pos)
{
	Inventory::SetPosition(pos);
}

void BoxInven::SetIvenSlot(int x, int y, ItemData* data)
{
	// Inventory::SetIvenSlot(x, y, data);
	int index = y * countX + x;
	slots[index]->SetItem(data);
	secondSlots[index]->SetItem(data);
}

void BoxInven::UpdateSlots()
{
	Inventory::UpdateSlots();

	for (auto& secondSlot : smallslots)
	{
		secondSlot->SetEmpty();
	}

	//메인 인벤토리에 아이템 할당
	for (ItemData* secondSlot : *items)
	{
		int index = secondSlot->IndexY * countX + secondSlot->IndexX;
		SetIvenSlot(secondSlot->IndexX, secondSlot->IndexY, secondSlot);
	}

}

void BoxInven::UpdateSubSlots()
{
	Inventory::UpdateSubSlots();
}

void BoxInven::Draw(sf::RenderWindow& window)
{

	// U 키 입력에 의해 활성화된 추가 인벤토리 (BoxInven)
	if (!SecondisAble)
	{
		Inventory::Draw(window);;

		for (auto slot : slots)
		{
			slot->Draw(window);
		}
		itemInfoText.Draw(window);

		// BoxInven 인벤토리 그리기 로직...
		for (auto& secondSlot : secondSlots)
		{
			secondSlot->Draw(window);
		}
	}
}