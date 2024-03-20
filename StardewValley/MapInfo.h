#pragma once
enum class GroundType
{

};

enum class FloorType
{

};

struct MapInfo
{
	int indexX;
	int indexY;
	GroundType groundType;
	int groundSheetIDX;
	int groundSheetIDY;
	FloorType floorType;
	bool placedPossible;
	bool playerPassable;

	void LoadFromFile(const std::string& filePath, const float setOutlineThickness);
	void SaveToFile(const std::string& filePath);
};

