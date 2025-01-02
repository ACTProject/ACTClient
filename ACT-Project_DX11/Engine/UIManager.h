#pragma once
#include "Ui.h"

class UIManager
{
    DECLARE_SINGLE(UIManager);
public:
    void AddUI(const string& name, shared_ptr<Ui> ui);
    void AddObjUi(shared_ptr<Ui> ui);
    void RemoveUI(const string& name);
    void RemoveUIObj(shared_ptr<GameObject> obj);
    void Update();

    void UpdatePosition(float width, float height);
    shared_ptr<Ui> GetUi(const string& name);

    void ClearUI();
private:
    unordered_map<string, shared_ptr<Ui>> _uiList;
    vector<shared_ptr<Ui>> _ObjUiList;
};

