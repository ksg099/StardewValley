#include "pch.h"
#include "SceneDev.h"
#include "TileMap.h"

SceneDev::SceneDev(SceneIds id) : Scene(id)
{
}

SceneDev::~SceneDev()
{
}

void SceneDev::Init()
{
	rapidjson::Document doc;
	if (Utils::LoadFromFile("data/example.json", doc))
	{
		std::cout << doc["title"].GetString() << std::endl;
		doc["title"].SetString("Wow");
		Utils::EditFile("data/example.json", doc);
		Utils::EditFile("data/new_example.json", doc);
		Utils::EditFile("data/example2.json", doc);
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
