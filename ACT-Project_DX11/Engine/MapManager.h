#pragma once

enum MapObjSelect
{
	START = 0
};


struct MapObjDesc
{
public:
	Vec3 pos = { 0.f,0.f,0.f };
	Vec3 scale = { 0.001f,0.001f,0.001f };
	int fileLength = 0;
	int shaderLength = 0;
	wstring filename;
	wstring shadername;
	MapObjDesc() = default;
	MapObjDesc(wstring file, wstring sha) : filename(file), shadername(sha) {};
	MapObjDesc(wstring file, wstring sha, Vec3 scal) : filename(file), shadername(sha), scale(scal) {};
};

class MapManager
{
	DECLARE_SINGLE(MapManager);
public:
	void Init();
	bool ChekMapObjSelect() { return _selectObject != nullptr; };
	void Update();
	// �� �Լ��� Ŭ������ �� �߰��ϴ� �Լ�.
	shared_ptr<GameObject> Create(Vec3& pos);
	// �� �Լ��� ���Ͽ��� �ҷ��� �� ����ϴ� �Լ�
	shared_ptr<GameObject> Create(MapObjDesc& obj);
	// �ʱ� init
	void AddMapObj(shared_ptr<MapObjDesc>  obj);

	// ���Ͽ� �� ������Ʈ ���� �߰��ϴ� �Լ�
	bool ExportMapObj();
	// ������ �о� ���̴� �Լ�
	bool ImportMapObj();
private:
	// ���� ����� �����̸�.
	wstring _fileName = L"../Resources/MapObj/MapObjectLists.txt";
	// ���Ϸ� �����ų ������Ʈ�� ����Ʈ
	vector<shared_ptr<GameObject>> _mapObjList;
	// ������ ����ü �迭
	vector<shared_ptr<MapObjDesc>> _mapInfoList;
	shared_ptr<MapObjDesc> _selectObject;
};