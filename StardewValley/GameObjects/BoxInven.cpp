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

	//UŰ �������� ������ ���� �κ��丮 â�� �׾ȿ� �� ���� �����
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//���� ��ġ ���
			InvetorySlot* secondSlot = new InvetorySlot("Inventory Slot");
			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 310.f, (float)i * 60.f - 50.f }; //�ϵ��ڵ�
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
	Inventory::SetIvenSlot(x, y, data);
}

void BoxInven::UpdateSlots()
{
	Inventory::UpdateSlots();

}

void BoxInven::UpdateSubSlots()
{
	Inventory::UpdateSubSlots();
}

void BoxInven::Draw(sf::RenderWindow& window)
{

	// U Ű �Է¿� ���� Ȱ��ȭ�� �߰� �κ��丮 (BoxInven)
	if (!SecondisAble)
	{
		Inventory::Draw(window);;

		for (auto slot : slots)
		{
			slot->Draw(window);
		}
		itemInfoText.Draw(window);

		// BoxInven �κ��丮 �׸��� ����...
		for (auto& secondSlot : secondSlots)
		{
			secondSlot->Draw(window);
		}
	}
}