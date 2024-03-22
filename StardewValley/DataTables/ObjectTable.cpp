#include "pch.h"
#include "ObjectTable.h"

DataObject ObjectTable::Undefined;

ObjectTable::ObjectTable(DataTable::Types type) : DataTable(type)
{
}

ObjectTable::~ObjectTable()
{
	Release();
}

const DataObject& ObjectTable::Get(ObjectType type, int id)
{
	auto find = table.find(std::make_tuple(type, id));
	if (find == table.end())
	{
		std::cout << "Obejct Data에 해당 Type과 ID가 없습니다" << std::endl;
		return Undefined;
	}
	return find->second;
}

const DataObject& ObjectTable::Get(const std::string& name)
{
	auto find = nameTable.find(name);
	if (find == nameTable.end())
	{
		std::cout << "Obejct Data에 해당 이름이 없습니다" << std::endl;
		return Undefined;
	}

	return Get(std::get<0>(find->second), std::get<1>(find->second));
}

bool ObjectTable::Load(rapidjson::Document& doc)
{
	Release();

	auto infoArr = doc["Tile Info"]["Object"]["Type Info"].GetArray();
	int objectTypeCount = infoArr.Size();
	for (int i = 0; i < objectTypeCount; ++i)
	{
		int objectIdCount = infoArr[i]["Sheet Info"].Size();
		for (int j = 0; j < objectIdCount; ++j)
		{
			std::tuple<ObjectType, int> key = std::make_tuple((ObjectType)i, j);

			if (table.find(key) != table.end())
			{
				std::cout << "object table 중복 아이디" << std::endl;
				return false;
			}

			table[key] = DataObject();
			table[key].objectType = (ObjectType)i;
			table[key].objectId = j;
			table[key].textureId = infoArr[i]["Sheet Info"][j]["Resource"].GetString();
			table[key].sheetId = { infoArr[i]["Sheet Info"][j]["Sheet ID X"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID Y"].GetInt() };
			table[key].sheetSize = { infoArr[i]["Sheet Info"][j]["Sheet ID W"].GetInt(), infoArr[i]["Sheet Info"][j]["Sheet ID H"].GetInt() };

			nameTable[infoArr[i]["Sheet Info"][j]["Object Name"].GetString()] = key;
		}
	}

	return true;
}

void ObjectTable::Release()
{
	table.clear();
}
