#pragma once
#include "Ui.h"

class UIManager
{
    DECLARE_SINGLE(UIManager);
public:
    void AddUI(const string& name, shared_ptr<Ui> ui);
    void RemoveUI(const string& name);
    void Update();

    shared_ptr<Ui> GetUi(const string& name);
private:
    unordered_map<string, shared_ptr<Ui>> _uiList;
};

