#pragma once

class ObjectOnTile;
class FloorOnTile;

enum class Origins
{
	// T M B
	// L C R
	TL, TC, TR, ML, MC, MR, BL, BC, BR, Custom, Object
};

enum class Sides
{
	None = -1, Left, Right, Up, Down, Count
};

enum class Languages
{
	Korean, English
};


enum class ItemType
{
	None = -1,
	Tool,
	Seed,
	Harvest,
	Ingredient,
};

struct ItemData
{
	ItemType type;
	int itemId;

	int instanceId;
	int count = 0;
	bool canOverLap;

	int BoxId;
	int IndexX;
	int IndexY;

	std::string itemFilePath; 
};

enum class TileType
{
	NONE = -1,
	Ground,
	Floor,
	Object,
	COUNT,
};

enum class GroundType
{
	NONE = -1,
	WATER,
	DIRT,
	GRASS,
	COUNT,
};

enum class FloorType
{
	NONE = -1,
	DRIED_ARABLE_LAND,
	WET_ARABLE_LAND,
	WOOD_PATH,
	STONE_PATH,
	COUNT,
};

enum class ObjectType
{
	NONE = -1,
	STONE,
	TREE,
	BRANCH,
	STUMP,
	WEED,
	CROPS,
	FURNITURE,
	BOX,
	WALL,
	BUILDING,
	COUNT,
};

enum class HarvestType
{
	NONE = -1,
	CAULIFLOWER,
	PARSNIP,
	COUNT
};

struct TileData
{
	int indexX = 0;
	int indexY = 0;
	GroundType groundType = GroundType::DIRT;
	int groundId = 0;
	FloorOnTile* floor = nullptr;
	FloorType floorType = FloorType::NONE;
	int floorId = 0;
	ObjectOnTile* object = nullptr;
	ObjectType objectType = ObjectType::NONE;
	int objectId = 0;
	bool isPossiblePlace = true;
	bool isPassable = true;
};

struct hash_tuple {
	template <class T1, class T2>
	std::size_t operator () (const std::tuple<T1, T2>& tuple) const {
		auto hash1 = std::hash<T1>{}(std::get<0>(tuple));
		auto hash2 = std::hash<T2>{}(std::get<1>(tuple));
		return hash1 ^ hash2;
	}
};