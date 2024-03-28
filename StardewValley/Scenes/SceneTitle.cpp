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
	sf::Font& font = RES_MGR_FONT.Get("fonts/Arial.ttf");

	backGround = new SpriteGo("Logo");
	backGround->SetTexture("graphics/stardewPanorama.png");
	backGround->SetOrigin(Origins::TC);
	sf::Vector2u originalSize(640, 400);
	sf::Vector2f scale(1980.f / originalSize.x, 1080.f / originalSize.y);
	backGround->SetScale(scale);
	backGround->SetPosition({ scale.x, scale.y - 545.f});
	AddGo(backGround, Ui);

	TiltleLogo = new SpriteGo("Logo");
	TiltleLogo->SetTexture("graphics/TitleLogo.png");
	TiltleLogo->SetOrigin(Origins::TC);
	TiltleLogo->SetPosition({ 0.f, -worldView.getSize().y / 2.5f });
	AddGo(TiltleLogo, Ui);

	newGame = new SpriteGo("Logo");
	newGame->SetTexture("graphics/button_edit.png");
	newGame->SetOrigin(Origins::TC);
	newGame->SetScale({ 3.f, 3.f });
	newGame->SetPosition({ TiltleLogo->GetPosition().x - 450.f, TiltleLogo->GetPosition().y + 550.f });
	AddGo(newGame, Ui);

	LoadGame = new SpriteGo("Logo");
	LoadGame->SetTexture("graphics/button_edit.png");
	LoadGame->SetOrigin(Origins::TC);
	LoadGame->SetScale({ 3.f, 3.f });
	LoadGame->SetPosition({ TiltleLogo->GetPosition().x + 150.f, TiltleLogo->GetPosition().y + 550.f});
	AddGo(LoadGame, Ui);

	ReStartGame = new SpriteGo("Logo");
	ReStartGame->SetTexture("graphics/button_edit.png");
	ReStartGame->SetOrigin(Origins::TC);
	ReStartGame->SetScale({ 3.f, 3.f });
	ReStartGame->SetPosition({ TiltleLogo->GetPosition().x - 150.f, TiltleLogo->GetPosition().y + 550.f });
	AddGo(ReStartGame, Ui);

	makeMap = new SpriteGo("Logo");
	makeMap->SetTexture("graphics/button_edit.png");
	makeMap->SetOrigin(Origins::TC);
	makeMap->SetScale({ 3.f, 3.f });
	makeMap->SetPosition({ TiltleLogo->GetPosition().x + 450.f, TiltleLogo->GetPosition().y + 550.f });
	//makeMap->SetActive(true);
	AddGo(makeMap, Ui);

	loadBoxUi = new LoadBoxUi("Load Box UI");
	loadBoxUi->SetActive(false);
	AddGo(loadBoxUi, Ui);

	newGameBtn = new TextGo("newGameBtn");
	newGameBtn->Set(font, "NewStart", 30, sf::Color::Black);
	newGameBtn->SetPosition({ newGame->GetPosition().x - 5.f, newGame->GetPosition().y + 30.f });
	newGameBtn->SetOrigin(Origins::TC);
	AddGo(newGameBtn, Ui);

	ReStartGameBtn = new TextGo("newGameBtn");
	ReStartGameBtn->Set(font, "Restart", 30, sf::Color::Black);
	ReStartGameBtn->SetPosition({ ReStartGame->GetPosition().x - 5.f, ReStartGame->GetPosition().y + 30.f });
	ReStartGameBtn->SetOrigin(Origins::TC);
	AddGo(ReStartGameBtn, Ui);

	LoadGameBtn = new TextGo("newGameBtn");
	LoadGameBtn->Set(font, "LoadGame", 30, sf::Color::Black);
	LoadGameBtn->SetPosition({ LoadGame->GetPosition().x - 5.f, LoadGame->GetPosition().y + 30.f });
	LoadGameBtn->SetOrigin(Origins::TC);
	AddGo(LoadGameBtn, Ui);

	makeMapBtn = new TextGo("newGameBtn");
	makeMapBtn->Set(font, "MakeMap", 30, sf::Color::Black);
	makeMapBtn->SetPosition({ makeMap->GetPosition().x - 5.f, makeMap->GetPosition().y + 30.f });
	makeMapBtn->SetOrigin(Origins::TC);
	AddGo(makeMapBtn, Ui);

	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{
	Scene::Enter();
	sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	sf::Vector2f centerPos(0.f, 0.f);

	uiView.setSize(viewSize);
	uiView.setCenter(centerPos);

	FRAMEWORK.GetWindow().setView(uiView);

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

	//새게임 버튼
	sf::FloatRect newGameBtnBounds = newGame->GetGlobalBounds();
	if (newGameBtnBounds.contains(UiMousePos))
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			LoadData();
		}
	}

	//불러오기 게임 버튼
	sf::FloatRect LoadGameBtnBounds = LoadGame->GetGlobalBounds();
	if (LoadGameBtnBounds.contains(UiMousePos))
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			LoadData();
		}
	}
	
	//불러오기 게임 버튼
	sf::FloatRect ReStartGameBtnBounds = ReStartGame->GetGlobalBounds();
	if (ReStartGameBtnBounds.contains(UiMousePos))
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			LoadData();
		}
	}

	//맵툴 제작 게임 버튼
	sf::FloatRect makeMapBtnBounds = makeMap->GetGlobalBounds();
	if (makeMapBtnBounds.contains(UiMousePos))
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			SCENE_MGR.ChangeScene(SceneIds::SCENE_MAP_TOOL);
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		if (DT_MGR.LoadSaveData())
		{
			SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		Escape();
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneTitle::Escape()
{
	TiltleLogo->SetActive(true);
	makeMap->SetActive(true);
	LoadGame->SetActive(true);
	newGame->SetActive(true);
	ReStartGame->SetActive(true);

	newGameBtn->SetActive(true);
	ReStartGameBtn->SetActive(true);
	makeMapBtn->SetActive(true);
	LoadGameBtn->SetActive(true);

	loadBoxUi->SetActive(false);
}

void SceneTitle::LoadData()
{
	TiltleLogo->SetActive(false);
	makeMap->SetActive(false);
	LoadGame->SetActive(false);
	newGame->SetActive(false);
	ReStartGame->SetActive(false);

	newGameBtn->SetActive(false);
	ReStartGameBtn->SetActive(false);
	makeMapBtn->SetActive(false);
	LoadGameBtn->SetActive(false);

	loadBoxUi->SetActive(true);
}
