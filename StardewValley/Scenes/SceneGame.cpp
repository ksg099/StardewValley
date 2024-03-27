#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"

#include "SpriteGo.h"
#include "Inventory.h"

#include "BoxInven.h"

#include "TestMapTool.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	tileMap = new TileMap("Background");
	tileMap->sortLayer = -1;
	AddGo(tileMap);

	player = new Player("Player");
	player->sortLayer = 1;
	AddGo(player);

	//�κ��丮 ��ġ ���
	inventory = new Inventory("Inventory");
	inventory->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(inventory, Ui);

	boxInven = new BoxInven("Inventory");
	boxInven->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(boxInven, Ui);

	dailyTime = 2.f;
	/*overlayer.setSize({ (float)FRAMEWORK.GetWindowSize().x, (float)FRAMEWORK.GetWindowSize().y });
	overlayer.setFillColor(dayColor);
	overlayer.setOrigin(0.f, 0.f);
	overlayer.setPosition(0.f, 0.f);*/

	layer = new SpriteGo("layer");
	layer->SetTexture("graphics/layout.png");
	layer->SetOrigin(Origins::TL);
	layer->SetPosition({ -(float)FRAMEWORK.GetWindowSize().x * 0.5f,-(float)FRAMEWORK.GetWindowSize().y * 0.5f });
	layer->SetFillColor(dayColor);
	layer->sortLayer = 5;
	AddGo(layer, Layers::World);

	Plant1 = new SpriteGo("plant");
	Plant1->SetTexture("graphics/Plant/rice1.png");
	Plant1->SetScale({ tileSize.x / Plant1->GetLocalBounds().width, tileSize.y / Plant1->GetLocalBounds().height });
	Plant1->SetOrigin(Origins::MC);
	AddGo(Plant1, Layers::World);
	Plant1->SetActive(false);

	Plant2 = new SpriteGo("plant");
	Plant2->SetTexture("graphics/Plant/rice2.png");
	Plant2->SetScale({ tileSize.x / Plant2->GetLocalBounds().width, tileSize.y / Plant2->GetLocalBounds().height });
	Plant2->SetOrigin(Origins::MC);
	AddGo(Plant2, Layers::World);
	Plant2->SetActive(false);

	Plant3 = new SpriteGo("plant");
	Plant3->SetTexture("graphics/Plant/rice3.png");
	Plant3->SetScale({ tileSize.x / Plant3->GetLocalBounds().width, tileSize.y / Plant3->GetLocalBounds().height });
	Plant3->SetOrigin(Origins::MC);
	AddGo(Plant3, Layers::World);
	Plant3->SetActive(false);

	Plant4 = new SpriteGo("plant");
	Plant4->SetTexture("graphics/Plant/rice4.png");
	Plant4->SetScale({ tileSize.x / Plant4->GetLocalBounds().width, tileSize.y / Plant4->GetLocalBounds().height });
	Plant4->SetOrigin(Origins::MC);
	AddGo(Plant4, Layers::World);
	Plant4->SetActive(false);

	Plant5 = new SpriteGo("plant");
	Plant5->SetTexture("graphics/Plant/rice5.png");
	Plant5->SetScale({ tileSize.x / Plant5->GetLocalBounds().width, tileSize.y / Plant5->GetLocalBounds().height });
	Plant5->SetOrigin(Origins::MC);
	AddGo(Plant5, Layers::World);
	Plant5->SetActive(false);

	Plant6 = new SpriteGo("plant");
	Plant6->SetTexture("graphics/Plant/rice6.png");
	Plant6->SetScale({ tileSize.x / Plant6->GetLocalBounds().width, tileSize.y / Plant6->GetLocalBounds().height });
	Plant6->SetOrigin(Origins::MC);
	Plant6->SetActive(false);
	AddGo(Plant6, Layers::World);

	Seed = new SpriteGo("plant");
	Seed->SetTexture("graphics/Seeds/Cauliflower_Seeds.png");
	//Seed->SetPosition(IndexToPos(2));
	Seed->SetOrigin(Origins::MC);
	Seed->SetActive(false);
	AddGo(Seed, Layers::World);

	//for (int i = 0; i < 6; i++)
	//{
	//	Plants = new SpriteGo("plant");
	//	Plants->SetTexture("graphics/Plant/rice6.png");
	//	Plants->SetPosition(IndexToPos(2));
	//	Plants->SetOrigin(Origins::MC);
	//	Plants->SetActive(false);
	//	AddGo(Plants, Layers::World);
	//	Plants->SetActive(false);
	//}

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	tileMap->LoadTileMap("Farm");
	tileMap->SetOrigin(Origins::MC);

	inventory->SetActive(false);
	boxInven->SetActive(false);

	Scene::Enter();

	//sf::Vector2f playerPosition = player->GetPosition();

	//Plant1->SetPosition(playerPosition);
	//Plant2->SetPosition(playerPosition);
	//Plant3->SetPosition(playerPosition);
	//Plant4->SetPosition(playerPosition);
	//Plant5->SetPosition(playerPosition);
	//Plant6->SetPosition(playerPosition);
}

