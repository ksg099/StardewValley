#include "pch.h"
#include "SceneTitle.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "LoadBoxUi.h"

SceneTitle::SceneTitle(SceneIds id) : Scene(id)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	title.setFillColor(sf::Color::Color(34, 213, 226));
	Utils::SetOrigin(title, Origins::MC);
	title.setPosition({ 0.f, 0.f });
	title.setSize((sf::Vector2f)FRAMEWORK.GetWindowSize());

	logo = new SpriteGo("Logo");
	logo->SetTexture("graphics/yellowLettersLogo.png");
	logo->SetOrigin(Origins::TC);
	// logo->SetScale({ 0.5f, 0.5f });
	logo->SetPosition({0.f, -worldView.getSize().y / 2.5f});
	AddGo(logo);

	loadBoxUi = new LoadBoxUi("Load Box UI");
	AddGo(loadBoxUi, Ui);

	/*textMessage = new TextGo("Message");
	textMessage->Set(RES_MGR_FONT.Get("fonts/Arial.ttf"),
		"Press Enter to Start!", 50, sf::Color::White);
	textMessage->SetOrigin(Origins::TC);
	textMessage->SetPosition({ 0.f, 100.f });
	AddGo(textMessage);*/

	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{
	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		if (DT_MGR.LoadSaveData())
		{
			SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
		}
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	window.draw(title);

	Scene::Draw(window);
}
