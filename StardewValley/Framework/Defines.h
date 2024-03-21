#pragma once

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
	NONE,
	STONE,
	TREE,
	WEED,
	CROPS,
	FURNITURE,
	BOX,
	WALL,
	BUILDING,
};