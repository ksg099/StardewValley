#include "pch.h"
#include "LoadTileMapBoxUi.h"
#include "TextGo.h"

LoadTileMapBoxUi::LoadTileMapBoxUi(const std::string& name) : GameObject(name)
{
}

void LoadTileMapBoxUi::Init()
{
	LoadTileMaps();

	scrollBoxUi.Init();
	scrollBoxUi.SetFileNames(tileMapFiles);
}

void LoadTileMapBoxUi::Release()
{
	scrollBoxUi.Release();
}

void LoadTileMapBoxUi::Reset()
{
	scrollBoxUi.Reset();
}

void LoadTileMapBoxUi::Update(float dt)
{
	scrollBoxUi.Update(dt);

	int mouseTileMapIndex = -1;

	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);

	for (int i = 0; i < tileMapRect.size(); ++i)
	{
		sf::FloatRect bounds = tileMapRect[i]->getGlobalBounds();
		if (bounds.contains(uiPos))
		{
			tileMapRect[i]->setOutlineColor(sf::Color::Red);
			mouseTileMapIndex = i;
			break;
		}
		tileMapRect[i]->setOutlineColor(sf::Color::Transparent);
	}

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left) && mouseTileMapIndex != -1)
	{
		for (auto rect : tileMapRect)
		{
			rect->setFillColor(sf::Color::Color(185, 122, 87));
		}

		tileMapRect[mouseTileMapIndex]->setFillColor(sf::Color::Red);
		DT_MGR.SetTileMapSelect(DT_MGR.GetTileMapData()[mouseTileMapIndex]);
	}
}

void LoadTileMapBoxUi::Draw(sf::RenderWindow& window)
{
	for (auto rect : tileMapRect)
	{
		window.draw(*rect);
	}
	for (auto tileMap : tileMapFiles)
	{
		tileMap->Draw(window);
	}

	scrollBoxUi.Draw(window);
}

void LoadTileMapBoxUi::SetTitleLogoPosition(const sf::Vector2f& position)
{
	titleLogoPosition = position;
}

void LoadTileMapBoxUi::LoadTileMaps()
{
	// Tile Maps
	const std::vector<std::string>& tileMaps = DT_MGR.GetTileMapData();
	for (int i = 0; i < tileMaps.size(); ++i)
	{
		std::string fullPath = tileMaps[i];
		std::string fileNameOnly;
		int lastSlashPos = -1; // 구분자가 없음을 나타내기 위해 -1로 초기화합니다.
		for (int j = 0; j < fullPath.length(); ++j)
		{
			if (fullPath[j] == '/' || fullPath[j] == '\\')
			{
				lastSlashPos = j;
			}
		}
		if (lastSlashPos != -1) // 구분자가 있었다면
		{
			for (int j = lastSlashPos + 1; j < fullPath.length(); ++j)
			{
				fileNameOnly += fullPath[j]; // 구분자 뒤의 문자열을 추출합니다.
			}
		}
		else // 구분자가 없었다면
		{
			fileNameOnly = fullPath; // 전체 경로가 파일명입니다.
		}

		TextGo* fileName = new TextGo("Message");
		fileName->Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), fileNameOnly, 50, sf::Color::White);
		fileName->SetPosition({ titleLogoPosition.x - 500.f, titleLogoPosition.y - 300.f + 100.f * i });
		fileName->SetOrigin(Origins::MC);
		tileMapFiles.push_back(fileName);

		sf::RectangleShape* rect = new sf::RectangleShape;
		rect->setSize({ 500.f, 80.f });
		rect->setPosition(fileName->GetPosition());
		rect->setFillColor(sf::Color::Color(185, 122, 87));
		rect->setOutlineThickness(3.f);
		rect->setOutlineColor(sf::Color::Transparent);
		Utils::SetOrigin(*rect, Origins::MC);
		tileMapRect.push_back(rect);
	}
}
