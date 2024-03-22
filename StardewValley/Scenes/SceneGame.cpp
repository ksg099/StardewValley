#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"

#include "SpriteGo.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	// 불러오기: TO-DO: 게임 초기에 수행되도록 위치 변경해야 함
	rapidjson::Document dataDoc;
	if (Utils::LoadFromFile("data/DataTable.json", dataDoc))
	{
		GROUND_TABLE->Load(dataDoc);
		OBJECT_TABLE->Load(dataDoc);
	}

	// 테스트용
	rapidjson::Document SaveDoc;
	if (Utils::LoadFromFile("data/example.json", SaveDoc))
	{
		TileMap* tileMap = new TileMap("Background");
		tileMap->LoadTileMap(SaveDoc, tileSize);
		tileMap->SetOrigin(Origins::MC);
		AddGo(tileMap);
	}

	Player* player = new Player("Player");
	AddGo(player);

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
