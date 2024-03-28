#pragma once
#include "Scene.h"

class SpriteGo;
class TextGo;
class LoadBoxUi;

class SceneTitle : public Scene
{
private:
	sf::RectangleShape title;

	SpriteGo* newGame = nullptr;
	SpriteGo* LoadGame = nullptr;
	SpriteGo* makeMap = nullptr;
	SpriteGo* backGround = nullptr;
	//SpriteGo* GameOver = nullptr;

	SpriteGo* TiltleLogo = nullptr;

	TextGo* textMessage = nullptr;
	LoadBoxUi* loadBoxUi = nullptr;

public:
	SceneTitle(SceneIds id);
	virtual ~SceneTitle();

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

