#pragma once
#include "Scene.h"
#include "MapToolUI.h"

struct Tile //UI���� ������ �ִ� ������ �״�� �������� ���� ���� �����ϰ� ������
{
	sf::Sprite tileSprite;
	int worldIndexNum; //MapToolUI ���ڿ� ��ġ�� �ε��� ��ȣ
	int ID = 0;
	std::string resource;
	TileType tileType;
	GroundType groundType = GroundType::DIRT; // �ƹ��͵� �������ϸ� ���
	FloorType floorType = FloorType::NONE;
	ObjectType objectType = ObjectType::NONE;
	int sheetID_X;
	int sheetID_Y;
	int sheetID_W;
	int sheetID_H;
};

struct TileLayer
{
	Tile groundLayer;
	Tile floorLayer;
	Tile objectLayer;
	int IndexX;
	int IndexY;
	bool placedPossible = true;
	bool playerPassable = true;
};

class TestMapTool : public Scene
{
protected:
	sf::Texture imageFloor;        //�߰�          �̸��� �ʿ信 �°� ���� 
	sf::Sprite spriteFloor;        //�߰�

	sf::VertexArray grid;
	int col = 10;
	int row = 10;
	int size = 15;

	float gridStartX = ((float)FRAMEWORK.GetWindowSize().x * 0.6f) * 0.35f;
	float gridStartY = ((float)FRAMEWORK.GetWindowSize().y * 0.4f);

	float timer = 0.f;
	float duration = 3.f;

	bool isErase = false;

	rapidjson::Document doc;

	MapToolUI* mapToolUI;
	std::vector<TileLayer> mapData;
	sf::RenderWindow window; 
	
	std::vector<TileData*>* tiles;

	sf::Transform transform;

public:
	TestMapTool(SceneIds id);
	virtual ~TestMapTool();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void ErasePlacedTile(int indexNum);
	void PlaceTileToIndex(int indexNum, MapSheet& tile);

	void Draw(sf::RenderWindow& window, Layers layer = Layers::Everything) override;    
	
	void DrawGrid();

	int PosToIndex(sf::Vector2f pos);
	sf::Vector2f IndexToPos(int index);
	int SelectIndex(sf::Vector2f pos); //������ �� ���콺 �������� �޾Ƽ� �ε��� ��ȣ�� �ٲ��ֱ�

	void SetMapToolSize(int xCount, int yCount);
	void SaveMapContent();
	void LoadTileMap(const std::string& name);
	void LoadMapFile();
	void UpdateTransform();

	std::wstring SaveFilePath(); 
	std::string WstringToString(const std::wstring& var);
	std::wstring OpenFilePath();

	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //�߰�
};