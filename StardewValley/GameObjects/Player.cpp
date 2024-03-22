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
	SetOrigin(Origins::BC);
	playerHalfWidth = GetLocalBounds().width / 2.f;
}

void Player::Release()
{
	SpriteGo::Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	hasHitBox = true;

	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));

	gridIndex = { 1, 0 };
	currentGridPosition = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
	SetPosition(currentGridPosition);
	isMove = false;
	freeMoveDirection = { 0, 0 };
	moveTimer = 0.f;
	moveDuration = 0.f;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	// 플레이어의 타일 단위 이동
	// MoveTileUnit(dt);

	// 플레이어의 자유 이동
	freeMoveDirection.x = InputMgr::GetAxisRaw(Axis::Horizontal);
	freeMoveDirection.y = InputMgr::GetAxisRaw(Axis::Vertical);
	
	sf::Vector2f prevPosition = position;
	sf::Vector2f nextPosition = position + (freeMoveDirection * speed * dt);

	if (nextPosition.x - playerHalfWidth < currentGridPosition.x - tileMap->GetCellSize().x / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x - 1, gridIndex.y))
			nextPosition.x = prevPosition.x;
	}
	else if (nextPosition.x + playerHalfWidth > currentGridPosition.x + tileMap->GetCellSize().x / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x + 1, gridIndex.y))
			nextPosition.x = prevPosition.x;
	}
	if (nextPosition.y < currentGridPosition.y - tileMap->GetCellSize().y / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x, gridIndex.y - 1))
			nextPosition.y = prevPosition.y;
	}
	else if (nextPosition.y > currentGridPosition.y + tileMap->GetCellSize().y / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x, gridIndex.y + 1))
			nextPosition.y = prevPosition.y;
	}

	if (nextPosition.x < currentGridPosition.x - tileMap->GetCellSize().x / 2.f && gridIndex.x > 0)
	{
		--gridIndex.x;
	}
	else if (nextPosition.x > currentGridPosition.x + tileMap->GetCellSize().x / 2.f && gridIndex.x < tileMap->GetCellCount().x - 1)
	{
		++gridIndex.x;
	}
	if (nextPosition.y < currentGridPosition.y - tileMap->GetCellSize().y / 2.f && gridIndex.y > 0)
	{
		--gridIndex.y;
	}
	else if (nextPosition.y > currentGridPosition.y + tileMap->GetCellSize().y / 2.f && gridIndex.y < tileMap->GetCellCount().y - 1)
	{
		++gridIndex.y;
	}
	// std::cout << gridIndex.x << ", " << gridIndex.y << std::endl;
	currentGridPosition = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
	SetPosition(nextPosition);
	


	// 플레이어 좌클릭 처리
	/*if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		ObjectOnTile* obj = tileMap->GetTileData(gridIndex.x + lookDir.x, gridIndex.y + lookDir.y)->object;
		if(obj != nullptr)
		{
			tileMap->SetPlayerPassable(gridIndex.x + lookDir.x, gridIndex.y + lookDir.y, true);
			obj->InteractWithPlayer();
		}
	}*/
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::MoveTileUnit(float dt)
{
	if (isMove)
	{
		if (moveTimer < moveDuration)
		{
			Translate((sf::Vector2f)tileMoveDirection * speed * dt);
			moveTimer += dt;
		}
		else
		{
			currentGridPosition = nextGridPosition;
			SetPosition(currentGridPosition);
			gridIndex += tileMoveDirection;
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
				tileMoveDirection = { 0, 0 };
			}
			else
			{
				lookDir = dir;
				tileMoveDirection = dir;

				nextGridPosition = tileMap->GetGridPosition(gridIndex.x + tileMoveDirection.x, gridIndex.y + tileMoveDirection.y);
				if (nextGridPosition != currentGridPosition)
				{
					moveDuration = Utils::Magnitude(nextGridPosition - currentGridPosition) / speed;
					isMove = true;
				}
			}
		}
	}
}
