#pragma once
#include "GameObject.h"
#include "SpriteGo.h"

struct MapSheet
{
    sf::Sprite objSprite;
    int indexNumber;
    int objectID;
    std::string resource;
    int sheetID_X;
    int sheetID_Y;
    int sheetID_W;
    int sheetID_H;
};

class MapToolUI : public GameObject
{
protected:
    sf::RectangleShape outLine;
    sf::RectangleShape centerLine;

    sf::RectangleShape background;

    sf::VertexArray backFloor;
    std::string backFloorSheetId;
    sf::Texture* backFloorTexture;
    sf::Vector2i cellCount;
    sf::Vector2f cellSize;

    SpriteGo logo;
    SpriteGo arrowLeft;
    SpriteGo arrowRight;

    sf::VertexArray grid;

    SpriteGo saveButton;
    SpriteGo loadButton;
    SpriteGo eraseButton;
    SpriteGo moveScreenButton;

    int currentPage;
    std::vector<std::vector<MapSheet>> categories;

    int col = 40;
    int row = 30;
    float size = 15;
    sf::Vector2f gridStart = { 1250.f,100.f };

public:
    MapToolUI(const std::string& name = "");
    ~MapToolUI() override = default;

    void SetBackFloor(const sf::Vector2i& count, const sf::Vector2f& size);
    void SetSpriteSheetId(const std::string& id);
    void DrawGrid();
    sf::Vector2f IndexToPos(int index);

    sf::FloatRect GetSaveButtonGB() { return saveButton.GetGlobalBounds(); }
    sf::FloatRect GetLoadButtonGB() { return loadButton.GetGlobalBounds(); }
    sf::FloatRect GetEraseButtonGB() { return eraseButton.GetGlobalBounds(); }
    sf::FloatRect GetMoveScreenButtonGB() { return moveScreenButton.GetGlobalBounds(); }
    
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};

