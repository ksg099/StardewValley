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

	//backGround = new SpriteGo("Logo");
	//backGround->SetTexture("graphics/stardewPanorama.png");
	//backGround->SetOrigin(Origins::TC);
	//backGround->SetScale({ 2.f, 2.f });
	//backGround->SetPosition({ -400.f, -worldView.getSize().y / 2.5f });
	//AddGo(backGround);

	TiltleLogo = new SpriteGo("Logo");
	TiltleLogo->SetTexture("graphics/TitleLogo.png");
	TiltleLogo->SetOrigin(Origins::TC);
	TiltleLogo->SetPosition({ 0.f, -worldView.getSize().y / 2.5f });
	AddGo(TiltleLogo);

	newGame = new SpriteGo("Logo");
	newGame->SetTexture("graphics/button_edit.png");
	newGame->SetOrigin(Origins::TC);
	newGame->SetScale({ 3.f, 3.f });
	newGame->SetPosition({ TiltleLogo->GetPosition().x - 400.f, TiltleLogo->GetPosition().y + 500.f });
	AddGo(newGame);

	LoadGame = new SpriteGo("Logo");
	LoadGame->SetTexture("graphics/button_edit.png");
	LoadGame->SetOrigin(Origins::TC);
	LoadGame->SetScale({ 3.f, 3.f });
	LoadGame->SetPosition({ TiltleLogo->GetPosition().x, TiltleLogo->GetPosition().y + 500.f});
	AddGo(LoadGame);

	makeMap = new SpriteGo("Logo");
	makeMap->SetTexture("graphics/button_edit.png");
	makeMap->SetOrigin(Origins::TC);
	makeMap->SetScale({ 3.f, 3.f });
	makeMap->SetPosition({ TiltleLogo->GetPosition().x + 400.f, TiltleLogo->GetPosition().y + 500.f });
	AddGo(makeMap);

	/*loadBoxUi = new LoadBoxUi("Load Box UI");
	AddGo(loadBoxUi, Ui);*/

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

	sf::Vector2f currMousePos = InputMgr::GetMousePos();
	sf::Vector2f UiMousePos = ScreenToUi((sf::Vector2i)currMousePos);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		if (DT_MGR.LoadSaveData())
		{
			SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
		}
	}

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		sf::FloatRect newGameBtnBounds = newGame->GetGlobalBounds();
		if (newGameBtnBounds.contains(UiMousePos))
		{

			SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);

		}

		sf::FloatRect LoadGameBtnBounds = LoadGame->GetGlobalBounds();
		if (newGameBtnBounds.contains(UiMousePos))
		{
			SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
		}

		sf::FloatRect makeMapBtnBounds = makeMap->GetGlobalBounds();
		if (newGameBtnBounds.contains(UiMousePos))
		{
			SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
		}
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	window.draw(title);
	newGame->Draw(window);
//	LoadGame->Draw(window);
	//makeMap->Draw(window);

	Scene::Draw(window);
}
