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
	sf::Texture imageFloor;        //추가          이름은 필요에 맞게 변경 
	sf::Sprite spriteFloor;        //추가

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
	sf::Vector2f IndexToPos(int index);

	void SetMapToolSize(int xCount, int yCount);
	void SaveMapContent();
	void LoadMapFile(std::vector<std::vector<Tile>>& data, const std::string& filePath);

	std::wstring SelectFile(); 
	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //추가
};