#include "pch.h"
#include "Player.h"
#include "TileMap.h"
#include "ObjectOnTile.h"

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

	// 플레이어의 타일 단위 이동
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
		sf::Vector2i dir = InputMgr::GetAxisOne();
		if (dir != sf::Vector2i(0, 0))
		{
			if (!tileMap->GetTileData(gridIndex.x + dir.x, gridIndex.y + dir.y)->isPassable)
			{
				lookDir = dir;
				direction = { 0, 0 };
			}
			else
			{
				lookDir = dir;
				direction = dir;

				nextPosition = tileMap->GetGridPosition(gridIndex.x + direction.x, gridIndex.y + direction.y);
				if (nextPosition != currentPosition)
				{
					moveDuration = Utils::Magnitude(nextPosition - currentPosition) / speed;
					isMove = true;
				}
			}
		}
	}


	// 플레이어 좌클릭 처리
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		ObjectOnTile* obj = tileMap->GetTileData(gridIndex.x + lookDir.x, gridIndex.y + lookDir.y)->object;
		if(obj != nullptr)
		{
			tileMap->SetPlayerPassable(gridIndex.x + lookDir.x, gridIndex.y + lookDir.y, true);
			obj->InteractWithPlayer();
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
