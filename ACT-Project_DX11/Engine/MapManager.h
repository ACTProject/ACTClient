#pragma once

enum MapObjSelect
{
	START = 0
};


struct MapObjDesc
{
public:
	bool isCollision = true;
	Vec3 extent = Vec3{ 1.0f };
	Vec3 offset = Vec3{ 0.0f };
	Vec3 pos = { 0.f,0.f,0.f };
	Vec3 scale = { 0.001f,0.001f,0.001f };
	Vec3 rotation = {0.0f, 0.0f, 0.0f};
	int fileLength = 0;
	int shaderLength = 0;
	wstring filename;
	wstring shadername;
	MapObjDesc() = default;
	MapObjDesc(wstring file, wstring sha, bool isColl = true) : filename(file), shadername(sha), isCollision(isColl) {};
};

class MapManager
{
	DECLARE_SINGLE(MapManager);
public:
	void Init();
	void Update();
	bool ChekMapDescSelect() { return  _mapSelectDesc == nullptr; };
	bool ChekMapObj() { return _mapSelectObj != nullptr; };
	bool ChekMapObjectSelect(shared_ptr<GameObject> obj);

	// �� �Լ��� Ŭ������ �� �߰��ϴ� �Լ�.
	shared_ptr<GameObject> Create(Vec3& pos);


	// �ʱ� init
	void AddMapObj(shared_ptr<MapObjDesc>  obj);
	void InitMapText();

	// ���Ͽ� �� ������Ʈ ���� �߰��ϴ� �Լ�
	bool ExportMapObj();
	// ������ �о� ���̴� �Լ�
	bool ImportMapObj();
private:
	// Ŭ������ �ʰ� �ʿ�����Ʈ ��ġ ���� ���̰Բ� �ϴ� �Լ�.
	void PreViewMapObject();
	shared_ptr<GameObject> CreatePreViewObj(Vec3 pickPos);

	void UpdateMapDescTransform();
	void UpdateMapObjTransform();
	void RemoveMapObj(shared_ptr<GameObject> obj);

	void ChangeMapObjPosition();
	void ChangeMapObjRotation();
	void ChangeMapObjScale();

	// �� �Լ��� ���Ͽ��� �ҷ��� �� ����ϴ� �Լ�
	shared_ptr<GameObject> Create(MapObjDesc& obj);
	// ImGui���� �ʿ�����Ʈ ���� �Լ�.
	void ImGuiSelectMapObject();
private:
	// ���� ����� �����̸�.
	wstring _fileName = L"../Resources/MapObj/MapObjectLists.txt";
	// ���Ϸ� �����ų ������Ʈ�� ����Ʈ
	vector<shared_ptr<GameObject>> _mapObjList;
	// ������ ����ü �迭
	vector<shared_ptr<MapObjDesc>> _mapInfoList;
	// �ʱ� �� ���� ����ü �迭.
	vector<shared_ptr<MapObjDesc>> _mapInitInfoList;
	shared_ptr<MapObjDesc> _mapSelectDesc;
	shared_ptr<GameObject> _mapSelectObj;
	shared_ptr<GameObject> _mapPreviewObj;

	Vec3 _pickPos;
	vector<string> _fileTextList;
	int _selectedObjIdx = -1;
	int _transformSelect = 0;
	bool _isZState = false;
	bool _isSelect = false;
};