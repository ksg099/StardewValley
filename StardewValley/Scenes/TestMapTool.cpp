#include "pch.h"
#include <Windows.h>
#include <filesystem>
#include "TestMapTool.h"
#include "MapToolUI.h"

namespace fs = std::filesystem;

std::string TestMapTool::ToRelativePath(const std::string& originalPath, const std::string& basePath)   //절대경로를 상대경로로 변경하기 위함
{
    fs::path relativePath = fs::relative(fs::path(originalPath), fs::path(basePath));
    return relativePath.string();
}

std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr)         //LPCWSTR 유형의 문자열을 std::string로 변환하기 위함
{
    int strLength
        = WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1,
            nullptr, 0, nullptr, nullptr);

    std::string str(strLength, 0);

    WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, &str[0],
        strLength, nullptr, nullptr);

    return str;
}

void TestMapTool::UpdateTransform()
{
    transform = sf::Transform::Identity;

    transform.scale(FRAMEWORK.GetWindowSize().x / worldView.getSize().x, FRAMEWORK.GetWindowSize().y / worldView.getSize().y, 0.f,0.f);

}

std::wstring TestMapTool::SaveFilePath()        //대화상자(폴더)를 열기 위한 함수
{
    OPENFILENAME ofn;
    wchar_t szFileName[MAX_PATH] = L"";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"json";

    if (GetSaveFileName(&ofn)) // 파일 저장 대화 상자를 표시
    {
        return ofn.lpstrFile; // 사용자가 지정한 파일 경로를 반환
    }
    return L"";
}

std::string TestMapTool::WstringToString(const std::wstring& var)
{
	static std::locale loc("");
	auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
	return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
}

TestMapTool::TestMapTool(SceneIds id)
    : Scene(id)
{
}

TestMapTool::~TestMapTool()
{
}

void TestMapTool::Init()
{
    DrawGrid();
    transform = sf::Transform::Identity;
    transform.scale(1.f, 1.f, 0.f, 0.f);
  
    mapToolUI = new MapToolUI("UI");
    AddGo(mapToolUI, Layers::Ui);

    mapData.resize(col * row);
    
    Scene::Init();
}

void TestMapTool::Release()
{
    Scene::Release();
}

void TestMapTool::Enter()
{
    Scene::Enter();
    sf::Vector2f windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
    worldView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);
    window.setView(worldView);
}

void TestMapTool::Exit()
{
    Scene::Exit();
}

void TestMapTool::Update(float dt)
{
    Scene::Update(dt);

    sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
    sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);

    timer += dt;
    if (timer > duration)
    {
        printf("WORLD : (%f, %f)\n", mouseWorldPos.x, mouseWorldPos.y);
        timer = 0.f;
    }

    float zoomAmount = 1.1f;

    sf::FloatRect worldMapBounds = { 15.f,15.f,(float)FRAMEWORK.GetWindowSize().x * 0.6f - 15.f, (float)FRAMEWORK.GetWindowSize().y - 15.f };
    if (worldMapBounds.contains(mouseWorldPos) && InputMgr::GetMouseWheelDown(sf::Mouse::VerticalWheel))
    {
        worldView.zoom(0.9f);
        UpdateTransform();
    }

    if (worldMapBounds.contains(mouseWorldPos) && InputMgr::GetMouseWheelUp(sf::Mouse::VerticalWheel))
    {
        worldView.zoom(1.1f);
        UpdateTransform();
    }

    if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        if (mapToolUI->GetSaveButtonGB().contains(mouseWorldPos))
        {
            //SaveMapContent();
            mapToolUI->isSelected = false;
        }
        if (mapToolUI->GetEraseButtonGB().contains(mouseWorldPos))
        {
            //배치되었던 오브젝트 지우기
            mapToolUI->isSelected = false;
        }
        if (mapToolUI->GetLoadButtonGB().contains(mouseWorldPos))
        {
            //저장했던 맵 정보 불러오기
            mapToolUI->isSelected = false;
        }
        if (mapToolUI->GetMoveScreenButtonGB().contains(mouseWorldPos))
        {
            //오브젝트 배치를 중지하고 격자 화면을 드래그앤 드롭해서 이동할 수 있도록
            mapToolUI->isSelected = false;
        }

        sf::FloatRect visibleMapBounds(15.f, 15.f, 1152.f, 1050.f);
        if (mapToolUI->isSelected && visibleMapBounds.contains(mouseWorldPos))
        {
            PlaceTileToIndex(SelectIndex(mouseWorldPos), mapToolUI->selectedTile);
        }
    }

    if (InputMgr::GetKeyDown(sf::Keyboard::X))
    {
        SaveMapContent();
    }
}

