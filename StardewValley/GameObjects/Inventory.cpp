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

	//�κ��丮 â ����
	SetTexture("graphics/Ui.png");

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
			
			slots.push_back(slot);
		}
	}

	itemInfoText.Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), "", 20, sf::Color::Black);
	itemInfoText.SetOutline(sf::Color::White, 1.f);
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
			slots[index]->SetIsDraw(true);
		}
	}
	
	UpdateSlots();
}

void Inventory::Update(float dt)
{
	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);
	int clickSlotIndex = -1;
	if (firstClickIndex != -1 && !isClick)
	{
		ItemData* itemData = GetItemData(firstClickIndex % countX, firstClickIndex / countX);
		if (itemData != nullptr)
		{
			auto item = ITEM_TABLE->Get(itemData->type, itemData->itemId);
			firstClickSprite.setTexture(RES_MGR_TEXTURE.Get(item.textureId));
			firstClickSprite.setTextureRect(sf::IntRect(item.sheetId.x, item.sheetId.y, item.sheetSize.x, item.sheetSize.y));
			float scale = item.sheetSize.x > item.sheetSize.y ? item.sheetSize.x : item.sheetSize.y;
			firstClickSprite.setScale(sf::Vector2f(50.f / scale, 50.f / scale));
			Utils::SetOrigin(firstClickSprite, Origins::MC);
			isClick = true;

			slots[firstClickIndex]->SetIsDraw(false);
		}
	}
	else if (firstClickIndex == -1 && isClick)
	{
		isClick = false;
		for (auto slot : slots)
		{
			slot->SetIsDraw(true);
		}
	}
	firstClickSprite.setPosition(uiPos);

	//���콺 ���� ��ư�� ���� ������ ���콺 ��ġ�� �ش��ϴ� ������
	//��ȸ�� ���� ã�� clickSlotIndex�� �Ҵ�
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
			if (firstClickIndex != -1) // �̹� ���õ� �������� ������
			{
				int fx = firstClickIndex % countX;
				int fy = firstClickIndex / countX;

				auto it = std::find_if(items->begin(), items->end(), [fx, fy](ItemData* elem) {
					return elem->IndexX == fx && elem->IndexY == fy;
					});

				if (it != items->end())
				{
					// ���õ� �������� items ����Ʈ���� ����
					delete *it;
					items->erase(it);

					// �κ��丮 UI ����
					UpdateSlots();

					// ���� �ʱ�ȭ
					firstClickIndex = -1;
				}
			}
		}
	}

	//���콺 ���� ��ư�� �������� ù��° �������� ���õ� ���¶��
	//���콺�� �������� �ش��ϴ� ������ �ٽ� ��ȸ�� ���� ã�� ����ġ�� �������� SwapItem�Ѵ�.
	if (InputMgr::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (firstClickIndex != -1) //�̹� ù��° �ε����� ���õǾ� ������
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

	//ù��° Ŭ�� �� �������� �ε����� ���� �ƹ��͵� ������ �ȵ� ���
	//���� ���ǵ��� ���
	if (clickSlotIndex != -1 && firstClickIndex == -1)
	{
		int fx = clickSlotIndex % countX;
		int fy = clickSlotIndex / countX;
		auto findFirst = std::find_if(items->begin(), items->end(), [fx, fy](ItemData* elem)
			{
				return elem->IndexX == fx && elem->IndexY == fy;
			});

		//�������� ������ Ŭ���� ���� �ε����� firstClickIndex�� �Ҵ��մϴ�.
		//ù��°�� ������ ������
		if (findFirst != items->end())
		{
			firstClickIndex = clickSlotIndex;
		}
	}
	itemInfoText.SetPosition(uiPos);
	//���콺 Ŀ�� ��ġ�� �ִ� ���� ������ ���� ǥ��
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

	if (clickSlotIndex == -1) //�� ������ ������ ��� ���õ� �ε����� �ʱ�ȭ�Ͽ� ������ ���� �ʱ�ȭ
	{
		firstClickIndex = -1;
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
	//���� �κ��丮���� �Ҵ��ϱ����� ����
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			int index = i * countX + j; //
			slots[index]->SetEmpty();
		}
	}

	//���� �κ��丮�� ������ �Ҵ�
	for (ItemData* item : *items)
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item);
	}
	
	//
	//for (auto& currentItem : *items) {
	//	bool placed = false;

	//	// �̹� �Ҵ�� ���Կ��� ������ id�� ���� �������� ã���ϴ�.
	//	for (int i = 0; i < slots.size() && !placed; i++)
	//	{
	//		if (item->IndexX == indexX && item->IndexY == indexY && item != nullptr && item == boxId)
	//		{
	//			//ã���� ������ �Ҵ��ϰ� �Ҵ�� ��Ҷ�� ����
	//			slots[i]->item->count += currentItem->count;
	//			placed = true;
	//		}
	//	}

	//	// ���� �������� ��ġ���� �ʾҴٸ�, ���Կ� �����Ͽ� �������� �Ҵ�
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

