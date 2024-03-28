#include "pch.h"
#include "LoadSaveBoxUi.h"
#include "TextGo.h"

LoadSaveBoxUi::LoadSaveBoxUi(const std::string& name) : GameObject(name)
{
}

void LoadSaveBoxUi::Init()
{
	LoadGameSaves();
}

void LoadSaveBoxUi::Release()
{
}

void LoadSaveBoxUi::Reset()
{
}

void LoadSaveBoxUi::Update(float dt)
{
	int mouseGameSaveIndex = -1;

	sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
	sf::Vector2f uiPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);

	for (int i = 0; i < gameSaveRect.size(); ++i)
	{
		sf::FloatRect bounds = gameSaveRect[i]->getGlobalBounds();
		if (bounds.contains(uiPos))
		{
			gameSaveRect[i]->setOutlineColor(sf::Color::Red);
			mouseGameSaveIndex = i;
			break;
		}
		gameSaveRect[i]->setOutlineColor(sf::Color::Transparent);
	}

	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left) && mouseGameSaveIndex != -1)
	{
		for (auto rect : gameSaveRect)
		{
			rect->setFillColor(sf::Color::Color(185, 122, 87));
		}

		gameSaveRect[mouseGameSaveIndex]->setFillColor(sf::Color::Red);
		DT_MGR.SetGameSaveSelect(DT_MGR.GetGameSaveData()[mouseGameSaveIndex]);
	}
}

void LoadSaveBoxUi::Draw(sf::RenderWindow& window)
{
	for (auto rect : gameSaveRect)
	{
		window.draw(*rect);
	}
	for (auto save : gameSaveFiles)
	{
		save->Draw(window);
	}
}

void LoadSaveBoxUi::SetTitleLogoPosition(const sf::Vector2f& position)
{
	titleLogoPosition = position;
}

void LoadSaveBoxUi::LoadGameSaves()
{
	// Game Saves
	const std::vector<std::string>& saveData = DT_MGR.GetGameSaveData();
	for (int i = 0; i < saveData.size(); ++i)
	{
		std::string fullPath = saveData[i];
		std::string fileNameOnly;
		int lastSlashPos = -1; // �����ڰ� ������ ��Ÿ���� ���� -1�� �ʱ�ȭ�մϴ�.
		for (int j = 0; j < fullPath.length(); ++j)
		{
			if (fullPath[j] == '/' || fullPath[j] == '\\')
			{
				lastSlashPos = j;
			}
		}
		if (lastSlashPos != -1) // �����ڰ� �־��ٸ�
		{
			for (int j = lastSlashPos + 1; j < fullPath.length(); ++j)
			{
				fileNameOnly += fullPath[j]; // ������ ���� ���ڿ��� �����մϴ�.
			}
		}
		else // �����ڰ� �����ٸ�
		{
			fileNameOnly = fullPath; // ��ü ��ΰ� ���ϸ��Դϴ�.
		}

		TextGo* fileName = new TextGo("Message");
		fileName->Set(RES_MGR_FONT.Get("fonts/Arial.ttf"), fileNameOnly, 50, sf::Color::White);
		fileName->SetPosition({ titleLogoPosition.x + 400.f, titleLogoPosition.y - 300.f + 100.f * i });
		fileName->SetOrigin(Origins::MC);
		gameSaveFiles.push_back(fileName);

		sf::RectangleShape* rect = new sf::RectangleShape;
		rect->setSize({ 900.f, 80.f });
		rect->setPosition(fileName->GetPosition());
		rect->setFillColor(sf::Color::Color(185, 122, 87));
		rect->setOutlineThickness(3.f);
		rect->setOutlineColor(sf::Color::Transparent);
		Utils::SetOrigin(*rect, Origins::MC);
		gameSaveRect.push_back(rect);
	}
}