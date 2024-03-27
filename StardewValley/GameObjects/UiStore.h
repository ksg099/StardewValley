#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

struct UiItem
{
	sf::Sprite itemBackground;
	TextGo itemInfo;
};

class UiStore : public GameObject
{
protected:
	sf::Vector2f referenceResolution = { 1920, 1080 };
	sf::Vector2f resolution = referenceResolution;

	sf::Sprite StoreBackground;
	std::vector<UiItem*> UipurchaseItems;
	
public:
	UiStore(const std::string& name = "");
	~UiStore() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};