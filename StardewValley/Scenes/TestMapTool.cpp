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

