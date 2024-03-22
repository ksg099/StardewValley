#include "pch.h"
#include "MapToolUI.h"

MapToolUI::MapToolUI(const std::string& name)
{
}

void MapToolUI::SetBackFloor(const sf::Vector2i& count, const sf::Vector2f& size)
{
    cellCount = count;
    cellSize = size;

    backFloor.clear();
    backFloor.setPrimitiveType(sf::Quads);
    backFloor.resize(count.x * count.y * 4);

    float tileWidth = 38.f;
    float tileHeight = 38.f;

    for (int i = 0; i < count.y; ++i)
    {
        for (int j = 0; j < count.x; ++j)
        {
            int quadIndex = (i * count.x + j) * 4;

            sf::Vector2f topLeftPos((float)(FRAMEWORK.GetWindowSize().x) * 0.6f + (float)j * size.x, (float)i * size.y);

            backFloor[quadIndex + 0].position = topLeftPos;
            backFloor[quadIndex + 1].position = topLeftPos + sf::Vector2f(size.x, 0.f);
            backFloor[quadIndex + 2].position = topLeftPos + sf::Vector2f(size.x, size.y);
            backFloor[quadIndex + 3].position = topLeftPos + sf::Vector2f(0.f, size.y);

            backFloor[quadIndex + 0].texCoords = sf::Vector2f(0.f, 0.f);
            backFloor[quadIndex + 1].texCoords = sf::Vector2f(tileWidth, 0.f);
            backFloor[quadIndex + 2].texCoords = sf::Vector2f(tileWidth, tileHeight);
            backFloor[quadIndex + 3].texCoords = sf::Vector2f(0.f, tileHeight);
        }
    }
}

void MapToolUI::SetSpriteSheetId(const std::string& id)
{
    backFloorSheetId = id;
    backFloorTexture = &RES_MGR_TEXTURE.Get(backFloorSheetId);
}

void MapToolUI::DrawGrid()
{
    grid.clear();
    grid.setPrimitiveType(sf::Lines);
    grid.resize((col + 1 + row + 1) * 2);

    int gridIndex = 0;

    // 세로선 그리기
    for (int i = 0; i <= row; ++i) {
        sf::Vector2f startLine = { gridStart.x, gridStart.y + i * size };
        sf::Vector2f endLine = { gridStart.x + col * size, gridStart.y + i * size };

        grid[gridIndex].color = sf::Color::Black;
        grid[gridIndex].position = startLine;
        grid[gridIndex + 1].color = sf::Color::Black;
        grid[gridIndex + 1].position = endLine;

        gridIndex += 2;
    }

    // 가로선 그리기
    for (int i = 0; i <= col; ++i) {
        sf::Vector2f startLine = { gridStart.x + i * size, gridStart.y };
        sf::Vector2f endLine = { gridStart.x + i * size, gridStart.y + row * size };

        grid[gridIndex].color = sf::Color::Black;
        grid[gridIndex].position = startLine;
        grid[gridIndex + 1].color = sf::Color::Black;
        grid[gridIndex + 1].position = endLine;

        gridIndex += 2;
    }
}

sf::Vector2f MapToolUI::IndexToPos(int index)
{
    int x = index % col;
    int y = index / col;
    return sf::Vector2f(gridStart.x + x * size + size / 2, gridStart.y + y * size + size / 2);
}



void MapToolUI::UpdatePalette()
{
    for (auto& category : categories)
    {
        for (auto& obj : category)
        {
            obj.SetActive(false);
        }
    }

    for (auto& obj : categories[currentPage])
    {
        obj.SetActive(true);
    }
}

