#include "pch.h"
#include "Inventory.h"
#include "InvetorySlot.h"

Inventory::Inventory(const std::string& name) : SpriteGo(name)
{
}

void Inventory::Init()
{
	SpriteGo::Init();

	//�κ��丮 â ����
	SetTexture("graphics/Ui.png");
	smallUi.SetTexture("graphics/smallUi.png");
	smallUi.SetPosition({970.f, 960.f});
	smallUi.SetOrigin(Origins::MC);

	//sf::Vector2f smallUiPosition = smallUi.GetPosition();

	Release();

	//IŰ �������� ������ ���� �κ��丮 â�� �׾ȿ� �� ���� ����� 3*10
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//���� ��ġ ���
			InvetorySlot* slot = new InvetorySlot("Inventory Slot");
			sf::Vector2f pos = position;
			pos += { (float)j * 60.f - 310.f, (float)i * 60.f - 300.f }; //�ϵ��ڵ�
			slot->SetPosition(pos);
			slot->SetOrigin(Origins::MC);
			slot->Init();
			slots.push_back(slot);
		}
	}

	//���� ȭ�� �ϴܿ� ������ ���� �κ��丮�� ���� �����
	for (int j = 0; j < countX; j++)
	{
		//���� ��ġ ���
		InvetorySlot* smallslot = new InvetorySlot("Inventory Slot");
		sf::Vector2f pos = smallUi.GetPosition();
		smallslot->SetPosition(pos - sf::Vector2f(350.f - 70.f * j, 35.f)); //�ϵ��ڵ�
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

	//�κ��丮�� �Ⱥ����ٸ� IŰ�� �������� ���̰� �ϱ�
	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		isAble = !isAble;
	}

	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);
	int clickSlotIndex = -1;

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
		auto findFirst = std::find_if(items.begin(), items.end(), [fx, fy](ItemData* elem)
			{
				return elem->IndexX == fx && elem->IndexY == fy;
			});

		//�������� ������ Ŭ���� ���� �ε����� firstClickIndex�� �Ҵ��մϴ�.
		//ù��°�� ������ ������
		if (findFirst != items.end())
		{
			firstClickIndex = clickSlotIndex;
		}
	}

	if (clickSlotIndex == -1) //�� ������ ������ ��� ���õ� �ε����� �ʱ�ȭ�Ͽ� ������ ���� �ʱ�ȭ
	{
		firstClickIndex = -1;
	}

	// tab�� ������ ���� �κ��丮 ����
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
	//���� ȭ�鿡 ���� �κ��丮�� ���� �׷��ֱ�
	smallUi.Draw(window);
	for (auto smallslot : smallslots)
	{
		smallslot->Draw(window);
	}

	//IŰ �������� ���� �κ��丮, ���� �׷��ֱ�
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
	//�ε��� �����ŭ ��ȸ
	for (int i = 0; i < doc["ItemData"].Size(); ++i)
	{
		//rapidjson::Document�� ��ü doc�� �̿��ؼ� �迭ItemData�� i��° ������ �д´�.
		//���� ������ itemData�� �Ҵ��Ѵ�.
		//[i * countX + j] Ư�� ĭ ã��
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
	for (ItemData* item : items)
	{
		int index = item->IndexY * countX + item->IndexX;
		SetIvenSlot(item->IndexX, item->IndexY, item);
	}
}

void Inventory::UpdateSubSlots()
{
	//���� �κ��丮 ���� ���� ����
	for (auto& smallslot : smallslots)
	{
		smallslot->SetEmpty();
	}

	// ���� �κ��丮 ���Կ� �������� �Ҵ��մϴ�.
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

	//ù��° ������ ã�� | ���ٽ�
	//elem ����Ʈ�� �����
	//ã�� ���� �������� ���� ���� ��Ÿ��
	auto findFirst = std::find_if(items.begin(), items.end(), [fx, fy](ItemData* elem)
	{
	return elem->IndexX == fx && elem->IndexY == fy;
	});

	//�ι�° ������ ã�� | ���ٽ�
	//elem ����Ʈ�� �����
	//ã�� ���� �������� ���� ���� ��Ÿ��
	auto findSecond = std::find_if(items.begin(), items.end(), [sx, sy](ItemData* elem)
	{
	return elem->IndexX == sx && elem->IndexY == sy;
	});

	// �ι�° �ε����� ������� ��� ù��° �����۰� ����� Ŭ���� ��ȯ
	//ù��° �������� �����ϰ� �ι�° �������� ���� ���
	if (findFirst != items.end() && findSecond == items.end())
	{
		(*findFirst)->IndexX = sx;
		(*findFirst)->IndexY = sy;
		//���� �ε��� ��ġ�� �ʱ�ȭ �ؾ��ϳ�?
	}

	// �׳� �����۳��� ��ȯ
	else if (findFirst != items.end() && findSecond != items.end())
	{
		int tempX = (*findFirst)->IndexX;
		int tempY = (*findFirst)->IndexY;

		(*findFirst)->IndexX = (*findSecond)->IndexX;
		(*findFirst)->IndexY = (*findSecond)->IndexY;

		(*findSecond)->IndexX = tempX;
		(*findSecond)->IndexY = tempY;
	}

	UpdateSlots(items); // ���� ���� ������Ʈ
	UpdateSubSlots();
}