void SceneGame::Exit()
{
	Scene::Exit();
}

sf::Color SceneGame::LerpColor(const sf::Color& start, const sf::Color& end, float t)
{
	return sf::Color(
		start.r + (end.r - start.r) * t,
		start.g + (end.g - start.g) * t,
		start.b + (end.b - start.b) * t,
		start.a + (end.a - start.a) * t);
}

sf::Vector2f SceneGame::IndexToPos(int index)
{
	const sf::Vector2f& size = tileMap->GetCellSize();
	col = tileMap->GetCellSize().y;

	int y = index / col;
	int x = index % col;
	return sf::Vector2f((x * size.x + size.x / 2) + gridStartX, (y * size.y + size.y / 2) + gridStartY);
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	// save the file
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		rapidjson::Document saveDoc;
		saveDoc.SetObject();
		TILEMAP_SAVE->Save(saveDoc);
		ITEM_SAVE->Save(saveDoc);
		Utils::EditFile(DT_MGR.GetGameSaveSelect(), saveDoc);
	}
	Scene::Update(dt);

	// �ð� ������Ʈ
	dailyTime += dt;

	// �Ϸ簡 ������ day�� ������Ű��, �Ĺ����� ���¸� �ʱ�ȭ

	// inventory active/inactive
	SetInventory();

	if (dailyTime >= 24)
	{
		dailyTime -= 24; // ���� ���� �Ѿ
		++day;
		std::cout << "Day : " << day << std::endl;

		SellAllItemsInBox();
	}

	// ZŰ �Է��� �����Ͽ� �Ĺ� ��ġ ����
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		sf::Vector2f playerPosition = player->GetPosition();
		// ù ��° �Ĺ��� ��ġ�� �÷��̾��� ���� ��ġ�� ����
		Plant1->SetPosition(playerPosition);
		Plant2->SetPosition(playerPosition);
		Plant3->SetPosition(playerPosition);
		Plant4->SetPosition(playerPosition);
		Plant5->SetPosition(playerPosition);
		Plant6->SetPosition(playerPosition);
	}

	// day�� ���� Ư�� �Ĺ� Ȱ��ȭ
	switch (day)
	{
	case 1:
		Plant1->SetActive(true);
		break;
	case 2:
		Plant2->SetActive(true);
		break;
	case 3:
		Plant3->SetActive(true);
		break;
	case 4:
		Plant4->SetActive(true);
		break;
	case 5:
		Plant5->SetActive(true);
		break;
	case 6:
		Plant6->SetActive(true);
		break;
	}

	//dailyTime += (dt);
	//if (dailyTime >= 24)
	//{
	//	dailyTime = 0.f;
	//	++day;
	//	std::cout << "Day : " << day << std::endl;
	//}

	if (dailyTime >= 6 && dailyTime < 16 && targetColor != dayColor)
	{
		targetColor = dayColor;
		currentColor = layer->GetFillColor();
		progress = 0.f;
	}
	else if (dailyTime >= 16 && dailyTime < 20 && targetColor != eveningColor)
	{
		targetColor = eveningColor;
		currentColor = layer->GetFillColor();
		progress = 0.f;
	}
	else if(dailyTime >= 20 || dailyTime < 6 && targetColor != nightColor)
	{
		targetColor = nightColor;
		currentColor = layer->GetFillColor();
		progress = 0.f;
	}

	if (progress < 7.f)
	{
		progress += dt / transitionDuration;
		if (progress > 5.f)
		{
			progress = 5.f;
		}
		layer->SetFillColor(LerpColor(layer->GetFillColor(), targetColor, progress));
	}
	if (!dropItemList.empty())
	{
		for (auto& item : dropItemList)
		{
			if (item != nullptr)
			{
				sf::Vector2f itemCurrentPos = item->itemSprite.getPosition();
				float distanceToPlayer = Utils::Distance(itemCurrentPos, player->GetPosition());
				float maxSpeed = player->GetSpeed() + 50.f;
				float minSpeed = player->GetSpeed() - 200.f;

				float t = 1.f - (distanceToPlayer / 200.f);
				float currentSpeed = Utils::Lerp(minSpeed, maxSpeed, t);

				sf::Vector2f itemDirection = player->GetPosition() - itemCurrentPos;
				Utils::Normalize(itemDirection);

				sf::Vector2f itemMovePos = itemCurrentPos + itemDirection * currentSpeed * dt;

				item->itemSprite.setPosition(itemMovePos);
			}
		}
	}

	auto it = dropItemList.begin();
	while (it != dropItemList.end())
	{
		auto* item = *it;
		if (item != nullptr)
		{
			if (Utils::Distance(item->itemSprite.getPosition(), player->GetPosition()) <= 5.f)
			{
				delete item;
				it = dropItemList.erase(it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			++it;
		}
	}

	//SpriteGo* plants[6] = { Plant1, Plant2, Plant3, Plant4, Plant5, Plant6 };
	//
	//for (int i = 0; i < 6; i++)
	//{
	//	if (day >= i + 1)
	//	{
	//		plants[i]->SetActive(true);
	//	}
	//	else
	//		plants[i]->SetActive(false);
	//}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	//{
	//	// �÷��̾��� ���� ��ġ�� ������
	//	sf::Vector2f playerPosition = player->GetPosition();

	//	// �Ĺ��� ��ġ�� ����������, ���� Ȱ��ȭ���� ����
	//	Plant1->SetPosition(playerPosition);
	//	Plant2->SetPosition(playerPosition);
	//	Plant3->SetPosition(playerPosition);
	//	Plant4->SetPosition(playerPosition);
	//	Plant5->SetPosition(playerPosition);
	//	Plant6->SetPosition(playerPosition);
	//}
	/*if (day == 1)
	//{
	//	Plant1->SetActive(true);
	//}

	//if (day == 2)
	//{
	//	Plant2->SetActive(true);
		Plant1->SetActive(false);
	}

	if (day == 3)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(true);
	}
	if (day == 4)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(false);
		Plant4->SetActive(true);
	}
	if (day == 5)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(false);
		Plant4->SetActive(false);
		Plant5->SetActive(true);
	}

	if (day == 6)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(false);
		Plant4->SetActive(false);
		Plant5->SetActive(false);
		Plant6->SetActive(true);
	}*/

}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	window.setView(worldView);
	for (auto& item : dropItemList)
	{
		window.draw(item->itemSprite);
	}
}

