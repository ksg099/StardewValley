#include "pch.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Player.h"
#include "SpriteGo.h"
#include "Inventory.h"
#include "SubInventory.h"
#include "BoxInven.h"
#include "UiStore.h"

#include "TestMapTool.h"
#include "ChangeHarvest.h"

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

	subInven = new SubInventory("SubInventory");
	subInven->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(subInven, Ui);

	boxInven = new BoxInven("Inventory");
	boxInven->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(boxInven, Ui);

	dailyTime = 2.f;

	overlay = new SpriteGo("layer");
	overlay->SetTexture("graphics/layout.png");
	overlay->SetOrigin(Origins::TL);
	overlay->SetPosition({ -(float)FRAMEWORK.GetWindowSize().x * 0.5f,-(float)FRAMEWORK.GetWindowSize().y * 0.5f });
	overlay->SetFillColor(dayColor);
	overlay->sortLayer = 5;
	AddGo(overlay, Layers::World);

	uiStore = new UiStore("UI STORE");
	AddGo(uiStore, Layers::Ui);

	sf::Vector2f mapSize = { tileMap->GetCellCount().x * tileMap->GetCellSize().x, tileMap->GetCellCount().y * tileMap->GetCellSize().y };
	sf::Vector2i windowSize = FRAMEWORK.GetWindowSize();
	float scaleX = windowSize.x / mapSize.x;
	float scaleY = windowSize.y / mapSize.y;
	float scale = std::min(scaleX, scaleY);




	Scene::Init();

	float zoom = FRAMEWORK.GetWindowSize().y / (20.f * 20.f);
	
	worldView.zoom(1/ zoom);
	worldView.setCenter(player->GetPosition());
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
	uiStore->SetActive(false);
	subInven->SetActive(true);
	isUiOpen = false;

	isNextDay = false;

	Scene::Enter();
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
	isNextDay = false;
	worldView.setCenter(Utils::Clamp(player->GetPosition(), {-300.f,-300.f,1000.f,1000.f})); //??????????????

	// save the file
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		rapidjson::Document saveDoc;
		saveDoc.SetObject();
		TILEMAP_SAVE->Save(saveDoc);
		ITEM_SAVE->Save(saveDoc);
		Utils::EditFile(DT_MGR.GetGameSaveSelect(), saveDoc);
	}

	// �ð� ������Ʈ
	dailyTime += dt;

	// �Ϸ簡 ������ day�� ������Ű��, �Ĺ����� ���¸� �ʱ�ȭ

	// inventory active/inactive
	SetInventory();

	if (dailyTime >= 24)
	{
		dailyTime -= 24; // ���� ���� �Ѿ
		isNextDay = true;
		++day;

		SellAllItemsInBox();
		std::cout << "Day : " << day << std::endl;
	}

	if (dailyTime >= 6 && dailyTime < 16 && targetColor != dayColor)
	{
		targetColor = dayColor;
		currentColor = overlay->GetFillColor();
		progress = 0.f;
	}
	else if (dailyTime >= 16 && dailyTime < 20 && targetColor != eveningColor)
	{
		targetColor = eveningColor;
		currentColor = overlay->GetFillColor();
		progress = 0.f;
	}
	else if (dailyTime >= 20 || dailyTime < 6 && targetColor != nightColor)
	{
		targetColor = nightColor;
		currentColor = overlay->GetFillColor();
		progress = 0.f;
	}

	if (progress < 7.f)
	{
		progress += dt / transitionDuration;
		if (progress > 5.f)
		{
			progress = 5.f;
		}
		overlay->SetFillColor(LerpColor(overlay->GetFillColor(), targetColor, progress));
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
}

void SceneGame::Draw(sf::RenderWindow& window, Layers layer)
{
	Scene::Draw(window, World);

	tileMap->Draw(window);
	for (auto& item : dropItemList)
	{
		window.draw(item->itemSprite);
	}
	overlay->Draw(window);

	Scene::Draw(window, Ui);
}

void SceneGame::CreateItem(DataItem data, int indexX, int indexY)
{
	DropItem* item = new DropItem();
	item->itemID = data.itemId;
	item->itemType = data.itemType;
	item->itemSprite.setTexture(RES_MGR_TEXTURE.Get(ITEM_TABLE->Get(item->itemType, item->itemID).textureId));
	item->itemSprite.setTextureRect({ data.sheetId.x, data.sheetId.y, data.sheetSize.x, data.sheetSize.y });
	int scale = data.sheetSize.x < data.sheetSize.y ? data.sheetSize.x : data.sheetSize.y;
	item->itemSprite.setScale({ tileMap->GetCellSize().x / scale, tileMap->GetCellSize().y / scale });
	Utils::SetOrigin(item->itemSprite, Origins::MC);
	sf::Vector2f pos = tileMap->GetGridPosition(indexX, indexY);
	item->itemSprite.setPosition(Utils::RandomRange(pos.x + 30.f, pos.x),Utils::RandomRange(pos.y + 30.f, pos.y));
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

	subInven->UpdateSlots();
}


void SceneGame::SetInventory()
{
	if (InputMgr::GetKeyDown(sf::Keyboard::I))
	{
		if (!isUiOpen)
		{
			inventory->SetActive(true);
			inventory->UpdateSlots();
			isUiOpen = true;
		}
		else if (inventory->GetActive())
		{
			inventory->SetActive(false);
			isUiOpen = false;
		}
	}

	/*if (InputMgr::GetKeyDown(sf::Keyboard::U))
	{
		if (!isUiOpen)
		{
			boxInven->SetActive(true);
			boxInven->UpdateSlots();
			isUiOpen = true;
		}
		else if (boxInven->GetActive())
		{
			boxInven->SetActive(false);
			isUiOpen = false;
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::P))
	{
		if (!isUiOpen)
		{
			uiStore->SetActive(true);
			isUiOpen = true;
		}
		else if (uiStore->GetActive())
		{
			uiStore->SetActive(false);
			isUiOpen = false;
		}
	}*/

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape) && isUiOpen)
	{
		inventory->SetActive(false);
		boxInven->SetActive(false);
		uiStore->SetActive(false);

		isUiOpen = false;
	}

	if (isUiOpen)
	{
		subInven->SetActive(false);
	}
	else
	{
		subInven->SetActive(true);
		subInven->UpdateSlots();
	}
}

void SceneGame::OpenBox(int boxId)
{
	if (!isUiOpen)
	{
		boxInven->SetActive(true);
		isUiOpen = true;
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

void SceneGame::UpdateInvenSlot()
{
	inventory->UpdateSlots();
	subInven->UpdateSlots();
}

void SceneGame::InteractPlayerWithTileMap(const int x, const int y, const ItemType type, const int id)
{
	tileMap->InteractWithTile(x, y, type, id);
}

void SceneGame::OpenStoreUi()
{
	if (!isUiOpen)
	{
		uiStore->SetActive(true);
		isUiOpen = true;
	}
}
