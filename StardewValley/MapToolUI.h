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
    std::string backFloorSheetId;
    sf::Texture* backFloorTexture;
    sf::Vector2i cellCount;
    sf::Vector2f cellSize;

    SpriteGo logo;
    SpriteGo arrowLeft;
    SpriteGo arrowRight;

    sf::VertexArray grid;

    SpriteGo stone_1;
    SpriteGo stone_2;
    SpriteGo stone_3;
    SpriteGo stone_4;

    SpriteGo tree_1;
    SpriteGo tree_2;
    SpriteGo tree_3;
    SpriteGo tree_4;

    SpriteGo saveButton;
    SpriteGo loadButton;
    SpriteGo eraseButton;
    SpriteGo moveScreenButton;

    int currentPage;
    std::vector<std::vector<SpriteGo>> categories = { {stone_1, stone_2, stone_3, stone_4, tree_1, tree_2, tree_3, tree_4}
    , { } }; //?

    int col = 40;
    int row = 30;
    int size = 15;
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

    void UpdatePalette();
    
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};

