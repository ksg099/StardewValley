#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"

#include "SpriteGo.h"
#include "Inventory.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// 테스트용
	rapidjson::Document SaveDoc;
	if (Utils::LoadFromFile("data/example.json", SaveDoc))
	{
		tileMap = new TileMap("Background");
		tileMap->LoadTileMap(SaveDoc, tileSize);
		tileMap->SetOrigin(Origins::MC);
		tileMap->sortLayer = -1;
		AddGo(tileMap);
	}

	Player* player = new Player("Player");
	player->sortLayer = 1;
	AddGo(player);

	//인벤토리 위치 잡기
	inventory = new Inventory("Inventory");
	inventory->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(inventory, Ui);

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
