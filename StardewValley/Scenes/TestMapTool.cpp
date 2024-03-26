#include "pch.h"
#include <Windows.h>
#include <filesystem>
#include "TestMapTool.h"
#include "MapToolUI.h"

namespace fs = std::filesystem;

std::string TestMapTool::ToRelativePath(const std::string& originalPath, const std::string& basePath)   //�����θ� ����η� �����ϱ� ����
{
    fs::path relativePath = fs::relative(fs::path(originalPath), fs::path(basePath));
    return relativePath.string();
}

std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr)         //LPCWSTR ������ ���ڿ��� std::string�� ��ȯ�ϱ� ����
{
    int strLength
        = WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1,
            nullptr, 0, nullptr, nullptr);

    std::string str(strLength, 0);

    WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, &str[0],
        strLength, nullptr, nullptr);

    return str;
}

void TestMapTool::LoadMapFile()
{
    std::wstring filePathW = OpenFilePath();
    if (filePathW.empty()) 
        return;

    std::string filePath = WstringToString(filePathW);

    std::ifstream inFile(filePath);
    if (!inFile.is_open()) 
        return;

    std::string content((std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());

    rapidjson::Document doc;
    doc.Parse(content.c_str());

    //������ ������ ������ ���� �����ϴ� ����
    if (doc.HasMember("Tile Map") && doc["Tile Map"].IsArray())
    {
        mapData.clear();
        int i = 0;
        const rapidjson::Value& tileMaps = doc["Tile Map"];
        for (const auto& tileMap : tileMaps.GetArray())
        {
            col = tileMap["Tile Count X"].GetInt();
            row = tileMap["Tile Count Y"].GetInt();
            mapData.resize(col * row);

            const rapidjson::Value& tiles = tileMap["Tiles"];
            for (const auto& tile : tiles.GetArray())
            {
                mapData[i].IndexX = tile["Index X"].GetInt();
                mapData[i].IndexY = tile["Index Y"].GetInt();
                
                //ground
                mapData[i].groundLayer.groundType = (GroundType)(tile["Ground Type"].GetInt());
                mapData[i].groundLayer.ID = tile["Ground ID"].GetInt();
                DataGround dataGround = GROUND_TABLE->Get(mapData[i].groundLayer.groundType, mapData[i].groundLayer.ID);
                mapData[i].groundLayer.resource = GROUND_TABLE->GetTextureId();
                mapData[i].groundLayer.tileSprite.setTexture(RES_MGR_TEXTURE.Get(GROUND_TABLE->GetTextureId()));
                mapData[i].groundLayer.sheetID_X = dataGround.sheetId.x;
                mapData[i].groundLayer.sheetID_Y = dataGround.sheetId.y;
                mapData[i].groundLayer.sheetID_W = dataGround.sheetSize.x;
                mapData[i].groundLayer.sheetID_H = dataGround.sheetSize.y;
                mapData[i].groundLayer.tileSprite.setTextureRect({ mapData[i].groundLayer.sheetID_X,
                    mapData[i].groundLayer.sheetID_Y,mapData[i].groundLayer.sheetID_W, mapData[i].groundLayer.sheetID_H });
                mapData[i].groundLayer.tileSprite.setOrigin({ mapData[i].groundLayer.tileSprite.getLocalBounds().width * 0.5f,
                    mapData[i].groundLayer.tileSprite.getLocalBounds().height * 0.5f });
                mapData[i].groundLayer.tileSprite.setPosition(IndexToPos(mapData[i].IndexY * col + mapData[i].IndexX));
                
                //floor
                mapData[i].floorLayer.floorType = (FloorType)(tile["Floor Type"].GetInt());
                mapData[i].floorLayer.ID = tile["Floor ID"].GetInt();
                auto floorData = FLOOR_TABLE->Get(mapData[i].floorLayer.floorType, mapData[i].floorLayer.ID);
                mapData[i].floorLayer.resource = floorData.textureId;
                mapData[i].floorLayer.tileSprite.setTexture(RES_MGR_TEXTURE.Get(mapData[i].floorLayer.resource));
                mapData[i].floorLayer.sheetID_X = floorData.sheetId.x;
                mapData[i].floorLayer.sheetID_Y = floorData.sheetId.y;
                mapData[i].floorLayer.sheetID_W = floorData.sheetSize.x;
                mapData[i].floorLayer.sheetID_H = floorData.sheetSize.y;
                mapData[i].floorLayer.tileSprite.setTextureRect({ mapData[i].floorLayer.sheetID_X, 
                    mapData[i].floorLayer.sheetID_Y, mapData[i].floorLayer.sheetID_W, mapData[i].floorLayer.sheetID_H });
                mapData[i].floorLayer.tileSprite.setOrigin({ mapData[i].floorLayer.tileSprite.getLocalBounds().width * 0.5f,
                   mapData[i].floorLayer.tileSprite.getLocalBounds().height * 0.5f });
                mapData[i].floorLayer.tileSprite.setPosition(IndexToPos(mapData[i].IndexY * col + mapData[i].IndexX));
                
                //object
                mapData[i].objectLayer.objectType = (ObjectType)(tile["Object Type"].GetInt());
                mapData[i].objectLayer.ID = tile["Object ID"].GetInt();
                auto objectData = OBJECT_TABLE->Get(mapData[i].objectLayer.objectType, mapData[i].objectLayer.ID);
                mapData[i].objectLayer.resource = objectData.textureId;
                mapData[i].objectLayer.tileSprite.setTexture(RES_MGR_TEXTURE.Get(mapData[i].objectLayer.resource));
                mapData[i].objectLayer.sheetID_X = objectData.sheetId.x;
                mapData[i].objectLayer.sheetID_Y = objectData.sheetId.y;
                mapData[i].objectLayer.sheetID_W = objectData.sheetSize.x;
                mapData[i].objectLayer.sheetID_H = objectData.sheetSize.y;
                mapData[i].objectLayer.tileSprite.setTextureRect({ mapData[i].objectLayer.sheetID_X ,
                    mapData[i].objectLayer.sheetID_Y, mapData[i].objectLayer.sheetID_W, mapData[i].objectLayer.sheetID_H });
                if (mapData[i].objectLayer.tileSprite.getLocalBounds().height > size)
                {
                    mapData[i].objectLayer.tileSprite.setOrigin(mapData[i].objectLayer.tileSprite.getLocalBounds().width * 0.5f, mapData[i].objectLayer.tileSprite.getLocalBounds().height - size * 0.5);
                }
                else
                {
                    mapData[i].objectLayer.tileSprite.setOrigin(mapData[i].objectLayer.tileSprite.getLocalBounds().width * 0.5f, mapData[i].objectLayer.tileSprite.getLocalBounds().height * 0.5f);
                }
                mapData[i].objectLayer.tileSprite.setPosition(IndexToPos(mapData[i].IndexY * col + mapData[i].IndexX));

                //bool
                mapData[i].placedPossible = tile["Placed Possible"].GetBool();
                mapData[i].playerPassable = tile["Player Passable"].GetBool();
                ++i;
            }
        }
    }
    std::cout << "380 objectType : " << (int)(mapData[380].objectLayer.objectType) << std::endl;
    std::cout << "380 objectID : " << mapData[380].objectLayer.ID << std::endl;
    std::cout << "381 objectType : " << (int)(mapData[381].objectLayer.objectType) << std::endl;
    std::cout << "381 objectID : " << mapData[381].objectLayer.ID << std::endl;
    std::cout << "382 objectType : " << (int)(mapData[382].objectLayer.objectType) << std::endl;
    std::cout << "382 objectID : " << mapData[382].objectLayer.ID << std::endl;
    std::cout << "383 objectType : " << (int)(mapData[383].objectLayer.objectType) << std::endl;
    std::cout << "383 objectID : " << mapData[383].objectLayer.ID << std::endl;
    
}

void TestMapTool::UpdateTransform()
{
    transform = sf::Transform::Identity;

    transform.scale(FRAMEWORK.GetWindowSize().x / worldView.getSize().x, FRAMEWORK.GetWindowSize().y / worldView.getSize().y, 0.f,0.f);

}

std::wstring TestMapTool::SaveFilePath()        //��ȭ����(����)�� ���� ���� �Լ�
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

    if (GetSaveFileName(&ofn)) // ���� ���� ��ȭ ���ڸ� ǥ��
    {
        return ofn.lpstrFile; // ����ڰ� ������ ���� ��θ� ��ȯ
    }
    return L"";
}

