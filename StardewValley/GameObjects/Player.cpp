#include "pch.h"
#include "Player.h"
#include "TileMap.h"
#include "ObjectOnTile.h"
#include "Inventory.h"
#include "SceneGame.h"
#include "Scene.h"
#include "ChangeHarvest.h"

Player::Player(const std::string& name) : SpriteGo(name)
{
}

void Player::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/player_edit.png");
	SetTextureRect(sf::IntRect(4, 1, 15, 30));
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
	SceneGame* currentScene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene()->FindGo("newSeed"));

	gridIndex = { 1, 0 };
	currentGridPosition = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
	SetPosition(currentGridPosition);
	isMove = false;
	freeMoveDirection = { 0, 0 };
	moveTimer = 0.f;
	moveDuration = 0.f;

	animator.Play("animations/PlayerMoveSide.csv");

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
	PlayMoveAnimation(posDiff);

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

		// TO-DO: 텍스처 처리(나중에 수정)
		if (lookDir.x < 0)
		{
			SetFlipX(true);
			SetTextureRect(sf::IntRect(4, 34, 15, 32));
		}
		else if (lookDir.x > 0)
		{
			SetFlipX(false);
			SetTextureRect(sf::IntRect(4, 34, 15, 32));
		}
		else if (lookDir.y > 0)
		{
			SetTextureRect(sf::IntRect(4, 70, 16, 27));
		}
		else if (lookDir.y < 0)
		{
			SetTextureRect(sf::IntRect(4, 1, 15, 30));
		}
	}
	
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (itemInUse != nullptr)
			tileMap->InteractWithTile(gridIndex.x + lookDir.x, gridIndex.y + lookDir.y, itemInUse->type, itemInUse->itemId);
		else
			tileMap->InteractWithTile(gridIndex.x + lookDir.x, gridIndex.y + lookDir.y, ItemType::None, -1);
	}

	SetPlant();

}

void Player::SetTextureRect(const sf::IntRect& rect)
{
	SpriteGo::SetTextureRect(rect);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::PlayMoveAnimation(sf::Vector2f posDiff)
{
	if (posDiff.x > 0.f && side != Sides::Right)
	{
		animator.Play("animations/PlayerMoveSide.csv");
		SetFlipX(false);
		side = Sides::Right;
	}
	else if (posDiff.x < 0.f && side != Sides::Left)
	{
		animator.Play("animations/PlayerMoveSide.csv");
		SetFlipX(true);
		side = Sides::Left;
	}
	else if (posDiff.y > 0 && side != Sides::Down)
	{
		animator.Play("animations/PlayerMoveFront.csv");
		side = Sides::Down;
	}
	else if (posDiff.y < 0 && side != Sides::Up)
	{
		animator.Play("animations/PlayerMoveBack.csv");
		side = Sides::Up;
	}
	else if (posDiff.x == 0 && posDiff.y == 0)
	{
		animator.Stop();
		switch (side)
		{
		case Sides::Left:
			SetFlipX(true);
			SetTextureRect(sf::IntRect(4, 34, 15, 32));
			break;
		case Sides::Right:
			SetFlipX(false);
			SetTextureRect(sf::IntRect(4, 34, 15, 32));
			break;
		case Sides::Up:
			SetTextureRect(sf::IntRect(21, 70, 16, 27));
			break;
		case Sides::Down:
			SetTextureRect(sf::IntRect(4, 1, 15, 30));
			break;
		default:
			break;
		}
		side = Sides::None;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Z))
	{

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

void Player::SetPlant()
{
	//cauliflower 심기
	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		//std::cout << gridIndex.x <<  gridIndex.y << std::endl;
		TileData* tiledata = tileMap->GetTileData(gridIndex.x, gridIndex.y);
		tiledata->objectType = ObjectType::CROPS;
		tiledata->objectId = 0;

		ChangeHarvest* cauliflower = new ChangeHarvest("changeHarvest");
		cauliflower->SetData(&HARVEST_TABLE->Get(HarvestType::CAULIFLOWER));
		cauliflower->SetObjectType(tiledata->objectType);
		cauliflower->SetObjectId(tiledata->objectId);
		cauliflower->SetPosition(tileMap->GetGridPosition(gridIndex.x, gridIndex.y));

		tiledata->object = cauliflower;

		cauliflower->Init();
		cauliflower->Reset();
	}

	//PARSNIP 심기
	if (InputMgr::GetKeyDown(sf::Keyboard::V))
	{
		TileData* tiledata = tileMap->GetTileData(gridIndex.x, gridIndex.y);
		tiledata->objectType = ObjectType::CROPS;
		tiledata->objectId = 6; // 6: Parsnip1

		ChangeHarvest* parsnip = new ChangeHarvest("changeHarvest");
		parsnip->SetData(&HARVEST_TABLE->Get(HarvestType::PARSNIP));
		parsnip->SetObjectType(tiledata->objectType);
		parsnip->SetObjectId(tiledata->objectId);
		parsnip->SetPosition(tileMap->GetGridPosition(gridIndex.x, gridIndex.y));

		tiledata->object = parsnip;

		//auto& objData = OBJECT_TABLE->Get(ObjectType::CROPS, (int)HarvestType::CAULIFLOWER);
		//obj->SetTexture(objData.textureId);
		//obj->SetTextureRect(sf::IntRect(objData.sheetId.x, objData.sheetId.y, objData.sheetSize.x, objData.sheetSize.y));
		//obj->SetOrigin(Origins::MC);
		parsnip->Init();
		parsnip->Reset();
	}
}
