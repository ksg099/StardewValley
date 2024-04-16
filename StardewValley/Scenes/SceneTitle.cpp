#include "pch.h"
#include "SceneTitle.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "LoadTileMapBoxUi.h"
#include "LoadSaveBoxUi.h"

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
	sf::Vector2f scale(1980.f / originalSize.x, (1080.f * 3) / originalSize.y);
	backGround->SetScale(scale);
	backGround->SetPosition({ scale.x, scale.y - 545.f});
	//backGround->SetPosition({ 0.f, scale.y - 1080.f });
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

	bird1 = new SpriteGo("bird1");
	bird1->SetTexture("graphics/bird.png");
	bird1->SetScale({ 1.5f, 1.5f });
	bird1->SetOrigin(Origins::MC);
	bird1->SetPosition({ 100.f, 100.f });

	bird2 = new SpriteGo("bird2");
	bird2->SetTexture("graphics/bird.png");
	bird2->SetScale({ 1.5f, 1.5f });
	bird2->SetOrigin(Origins::MC);
	bird2->SetPosition({ 100.f, 100.f });

	mountFront = new SpriteGo("mountFront");
	mountFront->SetScale({ 1.5f, 1.5f });
	mountFront->SetOrigin(Origins::MC);
	mountFront->SetPosition({ 0.f, 160.f });

	mountBack = new SpriteGo("mountBack");
	mountBack->SetScale({ 1.5f, 1.5f });
	mountBack->SetOrigin(Origins::MC);
	mountBack->SetPosition({ 0.f, 160.f });

	bush = new SpriteGo("bush");
	bush->SetScale({ 1.5f, 1.5f });
	bush->SetOrigin(Origins::MC);
	bush->SetPosition({ 0.f, 160.f });
	loadTileMapBoxUi = new LoadTileMapBoxUi("Load TileMap Box UI");
	//loadBoxUi->SetActive(false);
	AddGo(loadTileMapBoxUi, Ui);

	loadSaveBoxUi = new LoadSaveBoxUi("Load Save Box UI");
	AddGo(loadSaveBoxUi, Ui);

	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{
	Scene::Enter();
	time = 0;
	sf::Vector2f viewSize = sf::Vector2f(1920, 1080);
	sf::Vector2f centerPos(0.f, 1000.f);

	uiView.setSize(viewSize);
	uiView.setCenter(centerPos);

	loadTileMapBoxUi->SetActive(false);
	loadSaveBoxUi->SetActive(false);
	newGame->SetActive(false);
	LoadGame->SetActive(false);
	ReStartGame->SetActive(false);
	makeMap->SetActive(false);
	newGameBtn->SetActive(false);
	LoadGameBtn->SetActive(false);
	ReStartGameBtn->SetActive(false);
	makeMapBtn->SetActive(false);

	FRAMEWORK.GetWindow().setView(uiView);

}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);
	time += dt;
	sf::Vector2f center = uiView.getCenter();

	if (time > 2.0f && center.y > 0.f)
	{
		// 이동 속도 조정
		float speed = 200.f;
		center.y -= speed * dt;

		// 뷰의 새로운 중심 위치 설정
		uiView.setCenter(center);
		FRAMEWORK.GetWindow().setView(uiView);
	}

	if (time > 7.0f)
	{
		newGame->SetActive(true);
		newGameBtn->SetActive(true);
	}

	if (time > 8.0f)
	{
		ReStartGame->SetActive(true);
		ReStartGameBtn->SetActive(true);
	}

	if (time > 9.0f)
	{
		LoadGame->SetActive(true);
		LoadGameBtn->SetActive(true);
	}

	if (time > 10.0f)
	{
		makeMap->SetActive(true);
		makeMapBtn->SetActive(true);
	}

	sf::Vector2f currMousePos = InputMgr::GetMousePos();
	sf::Vector2f UiMousePos = ScreenToUi((sf::Vector2i)currMousePos);

	//������ ��ư
	sf::FloatRect newGameBtnBounds = newGame->GetGlobalBounds();
	if (newGameBtnBounds.contains(UiMousePos))
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			LoadData();

			DT_MGR.SetGameSaveSelect("data/default_save.json");
			loadTileMapBoxUi->SetActive(true);
			loadSaveBoxUi->SetActive(false);
		}
	}

	//�ҷ����� ���� ��ư
	sf::FloatRect LoadGameBtnBounds = LoadGame->GetGlobalBounds();
	if (LoadGameBtnBounds.contains(UiMousePos))
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			LoadData();

			loadTileMapBoxUi->SetActive(false);
			loadSaveBoxUi->SetActive(true);
		}
	}
	
	//����� ���� ��ư
	sf::FloatRect ReStartGameBtnBounds = ReStartGame->GetGlobalBounds();
	if (ReStartGameBtnBounds.contains(UiMousePos))
	{
		if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
		{
			DT_MGR.SetRecentFileSelect();
			if (DT_MGR.LoadSaveData())
			{
				SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
			}
			// DT_MGR.SetGameSaveSelect("data/recent_file_path.json");
		}
	}

	//���� ���� ���� ��ư
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
		DT_MGR.SetSelectReset();
	}
}

void SceneTitle::Draw(sf::RenderWindow& window, Layers layer)
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

	loadTileMapBoxUi->SetActive(false);
	loadSaveBoxUi->SetActive(false);
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

	loadTileMapBoxUi->SetActive(true);
	loadSaveBoxUi->SetActive(true);
}

void SceneTitle::Restart()
{
	loadTileMapBoxUi->SetActive(false);
	loadSaveBoxUi->SetActive(true);
	loadTileMapBoxUi->Init();
}

void SceneTitle::LoadSave()
{
	loadTileMapBoxUi->SetActive(false);
	loadSaveBoxUi->SetActive(true);
	loadTileMapBoxUi->Init();
}
