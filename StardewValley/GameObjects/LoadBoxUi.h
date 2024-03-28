#pragma once
#include "GameObject.h"

class SpriteGo;
class TextGo;

class LoadBoxUi : public GameObject
{

protected:

	std::vector<TextGo*> tileMapFiles;
	std::vector<sf::RectangleShape*> tileMapRect;

	std::vector<TextGo*> gameSaveFiles;
	std::vector<sf::RectangleShape*> gameSaveRect;

	sf::Vector2f titleLogoPosition;

	bool isGameSaveCheck = false;
	bool isTileMapCheck = false;


public:
	LoadBoxUi(const std::string& name = "");
	~LoadBoxUi() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetTitleLogoPosition(const sf::Vector2f& position);
	void LoadGameSaves();
	void LoadTileMaps();

	void SetIsGameSaveCheck(bool check) { isGameSaveCheck = check; }
	void SetIsTileMapCheck(bool check) { isTileMapCheck = check; }
};