//������ �˻� �� ���� ����
//void Inventory::AddItem(ItemData* currentItem)
//{
//	//for (auto& item : *items) {
//	//	if (item->type == currentItem->type && item->itemId == currentItem->itemId) {
//	//		// �̹� �����ϴ� �������̸� ������ ������Ŵ
//	//		item->count += currentItem->count;
//	//		UpdateSlots(); // UI ����
//	//		return;
//	//	}
//	//}
//
//	//// �� �������̸� ���� �� ���Կ� �߰�
//
//
//	//UpdateSlots(); // UI ����
//}

void Inventory::SwapItem(int firstClickIndex, int secondClickIndex)
{

	int fx = firstClickIndex % countX; //����
	int fy = firstClickIndex / countX; //����

	int sx = secondClickIndex % countX; //����
	int sy = secondClickIndex / countX; //����

	//ù��° ������ ã�� | ���ٽ�
	//elem ����Ʈ�� �����
	//ã�� ���� �������� ���� ���� ��Ÿ��
	auto findFirst = std::find_if(items->begin(), items->end(), [fx, fy](ItemData* elem)
	{
	return elem->IndexX == fx && elem->IndexY == fy;
	});

	//�ι�° ������ ã�� | ���ٽ�
	//elem ����Ʈ�� �����
	//ã�� ���� �������� ���� ���� ��Ÿ��
	auto findSecond = std::find_if(items->begin(), items->end(), [sx, sy](ItemData* elem)
	{
	return elem->IndexX == sx && elem->IndexY == sy;
	});

	// �ι�° �ε����� ������� ��� ù��° �����۰� ����� Ŭ���� ��ȯ
	//ù��° �������� �����ϰ� �ι�° �������� ���� ���
	if (findFirst != items->end() && findSecond == items->end())
	{
		(*findFirst)->IndexX = sx;
		(*findFirst)->IndexY = sy;
		//(*findFirst)->BoxId = secondBoxId;
	}

	// �׳� �����۳��� ��ȯ
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

	UpdateSlots(); // ���� ���� ������Ʈ
}

void Inventory::DisplayItemInfo(ItemData& itemData, sf::Vector2f& position)
{
	//itemData.typeŸ���� int���� �ƴ϶� ����ȯ��
	//�������� type�� id�� ���
	//�ε��� �κ� ������ + "IndexX "+ std::to_string(itemData.IndexX) + "IndexY" + std::to_string(itemData.IndexY)
	std::string info = "Box: " + std::to_string(itemData.BoxId) + ", X: " + std::to_string(itemData.IndexX) + ", Y: " + std::to_string(itemData.IndexY)
		+ ",\nType: " + std::to_string((int)itemData.type) + ", ID: " + std::to_string(itemData.itemId)
		+ ",\nName: " + ITEM_TABLE->Get(itemData.type, itemData.itemId).name
		+ "\nSelling Price: " + std::to_string(ITEM_TABLE->Get(itemData.type, itemData.itemId).sellingPrice) + ", Purchase Price: " + std::to_string(ITEM_TABLE->Get(itemData.type, itemData.itemId).purchasePrice);
	itemInfoText.SetString(info);
}

void Inventory::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	for (auto slot : slots)
	{
		slot->Draw(window);
	}
	itemInfoText.Draw(window);

	if (isClick)
	{
		window.draw(firstClickSprite);
	}
}

ItemData* Inventory::GetItemData(const int x, const int y) const
{
	if (x < 0 || y < 0 || x >= countX || y >= countY)
		return nullptr;

	return slots[y * countX + x]->GetItemData();
	
}