#pragma once
#include "Scene.h"
#include "MapToolUI.h"

//class MapToolUI;

struct Tile //UI에서 가지고 있는 정보들 그대로 받으려고 내부 형식 동일하게 적어줌
{
	sf::Sprite tileSprite;
	int indexNum; //MapToolUI 격자에 배치된 인덱스 번호
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
	sf::Texture imageFloor;        //추가          이름은 필요에 맞게 변경 
	sf::Sprite spriteFloor;        //추가

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
	void UpdateTransform();

	std::wstring SelectFile(); 
	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //추가
};