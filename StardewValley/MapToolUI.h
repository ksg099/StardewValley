#pragma once
#include "GameObject.h"
#include "SpriteGo.h"

struct MapSheet
{
    sf::Sprite tileSprite;
    sf::Sprite originalSprite;
    int uiIndexNumber; //UI 격자 내에 배치된 인덱스
    int objectID;
    TileType tileType;
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

    float timer = 0.f;
    float duration = 3.f;

    MapSheet selectedTile; //선택한 타일의 정보를 저장하려고 만듦
    bool isSelected = false; //타일 선택했는지 확인

    void SetBackFloor(const sf::Vector2i& count, const sf::Vector2f& size);
    void SetSpriteSheetId(const std::string& id);
    void DrawGrid();
    sf::Vector2f IndexToPos(int index);
    int PosToIndex(sf::Vector2f pos);
    int SelectIndex(sf::Vector2f pos); //선택할 때 마우스 포지션을 받아서 인덱스 번호로 바꿔주기

    const MapSheet& GetSelectedObject() const { return selectedTile; }
    void SelectTile(int index);

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

