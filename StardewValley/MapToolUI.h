#pragma once
#include "GameObject.h"
#include "SpriteGo.h"

struct MapSheet
{
    sf::Sprite tileSprite;
    sf::Sprite originalSprite;
    int uiIndexNumber; //UI ���� ���� ��ġ�� �ε���
    int objectID;
    TileType tileType;
    std::string resource;
    GroundType groundType = GroundType::NONE;
    FloorType floorType = FloorType::NONE;
    ObjectType objectType = ObjectType::NONE;
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
    SpriteGo titleButton;

    int currentPage;
    std::vector<std::vector<MapSheet>> categories;


    int col = 15;
    int row = 10;
    float size = 40;
    sf::Vector2f gridStart = { 1250.f,100.f };

public:
    MapToolUI(const std::string& name = "");
    ~MapToolUI() override = default;

    float timer = 0.f;
    float duration = 3.f;

    MapSheet selectedTile; //������ Ÿ���� ������ �����Ϸ��� ����
    bool isSelected = false; //Ÿ�� �����ߴ��� Ȯ��

    void SetBackFloor(const sf::Vector2i& count, const sf::Vector2f& size);
    void SetSpriteSheetId(const std::string& id);
    void DrawGrid();
    sf::Vector2f IndexToPos(int index);
    int PosToIndex(sf::Vector2f pos);
    int SelectIndex(sf::Vector2f pos); //������ �� ���콺 �������� �޾Ƽ� �ε��� ��ȣ�� �ٲ��ֱ�

    const MapSheet& GetSelectedObject() const { return selectedTile; }
    void SelectTile(int index);

    sf::FloatRect GetSaveButtonGB() { return saveButton.GetGlobalBounds(); }
    sf::FloatRect GetLoadButtonGB() { return loadButton.GetGlobalBounds(); }
    sf::FloatRect GetEraseButtonGB() { return eraseButton.GetGlobalBounds(); }
    sf::FloatRect GetTitleButtonGB() { return titleButton.GetGlobalBounds(); }
    
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};

