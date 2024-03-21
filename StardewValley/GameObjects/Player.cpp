#include "pch.h"
#include "Player.h"
#include "TileMap.h"

Player::Player(const std::string& name) : SpriteGo(name)
{
}

void Player::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/White Chicken.png");
	SetTextureRect(sf::IntRect(0, 0, 15, 15));
	SetOrigin(Origins::MC);
}

void Player::Release()
{
	SpriteGo::Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));

	gridIndex = { 0, 0 };
	SetPosition(tileMap->GetGridPosition(gridIndex.x, gridIndex.y));
	isMove = false;
	direction = { 0, 0 };
	moveTimer = 0.f;
	moveDuration = 0.f;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	// �÷��̾��� Ÿ�� ���� �̵�
	if (isMove)
	{
		if (moveTimer < moveDuration)
		{
			Translate((sf::Vector2f)direction * speed * dt);
			moveTimer += dt;
		}
		else
		{
			currentPosition = nextPosition;
			SetPosition(currentPosition);
			gridIndex += direction;
			moveTimer = 0.f;
			isMove = false;
		}
	}
	else
	{
		direction = InputMgr::GetAxisOne();
		if (!tileMap->GetTileData(gridIndex.x + direction.x, gridIndex.y + direction.y)->isPassable)
		{
			direction = { 0, 0 };
		}
		else
		{
			nextPosition = tileMap->GetGridPosition(gridIndex.x + direction.x, gridIndex.y + direction.y);
			if (nextPosition != currentPosition)
			{
				moveDuration = Utils::Magnitude(nextPosition - currentPosition) / speed;
				isMove = true;
			}
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
