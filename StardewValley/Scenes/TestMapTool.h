#pragma once
#include "Scene.h"
#include "MapToolUI.h"

struct Tile //UI에서 가지고 있는 정보들 그대로 받으려고 내부 형식 동일하게 적어줌
{
	sf::Sprite tileSprite;
	int worldIndexNum; //MapToolUI 격자에 배치된 인덱스 번호
	int ID;
	std::string resource;
	TileType tileType;
	GroundType groundType;
	FloorType floorType;
	ObjectType objectType;
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
	sf::Texture imageFloor;        //추가          이름은 필요에 맞게 변경 
	sf::Sprite spriteFloor;        //추가

	sf::VertexArray grid;
	int col = 10;
	int row = 10;
	int size = 30;

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
	int SelectIndex(sf::Vector2f pos); //선택할 때 마우스 포지션을 받아서 인덱스 번호로 바꿔주기

	void SetMapToolSize(int xCount, int yCount);
	void SaveMapContent();
	void LoadMapFile(std::vector<std::vector<Tile>>& data, const std::string& filePath);
	void UpdateTransform();

	std::wstring SaveFilePath(); 
	std::string WstringToString(const std::wstring& var);

	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //추가
};