std::string TestMapTool::WstringToString(const std::wstring& var)
{
	static std::locale loc("");
	auto& facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
	return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(var);
}

std::wstring TestMapTool::OpenFilePath()
{
    OPENFILENAME ofn;       // ���� ��ȭ ���� ����ü
    wchar_t szFile[260];    // ���� �̸��� ������ �迭
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // ��ȭ ������ �θ� ������ �ڵ�
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        return ofn.lpstrFile; // ����ڰ� ������ ���� ��� ��ȯ
    }
    return L""; // ����ڰ� ����ϸ� �� ���ڿ� ��ȯ
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

    /*timer += dt;
    if (timer > duration)
    {
        printf("WORLD : (%f, %f)\n", mouseWorldPos.x, mouseWorldPos.y);
        timer = 0.f;
    }*/

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
            //��ġ�Ǿ��� ������Ʈ �����
            mapToolUI->isSelected = false;
        }
        if (mapToolUI->GetLoadButtonGB().contains(mouseWorldPos))
        {
            //�����ߴ� �� ���� �ҷ�����
            mapToolUI->isSelected = false;
        }
        if (mapToolUI->GetMoveScreenButtonGB().contains(mouseWorldPos))
        {
            //������Ʈ ��ġ�� �����ϰ� ���� ȭ���� �巡�׾� ����ؼ� �̵��� �� �ֵ���
            mapToolUI->isSelected = false;
        }

        sf::FloatRect visibleMapBounds(15.f, 15.f, 1152.f, 1050.f);
        if (mapToolUI->isSelected && visibleMapBounds.contains(mouseWorldPos))
        {
            PlaceTileToIndex(SelectIndex(mouseWorldPos), mapToolUI->selectedTile);
        }

        if (mapToolUI->GetSaveButtonGB().contains(mouseWorldPos))
        {
            SaveMapContent();
        }

        if (mapToolUI->GetLoadButtonGB().contains(mouseWorldPos))
        {
            LoadMapFile();
        }
    }

    
}