void SceneGame::CreateItem(DataItem data, int indexX, int indexY)
{
	DropItem* item = new DropItem();
	item->itemID = data.itemId;
	item->itemType = data.itemType;
	item->itemSprite.setTexture(RES_MGR_TEXTURE.Get(ITEM_TABLE->Get(item->itemType, item->itemID).textureId));
	item->itemSprite.setTextureRect({ data.sheetId.x, data.sheetId.y, data.sheetSize.x, data.sheetSize.y });
	Utils::SetOrigin(item->itemSprite, Origins::MC);
	item->itemSprite.setPosition(tileMap->GetGridPosition(indexX, indexY));
	std::cout << item->itemSprite.getPosition().x << std::endl;
	std::cout << item->itemSprite.getPosition().y << std::endl;
	std::cout << std::endl;
	std::cout << player->GetPosition().x << std::endl;
	std::cout << player->GetPosition().y << std::endl;
	item->count = 1;
	dropItemList.push_back(item);

	auto inven = ITEM_SAVE->Get(0);
	for (auto& invenItem : *inven)
	{
		if (invenItem->type == item->itemType && invenItem->itemId == item->itemID)
		{
			++invenItem->count;
			return;
		}
	}
	if (inven->size() <= 30)
	{
		ItemData* newItem = new ItemData();
		newItem->BoxId = 0;
		newItem->count = 1;
		int index = 0;
		for (int i = 0; i < 30; ++i)
		{
			bool isExist = true;
			for (auto& inV : *inven)
			{
				if (inV->IndexY * 10 + inV->IndexX == i)
				{
					isExist = false;
					break;
				}
			}
			if (isExist)
			{
				index = i;
				break;
			}
		}
		newItem->IndexX = index % 10;
		newItem->IndexY = index / 10;
		newItem->itemId = item->itemID;
		newItem->type = item->itemType;
		inven->push_back(newItem);
	}
}

void SceneGame::SetInventory()
{
	if (InputMgr::GetKeyDown(sf::Keyboard::I) && !boxInven->GetActive())
	{
		inventory->SetActive(!inventory->GetActive());
		if (inventory->GetActive())
		{
			inventory->UpdateSlots();
		}
	}

	//�κ��丮�� �Ⱥ����ٸ� IŰ�� �������� ���̰� �ϱ�
	if (InputMgr::GetKeyDown(sf::Keyboard::U) && !inventory->GetActive())
	{
		boxInven->SetActive(!boxInven->GetActive());
		if (boxInven->GetActive())
		{
			boxInven->UpdateSlots();
		}
	}
}

void SceneGame::SellAllItemsInBox()
{
	auto sellingBox = ITEM_SAVE->Get(sellingBoxId);
	int sellingPrice = 0;

	for (auto item : *sellingBox)
	{
		if (item != nullptr)
		{
			// ������ ���� ���
			sellingPrice += ITEM_TABLE->Get(item->type, item->itemId).sellingPrice * item->count;

			// ������ ����
			delete item;
			item = nullptr;
		}
	}
	sellingBox->clear();
}
