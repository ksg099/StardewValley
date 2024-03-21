#pragma once

class ObjectOnTile;

enum class Origins
{
	// T M B
	// L C R
	TL, TC, TR, ML, MC, MR, BL, BC, BR, Custom,
};

enum class Sides
{
	None = -1, Left, Right, Count
};

enum class Languages
{
	Korean, English
};


enum class ItemType
{
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

	std::string itemFilePath; 
};

enum class GroundType
{
	WATER,
	DIRT,
	GRASS,
};

enum class FloorType
{
	NONE,
	DRIED_ARABLE_LAND,
	WET_ARABLE_LAND,
	WOOD_PATH,
	STONE_PATH,
};

enum class ObjectType
{
	NONE = -1,
	STONE,
	TREE,
	WEED,
	CROPS,
	FURNITURE,
	BOX,
	WALL,
	BUILDING,
};

struct TileData
{
	int indexX = 0;
	int indexY = 0;
	GroundType groundType = GroundType::DIRT;
	int groundId = 0;
	// FloorOnTile* floor = nullptr;
	// int floorId = 0;
	ObjectOnTile* object = nullptr;
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