#include "pch.h"
#include "SpriteGo.h"

SpriteGo::SpriteGo(const std::string& name)
	: GameObject(name)
{
}

sf::FloatRect SpriteGo::GetLocalBounds()
{
	return sprite.getLocalBounds();
}

sf::FloatRect SpriteGo::GetGlobalBounds()
{
	return sprite.getGlobalBounds();
}

void SpriteGo::SetTexture(const std::string& textureId)
{
	this->textureId = textureId;
	sprite.setTexture(RES_MGR_TEXTURE.Get(textureId));
}

void SpriteGo::SetTextureByName(const std::string& name)
{
	SetTexture(OBJECT_TABLE->Get(name).textureId);
	SetTextureRect(sf::IntRect(OBJECT_TABLE->Get(name).sheetId.x, OBJECT_TABLE->Get(name).sheetId.y,
		OBJECT_TABLE->Get(name).sheetSize.x, OBJECT_TABLE->Get(name).sheetSize.y));
}

void SpriteGo::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTextureRect(rect);
}

void SpriteGo::SetFillColor(sf::Color color)
{
	sprite.setColor(color);
}

sf::Color SpriteGo::GetFillColor()
{
	return sprite.getColor();
}

void SpriteGo::SetPosition(const sf::Vector2f& pos)
{
	//GameObject::SetPosition(pos);
	position = pos;
	sprite.setPosition(pos);
}

void SpriteGo::Translate(const sf::Vector2f& delta)
{
	position += delta;
	SetPosition(position);
}

void SpriteGo::SetRotation(const float r)
{
	rotation = r;
	sprite.setRotation(r);
}

void SpriteGo::SetOrigin(Origins preset)
{
	if (preset == Origins::Custom)
	{
		preset = Origins::TL;
	}
	else if (preset == Origins::Object)
	{
		sprite.setOrigin({ (float)(sprite.getLocalBounds().width * 0.5f), (float)(sprite.getLocalBounds().height - 7.5f) });
		return;
	}
	originPreset = preset;
	origin = Utils::SetOrigin(sprite, originPreset);
}

void SpriteGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	sprite.setOrigin(newOrigin);
}

void SpriteGo::SetScale(const sf::Vector2f& s)
{
	sf::Vector2f spriteScale = scale = s;
	if (isFlipX)
	{
		spriteScale.x = -scale.x;
	}
	if (isFlipY)
	{
		spriteScale.y = -scale.y;
	}
	sprite.setScale(spriteScale);
}

void SpriteGo::SetFlipX(bool filp)
{
	if (isFlipX == filp)
		return;

	isFlipX = filp;
	SetScale(scale);
}

void SpriteGo::SetFlipY(bool filp)
{
	if (isFlipY == filp)
		return;

	isFlipY = filp;
	SetScale(scale);
}

void SpriteGo::Reset()
{
	// ���� �ٽ� ������ ��, �ؽ��Ͱ� ���� ��Ȳ�� �߻��� �� �����Ƿ�...
	sprite.setTexture(RES_MGR_TEXTURE.Get(textureId));
}

void SpriteGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	if (hasHitBox && SCENE_MGR.GetDeveloperMode())
	{
		sf::RectangleShape globalHitBox;
		sf::FloatRect globalBound = sprite.getGlobalBounds();

		sf::RectangleShape imageBox;

		sf::RectangleShape localHitBox;
		sf::FloatRect localBound = sprite.getLocalBounds();

		globalHitBox.setPosition(globalBound.left, globalBound.top);
		globalHitBox.setSize({ globalBound.width, globalBound.height });
		globalHitBox.setOutlineColor(sf::Color::Red);
		globalHitBox.setOutlineThickness(1.f);
		globalHitBox.setFillColor(sf::Color::Transparent);

		imageBox.setPosition(position);
		imageBox.setOrigin(origin);
		imageBox.setRotation(sprite.getRotation());
		imageBox.setSize({ localBound.width, localBound.height });
		imageBox.setOutlineColor(sf::Color::Green);
		imageBox.setOutlineThickness(1.f);
		imageBox.setFillColor(sf::Color::Transparent);

		localHitBox.setPosition(localBound.left, localBound.top);
		localHitBox.setSize({ localBound.width, localBound.height });
		localHitBox.setOutlineColor(sf::Color::Blue);
		localHitBox.setOutlineThickness(1.f);
		localHitBox.setFillColor(sf::Color::Transparent);

		window.draw(globalHitBox);
		window.draw(imageBox);
		window.draw(localHitBox);
	}
}
