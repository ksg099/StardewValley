#pragma once
#include "GameObject.h"
#include "SpriteGo.h"
#include "TextGo.h"

struct SlotBox
{
	SpriteGo slotBoxImg;
	// SpriteGo slotImgBox;
	// SpriteGo slotImg;
	TextGo slotName;
	sf::RectangleShape clickBox;
};

class ScrollBoxUi : public GameObject
{
protected:
	SpriteGo slotBoxesBackground;
	std::vector<SlotBox*> slotBoxes;
	SpriteGo scrollBar;
	SpriteGo scroll;

	std::vector<TextGo*>* fileNames = nullptr;

	const int slotCount = 4; // ȭ�鿡 �������� ���� ����
	int currentIndex = 0; // ȭ�� ��� ���� �ε���

	// ���� Ŭ�� ó��
	int clickIndex = -1;
	int doubleClickIndex = -1;
	float doubleClickTimer = 0.f;
	float doubleClickDuration = 0.3f;
	bool isClick = false;

public:
	ScrollBoxUi(const std::string& name = "");
	~ScrollBoxUi() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetFileNames(std::vector<TextGo*>& files);

	void UpdateIndex();
	void SelectByDoubleClick(float dt);
};

