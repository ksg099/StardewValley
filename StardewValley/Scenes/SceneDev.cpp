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
	//? 없어도 될거같은데
	inventory = new Inventory("Inventory");
	inventory->SetPosition({1920/2, 1080/2 });
	AddGo(inventory, Ui);

	//rapidjson::Document doc파일을 load하고 위에서 동적할당한 inven의 LoadData에 doc를 적용한다.
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

void SceneDev::Update(float dt) //다른 데이터가 들어오면 변경
{
	Scene::Update(dt);
}

void SceneDev::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
