#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"

#include "SpriteGo.h"
#include "Inventory.h"

#include "BoxInven.h"

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

	Player* player = new Player("Player");
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

}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	//window.draw(overlayer);
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
}