void TestMapTool::PlaceTileToIndex(int indexNum, MapSheet& tile)
{
    //���� Type�̸� �̹� �ִ��� �������� ���� �ø��� �����ϴ°Ű�
    //�ٸ� Type�̸� ground -> floor -> object ������ ���� �׾ƿø� �� �־�� ��
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
            mapData[indexNum].placedPossible = cell.placedPossible;
            mapData[indexNum].playerPassable = cell.playerPassable;
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
            cell.placedPossible = true;
            cell.playerPassable = true;
            mapData[indexNum].placedPossible = cell.placedPossible;
            mapData[indexNum].playerPassable = cell.playerPassable;
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
            cell.placedPossible = false;
            cell.playerPassable = false;
            mapData[indexNum].placedPossible = cell.placedPossible;
            mapData[indexNum].playerPassable = cell.playerPassable;
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
	//		return &tile.second; // �ش� �ε����� Ÿ�� ã��
	//	}
	//}
	//return nullptr; // Ÿ���� ������ nullptr ��ȯ
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
    grid.resize((col + 2 + row + 2) * 2);

    sf::Vector2f startLine = { gridStartX, gridStartY };
    sf::Vector2f endLine = startLine;

    int gridIndex = 0;

    for (int i = 0; i < row + 1; ++i)
    {
        startLine = { startLine.x , (float)(i * size) + gridStartY };
        endLine = { (float)FRAMEWORK.GetWindowSize().x * 0.6f - 15.f, startLine.y };

        grid[gridIndex].color = sf::Color::White;
        grid[gridIndex].position = startLine;
        grid[gridIndex + 1].color = sf::Color::White;
        grid[gridIndex + 1].position = endLine;

        gridIndex += 2;
    }

    startLine = { gridStartX, gridStartY };
    endLine = startLine;

    for (int j = 0; j < col + 1; ++j)
    {
        startLine = { (float)(j * size) + gridStartX, startLine.y };
        endLine = { startLine.x , (float)FRAMEWORK.GetWindowSize().y };

        grid[gridIndex].color = sf::Color::White;
        grid[gridIndex].position = startLine;
        grid[gridIndex + 1].color = sf::Color::White;
        grid[gridIndex + 1].position = endLine;

        gridIndex += 2;
    }
}