void TestMapTool::PlaceTileToIndex(int indexNum, MapSheet& tile)
{
    //같은 Type이면 이미 있던게 지워지고 새로 올린게 들어가야하는거고
    //다른 Type이면 ground -> floor -> object 순으로 위에 쌓아올릴 수 있어야 함
    if (indexNum >= 0 && indexNum <= col * row )
    {
        TileLayer cell;

        auto groundData = GROUND_TABLE->Get(tile.groundType, tile.objectID);
        auto floorData = FLOOR_TABLE->Get(tile.floorType, tile.objectID);
        auto objectData = OBJECT_TABLE->Get(tile.objectType, tile.objectID);

        switch (tile.tileType)
        {
        case TileType::Ground:
            cell.groundLayer.ID = tile.objectID;
            cell.groundLayer.resource = GROUND_TABLE->GetTextureId();
            cell.groundLayer.sheetID_X = groundData.sheetId.x;
            cell.groundLayer.sheetID_Y = groundData.sheetId.y;
            cell.groundLayer.sheetID_W = groundData.sheetSize.x;
            cell.groundLayer.sheetID_H = groundData.sheetSize.y;
            cell.groundLayer.tileSprite = tile.originalSprite;
            cell.groundLayer.worldIndexNum = indexNum;
            cell.groundLayer.tileType = tile.tileType;
            cell.groundLayer.groundType = tile.groundType;
            cell.groundLayer.tileSprite.setOrigin(cell.groundLayer.tileSprite.getLocalBounds().width * 0.5f, cell.groundLayer.tileSprite.getLocalBounds().height * 0.5f);
            cell.groundLayer.tileSprite.setPosition(IndexToPos(indexNum));
            cell.IndexX = indexNum / row + indexNum % row;
            cell.IndexY = indexNum / col + indexNum % col;
            if (tile.groundType == GroundType::WATER)
            {
                cell.placedPossible = false;
                cell.playerPassable = false;
            }
            else
            {
                cell.placedPossible = true;
                cell.playerPassable = true;
            }
            mapData[indexNum].groundLayer = cell.groundLayer;
            break;
        case TileType::Floor:
            cell.floorLayer.ID = tile.objectID;
            cell.floorLayer.resource = tile.resource;
            cell.floorLayer.sheetID_X = floorData.sheetId.x;
            cell.floorLayer.sheetID_Y = floorData.sheetId.y;
            cell.floorLayer.sheetID_W = floorData.sheetSize.x;
            cell.floorLayer.sheetID_H = floorData.sheetSize.y;
            cell.floorLayer.tileSprite = tile.originalSprite;
            cell.floorLayer.worldIndexNum = indexNum;
            cell.floorLayer.tileType = tile.tileType;
            cell.floorLayer.floorType = tile.floorType;
            cell.floorLayer.tileSprite.setOrigin(cell.floorLayer.tileSprite.getLocalBounds().width * 0.5f, cell.floorLayer.tileSprite.getLocalBounds().height * 0.5f);
            cell.floorLayer.tileSprite.setPosition(IndexToPos(indexNum));
            cell.IndexX = indexNum / row + indexNum % row;
            cell.IndexY = indexNum / col + indexNum % col;
            cell.placedPossible = true;
            cell.playerPassable = true;
            mapData[indexNum].floorLayer = cell.floorLayer;
            break;
        case TileType::Object:
            cell.objectLayer.ID = tile.objectID;
            cell.objectLayer.resource = tile.resource;
            cell.objectLayer.sheetID_X = objectData.sheetId.x;
            cell.objectLayer.sheetID_Y = objectData.sheetId.y;
            cell.objectLayer.sheetID_W = objectData.sheetSize.x;
            cell.objectLayer.sheetID_H = objectData.sheetSize.y;
            cell.objectLayer.tileSprite = tile.originalSprite;
            cell.objectLayer.worldIndexNum = indexNum;
            cell.objectLayer.tileType = tile.tileType;
            cell.objectLayer.objectType = tile.objectType;
            cell.IndexX = indexNum / row + indexNum % row;
            cell.IndexY = indexNum / col + indexNum % col;
            cell.placedPossible = false;
            cell.playerPassable = false;
            if (cell.objectLayer.tileSprite.getLocalBounds().height > size)
            {
                cell.objectLayer.tileSprite.setOrigin(cell.objectLayer.tileSprite.getLocalBounds().width * 0.5f, cell.objectLayer.tileSprite.getLocalBounds().height - size*0.5);
            }
            else
            {
                cell.objectLayer.tileSprite.setOrigin(cell.objectLayer.tileSprite.getLocalBounds().width * 0.5f, cell.objectLayer.tileSprite.getLocalBounds().height * 0.5f);
            }
            cell.objectLayer.tileSprite.setPosition(IndexToPos(indexNum));
            mapData[indexNum].objectLayer = cell.objectLayer;
            break;
        default:
            break;
        }
    }
}

