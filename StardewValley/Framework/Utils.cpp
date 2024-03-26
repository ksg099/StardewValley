#include "pch.h"
#include "Utils.h"
#include <filesystem>

float Utils::Clamp(float v, float min, float max)
{
	if (v < min)
		return min;
	if (v > max)
		return max;
	return v;
}

sf::Vector2f Utils::Clamp(const sf::Vector2f& v, const sf::FloatRect& rect)
{
	return sf::Vector2f(Clamp(v.x, rect.left, rect.left + rect.width),
		Clamp(v.y, rect.top, rect.top + rect.height));
}

sf::FloatRect Utils::ResizeRect(const sf::FloatRect& rect, const sf::Vector2f& delta)
{
	sf::FloatRect newRect = rect;
	newRect.width += delta.x;
	newRect.height += delta.y;

	newRect.left -= delta.x * 0.5f;
	newRect.top -= delta.y * 0.5f;

	return newRect;
}

float Utils::RandomValue()
{
	return (float)rand() / RAND_MAX;
}

float Utils::RandomRange(float min, float max)
{
	float value = RandomValue();
	value *= max - min;	
	value += min;
	return value;
}

sf::Vector2f Utils::RandomOnUnitCircle()
{
	sf::Transform rotation;
	rotation.rotate(RandomRange(0.f, 360.f));
	return rotation * sf::Vector2f (1.f, 0.f);
}

sf::Vector2f Utils::RandomInUnitCircle()
{
	return RandomOnUnitCircle() * RandomValue();
}

sf::Vector2f Utils::RandomInRing(float outRadius, float inRadius)
{
	sf::Vector2f pos(0, 0);
	if (inRadius >= outRadius)
		return pos;
	do
	{
		pos = RandomInUnitCircle() * outRadius;
	} while (Magnitude(pos) <= inRadius);
	return pos;
}

int Utils::RandomRange(int min, int maxExclude)
{
	int range = maxExclude - min;
	int rand = std::rand() % range;
	rand += min;
	return rand;
}

sf::Vector2f Utils::SetOrigin(sf::Transformable& obj, Origins originPreset, const sf::FloatRect& rect)
{
	// Rect Width, Height
	sf::Vector2f newOrigin(rect.width, rect.height);
	newOrigin.x *= ((int)originPreset % 3) * 0.5f; // 0 1 2 => 0 0.5 1
	newOrigin.y *= ((int)originPreset / 3) * 0.5f; // 0 1 2 => 0 0.5 1
	obj.setOrigin(newOrigin);

	return newOrigin;
}

sf::Vector2f Utils::SetOrigin(sf::Sprite& obj, Origins originPreset)
{
	return SetOrigin(obj, originPreset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Text& obj, Origins originPreset)
{
	return SetOrigin(obj, originPreset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Shape& obj, Origins originPreset)
{
	return SetOrigin(obj, originPreset, obj.getLocalBounds());
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

void Utils::Normalize(sf::Vector2f& vec)
{
	float mag = Magnitude(vec);
	if (mag != 0.f)
	{
		vec /= mag;
	}
}

sf::Vector2f Utils::GetNormal(const sf::Vector2f& vec)
{
	float mag = Magnitude(vec);
	if (mag == 0.f)
		return vec;
	return vec / mag;
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return Magnitude(p2 - p1);
}

float Utils::RadianToDegree(float radian)
{
	return radian * (180.0 / M_PI);
}

float Utils::DegreeToRadian(float degree)
{
	return degree * (M_PI / 180.0);
}

float Utils::Angle(const sf::Vector2f& vec)
{
	return RadianToDegree(std::atan2f(vec.y, vec.x));
}

float Utils::Lerp(float min, float max, float t)
{
	// t: 0.0 ~ 1.0
	t = Clamp(t, 0.f, 1.f);
	float v = min + (max - min) * t;
	return v;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t)
{
	t = Clamp(t, 0.f, 1.f);

	return sf::Vector2f(Lerp(min.x, max.x, t), Lerp(min.y, max.y, t));
}

bool Utils::LoadFromFile(const std::string& rFilePath, rapidjson::Document& doc)
{
	// 읽기용 JSON 파일 불러오기
	std::ifstream ifs(rFilePath);

	if (!ifs.is_open())
	{
		std::cout << "TileMap::LoadFromFile(): ifs 파일이 열리지 않았습니다." << std::endl;
		return false;
	}

	// 파싱
	std::string contents((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	if (doc.Parse(contents.c_str()).HasParseError())
	{
		std::cout << "파싱 에러!" << std::endl;
		return false;
	}

	ifs.close();
	return true;
}

void Utils::EditFile(const std::string& wFilePath, rapidjson::Document& doc)
{
	// JSON 문자열 생성
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	// 쓰기용 JSON 파일 불러오고, 해당 파일에 문자열 쓰기
	std::ofstream ofs(wFilePath);
	if (!ofs.is_open())
	{
		std::cout << "TileMap::LoadFromFile(): ofs 파일이 열리지 않았습니다." << std::endl;
		return;
	}
	ofs << buffer.GetString();

	ofs.close();
}

void Utils::ListFiles(const std::string& path, std::vector<std::string>& fileNames)
{
	try {
		// 주어진 경로가 존재하고, 디렉토리인지 확인
		if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
		{
			// 디렉토리 내의 모든 항목을 순회
			for (const auto& entry : std::filesystem::directory_iterator(path))
			{
				auto fileName = entry.path().filename().string(); // 파일 이름 추출
				if (fileName.find(".json") != std::string::npos) // json 파일 검사
				{
					std::cout << fileName << std::endl;
					fileNames.push_back(path + "/" + fileName);
				}
			}
		}
		else
		{
			std::cout << "Path does not exist or is not a directory." << std::endl;
		}
	}
	catch (const std::filesystem::filesystem_error& err)
	{
		std::cerr << "Filesystem error: " << err.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "General error: " << e.what() << std::endl;
	}
}
