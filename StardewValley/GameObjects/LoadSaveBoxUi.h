#pragma once
#include "GameObject.h"

class SpriteGo;
class TextGo;

class LoadSaveBoxUi : public GameObject
{
protected:
	std::vector<TextGo*> gameSaveFiles;
	std::vector<sf::RectangleShape*> gameSaveRect;

	sf::Vector2f titleLogoPosition;

public:
	LoadSaveBoxUi(const std::string& name = "");
	~LoadSaveBoxUi() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetTitleLogoPosition(const sf::Vector2f& position);
	void LoadGameSaves();
};

