#pragma once
#include "GameObject.h"
#include "ScrollBoxUi.h"

class SpriteGo;
class TextGo;

class LoadTileMapBoxUi : public GameObject
{

protected:
	std::vector<TextGo*> tileMapFiles;
	std::vector<sf::RectangleShape*> tileMapRect;

	sf::Vector2f titleLogoPosition;

	ScrollBoxUi scrollBoxUi = ScrollBoxUi();

public:
	LoadTileMapBoxUi(const std::string& name = "");
	~LoadTileMapBoxUi() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetTitleLogoPosition(const sf::Vector2f& position);
	void LoadTileMaps();
};

