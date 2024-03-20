#pragma once
#include "GameObject.h"
#include "SpriteGo.h"

class MapToolUI : public GameObject
{
protected:
    sf::RectangleShape outLine;
    sf::VertexArray centerLine;

    sf::RectangleShape background;
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

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};

