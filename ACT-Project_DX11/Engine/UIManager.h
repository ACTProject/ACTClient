#pragma once
#include "Ui.h"

class UIManager
{
    DECLARE_SINGLE(UIManager);
public:
    void AddUI(shared_ptr<Ui> ui);
    void ReMoveUI(shared_ptr<GameObject> obj);
    void Update();

    vector<shared_ptr<Ui>> GetUIList() { return _uiList; }
private:
    vector<shared_ptr<Ui>> _uiList;
};

