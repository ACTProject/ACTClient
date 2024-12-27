#include "pch.h"
#include "UIManager.h"

void UIManager::AddUI(const string& name, shared_ptr<Ui> ui) {
    _uiList[name] = ui;
}

void UIManager::RemoveUI(const string& name) {
    _uiList.erase(name);
}

shared_ptr<Ui> UIManager::GetUi(const string& name) {
    auto it = _uiList.find(name);
    if (it != _uiList.end())
        return it->second;
    return nullptr; // 찾지 못하면 nullptr 반환
}


void UIManager::Update()
{

}
