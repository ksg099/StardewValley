#pragma once

class Inventory;


class SceneDev : public Scene
{
protected:
	sf::RectangleShape land;


	Inventory* inventory;
public:
	SceneDev(SceneIds id);
	virtual ~SceneDev();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;
};