#include "pch.h"
#include "SceneDev.h"
#include "TileMap.h"
#include "Inventory.h"

SceneDev::SceneDev(SceneIds id) : Scene(id)
{
}

SceneDev::~SceneDev()
{
}

void SceneDev::Init()
{
	//? ��� �ɰŰ�����
	inventory = new Inventory("Inventory");
	AddGo(inventory);

	//rapidjson::Document doc������ load�ϰ� ������ �����Ҵ��� inven�� LoadData�� doc�� �����Ѵ�.
	rapidjson::Document doc;
	if (Utils::LoadFromFile("data/itemDataExample.json", doc))
	{
		inventory->LoadData(doc);
	}

	

	Scene::Init();
}

void SceneDev::Release()
{
	Scene::Release();
}

void SceneDev::Enter()
{
	Scene::Enter();
}

void SceneDev::Exit()
{
	Scene::Exit();
}

void SceneDev::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
