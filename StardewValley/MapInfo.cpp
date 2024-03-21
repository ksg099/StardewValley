#include "pch.h"
#include "MapInfo.h"
#include "SpriteGo.h"
#include <cstdio> // C 표준 입출력 관련 함수
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <commdlg.h> // 공통 대화 상자 함수를 위한 헤더

void MapInfo::LoadFromFile(const std::string& filePath, const float setOutlineThickness)
{
	std::ifstream file(filePath);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string contents = buffer.str();

	rapidjson::Document doc;
	if (doc.Parse(contents.c_str()).HasParseError())
	{
		return; // 파싱 실패 처리
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

		// 텍스처 파일 경로가 있으면 텍스처 로드
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
	Document::AllocatorType& allocator = doc.GetAllocator(); // 메모리 할당자 획득

	// 타일맵 크기 저장
	doc.AddMember("tileSizeX", Value(tileSize.x), allocator);
	doc.AddMember("tileSizeY", Value(tileSize.y), allocator);
	doc.AddMember("mapSizeX", Value(tileMap.x), allocator);
	doc.AddMember("mapSizeY", Value(tileMap.y), allocator);

	// 각 타일의 정보를 담을 JSON 배열 객체 생성
	Value tilesArray(kArrayType);

	for (const auto& row : tiles)
	{
		for (const auto& tile : row)
		{
			Value tileObject(kObjectType); // 개별 타일 정보를 담을 JSON 객체 생성
			tileObject.AddMember("type", (int)tile.type, allocator); // 타일 타입 정보 추가
			// rapidjson 은 std::string 호환이 안되서 const char* 으로 넘겨줘야함 
			// TODO : 임시방편으로 상대경로로 변환뒤 저장
			std::string relativePath = Utils::ConvertToRelativePath(tile.textureFilePath);
			tileObject.AddMember("texture FilePath", rapidjson::Value(relativePath.c_str(), allocator), allocator);
			tileObject.AddMember("x Pos", tile.shape.getPosition().x, allocator);
			tileObject.AddMember("y Pos", tile.shape.getPosition().y, allocator);

			// 타일 객체를 타일 배열에 추가
			tilesArray.PushBack(tileObject, allocator);
		}
	}

	// 타일 배열들 저장
	doc.AddMember("tiles", tilesArray, allocator);

	// 이제 파일을 저장해야함
	std::wstring savePath = Utils::OpenSaveFileDialog();
	if (savePath.empty())
	{
		return; // 취소할 경우 return
	}

	// JSON 문서를 파일에 쓰기
	// TODO : fopen 수정하기
	std::string sSavePath = Utils::WSTRINGToString(savePath);
	FILE* fp = fopen(sSavePath.c_str(), "wb");

	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	doc.Accept(writer);
	fclose(fp);
}