int TestMapTool::PosToIndex(sf::Vector2f pos)
{
    int rowIndex = static_cast<int>((pos.y - gridStartY - (size / 2))) / col;
    int columnIndex = static_cast<int>((pos.x - gridStartX - (size / 2))) / col;
    int index = rowIndex * col + columnIndex;

    return index;
}

sf::Vector2f TestMapTool::IndexToPos(int index)
{
    int x = index % col;
    int y = index / col;
    return sf::Vector2f( (x * size + size / 2) + gridStartX,  (y * size + size / 2) + gridStartY);
}

int TestMapTool::SelectIndex(sf::Vector2f pos)
{
    int rowIndex = (int)((pos.y - gridStartY) / size);
    int columnIndex = (int)((pos.x - gridStartX) / size);
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
    Document::AllocatorType& allocator = doc.GetAllocator(); // �޸� �Ҵ��� ȹ��
    
    Value array(kArrayType);
    Value tileMapData;
    tileMapData.SetObject();

    tileMapData.AddMember("TileMap Name", Value("Farm"), allocator);
    tileMapData.AddMember("Tile Count X", Value(col), allocator);
    tileMapData.AddMember("Tile Count Y", Value(row), allocator);

    // �� Ÿ���� ������ ���� JSON �迭 ��ü ����
    Value tilesArray(kArrayType);

    int X = 0;
    int Y = 0;

    for (const auto& map : mapData)
    {
            Value tileContent(kObjectType); // ���� Ÿ�� ������ ���� JSON ��ü ����
            
            if (X >= row)
            {
                ++Y;
                X = 0;
            }
            tileContent.AddMember("Index X", X, allocator);
            tileContent.AddMember("Index Y", Y, allocator);
            tileContent.AddMember("Ground Type", (int)(map.groundLayer.groundType), allocator);
            tileContent.AddMember("Ground ID", map.groundLayer.ID, allocator);
            tileContent.AddMember("Floor Type", (int)(map.floorLayer.floorType), allocator);
            tileContent.AddMember("Floor ID", map.floorLayer.ID, allocator);
            tileContent.AddMember("Object Type", (int)(map.objectLayer.objectType), allocator);
            tileContent.AddMember("Object ID", map.objectLayer.ID, allocator);
            tileContent.AddMember("Placed Possible", map.placedPossible, allocator);
            tileContent.AddMember("Player Passable", map.playerPassable, allocator);
            ++X;
            // Ÿ�� ��ü�� Ÿ�� �迭�� �߰�
            tilesArray.PushBack(tileContent, allocator);
    }

    tileMapData.AddMember("Tiles", tilesArray, allocator);
    array.PushBack(tileMapData, allocator);
    doc.AddMember("Tile Map", array, allocator);

    std::wstring savePath = SaveFilePath();
    if (savePath.empty())
    {
        return; // ����� ��� return
    }

    // JSON ������ ���Ͽ� ����
    // TODO : fopen �����ϱ�
    std::string sSavePath = WstringToString(savePath);
    FILE* fp = fopen(sSavePath.c_str(), "wb");

    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    PrettyWriter<FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(fp);
}
//
//void TestMapTool::LoadTileMap(const std::string& name)
//{
//    tiles = TILEMAP_SAVE->Get(name);
//    if (tiles == nullptr)
//        return;
//
//    sf::Vector2i cellCount;
//    cellCount.x = TILEMAP_SAVE->GetTileMapSize(name)->x;
//    cellCount.y = TILEMAP_SAVE->GetTileMapSize(name)->y;
//    SetSpriteSheetId(GROUND_TABLE->GetTextureId());
//
//    cellSize = tileSize;
//
//    va.clear();
//    va.setPrimitiveType(sf::Quads);
//    va.resize(cellCount.x * cellCount.y * 4);
//
//    sf::Vector2f posOffsets[4] = {
//        { 0, 0 },
//        { tileSize.x, 0 },
//        { tileSize.x, tileSize.y },
//        { 0, tileSize.y }
//    };
//
//    for (auto tile : *tiles)
//    {
//        tile->floor = CreateFloor(tile->floorType, tile->floorId);
//        if (tile->floor != nullptr)
//            tile->floor->SetTileData(tile);
//
//        tile->object = CreateObject(tile->objectType, tile->objectId);
//        if (tile->object != nullptr)
//            tile->object->SetTileData(tile);
//
//        sf::Vector2f sheetSize = (sf::Vector2f)GROUND_TABLE->Get(tile->groundType, tile->groundId).sheetSize;
//        sf::Vector2f texCoord0[4] = {
//            { 0, 0 },
//            { sheetSize.x, 0 },
//            { sheetSize.x, sheetSize.y },
//            { 0, sheetSize.y }
//        };
//
//        int quadIndex = tile->indexY * cellCount.x + tile->indexX; // 2���� �ε����� 1���� �ε����� ��ȯ
//        sf::Vector2f quadPos(tileSize.x * tile->indexX, tileSize.y * tile->indexY);
//
//        for (int k = 0; k < 4; k++)
//        {
//            int vertexIndex = (quadIndex * 4) + k;
//            va[vertexIndex].position = quadPos + posOffsets[k];
//            va[vertexIndex].texCoords = texCoord0[k];
//            va[vertexIndex].texCoords.x += GROUND_TABLE->Get(tile->groundType, tile->groundId).sheetId.x;
//            va[vertexIndex].texCoords.y += GROUND_TABLE->Get(tile->groundType, tile->groundId).sheetId.y;
//        }
//    }
//}
//
//void TestMapTool::LoadMapFile(const std::string& name)
//{
//    std::string filePath = 
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
//        TileLayer tileData;
//        tileData.IndexX = tile["Index X"].GetInt();
//        tileData.IndexY = tile["Index Y"].GetInt();
//        tileData.groundLayer.groundType = (GroundType)tile["Ground Type"].GetInt();
//        tileData.groundLayer.ID = tile["Ground ID"].GetInt();
//        tileData.floorLayer.floorType = (FloorType)tile["Floor Type"].GetInt();
//        tileData.floorLayer.ID = tile["Floor ID"].GetInt();
//        tileData.objectLayer.objectType = (ObjectType)tile["Object Type"].GetInt();
//        tileData.objectLayer.ID = tile["Object ID"].GetInt();
//        tileData.placedPossible = tile["Placed Possible"].GetBool();
//        tileData.playerPassable = tile["Player Passable"].GetBool();
//    }
//}
//
