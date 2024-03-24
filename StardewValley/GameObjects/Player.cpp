#include "pch.h"
#include "Player.h"
#include "TileMap.h"
#include "ObjectOnTile.h"
#include "Inventory.h"

Player::Player(const std::string& name) : SpriteGo(name)
{
}

void Player::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/player.png");
	SetTextureRect(sf::IntRect(0, 0, 14, 27));
	SetOrigin(Origins::BC);
	playerHalfWidth = GetLocalBounds().width / 2.f;
	animator.SetTarget(&sprite);
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
	inventory = dynamic_cast<Inventory*>(SCENE_MGR.GetCurrentScene()->FindGo("Inventory"));

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
	animator.Update(dt);

	// 플레이어의 타일 단위 이동
	// MoveTileUnit(dt);

	// 플레이어의 자유 이동
	freeMoveDirection.x = InputMgr::GetAxisRaw(Axis::Horizontal);
	freeMoveDirection.y = InputMgr::GetAxisRaw(Axis::Vertical);
	
	sf::Vector2f prevPosition = position;
	sf::Vector2f nextPosition = position + (freeMoveDirection * speed * dt);

	CheckCollision(nextPosition, prevPosition);
	ChangeGridIndex(nextPosition);
	SetPosition(nextPosition);

	// TO-DO: 애니메이션은 임시로만 구현하고, 자세한건 나중에
	sf::Vector2f posDiff = nextPosition - prevPosition;
	if (posDiff.x != 0.f)
	{
		animator.Play("animations/PlayerMoveSide.csv");

		if (posDiff.x < 0)
			SetFlipX(true);
		else
			SetFlipX(false);
	}
	if (posDiff.y > 0)
	{
		animator.Play("animations/PlayerMoveFront.csv");
	}
	else if (posDiff.y < 0)
	{
		animator.Play("animations/PlayerMoveBack.csv");
	}

	// 퀵슬롯으로부터 아이템 셋팅
	auto numKey = InputMgr::GetKeyNumber();
	if (numKey.first)
	{
		int num = numKey.second - 1;
		if (num == -1)
			num = 9;
		itemInUse = inventory->GetItemData(num, inventory->GetSubSlotIndexY());
		if (itemInUse == nullptr)
		{
			std::cout << "아이템 없음" << std::endl;
		}
		else
		{
			std::cout << "box ID: " << itemInUse->BoxId << ", index X: " << itemInUse->IndexX << ", index Y: " << itemInUse->IndexY << std::endl;
			std::cout << "Item Type: " << (int)itemInUse->type << ", Item ID: " << itemInUse->itemId << std::endl;
		}
	}

	// 플레이어 좌클릭 처리
	sf::Vector2i dirOne = InputMgr::GetAxisOne();
	if (dirOne != sf::Vector2i(0, 0))
	{
		lookDir = dirOne;
	}
	
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

void Player::CheckCollision(sf::Vector2f& nextPos, sf::Vector2f& prevPos)
{
	if (nextPos.x - playerHalfWidth < currentGridPosition.x - tileMap->GetCellSize().x / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x - 1, gridIndex.y))
			nextPos.x = prevPos.x;
	}
	else if (nextPos.x + playerHalfWidth > currentGridPosition.x + tileMap->GetCellSize().x / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x + 1, gridIndex.y))
			nextPos.x = prevPos.x;
	}
	if (nextPos.y < currentGridPosition.y - tileMap->GetCellSize().y / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x, gridIndex.y - 1))
			nextPos.y = prevPos.y;
	}
	else if (nextPos.y > currentGridPosition.y + tileMap->GetCellSize().y / 2.f)
	{
		if (!tileMap->IsPassable(gridIndex.x, gridIndex.y + 1))
			nextPos.y = prevPos.y;
	}
}

void Player::ChangeGridIndex(sf::Vector2f& nextPos)
{
	if (nextPos.x < currentGridPosition.x - tileMap->GetCellSize().x / 2.f && gridIndex.x > 0)
	{
		--gridIndex.x;
	}
	else if (nextPos.x > currentGridPosition.x + tileMap->GetCellSize().x / 2.f && gridIndex.x < tileMap->GetCellCount().x - 1)
	{
		++gridIndex.x;
	}
	if (nextPos.y < currentGridPosition.y - tileMap->GetCellSize().y / 2.f && gridIndex.y > 0)
	{
		--gridIndex.y;
	}
	else if (nextPos.y > currentGridPosition.y + tileMap->GetCellSize().y / 2.f && gridIndex.y < tileMap->GetCellCount().y - 1)
	{
		++gridIndex.y;
	}
	// std::cout << gridIndex.x << ", " << gridIndex.y << std::endl;
	currentGridPosition = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
}
