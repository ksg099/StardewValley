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
	//invenLine.SetTexture("graphics/invenBoxLine.png");

	Release();
	for (int i = 0; i < countY; i++)
	{
		for (int j = 0; j < countX; j++)
		{
			//���� ��ġ ���
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

	//�ΰ� ������ ����� ��ġ�� ������ ���� ��ȯ
	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);
	int clickSlotIndex = -1;

	if (InputMgr::GetMouseButton(sf::Mouse::Button::Left))
	{
		//���Կ� ����� �����ŭ ��ȸ�� �ϰ� clickSlotIndex�� �Ҵ��Ѵ�.
		// �� ���� ���콺�� ��ư�� ������ �ִ� ���� ã��
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

	if (InputMgr::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (firstClickIndex != -1) //�̹� ù��° �ε����� ���õǾ� ������
		{
			//���Կ� ����� �����ŭ ��ȸ�� �ϰ� clickSlotIndex�� �Ҵ��Ѵ�.
			// �� ���콺 ��ư�� ���� ���� ã��
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

	if (clickSlotIndex == -1) //�� ������ ������ ��� ����
	{
		firstClickIndex = -1;
	}


	if (firstClickIndex == -1)
	{
		int fx = clickSlotIndex % countX;
		int fy = clickSlotIndex / countX;
		auto findFirst = std::find_if(items.begin(), items.end(), [fx, fy](ItemData* elem)
			{
				return elem->IndexX == fx && elem->IndexY == fy;
			});

		if (findFirst != items.end())
		{
			firstClickIndex = clickSlotIndex;
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
}
