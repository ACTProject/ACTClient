#include "pch.h"
#include "UIManager.h"

void UIManager::AddUI(shared_ptr<Ui> ui)
{
    _uiList.push_back(ui);
}

void UIManager::ReMoveUI(shared_ptr<GameObject> obj)
{
    auto it = std::find(_uiList.begin(), _uiList.end(), obj->GetUI());

    if (it != _uiList.end())
    {
        _uiList.erase(it);
    }
}

void UIManager::Update()
{
    for (auto& ui : _uiList)
    {
        ui->Update();
    }
}
