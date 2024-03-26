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

	//�κ��丮 â ����
	SetTexture("graphics/Ui.png");
	//smallUi.SetTexture("graphics/smallUi.png");
	//smallUi.SetPosition({ 970.f, 960.f });
	//smallUi.SetOrigin(Origins::MC);

	Release();

	//IŰ �������� ������ ���� �κ��丮 â�� �׾ȿ� �� ���� �����
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//���� ��ġ ���
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			slot->Init();
			slot->Reset();

			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 270.f, (float)i * 60.f - 250.f }; //�ϵ��ڵ�
			slot->SetPosition(pos);
			slot->SetOrigin(Origins::MC);

			firtstSlots.push_back(slot);
		}
	}

	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//���� ��ġ ���
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			slot->Init();
			slot->Reset();

			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 270.f, (float)i * 60.f + 100.f }; //�ϵ��ڵ�
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
	// �κ��丮 ����/�ݱ� ���
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
		//������ ��ȸ���Ƽ� ����
		for (int i = 0; i < firtstSlots.size(); ++i)
		{
			if (firtstSlots[i]->GetGlobalBounds().contains(uiPos))
			{
				//�϶� ù��° Ŭ������ ����
				if (firstclickSlotIndex == -1)
				{
					//Ŭ���� ������ �Ҵ��� ù��° �κ��� ������ Ȯ��
					firstclickSlotIndex = i;
					clickedInFirstInven = true;
				}

				//�϶� �ι�° Ŭ������ ����
				else if (secondclickSlotIndex == -1)
				{
					//Ŭ���� ������ �Ҵ��� �ι�° �κ��� ������ Ȯ��
					secondclickSlotIndex = i;
					clickedInSecondInven = true;
				}
				else //�ƹ��͵� Ŭ������ �ʾ�����
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
				//�϶� ù��° Ŭ������ ����
				if (firstclickSlotIndex == -1) // ù ��° Ŭ���̶��
				{
					firstclickSlotIndex = i;
					clickedInFirstInven = false;
				}

				//�϶� �ι�° Ŭ������ ����
				else if (secondclickSlotIndex == -1)
				{
					secondclickSlotIndex = i;
				}
				else //�ƹ��͵� Ŭ������ �ʾ�����
				{
					firstclickSlotIndex = -1;
					secondclickSlotIndex = -1;
					clickedInFirstInven = false;
					clickedInSecondInven = false;
				}
				break;
			}
		}



		// Ŭ���� ���� ó��
		if (firstclickSlotIndex != -1 && secondclickSlotIndex != -1)
		{
			bool firstItemInFirstInven = clickedInFirstInven;
			bool secondItemInFirstInven = clickedInSecondInven;

			// ù ��° Ŭ���� ���԰� �� ��° Ŭ���� ������ ���� �κ��丮�� ���ϴ� ���
			// �Ǵ� ���� �ٸ� �κ��丮�� ���ϴ� ��츦 ��� ó���� �� ����
			SwapItem(firstclickSlotIndex, secondclickSlotIndex, firstItemInFirstInven, secondItemInFirstInven);

			// Ŭ�� �ε��� �ʱ�ȭ
			firstclickSlotIndex = -1;
			secondclickSlotIndex = -1;
		}
	}
	itemInfoText.SetPosition(uiPos);
	//���콺 Ŀ�� ��ġ�� �ִ� ���� ������ ���� ǥ��
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

	if (clickedInFirstInven == -1) //�� ������ ������ ��� ���õ� �ε����� �ʱ�ȭ�Ͽ� ������ ���� �ʱ�ȭ
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
	//���� �κ��丮���� �Ҵ��ϱ����� ����
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j; //
			firtstSlots[index]->SetEmpty();
		}
	}

	//���� �κ��丮�� ������ �Ҵ�
	for (ItemData* item : *firstItems)
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item, firtstSlots);
	}

	//���� �κ��丮���� �Ҵ��ϱ����� ����
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j; //
			secondSlots[index]->SetEmpty();
		}
	}

	//���� �κ��丮�� ������ �Ҵ�
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

//������ ���� ����ϱ�
void BoxInven::DisplayItemInfo(ItemData& itemData, sf::Vector2f& position)
{
	//itemData.typeŸ���� int���� �ƴ϶� ����ȯ��
	//�������� type�� id�� ���
	//�ε��� �κ� ������ + "IndexX "+ std::to_string(itemData.IndexX) + "IndexY" + std::to_string(itemData.IndexY)
	std::string info = "Box: " + std::to_string(itemData.BoxId) + ", X: " + std::to_string(itemData.IndexX) + ", Y: " + std::to_string(itemData.IndexY)
		+ ",\nType: " + std::to_string((int)itemData.type) + ", ID: " + std::to_string(itemData.itemId)
		+ ",\nName: " + ITEM_TABLE->Get(itemData.type, itemData.itemId).name;
	itemInfoText.Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), info, 20, sf::Color::Black);
}

void BoxInven::Draw(sf::RenderWindow& window)
{

	//IŰ �������� ���� �κ��丮, ���� �׷��ֱ�
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