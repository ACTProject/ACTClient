#pragma once
#include "Model.h"
#include "Shader.h"
#include "Terrain.h"
#include "DynamicObj.h"



struct MapObjDesc
{
public:
    bool isAnim = false;
    bool isDynamic = false;
    DynamicType dynamicType = DynamicType::None;
    bool isMesh = false;
	bool isCollision = true;
    bool isBillBoard = false;
    int  billboardCount = 0;
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
    MapObjDesc(wstring file, wstring sha, bool isanim = false, bool isdynamic = false, DynamicType type = DynamicType::None, bool isColl = true, bool isMes = false, bool isbill = false) : filename(file), shadername(sha),isAnim(isanim), isDynamic(isdynamic), dynamicType(type), isCollision(isColl), isMesh(isMes), isBillBoard(isbill) {};
};

struct MapModel
{
	shared_ptr<Model> _model = nullptr;
    shared_ptr<Material> _material = nullptr;
	shared_ptr<Mesh> _mesh = nullptr;
	shared_ptr<Shader> _shader = nullptr;
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

	// 이 함수는 클릭했을 때 추가하는 함수.
	shared_ptr<GameObject> Create(Vec3& pos);

    void SetTerrain(shared_ptr<Terrain> vec) { _terrain = vec; };

	// 초기 init
	void AddMapObj(shared_ptr<MapObjDesc>  obj);
	void InitMapText();

    Matrix UpdateWaveMat();


	// 파일에 맵 오브젝트 정보 추가하는 함수
	bool ExportMapObj(wstring _fileName);
	// 파일을 읽어 들이는 함수
	bool ImportMapObj(wstring _fileName);
private:
    // 동적맵오브젝트 생성 함수.
    void CreateDynamicObject(shared_ptr<GameObject> obj, DynamicType type);

    void ClearMap();
    // 빌보드 요소 추가 함수
    void AddBillBoard(Vec3 pos);

	// 클릭하지 않고도 맵오브젝트 설치 전에 보이게끔 하는 함수.
	void PreViewMapObject();
	shared_ptr<GameObject> CreatePreViewObj(Vec3 pickPos);

	void UpdateMapDescTransform();
	void UpdateMapObjTransform();
	void RemoveMapObj(shared_ptr<GameObject> obj);

	void ChangeMapObjPosition();
	void ChangeMapObjRotation();
	void ChangeMapObjScale();

    // 바닥에 붙이는 함수.
    void CreateQuadTerrain(shared_ptr<Mesh> mesh, shared_ptr<GameObject> obj, Vec3 pos);

    void CreateBillBoardMesh(shared_ptr<MapObjDesc> objs);

	// 이 함수는 파일에서 불러올 때 사용하는 함수
	shared_ptr<GameObject> Create(MapObjDesc& obj);
	// ImGui에서 맵오브젝트 고르는 함수.
	void ImGuiSelectMapObject();
private:
	// 정보 저장된 파일이름.
	wstring _fileNames = L"../Resources/MapFile/MapObjectLists.txt";
	wstring _meshFileName = L"..\\Resources\\Textures\\Terrain\\";
	// 파일로 저장시킬 오브젝트들 리스트
	vector<shared_ptr<GameObject>> _mapObjList;
	// 맵정보 구조체 배열
	map<wstring, shared_ptr<MapModel>> _mapInfoList;
	// 초기 맵 정보 구조체 배열.
	vector<shared_ptr<MapObjDesc>> _mapInitInfoList;

    // 빌보드 설치 위치 저장 배열
    vector<Vec3> _mapBillBoradList;

	shared_ptr<MapObjDesc> _mapSelectDesc;
	shared_ptr<GameObject> _mapSelectObj;
	shared_ptr<GameObject> _mapPreviewObj;
    shared_ptr<GameObject> _mapBillBoard;

	Vec3 _pickPos;
	vector<string> _fileTextList;
	int _selectedObjIdx = -1;
	int _transformSelect = 0;
	bool _isZState = false;
	bool _isSelect = false;

    int _billBoardCount = 0;
    float amplitude = 0.3f; // 흔들림의 크기 (전단 계수의 최대값)
    float frequency = 0.5f; // 흔들림의 주파수 (주기의 빠르기)

    shared_ptr<Terrain> _terrain;
};
