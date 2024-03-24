#include "pch.h"
#include "TileMap.h"
#include "ObjectOnTile.h"
#include "FloorOnTile.h"

TileMap::TileMap(const std::string& name) : GameObject(name)
{
}

sf::FloatRect TileMap::GetLocalBounds()
{
	sf::FloatRect bounds = va.getBounds();
	bounds.left = origin.x;
	bounds.top = origin.y;
	return bounds;
}

sf::FloatRect TileMap::GetGlobalBounds()
{
	sf::FloatRect bounds = va.getBounds();
	return transform.transformRect(bounds);
}

const sf::Vector2f& TileMap::GetGridPosition(int x, int y) const
{
	x = Utils::Clamp(x, 0, cellCount.x - 1);
	y = Utils::Clamp(y, 0, cellCount.y - 1);

	return transform.transformPoint(va[(y * cellCount.x + x) * 4].position) + cellSize / 2.f;
}

const TileData* TileMap::GetTileData(int x, int y) const
{
	if (IsOutOfRange(x, y))
	{
		return nullptr;
	}

	return tiles[y * cellCount.x + x];
}

void TileMap::Set(const sf::Vector2i& count, const sf::Vector2f& size)
{
	cellCount = count;
	cellSize = size;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4);

	sf::Vector2f posOffsets[4] = {
		{ 0, 0 },
		{ size.x, 0 },
		{ size.x, size.y },
		{ 0, size.y }
	};

	sf::Vector2f texCoord0[4] = {
		{ 0, 0 },
		{ 50.f, 0 },
		{ 50.f, 50.f },
		{ 0, 50.f }
	};

	for (int i = 0; i < count.y; i++)
	{
		for (int j = 0; j < count.x; j++)
		{
			int texIndex = Utils::RandomRange(0, 3);
			if (i == 0 || i == count.y - 1 || j == 0 || j == count.x - 1) // 외곽
			{
				texIndex = 3;
			}

			int quadIndex = i * count.x + j; // 2차원 인덱스를 1차원 인덱스로 변환
			sf::Vector2f quadPos(size.x * j, size.y * i);

			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = (quadIndex * 4) + k;
				va[vertexIndex].position = quadPos + posOffsets[k];
				va[vertexIndex].texCoords = texCoord0[k];
				va[vertexIndex].texCoords.y += texIndex * 50.f;
			}
		}
	}
}

void TileMap::SetSpriteSheetId(const std::string& id)
{
	spriteSheetId = id;
	texture = &RES_MGR_TEXTURE.Get(spriteSheetId);
}

void TileMap::UpdateTransform()
{
	transform = sf::Transform::Identity; // 기본 변환

	float scaleX = isFlipX ? -scale.x : scale.x;
	float scaleY = isFlipY ? -scale.y : scale.y;
	transform.scale(scaleX, scaleY, position.x, position.y);
	transform.rotate(rotation, position.x, position.y);
	transform.translate(position - origin);

	for (auto tile : tiles)
	{
		if (tile->object != nullptr)
		{
			tile->object->SetPosition(GetGridPosition(tile->indexX, tile->indexY));
		}
		if (tile->floor != nullptr)
		{
			tile->floor->SetPosition(GetGridPosition(tile->indexX, tile->indexY));
		}
	}
}

void TileMap::SetOrigin(Origins preset)
{
	if (preset == Origins::Custom)
		return;

	originPreset = preset;
	sf::Vector2f newOrigin(va.getBounds().width, va.getBounds().height);
	newOrigin.x *= ((int)originPreset % 3) * 0.5f;
	newOrigin.y *= ((int)originPreset / 3) * 0.5f;
	
	origin = newOrigin;

	UpdateTransform();
	
	/*for (int i = 0; i < va.getVertexCount(); i++)
	{
		va[i].position -= origin;
	}*/
}

void TileMap::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;

	origin = newOrigin;

	UpdateTransform();

	/*for (int i = 0; i < va.getVertexCount(); i++)
	{
		va[i].position -= origin;
	}*/
}

void TileMap::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	UpdateTransform();

	/*sf::Vector2f delta = pos - position;
	for (int i = 0; i < va.getVertexCount(); i++)
	{
		va[i].position += delta;
	}
	position = pos;*/
}

void TileMap::Translate(const sf::Vector2f& delta)
{
	GameObject::Translate(delta);
	UpdateTransform();
}

void TileMap::SetRotation(const float r)
{
	GameObject::SetRotation(r);
	UpdateTransform();
}

void TileMap::SetScale(const sf::Vector2f& scale)
{
	GameObject::SetScale(scale);
	UpdateTransform();
}

void TileMap::SetFlipX(bool flip)
{
	GameObject::SetFlipX(flip);
	UpdateTransform();
}

void TileMap::SetFlipY(bool flip)
{
	GameObject::SetFlipY(flip);
	UpdateTransform();
}

void TileMap::Init()
{
	GameObject::Init();

	// SetSpriteSheetId("graphics/background_sheet.png");
	// Set({ 10, 10 }, { 50.f, 50.f });
}

void TileMap::Release()
{
	GameObject::Release();
}

void TileMap::Reset()
{
	GameObject::Reset();
}

void TileMap::Update(float dt)
{
	GameObject::Update(dt);
}

void TileMap::Draw(sf::RenderWindow& window)
{
	sf::RenderStates state;
	state.texture = texture;
	state.transform = transform;

	window.draw(va, state);

	for (auto tile : tiles)
	{
		if (tile->floor != nullptr && tile->floor->GetActive())
		{
			tile->floor->Draw(window);
		}
		if (tile->object != nullptr && tile->object->GetActive())
		{
			tile->object->Draw(window);
		}
	}
}

