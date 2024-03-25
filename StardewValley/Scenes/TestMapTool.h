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
	bool placedPossible = false;
	bool playerPassable = false;
};

class TestMapTool : public Scene
{
protected:
	sf::Texture imageFloor;        //�߰�          �̸��� �ʿ信 �°� ���� 
	sf::Sprite spriteFloor;        //�߰�

	sf::VertexArray grid;
	int col = 100;
	int row = 100;
	int size = 15;

	float gridStartX = 30.f;
	float gridStartY = 30.f;

	float timer = 0.f;
	float duration = 3.f;

	rapidjson::Document doc;

	MapToolUI* mapToolUI;
	std::vector<TileLayer> mapData;
	sf::RenderWindow window;

	sf::Transform transform;

public:
	TestMapTool(SceneIds id);
	virtual ~TestMapTool();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void PlaceTileToIndex(int indexNum, MapSheet& tile);
	Tile* GetIndexState(int index);

	void Draw(sf::RenderWindow& window) override;    
	
	void DrawGrid();

	int PosToIndex(sf::Vector2f pos);
	sf::Vector2f IndexToPos(int index);
	int SelectIndex(sf::Vector2f pos); //������ �� ���콺 �������� �޾Ƽ� �ε��� ��ȣ�� �ٲ��ֱ�

	void SetMapToolSize(int xCount, int yCount);
	void SaveMapContent();
	void LoadMapFile(std::vector<std::vector<Tile>>& data, const std::string& filePath);
	void UpdateTransform();

	std::wstring SaveFilePath(); 
	std::string WstringToString(const std::wstring& var);

	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //�߰�
};