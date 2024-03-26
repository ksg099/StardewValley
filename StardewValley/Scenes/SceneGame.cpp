#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"

#include "SpriteGo.h"
#include "Inventory.h"

#include "BoxInven.h"

#include "TestMapTool.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	tileMap = new TileMap("Background");
	tileMap->sortLayer = -1;
	AddGo(tileMap);

	Player* player = new Player("Player");
	player->sortLayer = 1;
	AddGo(player);

	//인벤토리 위치 잡기
	inventory = new Inventory("Inventory");
	inventory->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(inventory, Ui);

	boxInven = new BoxInven("Inventory");
	boxInven->SetPosition({ 1920 / 2, 1080 / 2 });
	AddGo(boxInven, Ui);

	dailyTime = 2.f;
	/*overlayer.setSize({ (float)FRAMEWORK.GetWindowSize().x, (float)FRAMEWORK.GetWindowSize().y });
	overlayer.setFillColor(dayColor);
	overlayer.setOrigin(0.f, 0.f);
	overlayer.setPosition(0.f, 0.f);*/

	layer = new SpriteGo("layer");
	layer->SetTexture("graphics/layout.png");
	layer->SetOrigin(Origins::TL);
	layer->SetPosition({ -(float)FRAMEWORK.GetWindowSize().x * 0.5f,-(float)FRAMEWORK.GetWindowSize().y * 0.5f });
	layer->SetFillColor(dayColor);
	layer->sortLayer = 5;
	AddGo(layer, Layers::World);

	Plant1 = new SpriteGo("plant");
	Plant1->SetTexture("graphics/Plant/rice1.png");
	Plant1->SetScale({ tileSize.x / Plant1->GetLocalBounds().width, tileSize.y / Plant1->GetLocalBounds().height });
	Plant1->SetOrigin(Origins::MC);
	AddGo(Plant1, Layers::World);
	Plant1->SetActive(false);

	Plant2 = new SpriteGo("plant");
	Plant2->SetTexture("graphics/Plant/rice2.png");
	Plant2->SetScale({ tileSize.x / Plant2->GetLocalBounds().width, tileSize.y / Plant2->GetLocalBounds().height });
	Plant2->SetOrigin(Origins::MC);
	AddGo(Plant2, Layers::World);
	Plant2->SetActive(false);

	Plant3 = new SpriteGo("plant");
	Plant3->SetTexture("graphics/Plant/rice3.png");
	Plant3->SetScale({ tileSize.x / Plant3->GetLocalBounds().width, tileSize.y / Plant3->GetLocalBounds().height });
	Plant3->SetOrigin(Origins::MC);
	AddGo(Plant3, Layers::World);
	Plant3->SetActive(false);

	Plant4 = new SpriteGo("plant");
	Plant4->SetTexture("graphics/Plant/rice4.png");
	Plant4->SetScale({ tileSize.x / Plant4->GetLocalBounds().width, tileSize.y / Plant4->GetLocalBounds().height });
	Plant4->SetOrigin(Origins::MC);
	AddGo(Plant4, Layers::World);
	Plant4->SetActive(false);

	Plant5 = new SpriteGo("plant");
	Plant5->SetTexture("graphics/Plant/rice5.png");
	Plant5->SetScale({ tileSize.x / Plant5->GetLocalBounds().width, tileSize.y / Plant5->GetLocalBounds().height });
	Plant5->SetOrigin(Origins::MC);
	AddGo(Plant5, Layers::World);
	Plant5->SetActive(false);

	Plant6 = new SpriteGo("plant");
	Plant6->SetTexture("graphics/Plant/rice6.png");
	Plant6->SetScale({ tileSize.x / Plant6->GetLocalBounds().width, tileSize.y / Plant6->GetLocalBounds().height });
	Plant6->SetOrigin(Origins::MC);
	Plant6->SetActive(false);
	AddGo(Plant6, Layers::World);

	Seed = new SpriteGo("plant");
	Seed->SetTexture("graphics/Seeds/Cauliflower_Seeds.png");
	//Seed->SetPosition(IndexToPos(2));
	Seed->SetOrigin(Origins::MC);
	Seed->SetActive(false);
	AddGo(Seed, Layers::World);

	//for (int i = 0; i < 6; i++)
	//{
	//	Plants = new SpriteGo("plant");
	//	Plants->SetTexture("graphics/Plant/rice6.png");
	//	Plants->SetPosition(IndexToPos(2));
	//	Plants->SetOrigin(Origins::MC);
	//	Plants->SetActive(false);
	//	AddGo(Plants, Layers::World);
	//	Plants->SetActive(false);
	//}

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	tileMap->LoadTileMap("Farm");
	tileMap->SetOrigin(Origins::MC);
	Scene::Enter();
	Plant1->SetPosition(IndexToPos(0));
	Plant2->SetPosition(IndexToPos(0));
	Plant3->SetPosition(IndexToPos(0));
	Plant4->SetPosition(IndexToPos(0));
	Plant5->SetPosition(IndexToPos(0));
	Plant6->SetPosition(IndexToPos(0));



}