void TileMap::LoadTileMap(rapidjson::Document& doc, const sf::Vector2f& tileSize)
{
	cellCount.x = doc["Tile Map"][0]["Tile Count X"].GetInt();
	cellCount.y = doc["Tile Map"][0]["Tile Count Y"].GetInt();
	SetSpriteSheetId(GROUND_TABLE->GetTextureId());

	cellSize = tileSize;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(cellCount.x * cellCount.y * 4);

	sf::Vector2f posOffsets[4] = {
		{ 0, 0 },
		{ tileSize.x, 0 },
		{ tileSize.x, tileSize.y },
		{ 0, tileSize.y }
	};

	for (int i = 0; i < cellCount.y; i++)
	{
		for (int j = 0; j < cellCount.x; j++)
		{
			int quadIndex = i * cellCount.x + j; // 2차원 인덱스를 1차원 인덱스로 변환
			sf::Vector2f quadPos(tileSize.x * j, tileSize.y * i);



			TileData* tile = new TileData();
			tile->indexX = j;
			tile->indexY = i;
			tile->groundType = (GroundType)doc["Tile Map"][0]["Tiles"][quadIndex]["Ground Type"].GetInt();
			tile->groundId = doc["Tile Map"][0]["Tiles"][quadIndex]["Ground ID"].GetInt();

			FloorType floorType = (FloorType)doc["Tile Map"][0]["Tiles"][quadIndex]["Floor Type"].GetInt(); // 해당 타입을 갖는 FloorOnTile 객체 생성
			int floorId = doc["Tile Map"][0]["Tiles"][quadIndex]["Floor ID"].GetInt();
			tile->floor = CreateFloor(floorType, floorId);
			if (tile->floor != nullptr)
			{
				tile->floor->SetTileData(tile);
			}
			
			ObjectType objType = (ObjectType)doc["Tile Map"][0]["Tiles"][quadIndex]["Object Type"].GetInt();
			int objId = doc["Tile Map"][0]["Tiles"][quadIndex]["Object ID"].GetInt();
			ObjectOnTile* obj = nullptr;
			if (objType != ObjectType::NONE)
			{
				obj = new ObjectOnTile("Object");
				obj->SetObjectType(objType);
				obj->SetObjectId(objId);
				auto& objData = OBJECT_TABLE->Get(objType, objId);
				obj->SetTexture(objData.textureId);
				obj->SetTextureRect(sf::IntRect(objData.sheetId.x, objData.sheetId.y, objData.sheetSize.x, objData.sheetSize.y));
				obj->SetOrigin(Origins::MC);
			}
			tile->object = obj;
			if (tile->object != nullptr)
			{
				tile->object->SetTileData(tile);
			}

			tile->isPossiblePlace = doc["Tile Map"][0]["Tiles"][quadIndex]["Placed Possible"].GetBool();
			tile->isPassable = doc["Tile Map"][0]["Tiles"][quadIndex]["Player Passable"].GetBool();

			tiles.push_back(tile);

			sf::Vector2f sheetSize = (sf::Vector2f)GROUND_TABLE->Get(tile->groundType, tile->groundId).sheetSize;
			sf::Vector2f texCoord0[4] = {
				{ 0, 0 },
				{ sheetSize.x, 0 },
				{ sheetSize.x, sheetSize.y },
				{ 0, sheetSize.y }
			};



			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = (quadIndex * 4) + k;
				va[vertexIndex].position = quadPos + posOffsets[k];
				va[vertexIndex].texCoords = texCoord0[k];
				va[vertexIndex].texCoords.x += GROUND_TABLE->Get(tile->groundType, tile->groundId).sheetId.x;
				va[vertexIndex].texCoords.y += GROUND_TABLE->Get(tile->groundType, tile->groundId).sheetId.y;
			}
		}
	}
}

FloorOnTile* TileMap::CreateFloor(const FloorType type, const int id)
{
	if (type == FloorType::NONE || type == FloorType::COUNT)
		return nullptr;

	FloorOnTile* floor = new FloorOnTile("Floor");
	floor->SetFloorType(type);
	floor->SetFloorId(id);
	auto& floorData = FLOOR_TABLE->Get(type, id);
	floor->SetTexture(floorData.textureId);
	floor->SetTextureRect(sf::IntRect(floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y));
	floor->SetOrigin(Origins::MC);

	return floor;
}

void TileMap::InteractWithTile(const int x, const int y, const ItemType type, const int id)
{
	if (IsOutOfRange(x, y))
		return;

	TileData* tile = tiles[y * cellCount.x + x];
	if (tile->object != nullptr) // object 상호작용
	{
		std::pair<bool, bool> passAndPlaced = tile->object->InteractWithObject(type, id);
		tile->isPassable = passAndPlaced.first;
		tile->isPossiblePlace = passAndPlaced.second;
	}
	else if (tile->floor != nullptr) // floor 상호작용
	{

	}
	else // ground 상호작용
	{
		if (tile->groundType != GroundType::WATER && type == ItemType::Tool && id == 3)
		{
			tile->floor = CreateFloor(FloorType::DRIED_ARABLE_LAND, 0);
			tile->floor->SetPosition(GetGridPosition(tile->indexX, tile->indexY));
			tile->floor->SetTileData(tile);
		}
	}
}

void TileMap::SetPlayerPassable(int x, int y, bool isPassable)
{
	if (x < 0 || y < 0 || x > cellCount.x - 1 || y > cellCount.y - 1)
		return;

	tiles[y * cellCount.x + x]->isPassable = isPassable;
}

bool TileMap::IsPassable(int x, int y)
{
	if (IsOutOfRange(x, y))
	{
		return false;
	}

	return tiles[y * cellCount.x + x]->isPassable;
}