Tile* TestMapTool::GetIndexState(int index)
{
	//for (auto& tile : mapData)
	//{
	//	if (tile.ground)
	//	{
	//		return &tile.second; // 해당 인덱스의 타일 찾음
	//	}
	//}
	//return nullptr; // 타일이 없으면 nullptr 반환
    return nullptr;
}

void TestMapTool::Draw(sf::RenderWindow& window)
{
    sf::RenderStates state;
    state.transform = transform;
    window.draw(grid, state);
    window.draw(spriteFloor);
    for (const auto& cell : mapData)
    {
        if (cell.groundLayer.tileSprite.getTexture())
        {
            window.draw(cell.groundLayer.tileSprite);
        }
        if (cell.floorLayer.tileSprite.getTexture())
        {
            window.draw(cell.floorLayer.tileSprite);
        }
        if (cell.objectLayer.tileSprite.getTexture())
        {
            window.draw(cell.objectLayer.tileSprite);
        }
    }
    Scene::Draw(window);
}

void TestMapTool::DrawGrid()
{
    grid.clear();
    grid.setPrimitiveType(sf::Lines);
    grid.resize((col + 1 + row + 1) * 2);

    sf::Vector2f startLine = { 0.f, 0.f };
    sf::Vector2f endLine = startLine;

    int gridIndex = 0;

    for (int i = 0; i < row + 1; ++i)
    {
        startLine = { 0.f , (float)(i * size) };
        endLine = { (float)FRAMEWORK.GetWindowSize().x, startLine.y };

        grid[gridIndex].color = sf::Color::White;
        grid[gridIndex].position = startLine;
        grid[gridIndex + 1].color = sf::Color::White;
        grid[gridIndex + 1].position = endLine;

        gridIndex += 2;
    }

    startLine = { 0.f, 0.f };
    endLine = startLine;

    for (int i = 0; i < col + 1; ++i)
    {
        startLine = { (float)(i * size), 0.f };
        endLine = { startLine.x, (float)FRAMEWORK.GetWindowSize().y };

        grid[gridIndex].color = sf::Color::White;
        grid[gridIndex].position = startLine;
        grid[gridIndex + 1].color = sf::Color::White;
        grid[gridIndex + 1].position = endLine;

        gridIndex += 2;
    }
}

int TestMapTool::PosToIndex(sf::Vector2f pos)
{
    int rowIndex = static_cast<int>((pos.y - (size / 2))) / size;
    int columnIndex = static_cast<int>((pos.x - (size / 2))) / size;
    int index = rowIndex * col + columnIndex;

    return index;
}

