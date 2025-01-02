#include "pch.h"
#include "UIManager.h"
#include "Button.h"

void UIManager::AddUI(const string& name, shared_ptr<Ui> ui) {
    _uiList[name] = ui;
}

void UIManager::AddObjUi(shared_ptr<Ui> ui)
{
    _ObjUiList.push_back(ui);
}

void UIManager::RemoveUI(const string& name) {
    _uiList.erase(name);
}

void UIManager::RemoveUIObj(const shared_ptr<GameObject> obj)
{
    if (obj->GetUI() != nullptr)
    {
        auto ui = obj->GetUI();
        auto it = std::find(_ObjUiList.begin(), _ObjUiList.end(), ui);
        if (it != _ObjUiList.end())
        {
            _ObjUiList.erase(it);
        }
    }
}

void UIManager::UpdatePosition(float width, float height)
{
    float widthRatio = (float)width / GRAPHICS->GetViewport().GetWidth();
    float heightRatio = (float)height / GRAPHICS->GetViewport().GetHeight();

    for (auto& ui : _ObjUiList)
    {
        Vec3 initScreenPos =  ui->GetScreenPos();
        ui->SetScreenPos(Vec3(initScreenPos.x*widthRatio,initScreenPos.y*heightRatio,0));
        Vec3 pos = ui->GetGameObject()->GetTransform()->GetLocalPosition();

        if (ui->GetuType() == UiType::SLIDER)
        {
            auto sliderUI = std::dynamic_pointer_cast<Slider>(ui);
            Vec3 sub = sliderUI->GetSub();

            pos = Vec3(pos.x * widthRatio, pos.y * heightRatio, pos.z);
            pos = Vec3(pos.x + sub.x,pos.y + sub.y,pos.z);

            ui->GetGameObject()->GetTransform()->SetLocalPosition(pos);
        }
        else
        {
            pos = Vec3(pos.x * widthRatio, pos.y * heightRatio, pos.z);
            ui->GetGameObject()->GetTransform()->SetLocalPosition(pos);
        }


        auto button = ui->GetGameObject()->GetButton();
        if (button != nullptr)
        {
            // ui가 버튼이면 rect 둘 다 변경.
            RECT rect = button->GetRect();
            Vec2 size = button->GetSize();

            Vec3 screenPos = Vec3(initScreenPos.x * widthRatio, initScreenPos.y * heightRatio, initScreenPos.z);
            // rect랑 size를 바꿔끼우기.
            rect.left = screenPos.x - size.x / 2;
            rect.right = screenPos.x + size.x / 2;
            rect.top = screenPos.y - size.y / 2;
            rect.bottom = screenPos.y + size.y / 2;

            button->SetRect(rect);
        }

    }
}

shared_ptr<Ui> UIManager::GetUi(const string& name) {
    auto it = _uiList.find(name);
    if (it != _uiList.end())
        return it->second;
    return nullptr; // 찾지 못하면 nullptr 반환
}

void UIManager::ClearUI()
{
    //unordered_map<string, shared_ptr<Ui>> _uiList;
    //vector<shared_ptr<Ui>> _ObjUiList;

    _uiList.clear();
    _ObjUiList.clear();
}




void UIManager::Update()
{

}
