#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// 테스틍용
	rapidjson::Document doc;
	if (Utils::LoadFromFile("data/example.json", doc))
	{
		TileMap* tileMap = new TileMap("Background");
		tileMap->LoadTileMap(doc, tileSize);
		tileMap->SetOrigin(Origins::MC);
		AddGo(tileMap);
	}

	Player* player = new Player("Player");
	AddGo(player);
	// 테스트용

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