sf::Vector2f TestMapTool::IndexToPos(int index)
{
    int x = index % col;
    int y = index / col;
    return sf::Vector2f( x * size + size / 2,  y * size + size / 2);
}

int TestMapTool::SelectIndex(sf::Vector2f pos)
{
    int rowIndex = static_cast<int>(pos.y / size);
    int columnIndex = static_cast<int>(pos.x / size);
    int index = rowIndex * col + columnIndex;

    return index;
}


void TestMapTool::SetMapToolSize(int xCount, int yCount)
{
    row = xCount;
    col = yCount;
}

void TestMapTool::SaveMapContent()
{
    using namespace rapidjson;

    rapidjson::Document doc;
    doc.SetObject();
    Document::AllocatorType& allocator = doc.GetAllocator(); // 메모리 할당자 획득

    doc.AddMember("TileMap Name", Value("NAME"), allocator);
    doc.AddMember("Tile Count X", Value(col), allocator);
    doc.AddMember("Tile Count Y", Value(row), allocator);

    // 각 타일의 정보를 담을 JSON 배열 객체 생성
    Value tilesArray(kArrayType);

    for (const auto& map : mapData)
    {
            Value tileContent(kObjectType); // 개별 타일 정보를 담을 JSON 객체 생성

            tileContent.AddMember("Index X", map.IndexX, allocator);
            tileContent.AddMember("Index Y", map.IndexY, allocator);
            tileContent.AddMember("Ground Type", (int)(map.groundLayer.groundType), allocator);
            tileContent.AddMember("Ground ID", map.groundLayer.ID, allocator);
            tileContent.AddMember("Floor Type", (int)(map.floorLayer.floorType), allocator);
            tileContent.AddMember("Floor ID", map.floorLayer.ID, allocator);
            tileContent.AddMember("Object Type", (int)(map.objectLayer.objectType), allocator);
            tileContent.AddMember("Object ID", map.objectLayer.ID, allocator);
            tileContent.AddMember("Placed Possible", map.placedPossible, allocator);
            tileContent.AddMember("Player Passable", map.playerPassable, allocator);

            // 타일 객체를 타일 배열에 추가
            tilesArray.PushBack(tileContent, allocator);
    }

    doc.AddMember("Tiles", tilesArray, allocator);

    std::wstring savePath = SaveFilePath();
    if (savePath.empty())
    {
        return; // 취소할 경우 return
    }

    // JSON 문서를 파일에 쓰기
    // TODO : fopen 수정하기
    std::string sSavePath = WstringToString(savePath);
    FILE* fp = fopen(sSavePath.c_str(), "wb");

    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    PrettyWriter<FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(fp);
}

//void TestMapTool::LoadMapFile(std::vector<std::vector<Tile>>& data, const std::string& filePath)
//{
//    std::ifstream inFile(filePath);
//    std::string content((std::istreambuf_iterator<char>(inFile)),
//        std::istreambuf_iterator<char>());
//
//    rapidjson::Document doc;
//    doc.Parse(content.c_str());
//    doc.SetObject();
//
//    const rapidjson::Value& tiles = doc["tiles"];
//    for (rapidjson::SizeType i = 0; i < tiles.Size(); i++)
//    {
//        const rapidjson::Value& tile = tiles[i];
//
//        Tile tileData;
//        tileData.sheetID_X = tile["Sheet ID X"].GetInt();
//        tileData.sheetID_Y = tile["Sheet ID Y"].GetInt();
//        tileData.sheetID_W = tile["Sheet ID W"].GetInt();
//        tileData.sheetID_Y = tile["Sheet ID H"].GetInt();
//        tileData.resource = tile["Resource"].GetString();
//        tileData.ID = tile["Object ID"].GetInt();
//        //tileData.placedPossible = tile["Placed Possible"].GetBool();
//        //tileData.playerPassable = tile["Player Passable"].GetBool();
//    }
//}

