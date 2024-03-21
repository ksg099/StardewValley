#pragma once
#include "Scene.h"

class TestMapTool : public Scene
{
protected:
	sf::Texture imageFloor;        //�߰�          �̸��� �ʿ信 �°� ���� 
	sf::Sprite spriteFloor;        //�߰�


public:
	TestMapTool(SceneIds id);
	virtual ~TestMapTool();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;         //�߰�
	std::wstring SelectFile();                            //�߰�
	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //�߰�
};