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

std::wstring TestMapTool::SelectFile()        //대화상자(폴더)를 열기 위한 함수
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

    if (GetOpenFileName(&ofn) == TRUE)               //파일을 선택하면 true
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

    mapToolUI = new MapToolUI("UI");
    AddGo(mapToolUI, Layers::Ui);
    
    Scene::Init();
}

void TestMapTool::Release()
{
    Scene::Release();
}

void TestMapTool::Enter()
{
    Scene::Enter();
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

    if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        if (mapToolUI->GetSaveButtonGB().contains(mouseWorldPos))
        {
            SaveMapContent();
        }
        if (mapToolUI->GetEraseButtonGB().contains(mouseWorldPos))
        {
            //배치되었던 오브젝트 지우기
        }
        if (mapToolUI->GetLoadButtonGB().contains(mouseWorldPos))
        {
            //저장했던 맵 정보 불러오기
        }
        if (mapToolUI->GetMoveScreenButtonGB().contains(mouseWorldPos))
        {
            //오브젝트 배치를 중지하고 격자 화면을 드래그앤 드롭해서 이동할 수 있도록
        }
    }


    if (InputMgr::GetKeyDown(sf::Keyboard::X))
    {
        std::wstring filePathW = SelectFile();        //x키를 누르면 대화상자(폴더)가 열리고 파일을 선택하면
        if (!filePathW.empty())
        {
            std::string filePath = ConvertLPCWSTRToString(filePathW.c_str());
            if (imageFloor.loadFromFile(filePath))
            {
                spriteFloor.setTexture(imageFloor);

                //TO-DO : 후에 인덱스에 맞게 배치되도록 수정하기
                spriteFloor.setPosition(FRAMEWORK.GetWindowSize().x / 2 - imageFloor.getSize().x / 2,
                    FRAMEWORK.GetWindowSize().y / 2 - imageFloor.getSize().y / 2);
                spriteFloor.setColor(sf::Color(255, 255, 255, 255));

                std::string relativePath = ToRelativePath(filePath, fs::current_path().string()); //절대경로를 상대경로로 저장
                //mapInfo.roomFloorTexId = relativePath;
            }
            else
            {
                std::cerr << "Image could not be loaded." << std::endl;
            }
        }
    }
}

void TestMapTool::Draw(sf::RenderWindow& window)
{
    window.draw(grid);
    window.draw(spriteFloor);
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


void TestMapTool::SetMapToolSize(int xCount, int yCount)
{
    row = xCount;
    col = yCount;
}

void TestMapTool::SaveMapContent()
{
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

    // 예제 데이터, 실제 데이터는 맵에서 가져온 데이터로 대체해야 합니다.
    rapidjson::Value tiles(rapidjson::kArrayType);

    // 여기서 타일 데이터를 JSON 배열로 추가
    for (const auto& row : mapData) 
    {
        for (const auto& tile : row) 
        {
           /* rapidjson::Value tileData(rapidjson::kObjectType);
            tileData.AddMember("PosX", tile.posX, allocator);
            tileData.AddMember("PosY", tile.posY, allocator);
            tileData.AddMember("Ground Type", tile.groundType, allocator);
            tileData.AddMember("Ground ID", tile.groundID, allocator);
            tileData.AddMember("Floor Type", tile.floorType, allocator);
            tileData.AddMember("Object Type", tile.objectType, allocator);
            tileData.AddMember("Object ID", tile.objectID, allocator);
            tileData.AddMember("Placed Possible", tile.placedPossible, allocator);
            tileData.AddMember("Player Passable", tile.playerPassable, allocator);
            tiles.PushBack(tileData, allocator);*/
        }
    }

    doc.AddMember("tiles", tiles, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    std::ofstream outFile("data"); //이게 맞나?
    outFile << buffer.GetString();
}

void TestMapTool::LoadMapFile(std::vector<std::vector<Tile>>& data, const std::string& filePath)
{
    std::ifstream inFile(filePath);
    std::string content((std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());

    rapidjson::Document doc;
    doc.Parse(content.c_str());

    const rapidjson::Value& tiles = doc["tiles"];
    for (rapidjson::SizeType i = 0; i < tiles.Size(); i++)
    {
        const rapidjson::Value& tile = tiles[i];

        Tile tileData;
        tileData.posX = tile["PosX"].GetInt();
        tileData.posY = tile["PosY"].GetInt();
        //tileData.groundType = tile["Ground Type"].GetType();
        tileData.groundID = tile["Ground ID"].GetInt();
        //tileData.floorType = tile["Floor Type"].GetType();
        tileData.floorID = tile["Floor ID"].GetInt();
        //tileData.objectType = tile["Object Type"].GetType();
        tileData.objectID = tile["Object ID"].GetInt();
        tileData.placedPossible = tile["Placed Possible"].GetBool();
        tileData.playerPassable = tile["Player Passable"].GetBool();
    }
}

