#include "pch.h"
#include "MapInfo.h"
#include "SpriteGo.h"
#include <cstdio> // C ǥ�� ����� ���� �Լ�
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <commdlg.h> // ���� ��ȭ ���� �Լ��� ���� ���

void MapInfo::LoadFromFile(const std::string& filePath, const float setOutlineThickness)
{
	std::ifstream file(filePath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string contents = buffer.str();

	rapidjson::Document doc;
	if (doc.Parse(contents.c_str()).HasParseError())
	{
		return; // �Ľ� ���� ó��
	}

	tileSize.x = doc["tileSizeX"].GetFloat();
	tileSize.y = doc["tileSizeY"].GetFloat();
	tileMap.x = doc["mapSizeX"].GetInt();
	tileMap.y = doc["mapSizeY"].GetInt();

	const rapidjson::Value& tilesArray = doc["tiles"];
	tiles.resize(tileMap.y, std::vector<Tile>(tileMap.x));

	SetCurrentDirectory(currentDirectory);

	for (rapidjson::SizeType i = 0; i < tilesArray.Size(); i++)
	{
		const rapidjson::Value& tileObject = tilesArray[i];

		int x = tileObject["x Pos"].GetFloat() / tileSize.x;
		int y = tileObject["y Pos"].GetFloat() / tileSize.y;

		TileType type = static_cast<TileType>(tileObject["type"].GetInt());
		std::string textureFilePath = tileObject["texture FilePath"].GetString();

		tiles[y][x] = Tile(sf::Vector2f(x * tileSize.x, y * tileSize.y), tileSize, (int)type);
		tiles[y][x].textureFilePath = textureFilePath;

		// �ؽ�ó ���� ��ΰ� ������ �ؽ�ó �ε�
		if (!textureFilePath.empty())
		{
			if (tiles[y][x].texture.loadFromFile(textureFilePath))
			{
				//std::cout << "Load" << std::endl;
			}
			else
			{
				std::cout << "Loadfail" << std::endl;
			}
			tiles[y][x].shape.setTexture(&tiles[y][x].texture);
			tiles[y][x].shape.setFillColor(sf::Color::White);
		}
		else
		{
			tiles[y][x].shape.setFillColor(sf::Color::Transparent);
		}
		if (setOutlineThickness > 0.1f)
		{
			switch (tiles[y][x].type)
			{
			case TileType::WALL:
				tiles[y][x].shape.setOutlineColor(sf::Color::Red);
				break;
			case TileType::PASS:
				tiles[y][x].shape.setOutlineColor(sf::Color::White);
				break;
			case TileType::WALLNOGRAB:
				tiles[y][x].shape.setOutlineColor(sf::Color::Blue);
				break;
			}
		}
		tiles[y][x].shape.setOutlineThickness(setOutlineThickness);
	}
}

void MapInfo::SaveToFile(const std::string& filePath)
{
	using namespace rapidjson;

	rapidjson::Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator(); // �޸� �Ҵ��� ȹ��

	// Ÿ�ϸ� ũ�� ����
	doc.AddMember("tileSizeX", Value(tileSize.x), allocator);
	doc.AddMember("tileSizeY", Value(tileSize.y), allocator);
	doc.AddMember("mapSizeX", Value(tileMap.x), allocator);
	doc.AddMember("mapSizeY", Value(tileMap.y), allocator);

	// �� Ÿ���� ������ ���� JSON �迭 ��ü ����
	Value tilesArray(kArrayType);

	for (const auto& row : tiles)
	{
		for (const auto& tile : row)
		{
			Value tileObject(kObjectType); // ���� Ÿ�� ������ ���� JSON ��ü ����
			tileObject.AddMember("type", (int)tile.type, allocator); // Ÿ�� Ÿ�� ���� �߰�
			// rapidjson �� std::string ȣȯ�� �ȵǼ� const char* ���� �Ѱ������ 
			// TODO : �ӽù������� ����η� ��ȯ�� ����
			std::string relativePath = Utils::ConvertToRelativePath(tile.textureFilePath);
			tileObject.AddMember("texture FilePath", rapidjson::Value(relativePath.c_str(), allocator), allocator);
			tileObject.AddMember("x Pos", tile.shape.getPosition().x, allocator);
			tileObject.AddMember("y Pos", tile.shape.getPosition().y, allocator);

			// Ÿ�� ��ü�� Ÿ�� �迭�� �߰�
			tilesArray.PushBack(tileObject, allocator);
		}
	}

	// Ÿ�� �迭�� ����
	doc.AddMember("tiles", tilesArray, allocator);

	// ���� ������ �����ؾ���
	std::wstring savePath = Utils::OpenSaveFileDialog();
	if (savePath.empty())
	{
		return; // ����� ��� return
	}

	// JSON ������ ���Ͽ� ����
	// TODO : fopen �����ϱ�
	std::string sSavePath = Utils::WSTRINGToString(savePath);
	FILE* fp = fopen(sSavePath.c_str(), "wb");

	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	doc.Accept(writer);
	fclose(fp);
}