void MapToolUI::Init()
{
    currentPage = 0;
    DrawGrid();

    outLine.setOutlineColor(sf::Color(134, 56, 10));
    outLine.setFillColor(sf::Color::Transparent);
    outLine.setOutlineThickness(-15.f);
    outLine.setSize((sf::Vector2f)(FRAMEWORK.GetWindowSize()));
    outLine.setOrigin(0.f, 0.f);
    outLine.setPosition(0.f, 0.f);

    centerLine.setFillColor(sf::Color(134, 56, 10));
    centerLine.setSize({ 15.f, (float)FRAMEWORK.GetWindowSize().y });
    centerLine.setOrigin(0.f, 0.f);
    centerLine.setPosition((float)FRAMEWORK.GetWindowSize().x * 0.6f, 0.f);

    background.setFillColor(sf::Color::White);
    background.setSize({ (float)FRAMEWORK.GetWindowSize().x * 0.4f, (float)FRAMEWORK.GetWindowSize().y });
    background.setOrigin(0.f, 0.f);
    background.setPosition((float)FRAMEWORK.GetWindowSize().x * 0.6f, 0.f);

    SetSpriteSheetId("graphics/backFloor.png");
    SetBackFloor({ 20,30 }, { 38.f,38.f });

    logo.SetTexture("graphics/yellowLettersLogo.png");
    logo.SetOrigin(Origins::MC);
    logo.SetScale({ 0.5f,0.5f });
    logo.SetPosition({ (float)(FRAMEWORK.GetWindowSize().x) * 0.8f, logo.GetLocalBounds().height * 0.5f });

    arrowLeft.SetTexture("graphics/arrow.png");
    arrowLeft.SetOrigin(Origins::MC);
    arrowLeft.SetPosition({ (float)(FRAMEWORK.GetWindowSize().x) * 0.8f - logo.GetLocalBounds().width * 0.5f
        , logo.GetLocalBounds().height * 0.5f });

    arrowRight.SetTexture("graphics/arrow.png");
    arrowRight.SetFlipX(true);
    arrowRight.SetOrigin(Origins::MC);
    arrowRight.SetPosition({ (float)(FRAMEWORK.GetWindowSize().x) * 0.8f + logo.GetLocalBounds().width * 0.5f
        , logo.GetLocalBounds().height * 0.5f });

    float buttonOffset = 20.f;

    saveButton.SetTexture("graphics/button.png");
    saveButton.SetOrigin(Origins::MC);
    saveButton.SetScale({ 2.f, 2.f });
    saveButton.SetPosition({ (float)FRAMEWORK.GetWindowSize().x * 0.675f
        , (float)FRAMEWORK.GetWindowSize().y * 0.85f });

    eraseButton.SetTexture("graphics/button.png");
    eraseButton.SetOrigin(Origins::MC);
    eraseButton.SetScale({ 2.f, 2.f });
    eraseButton.SetPosition({ saveButton.GetPosition().x + saveButton.GetLocalBounds().width * 2.f + buttonOffset
        , saveButton.GetPosition().y });

    loadButton.SetTexture("graphics/button.png");
    loadButton.SetOrigin(Origins::MC);
    loadButton.SetScale({ 2.f, 2.f });
    loadButton.SetPosition({ eraseButton.GetPosition().x + saveButton.GetLocalBounds().width * 2.f + buttonOffset
        ,saveButton.GetPosition().y });

    moveScreenButton.SetTexture("graphics/button.png");
    moveScreenButton.SetOrigin(Origins::MC);
    moveScreenButton.SetScale({ 2.f, 2.f });
    moveScreenButton.SetPosition({ loadButton.GetPosition().x + saveButton.GetLocalBounds().width * 2.f + buttonOffset
        , saveButton.GetPosition().y });


    rapidjson::Document doc;
    Utils::LoadFromFile("data/DataTable.json", doc);

    /*auto objSheetInfo = doc["Tile Info"]["Object"]["Type Info"].GetArray();
    int stone = (int)ObjectType::STONE;
    int stoneCount = objSheetInfo[stone]["Sheet Info"].Size();
    for (int i = 0; i < stoneCount; ++i)
    {
        auto elem = objSheetInfo[stone]["Sheet Info"][i].GetObject();
        std::cout << elem["Resoruce"].GetString() << std::endl;
        stone_1.setTexture(RES_MGR_TEXTURE.Get(elem["Resoruce"].GetString()));
        stone_1.setTextureRect({ elem["Sheet ID X"].GetInt(), elem["Sheet ID Y"].GetInt(), elem["Sheet ID W"].GetInt(), elem["Sheet ID H"].GetInt() });
        stone_1.setPosition({ (float)FRAMEWORK.GetWindowSize().x * 0.675f
    , (float)FRAMEWORK.GetWindowSize().y * 0.2f });
    }

    for (int i = 0; i < objSheetInfo.Size(); ++i)
    {
        ObjectType type = (ObjectType)i;
        auto objectArray = objSheetInfo[i]["Sheet Info"].GetArray();

        for (int j = 0; j < objectArray.Size(); ++j)
        {
            auto elem = objectArray[j].GetObject();

        }
    }
    */

    stone_1.SetTextureByName("stone1");
    stone_1.SetOrigin(Origins::MC);
    stone_1.SetPosition(IndexToPos(0));
    stone_2.SetTextureByName("stone2");
    stone_2.SetOrigin(Origins::MC);
    stone_2.SetPosition(IndexToPos(1));
    stone_3.SetTextureByName("stone3");
    stone_3.SetOrigin(Origins::MC);
    stone_3.SetPosition(IndexToPos(2));
    stone_4.SetTextureByName("stone4");
    stone_4.SetOrigin(Origins::MC);
    stone_4.SetPosition(IndexToPos(3));

    tree_1.SetTextureByName("tree1");
    tree_1.SetOrigin(Origins::Object);
    tree_1.SetPosition(IndexToPos(5));
    tree_2.SetTextureByName("tree2");
    tree_2.SetOrigin(Origins::Object);
    tree_2.SetPosition(IndexToPos(8));
    tree_3.SetTextureByName("tree3");
    tree_3.SetOrigin(Origins::Object);
    tree_3.SetPosition(IndexToPos(12));
    tree_4.SetTextureByName("tree4");
    tree_4.SetOrigin(Origins::Object);
    tree_4.SetPosition(IndexToPos(16));

    
}

