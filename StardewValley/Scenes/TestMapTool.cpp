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

void TestMapTool::UpdateTransform()
{
    transform = sf::Transform::Identity;

    transform.scale(FRAMEWORK.GetWindowSize().x / worldView.getSize().x, FRAMEWORK.GetWindowSize().y / worldView.getSize().y, 0.f,0.f);

}

std::wstring TestMapTool::SelectFile()        //��ȭ����(����)�� ���� ���� �Լ�
{
    wchar_t save[260];
    GetCurrentDirectory(MAX_PATH, save);

    OPENFILENAME ofn;
    wchar_t szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)               //������ �����ϸ� true
    {
        SetCurrentDirectory(save);
        return ofn.lpstrFile;
    }
    else
    {
        SetCurrentDirectory(save);
        return L"";
    }
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
            PlaceTileToIndex(PosToIndex(mouseWorldPos), mapToolUI->selectedTile);
        }
    }

    if (InputMgr::GetKeyDown(sf::Keyboard::X))
    {
        std::wstring filePathW = SelectFile();        //xŰ�� ������ ��ȭ����(����)�� ������ ������ �����ϸ�
        if (!filePathW.empty())
        {
            std::string filePath = ConvertLPCWSTRToString(filePathW.c_str());
            if (imageFloor.loadFromFile(filePath))
            {
                spriteFloor.setTexture(imageFloor);

                //TO-DO : �Ŀ� �ε����� �°� ��ġ�ǵ��� �����ϱ�
                spriteFloor.setPosition(FRAMEWORK.GetWindowSize().x / 2 - imageFloor.getSize().x / 2,
                    FRAMEWORK.GetWindowSize().y / 2 - imageFloor.getSize().y / 2);
                spriteFloor.setColor(sf::Color(255, 255, 255, 255));

                std::string relativePath = ToRelativePath(filePath, fs::current_path().string()); //�����θ� ����η� ����
                //mapInfo.roomFloorTexId = relativePath;
            }
            else
            {
                std::cerr << "Image could not be loaded." << std::endl;
            }
        }
    }
}

