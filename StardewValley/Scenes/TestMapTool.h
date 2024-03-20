#pragma once
#include "Scene.h"

class MapToolUI;

class TestMapTool : public Scene
{
protected:
	sf::Texture imageFloor;        //�߰�          �̸��� �ʿ信 �°� ���� 
	sf::Sprite spriteFloor;        //�߰�

	sf::VertexArray grid;
	int col = 200;
	int row = 200;
	int size = 150;

	rapidjson::Document doc;

	MapToolUI* mapToolUI;

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

	std::wstring SelectFile(); 
	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //�߰�
};