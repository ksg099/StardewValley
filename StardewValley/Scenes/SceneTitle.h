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
	
	TextGo* newGameBtn = nullptr;
	TextGo* ReStartGameBtn = nullptr;
	TextGo* makeMapBtn = nullptr;
	TextGo* LoadGameBtn = nullptr;

	LoadTileMapBoxUi* loadTileMapBoxUi = nullptr;
	LoadSaveBoxUi* loadSaveBoxUi = nullptr;

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

