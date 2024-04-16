#pragma once
#include "Scene.h"
#include "MapToolUI.h"

struct Tile //UI에서 가지고 있는 정보들 그대로 받으려고 내부 형식 동일하게 적어줌
{
	sf::Sprite tileSprite;
	int worldIndexNum; //MapToolUI 격자에 배치된 인덱스 번호
	int ID = 0;
	std::string resource;
	TileType tileType;
	GroundType groundType = GroundType::DIRT; // 아무것도 지정안하면 흙길
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
	sf::Texture imageFloor;        //추가          이름은 필요에 맞게 변경 
	sf::Sprite spriteFloor;        //추가

	sf::VertexArray grid;
	int col = 20;
	int row = 20;
	int size = 50;
	float magnification = 50.f / 15.f;

	/*float gridStartX = ((float)FRAMEWORK.GetWindowSize().x * 0.6f) * 0.35f;
	float gridStartY = ((float)FRAMEWORK.GetWindowSize().y * 0.4f);*/
	float gridStartX = 75.f;
	float gridStartY = 45.f;

	float timer = 0.f;
	float duration = 3.f;

	bool isErase = false;

	float currentZoomLevel = 1.f;
	const float minZoomLevel = 1.f;
	const float maxZoomLevel = 3.f;
	sf::Vector2f originalViewSize;

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
	int SelectIndex(sf::Vector2f pos); //선택할 때 마우스 포지션을 받아서 인덱스 번호로 바꿔주기

	void SetMapToolSize(int xCount, int yCount);
	void SaveMapContent();
	void LoadTileMap(const std::string& name);
	void LoadMapFile();
	void UpdateTransform();

	std::wstring SaveFilePath(); 
	std::string WstringToString(const std::wstring& var);
	std::wstring OpenFilePath();

	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //추가
};