#pragma once
#include "Ui.h"
#include "Button.h"

class UIManager
{
    DECLARE_SINGLE(UIManager);
public:
    void AddUI(const string& name, shared_ptr<Ui> ui);

    void AddButton(shared_ptr<Button> btn);
    void RemoveUI(const string& name);
    void Update();

    void RemoveButton(shared_ptr<GameObject> obj);
    void ResizeButton(float width, float height);
    shared_ptr<Ui> GetUi(const string& name);
private:
    unordered_map<string, shared_ptr<Ui>> _uiList;
    vector<shared_ptr<Button>> _buttonList;
};

