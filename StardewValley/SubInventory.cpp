#include "pch.h"
#include "SubInventory.h"
#include "InvetorySlot.h"

SubInventory::SubInventory(const std::string& name) : SpriteGo(name)
{
}

void SubInventory::Init()
{
	SpriteGo::Init();

	smallUi.SetTexture("graphics/smallUi.png");
	smallUi.SetPosition({ 970.f, 960.f });
	smallUi.SetOrigin(Origins::MC);

	Release();

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

	usingItemSlot.setFillColor(sf::Color::Transparent);
	usingItemSlot.setSize({ 65.f, 65.f });
	usingItemSlot.setOutlineThickness(3.f);
	usingItemSlot.setOutlineColor(sf::Color::Red);
	usingItemSlot.setPosition(smallslots[0]->GetPosition());
	Utils::SetOrigin(usingItemSlot, Origins::MC);
}

void SubInventory::Release()
{
	for (auto slot : smallslots)
	{
		if (slot != nullptr)
		{
			delete slot;
			slot = nullptr;
		}
	}
	smallslots.clear();

	SpriteGo::Release();
}

void SubInventory::Reset()
{
	SpriteGo::Reset();

	items = ITEM_SAVE->Get(inventoryBoxId);
	UpdateSlots();
}

void SubInventory::Update(float dt)
{
	SpriteGo::Update(dt);

	// tab을 누르면 서브 인벤토리 변경
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
	{
		subIndexY++;
		if (subIndexY >= countY)
		{
			subIndexY = 0;
		}
		UpdateSlots();
	}

	auto numKey = InputMgr::GetKeyNumber();
	if (numKey.first)
	{
		subIndexX = numKey.second - 1;
		if (subIndexX == -1)
			subIndexX = 9;

		usingItemSlot.setPosition(smallslots[subIndexX]->GetPosition());
	}
}

void SubInventory::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

	smallUi.Draw(window);
	for (auto smallslot : smallslots)
	{
		smallslot->Draw(window);
	}

	window.draw(usingItemSlot);
}

void SubInventory::UpdateSlots()
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
