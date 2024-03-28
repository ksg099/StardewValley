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

	//���� ȭ�� �ϴܿ� ������ ���� �κ��丮�� ���� �����
	for (int j = 0; j < countX; j++)
	{
		//���� ��ġ ���
		InvetorySlot* smallslot = new InvetorySlot("Inventory Slot");
		sf::Vector2f pos = smallUi.GetPosition();
		smallslot->Init();

		smallslot->SetPosition(pos - sf::Vector2f(320.f - 70.f * j, 0.f)); //�ϵ��ڵ�
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

	// tab�� ������ ���� �κ��丮 ����
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
	//���� �κ��丮 ���� ���� ����
	for (auto& smallslot : smallslots)
	{
		smallslot->SetEmpty();
	}

	// ���� �κ��丮 ���Կ� �������� �Ҵ��մϴ�.
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
