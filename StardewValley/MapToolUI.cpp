#include "pch.h"
#include "MapToolUI.h"

MapToolUI::MapToolUI(const std::string& name)
{
}

void MapToolUI::SetBackFloor(const sf::Vector2i& count, const sf::Vector2f& size)
{
    cellCount = count;
    cellSize = size;

    backFloor.clear();
    backFloor.setPrimitiveType(sf::Quads);
    backFloor.resize(count.x * count.y * 4);

    float tileWidth = 38.f;
    float tileHeight = 38.f;

    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
        {
            int quadIndex = (i * count.x + j) * 4;

            sf::Vector2f topLeftPos((float)(FRAMEWORK.GetWindowSize().x) * 0.6f + (float)j * size.x, (float)i * size.y);

            backFloor[quadIndex + 0].position = topLeftPos;
            backFloor[quadIndex + 1].position = topLeftPos + sf::Vector2f(size.x, 0.f);
            backFloor[quadIndex + 2].position = topLeftPos + sf::Vector2f(size.x, size.y);
            backFloor[quadIndex + 3].position = topLeftPos + sf::Vector2f(0.f, size.y);

            backFloor[quadIndex + 0].texCoords = sf::Vector2f(0.f, 0.f);
            backFloor[quadIndex + 1].texCoords = sf::Vector2f(tileWidth, 0.f);
            backFloor[quadIndex + 2].texCoords = sf::Vector2f(tileWidth, tileHeight);
            backFloor[quadIndex + 3].texCoords = sf::Vector2f(0.f, tileHeight);
        }
    }
}

void MapToolUI::SetSpriteSheetId(const std::string& id)
{
    spriteSheetId = id;
    texture = &RES_MGR_TEXTURE.Get(spriteSheetId);
}

void MapToolUI::Init()
{
    outLine.setOutlineColor(sf::Color(134, 56, 10));
    outLine.setFillColor(sf::Color::Transparent);
    outLine.setOutlineThickness(-15.f);
    outLine.setSize((sf::Vector2f)(FRAMEWORK.GetWindowSize()));
    outLine.setOrigin(0.f, 0.f);
    outLine.setPosition(0.f, 0.f);

    centerLine.setFillColor(sf::Color(134, 56, 10));
    centerLine.setSize({ 15.f, (float)FRAMEWORK.GetWindowSize().y });
    centerLine.setOrigin(0.f, 0.f);
    centerLine.setPosition((float)FRAMEWORK.GetWindowSize().x * 0.6f, 0.f);
   
    background.setFillColor(sf::Color::White);
    background.setSize({ (float)FRAMEWORK.GetWindowSize().x * 0.4f, (float)FRAMEWORK.GetWindowSize().y });
    background.setOrigin(0.f, 0.f);
    background.setPosition((float)FRAMEWORK.GetWindowSize().x * 0.6f, 0.f);

    SetSpriteSheetId("graphics/mapToolUIFloor2.png");
    SetBackFloor({ 20,20 }, { 76.f,76.f });
    
    logo.SetTexture("graphics/logo.png");
    logo.SetOrigin(Origins::MC);
    logo.SetScale({ 0.5f,0.5f });
    logo.SetPosition({ (float)(FRAMEWORK.GetWindowSize().x) * 0.8f, logo.GetLocalBounds().height * 0.5f });

    arrowLeft.SetTexture("graphics/arrow.png");
    arrowLeft.SetOrigin(Origins::MC);
    arrowLeft.SetScale({ 2.f, 2.f });
    arrowLeft.SetPosition({ (float)(FRAMEWORK.GetWindowSize().x) * 0.8f - logo.GetLocalBounds().width * 0.5f
        , logo.GetLocalBounds().height * 0.5f });

    arrowRight.SetTexture("graphics/arrow.png");
    arrowRight.SetFlipX(true);
    arrowRight.SetOrigin(Origins::MC);
    arrowRight.SetScale({ 2.f, 2.f });
    arrowRight.SetPosition({ (float)(FRAMEWORK.GetWindowSize().x) * 0.8f + logo.GetLocalBounds().width * 0.5f
        , logo.GetLocalBounds().height * 0.5f });

    float buttonOffset = 20.f;

    saveButton.SetTexture("graphics/button.png");
    saveButton.SetOrigin(Origins::MC);
	saveButton.SetScale({ 2.f, 2.f });
    saveButton.SetPosition({(float)FRAMEWORK.GetWindowSize().x *0.675f
        , (float)FRAMEWORK.GetWindowSize().y * 0.85f});
    
    eraseButton.SetTexture("graphics/button.png");
    eraseButton.SetOrigin(Origins::MC);
    eraseButton.SetScale({ 2.f, 2.f });
    eraseButton.SetPosition({ saveButton.GetPosition().x + saveButton.GetLocalBounds().width * 2.f + buttonOffset
        , saveButton.GetPosition().y });

    loadButton.SetTexture("graphics/button.png");
    loadButton.SetOrigin(Origins::MC);
    loadButton.SetScale({ 2.f, 2.f });
    loadButton.SetPosition({ eraseButton.GetPosition().x + saveButton.GetLocalBounds().width * 2.f + buttonOffset
        ,saveButton.GetPosition().y});

    moveScreenButton.SetTexture("graphics/button.png");
    moveScreenButton.SetOrigin(Origins::MC);
    moveScreenButton.SetScale({ 2.f, 2.f });
    moveScreenButton.SetPosition({ loadButton.GetPosition().x + saveButton.GetLocalBounds().width * 2.f + buttonOffset
        , saveButton.GetPosition().y });

}

void MapToolUI::Release()
{
    GameObject::Release();
}

void MapToolUI::Reset()
{
    GameObject::Reset();
}

void MapToolUI::Update(float dt)
{
    GameObject::Update(dt);
}

void MapToolUI::LateUpdate(float dt)
{
    GameObject::LateUpdate(dt);
}

void MapToolUI::FixedUpdate(float dt)
{
    GameObject::FixedUpdate(dt);
}

void MapToolUI::Draw(sf::RenderWindow& window)
{
    GameObject::Draw(window);
    window.draw(background);

    sf::RenderStates state;
    state.texture = texture;

    window.draw(backFloor, state);
    window.draw(outLine);
    window.draw(centerLine);
    logo.Draw(window);
    arrowLeft.Draw(window);
    arrowRight.Draw(window);
    saveButton.Draw(window);
    eraseButton.Draw(window);
    loadButton.Draw(window);
    moveScreenButton.Draw(window);
}