void TestMapTool::PlaceTileToIndex(int indexNum, MapSheet& tile)
{
    //���� Type�̸� �̹� �ִ��� �������� ���� �ø��� �����ϴ°Ű�
    //�ٸ� Type�̸� ground -> floor -> object ������ ���� �׾ƿø� �� �־�� ��
    if (indexNum >= 0 && indexNum <= col * row )
    {
        //Tile* placedTile = GetIndexState(indexNum); //�ƹ��͵� ������ null

        TileLayer cell;

        switch (tile.tileType)
        {
        case TileType::Ground:
            cell.groundLayer.ID = tile.objectID;
            cell.groundLayer.resource = tile.resource;
            cell.groundLayer.sheetID_X = tile.sheetID_X;
            cell.groundLayer.sheetID_Y = tile.sheetID_Y;
            cell.groundLayer.sheetID_W = tile.sheetID_W;
            cell.groundLayer.sheetID_H = tile.sheetID_H;
            cell.groundLayer.tileSprite = tile.originalSprite;
            cell.groundLayer.worldIndexNum = indexNum;
            cell.groundLayer.tileType = tile.tileType;
           // cell.groundLayer.tileSprite.setTexture(RES_MGR_TEXTURE.Get(tile.resource));
            //cell.groundLayer.tileSprite.setTextureRect({ tile.sheetID_X, tile.sheetID_Y, tile.sheetID_W, tile.sheetID_H });
            cell.groundLayer.tileSprite.setOrigin(cell.groundLayer.tileSprite.getLocalBounds().width * 0.5f, cell.groundLayer.tileSprite.getLocalBounds().height * 0.5f);
            cell.groundLayer.tileSprite.setPosition(IndexToPos(indexNum));
            mapData[indexNum].groundLayer = cell.groundLayer;
            break;
        case TileType::Floor:
            cell.floorLayer.ID = tile.objectID;
            cell.floorLayer.resource = tile.resource;
            cell.floorLayer.sheetID_X = tile.sheetID_X;
            cell.floorLayer.sheetID_Y = tile.sheetID_Y;
            cell.floorLayer.sheetID_W = tile.sheetID_W;
            cell.floorLayer.sheetID_H = tile.sheetID_H;
            cell.floorLayer.tileSprite = tile.originalSprite;
            cell.floorLayer.worldIndexNum = indexNum;
            cell.floorLayer.tileType = tile.tileType;
         //   cell.floorLayer.tileSprite.setTexture(RES_MGR_TEXTURE.Get(tile.resource));
          //  cell.floorLayer.tileSprite.setTextureRect({ tile.sheetID_X, tile.sheetID_Y, tile.sheetID_W, tile.sheetID_H });
            cell.floorLayer.tileSprite.setOrigin(cell.floorLayer.tileSprite.getLocalBounds().width * 0.5f, cell.floorLayer.tileSprite.getLocalBounds().height * 0.5f);
            cell.floorLayer.tileSprite.setPosition(IndexToPos(indexNum));
            mapData[indexNum].floorLayer = cell.floorLayer;
            break;
        case TileType::Object:
            cell.objectLayer.ID = tile.objectID;
            cell.objectLayer.resource = tile.resource;
            cell.objectLayer.sheetID_X = tile.sheetID_X;
            cell.objectLayer.sheetID_Y = tile.sheetID_Y;
            cell.objectLayer.sheetID_W = tile.sheetID_W;
            cell.objectLayer.sheetID_H = tile.sheetID_H;
            cell.objectLayer.tileSprite = tile.originalSprite;
            cell.objectLayer.worldIndexNum = indexNum;
            cell.objectLayer.tileType = tile.tileType;
            cell.objectLayer.tileType = tile.tileType;
           // cell.objectLayer.tileSprite.setTexture(RES_MGR_TEXTURE.Get(tile.resource));
           // cell.objectLayer.tileSprite.setTextureRect({ tile.sheetID_X, tile.sheetID_Y, tile.sheetID_W, tile.sheetID_H });
            cell.objectLayer.tileSprite.setOrigin(cell.objectLayer.tileSprite.getLocalBounds().width * 0.5f, cell.objectLayer.tileSprite.getLocalBounds().height * 0.5f);
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
    int rowIndex = (pos.y -  (size / 2)) / size;
    int columnIndex = (pos.x - (size / 2)) / size;
    int index = rowIndex * col + columnIndex;

    return index;
}

sf::Vector2f TestMapTool::IndexToPos(int index)
{
    int x = index % col;
    int y = index / col;
    return sf::Vector2f( x * size + size / 2,  y * size + size / 2);
}


void TestMapTool::SetMapToolSize(int xCount, int yCount)
{
    row = xCount;
    col = yCount;
}

//void TestMapTool::SaveMapContent()
//{
//    using namespace rapidjson;
//
//    rapidjson::Document doc;
//    doc.SetObject();
//    Document::AllocatorType& allocator = doc.GetAllocator(); // �޸� �Ҵ��� ȹ��
//
//    // Ÿ�ϸ� ũ�� ����
//    doc.AddMember("Resource", Value(tileSize.x), allocator);
//    doc.AddMember("tileSizeY", Value(tileSize.y), allocator);
//    doc.AddMember("mapSizeX", Value(tileMap.x), allocator);
//    doc.AddMember("mapSizeY", Value(tileMap.y), allocator);
//
//    // �� Ÿ���� ������ ���� JSON �迭 ��ü ����
//    Value tilesArray(kArrayType);
//
//    for (const auto& row : tiles)
//    {
//        for (const auto& tile : row)
//        {
//            Value tileObject(kObjectType); // ���� Ÿ�� ������ ���� JSON ��ü ����
//            tileObject.AddMember("type", (int)tile.type, allocator); // Ÿ�� Ÿ�� ���� �߰�
//            // rapidjson �� std::string ȣȯ�� �ȵǼ� const char* ���� �Ѱ������ 
//            // TODO : �ӽù������� ����η� ��ȯ�� ����
//            std::string relativePath = Utils::ConvertToRelativePath(tile.textureFilePath);
//            tileObject.AddMember("texture FilePath", rapidjson::Value(relativePath.c_str(), allocator), allocator);
//            tileObject.AddMember("x Pos", tile.shape.getPosition().x, allocator);
//            tileObject.AddMember("y Pos", tile.shape.getPosition().y, allocator);
//
//            // Ÿ�� ��ü�� Ÿ�� �迭�� �߰�
//            tilesArray.PushBack(tileObject, allocator);
//        }
//    }
//
//    // Ÿ�� �迭�� ����
//    doc.AddMember("tiles", tilesArray, allocator);
//
//    // ���� ������ �����ؾ���
//    std::wstring savePath = Utils::OpenSaveFileDialog();
//    if (savePath.empty())
//    {
//        return; // ����� ��� return
//    }
//
//    // JSON ������ ���Ͽ� ����
//    // TODO : fopen �����ϱ�
//    std::string sSavePath = Utils::WSTRINGToString(savePath);
//    FILE* fp = fopen(sSavePath.c_str(), "wb");
//
//    char writeBuffer[65536];
//    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
//    PrettyWriter<FileWriteStream> writer(os);
//    doc.Accept(writer);
//    fclose(fp);
//}
//
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

