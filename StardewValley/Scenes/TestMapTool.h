#pragma once
#include "Scene.h"

class TestMapTool : public Scene
{
protected:
	sf::Texture imageFloor;        //추가          이름은 필요에 맞게 변경 
	sf::Sprite spriteFloor;        //추가


public:
	TestMapTool(SceneIds id);
	virtual ~TestMapTool();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;         //추가
	std::wstring SelectFile();                            //추가
	std::string ToRelativePath(const std::string& originalPath, const std::string& basePath);  //추가
};