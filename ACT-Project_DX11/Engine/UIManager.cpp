#include "pch.h"
#include "UIManager.h"

void UIManager::AddUI(const string& name, shared_ptr<Ui> ui) {
    _uiList[name] = ui;
}

void UIManager::AddButton(shared_ptr<Button> btn)
{
    _buttonList.push_back(btn);
}

void UIManager::RemoveUI(const string& name) {
    _uiList.erase(name);
}

void UIManager::RemoveButton(shared_ptr<GameObject> obj)
{
    if (obj->GetButton() == nullptr)
        return;

    if (obj->GetUI() != nullptr && obj->GetUI()->GetUIState() == UiState::STATIC)
        return;

    auto btn = obj->GetButton();

    auto it = find(_buttonList.begin(),_buttonList.end(), btn);
    if (it != _buttonList.end())
    {
        _buttonList.erase(it);
    }
}

void UIManager::ResizeButton(float width, float height)
{
    for (auto btn : _buttonList)
    {
        btn->ResizeRect(width,height);
    }
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
