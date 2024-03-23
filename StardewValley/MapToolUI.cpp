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

int MapToolUI::PosToIndex(sf::Vector2f pos)
{
        int rowIndex = (pos.y - gridStart.y - (size / 2)) / size;
        int columnIndex = (pos.x - gridStart.x - (size / 2)) / size;

        int index = rowIndex * col + columnIndex;
        return index;
}

sf::Vector2f MapToolUI::IndexToPos(int index)
{
    int x = index % col;
    int y = index / col;
    return sf::Vector2f(gridStart.x + x * size + size / 2, gridStart.y + y * size + size / 2); //격자 가운데 배치
}

void MapToolUI::SelectTile(int index)
{
    if (index >= 0 && index < categories[currentPage].size()) 
    {
        selectedTile = categories[currentPage][index];
        isSelected = true;
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

    int index = 0;
    int objIndex = 0;
    categories.resize(3);
    for (int i = 0; i < (int)ObjectType::COUNT; ++i)
    {
        int objectCount = OBJECT_TABLE->Count((ObjectType)i);
        for (int j = 0; j < objectCount; ++j)
        {
            auto objectData = OBJECT_TABLE->Get((ObjectType)i, j);
            std::string textureId = objectData.textureId;
            categories[0].push_back(MapSheet());
            categories[0][objIndex].tileSprite.setTexture(RES_MGR_TEXTURE.Get(textureId));
            categories[0][objIndex].resource = textureId;
            categories[0][objIndex].objectID = objectData.objectId;
            categories[0][objIndex].sheetID_X = objectData.sheetId.x;
            categories[0][objIndex].sheetID_Y = objectData.sheetId.y;
            categories[0][objIndex].sheetID_W = objectData.sheetSize.x;
            categories[0][objIndex].sheetID_H = objectData.sheetSize.y;
            categories[0][objIndex].indexNumber = index;
            categories[0][objIndex].tileSprite.setTextureRect({ objectData.sheetId.x, objectData.sheetId.y, objectData.sheetSize.x, objectData.sheetSize.y });
            categories[0][objIndex].tileSprite.setOrigin({ objectData.sheetSize.x * 0.5f, objectData.sheetSize.y * 0.5f });
            categories[0][objIndex].tileSprite.setScale({ size / categories[0][objIndex].sheetID_W , size / categories[0][objIndex].sheetID_H });
            categories[0][objIndex].tileSprite.setPosition(IndexToPos(index));
            ++objIndex;
            ++index;
        }
    }

	index = 0;
	int groundIndex = 0;
	for (int i = 0; i < (int)GroundType::COUNT; ++i)
	{
		int groundCount = GROUND_TABLE->Count((GroundType)i);
		for (int j = 0; j < groundCount; ++j)
		{
			auto groundData = GROUND_TABLE->Get((GroundType)i, j);
			categories[1].push_back(MapSheet());
			std::string groundTextureId = GROUND_TABLE->GetTextureId();
			categories[1][groundIndex].resource = groundTextureId;
			categories[1][groundIndex].objectID = groundData.groundId;
			categories[1][groundIndex].sheetID_X = groundData.sheetId.x;
			categories[1][groundIndex].sheetID_Y = groundData.sheetId.y;
			categories[1][groundIndex].sheetID_W = groundData.sheetSize.x;
			categories[1][groundIndex].sheetID_H = groundData.sheetSize.y;
			categories[1][groundIndex].indexNumber = index;
			categories[1][groundIndex].tileSprite.setTextureRect({ groundData.sheetId.x, groundData.sheetId.y, groundData.sheetSize.x, groundData.sheetSize.y });
			categories[1][groundIndex].tileSprite.setOrigin({ groundData.sheetSize.x * 0.5f, groundData.sheetSize.y * 0.5f });
			categories[1][groundIndex].tileSprite.setScale({ size / categories[1][groundIndex].sheetID_W , size / categories[1][groundIndex].sheetID_H });
			categories[1][groundIndex].tileSprite.setPosition(IndexToPos(index));
			categories[1][groundIndex].tileSprite.setTexture(RES_MGR_TEXTURE.Get(groundTextureId));
			++groundIndex;
			++index;
		}
	}

    index = 0;
    int floorIndex = 0;
    for (int i = 0; i < (int)FloorType::COUNT; ++i)
    {
        int floorCount = FLOOR_TABLE->Count((FloorType)i);
        for (int j = 0; j < floorCount; ++j)
        {
            auto floorData = FLOOR_TABLE->Get((FloorType)i, j);
            std::string floorTextureId = floorData.textureId;
            categories[2].push_back(MapSheet());
            categories[2][floorIndex].resource = floorTextureId;
            categories[2][floorIndex].objectID = floorData.floorId;
            categories[2][floorIndex].sheetID_X = floorData.sheetId.x;
            categories[2][floorIndex].sheetID_Y = floorData.sheetId.y;
            categories[2][floorIndex].sheetID_W = floorData.sheetSize.x;
            categories[2][floorIndex].sheetID_H = floorData.sheetSize.y;
            categories[2][floorIndex].indexNumber = index;
            categories[2][floorIndex].tileSprite.setTextureRect({ floorData.sheetId.x, floorData.sheetId.y, floorData.sheetSize.x, floorData.sheetSize.y });
            categories[2][floorIndex].tileSprite.setOrigin({ floorData.sheetSize.x * 0.5f, floorData.sheetSize.y * 0.5f });
            categories[2][floorIndex].tileSprite.setScale({ size / categories[2][floorIndex].sheetID_W , size / categories[2][floorIndex].sheetID_H });
            categories[2][floorIndex].tileSprite.setPosition(IndexToPos(index));
            categories[2][floorIndex].tileSprite.setTexture(RES_MGR_TEXTURE.Get(floorTextureId));
            ++floorIndex;
            ++index;
        }
    }
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
    sf::Vector2f mouseUIPos = SCENE_MGR.GetCurrentScene()->ScreenToUi(mousePos);

    if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        SelectTile(PosToIndex(mouseUIPos));

        if (arrowLeft.GetGlobalBounds().contains(mouseUIPos))
        {
            --currentPage;
            if (currentPage < 0)
            {
                currentPage = categories.size() - 1;
            }
        }
        else if (arrowRight.GetGlobalBounds().contains(mouseUIPos))
        {
            ++currentPage;
            if (currentPage >= categories.size())
            {
                currentPage = 0;
            }
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

    for (auto& obj : categories[currentPage])
    {
        window.draw(obj.tileSprite);
    }

    saveButton.Draw(window);
    eraseButton.Draw(window);
    loadButton.Draw(window);
    moveScreenButton.Draw(window);
}
