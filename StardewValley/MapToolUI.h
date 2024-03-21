#pragma once
#include "GameObject.h"
#include "SpriteGo.h"

class MapToolUI : public GameObject
{
protected:
    sf::RectangleShape outLine;
    sf::RectangleShape centerLine;

    sf::RectangleShape background;

    sf::VertexArray backFloor;
    std::string spriteSheetId;
    sf::Texture* texture;
    sf::Vector2i cellCount;
    sf::Vector2f cellSize;

    SpriteGo logo;
    SpriteGo arrowLeft;
    SpriteGo arrowRight;

    SpriteGo saveButton;
    SpriteGo loadButton;
    SpriteGo eraseButton;
    SpriteGo moveScreenButton;


public:
    MapToolUI(const std::string& name = "");
    ~MapToolUI() override = default;

    void SetBackFloor(const sf::Vector2i& count, const sf::Vector2f& size);
    void SetSpriteSheetId(const std::string& id);

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};

