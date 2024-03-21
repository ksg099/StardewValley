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
	int BoxId;
	int IndexX;
	int IndexY;
	int itemId;
	int instanceId;
	int count = 0;
	bool canOverLap;

	std::string itemFilePath; 
};