void MapToolUI::Release()
{
    GameObject::Release();
}

void MapToolUI::Reset()
{
    GameObject::Reset();
}

void MapToolUI::Update(float dt)
{
    sf::Vector2i mousePos = (sf::Vector2i)InputMgr::GetMousePos();
    sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
    if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        if (arrowLeft.GetGlobalBounds().contains(mouseWorldPos))
        {
            --currentPage;
            if (currentPage < 0)
            {
                currentPage = categories.size() - 1;
            }
            UpdatePalette();
        }
        else if (arrowRight.GetGlobalBounds().contains(mouseWorldPos))
        {
            ++currentPage;
            if (currentPage >= categories.size())
            {
                currentPage = 0;
            }
            UpdatePalette();
        }
    }

    GameObject::Update(dt);
}

void MapToolUI::LateUpdate(float dt)
{
    GameObject::LateUpdate(dt);
}

void MapToolUI::FixedUpdate(float dt)
{
    GameObject::FixedUpdate(dt);
}

void MapToolUI::Draw(sf::RenderWindow& window)
{
    GameObject::Draw(window);
    window.draw(background);

    sf::RenderStates state;
    state.texture = backFloorTexture;

    window.draw(backFloor, state);
    window.draw(outLine);
    window.draw(centerLine);
    logo.Draw(window);
    arrowLeft.Draw(window);
    arrowRight.Draw(window);
    window.draw(grid);
    if (stone_1.GetActive()) { stone_1.Draw(window); }
    /*stone_2.Draw(window);
    stone_3.Draw(window);
    stone_4.Draw(window);
    tree_1.Draw(window);
    tree_2.Draw(window);
    tree_3.Draw(window);
    tree_4.Draw(window);*/
    saveButton.Draw(window);
    eraseButton.Draw(window);
    loadButton.Draw(window);
    moveScreenButton.Draw(window);
}
