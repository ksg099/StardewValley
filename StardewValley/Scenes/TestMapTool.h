#pragma once
#include "Scene.h"
#include "MapToolUI.h"

//class MapToolUI;

struct Tile //UI���� ������ �ִ� ������ �״�� �������� ���� ���� �����ϰ� ������
{
	sf::Sprite tileSprite;
	int indexNum; //MapToolUI ���ڿ� ��ġ�� �ε��� ��ȣ
	int ID;
	std::string resource;
	int sheetID_X;
	int sheetID_Y;
	int sheetID_W;
	int sheetID_H;
};

class TestMapTool : public Scene
{
protected:
	sf::Texture imageFloor;        //�߰�          �̸��� �ʿ信 �°� ���� 
	sf::Sprite spriteFloor;        //�߰�

	sf::VertexArray grid;
	int col = 200;
	int row = 200;
	int size = 15;

	rapidjson::Document doc;

	MapToolUI* mapToolUI;
	std::vector<Tile> mapData;
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

	void Draw(sf::RenderWindow& window) override;    
	
	void DrawGrid();

	int PosToIndex(sf::Vector2f pos);
	sf::Vector2f IndexToPos(int index);

	void SetMapToolSize(int xCount, int yCount);
	void SaveMapContent();
	void LoadMapFile(std::vector<std::vector<Tile>>& data, const std::string& filePath);

	std::wstring SelectFile(); 
	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //�߰�
};