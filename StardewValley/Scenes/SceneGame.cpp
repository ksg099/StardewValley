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
	// �׽�Ʈ
	rapidjson::Document dataDoc;
	if (Utils::LoadFromFile("data/DataTable.json", dataDoc))
	{
		GROUND_TABLE->Load(dataDoc);
		OBJECT_TABLE->Load(dataDoc);
	}

	// �׽�Ʈ��
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
