#include "pch.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Player.h"
#include "SpriteGo.h"
#include "Inventory.h"
#include "BoxInven.h"
#include "UiStore.h"

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

	//인벤토리 위치 잡기
	inventory = new Inventory("Inventory");
	inventory->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(inventory, Ui);

	boxInven = new BoxInven("Inventory");
	boxInven->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(boxInven, Ui);

	dailyTime = 6.f;
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

	uiStore = new UiStore("UI STORE");
	AddGo(uiStore, Layers::Ui);

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
	uiStore->SetActive(false);

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

	// inventory active/inactive
	SetInventory();

	

	dailyTime += (dt);
	if (dailyTime >= 24)
	{
		dailyTime = 0.f;
		++day;
		std::cout << "Day : " << day << std::endl;

		SellAllItemsInBox();
	}

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

	if (progress < 5.f)
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

	//인벤토리가 안보였다면 I키를 눌렀을때 보이게 하기
	if (InputMgr::GetKeyDown(sf::Keyboard::U) && !inventory->GetActive())
	{
		boxInven->SetActive(!boxInven->GetActive());
		if (boxInven->GetActive())
		{
			boxInven->UpdateSlots();
		}
	}

	// test store UI
	// TO-DO: Delete
	if (InputMgr::GetKeyDown(sf::Keyboard::P))
	{
		uiStore->SetActive(!uiStore->GetActive());
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
			// 아이템 가격 계산
			sellingPrice += ITEM_TABLE->Get(item->type, item->itemId).sellingPrice * item->count;

			// 아이템 삭제
			delete item;
			item = nullptr;
		}
	}
	sellingBox->clear();
}
