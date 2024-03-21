#pragma once
#include "Scene.h"

class MapToolUI;

struct Tile
{
	int posX;
	int posY;
	GroundType groundType;
	int groundID;
	FloorType floorType;
	int floorID;
	ObjectType objectType;
	int objectID;
	bool placedPossible;
	bool playerPassable;
};

class TestMapTool : public Scene
{
protected:
	sf::Texture imageFloor;        //�߰�          �̸��� �ʿ信 �°� ���� 
	sf::Sprite spriteFloor;        //�߰�

	sf::VertexArray grid;
	int col = 200;
	int row = 200;
	int size = 16;

	rapidjson::Document doc;

	MapToolUI* mapToolUI;
	std::vector<std::vector<Tile>> mapData;

public:
	TestMapTool(SceneIds id);
	virtual ~TestMapTool();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;    
	
	void DrawGrid();

	void SetMapToolSize(int xCount, int yCount);
	void SaveMapContent();
	void LoadMapFile(std::vector<std::vector<Tile>>& data, const std::string& filePath);

	std::wstring SelectFile(); 
	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //�߰�
};