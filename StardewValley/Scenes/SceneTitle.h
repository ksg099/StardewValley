#pragma once
#include "Scene.h"

class SpriteGo;
class TextGo;
class LoadTileMapBoxUi;
class LoadSaveBoxUi;

class SceneTitle : public Scene
{
private:
	
	SpriteGo* newGame = nullptr;
	SpriteGo* LoadGame = nullptr;
	SpriteGo* makeMap = nullptr;
	SpriteGo* backGround = nullptr;
	SpriteGo* ReStartGame = nullptr;
	SpriteGo* TiltleLogo = nullptr;
	SpriteGo* mountFront = nullptr;
	SpriteGo* mountBack = nullptr;
	SpriteGo* bush = nullptr;
	SpriteGo* bird1 = nullptr;
	SpriteGo* bird2 = nullptr;


	TextGo* newGameBtn = nullptr;
	TextGo* ReStartGameBtn = nullptr;
	TextGo* makeMapBtn = nullptr;
	TextGo* LoadGameBtn = nullptr;

	std::wstring newGameText = L"������";
	std::wstring restartText = L"�����";
	std::wstring loadGameText = L"�̾��ϱ�";
	std::wstring maptoolText = L"�ʿ�����";

	LoadTileMapBoxUi* loadTileMapBoxUi = nullptr;
	LoadSaveBoxUi* loadSaveBoxUi = nullptr;

	float time;
	float timer;

public:
	SceneTitle(SceneIds id);
	virtual ~SceneTitle();

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;

	void Draw(sf::RenderWindow& window, Layers layer = Layers::Everything) override;
	// void Draw(sf::RenderWindow& window) override;

	void Escape();
	void LoadData();
	void Restart();
	void LoadSave();
};

