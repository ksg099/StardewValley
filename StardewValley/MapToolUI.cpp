#include "pch.h"
#include "MapToolUI.h"

MapToolUI::MapToolUI(const std::string& name)
{
}

void MapToolUI::Init()
{
    outLine.setOutlineColor(sf::Color(134, 56, 10));
    outLine.setFillColor(sf::Color::Transparent);
    outLine.setOutlineThickness(-15.f);
    outLine.setSize((sf::Vector2f)(FRAMEWORK.GetWindowSize()));
    outLine.setOrigin(0.f, 0.f);
    outLine.setPosition(0.f, 0.f);

    //두께 주는 방법 찾아보기
    centerLine.clear();
    centerLine.setPrimitiveType(sf::Lines);
    centerLine.resize(2);
    centerLine[0].position = { (float)FRAMEWORK.GetWindowSize().x * 0.6f, 0.f };
    centerLine[0].color = sf::Color(134, 56, 10);
    centerLine[1].position = { (float)FRAMEWORK.GetWindowSize().x * 0.6f, (float)FRAMEWORK.GetWindowSize().y };
    centerLine[1].color = sf::Color(134, 56, 10);

    background.setFillColor(sf::Color::White);
    background.setSize({ (float)FRAMEWORK.GetWindowSize().x * 0.4f, (float)FRAMEWORK.GetWindowSize().y });
    background.setOrigin(0.f, 0.f);
    background.setPosition((float)FRAMEWORK.GetWindowSize().x * 0.6f, 0.f);

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