void SceneGame::Exit()
{
	Scene::Exit();
}

sf::Color SceneGame::LerpColor(const sf::Color& start, const sf::Color& end, float t)
{
	return sf::Color(
		start.r + (end.r - start.r) * t,
		start.g + (end.g - start.g) * t,
		start.b + (end.b - start.b) * t,
		start.a + (end.a - start.a) * t);
}

sf::Vector2f SceneGame::IndexToPos(int index)
{
	const sf::Vector2f& size = tileMap->GetCellSize();
	col = tileMap->GetCellSize().y;

	int y = index / col;
	int x = index % col;
	return sf::Vector2f((x * size.x + size.x / 2) + gridStartX, (y * size.y + size.y / 2) + gridStartY);
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		rapidjson::Document saveDoc;
		saveDoc.SetObject();
		TILEMAP_SAVE->Save(saveDoc);
		ITEM_SAVE->Save(saveDoc);
		Utils::EditFile(DT_MGR.GetGameSaveSelect(), saveDoc);
	}

	dailyTime += (dt);
	if (dailyTime >= 24)
	{
		dailyTime = 0.f;
		++day;
		std::cout << "Day : " << day << std::endl;
	}

	if (dailyTime >= 6 && dailyTime < 16 && targetColor != dayColor)
	{
		targetColor = dayColor;
		currentColor = layer->GetFillColor();
		progress = 0.f;
	}
	else if (dailyTime >= 16 && dailyTime < 20 && targetColor != eveningColor)
	{
		targetColor = eveningColor;
		currentColor = layer->GetFillColor();
		progress = 0.f;
	}
	else if(dailyTime >= 20 || dailyTime < 6 && targetColor != nightColor)
	{
		targetColor = nightColor;
		currentColor = layer->GetFillColor();
		progress = 0.f;
	}

	if (progress < 7.f)
	{
		progress += dt / transitionDuration;
		if (progress > 5.f)
		{
			progress = 5.f;
		}
		layer->SetFillColor(LerpColor(layer->GetFillColor(), targetColor, progress));
	}

	//SpriteGo* plants[6] = { Plant1, Plant2, Plant3, Plant4, Plant5, Plant6 };
	//
	//for (int i = 0; i < 6; i++)
	//{
	//	if (day >= i + 1)
	//	{
	//		plants[i]->SetActive(true);
	//	}
	//	else
	//		plants[i]->SetActive(false);
	//}
	if (day == 1)
	{
		Plant1->SetActive(true);
	}

	if (day == 2)
	{
		Plant2->SetActive(true);
		Plant1->SetActive(false);
	}

	if (day == 3)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(true);
	}
	if (day == 4)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(false);
		Plant4->SetActive(true);
	}
	if (day == 5)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(false);
		Plant4->SetActive(false);
		Plant5->SetActive(true);
	}

	if (day == 6)
	{
		Plant1->SetActive(false);
		Plant2->SetActive(false);
		Plant3->SetActive(false);
		Plant4->SetActive(false);
		Plant5->SetActive(false);
		Plant6->SetActive(true);
	}

}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	//window.draw(overlayer);
}
