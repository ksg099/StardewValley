#include "pch.h"
#include "TileMapSave.h"

TileMapSave::TileMapSave(DataTable::Types type) : DataTable(type)
{
}

TileMapSave::~TileMapSave()
{
	Release();
}

std::vector<TileData*>* TileMapSave::Get(const std::string& name)
{
	auto find = tileMaps.find(name);
	if (find == tileMaps.end())
	{
		std::cout << "해당 이름의 타일맵이 없습니다." << std::endl;
		return nullptr;
	}
	return find->second;
}

sf::Vector2i* TileMapSave::GetTileMapSize(const std::string& name)
{
	auto find = tileMapsSize.find(name);
	if (find == tileMapsSize.end())
	{
		std::cout << "해당 이름의 타일맵이 없습니다." << std::endl;
		return nullptr;
	}
	return &find->second;
}

void TileMapSave::Save(rapidjson::Document& doc)
{
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value tileMapArr(rapidjson::kArrayType);

	for (auto tileMap : tileMaps)
	{
		if (tileMap.second == nullptr)
			continue;

		rapidjson::Value tileMapData;
		tileMapData.SetObject();

		std::string tileMapName = tileMap.first;

		tileMapData.AddMember("TileMap Name", rapidjson::Value(tileMapName.c_str(), allocator), allocator);
		tileMapData.AddMember("Tile Count X", GetTileMapSize(tileMapName)->x, allocator);
		tileMapData.AddMember("Tile Count Y", GetTileMapSize(tileMapName)->y, allocator);

		rapidjson::Value tileArr(rapidjson::kArrayType);

		for (auto tile : *tileMap.second)
		{
			if (tile == nullptr)
				continue;

			rapidjson::Value tileData;
			tileData.SetObject();
			tileData.AddMember("Index X", tile->indexX, allocator);
			tileData.AddMember("Index Y", tile->indexY, allocator);
			tileData.AddMember("Ground Type", (int)tile->groundType, allocator);
			tileData.AddMember("Ground ID", tile->groundId, allocator);
			tileData.AddMember("Floor Type", (int)tile->floorType, allocator);
			tileData.AddMember("Floor ID", tile->floorId, allocator);
			tileData.AddMember("Object Type", (int)tile->objectType, allocator);
			tileData.AddMember("Object ID", tile->objectId, allocator);
			tileData.AddMember("Placed Possible", tile->isPossiblePlace, allocator);
			tileData.AddMember("Player Passable", tile->isPassable, allocator);

			tileArr.PushBack(tileData, allocator);
		}
		tileMapData.AddMember("Tiles", tileArr, allocator);

		tileMapArr.PushBack(tileMapData, allocator);
	}

	doc.AddMember("Tile Map", tileMapArr, allocator);
}

bool TileMapSave::Load(rapidjson::Document& doc)
{
	Release();

	auto infoArr = doc["Tile Map"].GetArray();
	int tileMapCount = infoArr.Size();
	for (int i = 0; i < tileMapCount; ++i)
	{
		std::string tileMapName = infoArr[i]["TileMap Name"].GetString();
		int countX = infoArr[i]["Tile Count X"].GetInt();
		int countY = infoArr[i]["Tile Count Y"].GetInt();
		tileMapsSize[tileMapName] = sf::Vector2i(countX, countY);

		auto tileMapArr = infoArr[i]["Tiles"].GetArray();
		if (countX * countY != tileMapArr.Size())
			return false;

		std::vector<TileData*>* tileMap = new std::vector<TileData*>;
		tileMaps[tileMapName] = tileMap;

		for (int j = 0; j < tileMapArr.Size(); ++j)
		{
			TileData* tileData = new TileData;
			tileData->indexX = tileMapArr[j]["Index X"].GetInt();
			tileData->indexY = tileMapArr[j]["Index Y"].GetInt();
			tileData->groundType = (GroundType)tileMapArr[j]["Ground Type"].GetInt();
			tileData->groundId = tileMapArr[j]["Ground ID"].GetInt();
			tileData->floorType = (FloorType)tileMapArr[j]["Floor Type"].GetInt();
			tileData->floorId = tileMapArr[j]["Floor ID"].GetInt();
			tileData->objectType = (ObjectType)tileMapArr[j]["Object Type"].GetInt();
			tileData->objectId = tileMapArr[j]["Object ID"].GetInt();
			tileData->isPossiblePlace = tileMapArr[j]["Placed Possible"].GetBool();
			tileData->isPassable = tileMapArr[j]["Player Passable"].GetBool();

			tileMap->push_back(tileData);
		}
	}

	return true;
}

void TileMapSave::Release()
{
	for (auto tileMap : tileMaps)
	{
		if (tileMap.second == nullptr)
			continue;

		for (auto tile : *tileMap.second)
		{
			if (tile == nullptr)
				continue;

			delete tile;
			tile = nullptr;
		}

		delete tileMap.second;
		tileMap.second = nullptr;
	}

	tileMaps.clear();
}
