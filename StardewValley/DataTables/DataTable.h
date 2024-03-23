#pragma once

class DataTable
{
public:
	enum class Types
	{
		GROUND,
		FLOOR,
		OBJECT,
		ITEM,
		SAVE,
	};
	static const int TotalTypes = 3;
	
private:
	Types type;

public:
	DataTable(Types t) : type(t) {};
	virtual ~DataTable() = default;

	// TO-DO: ����, �̵� ������ �� ���Կ����� delete
	DataTable(const DataTable& ref) = delete;
	DataTable& operator=(const DataTable& rhs) = delete;
	DataTable(DataTable&& ref) = delete;
	DataTable& operator=(DataTable&& rhs) = delete;

	virtual bool Load(rapidjson::Document& doc) = 0;
	virtual void Release() = 0;
};

