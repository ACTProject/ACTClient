#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	// Weapon
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		// FBX -> Memory
		converter->ReadAssetFile(L"Fork/Fork.fbx");

		// Memory -> CustomData (File)
		converter->ExportMaterialData(L"Fork/Fork");
		converter->ExportModelData(L"Fork/Fork");

		// CustomData (File) -> Memory
	}

	// Player
	{
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Player.fbx");
			converter->ExportMaterialData(L"Player/Player");
			converter->ExportModelData(L"Player/Player");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Crab_Idle.fbx");
			converter->ExportAnimationData(L"Player/Idle");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Crab_Walk.fbx");
			converter->ExportAnimationData(L"Player/Walk");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Crab_Run.fbx");
			converter->ExportAnimationData(L"Player/Run");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Crab_Atk_Combo1.fbx");
			converter->ExportAnimationData(L"Player/Crab_Atk_Combo1");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Crab_Atk_Combo2.fbx");
			converter->ExportAnimationData(L"Player/Crab_Atk_Combo2");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Crab_Atk_Combo3.fbx");
			converter->ExportAnimationData(L"Player/Crab_Atk_Combo3");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Player/Crab_Atk_Combo4.fbx");
			converter->ExportAnimationData(L"Player/Crab_Atk_Combo4");
		}
	}
	// Enemy
	{
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_mesh.fbx");
			converter->ExportMaterialData(L"Enemy/Rangoon");
			converter->ExportModelData(L"Enemy/Rangoon");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_aggro.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_aggro");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_atkBigSnippy.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_atkBigSnippy");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_atkSmallSnippy.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_atkSmallSnippy");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_atkSmash.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_atkSmash");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_die.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_die");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_hit.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_hit");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_idle.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_idle");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_roar.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_roar");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"Enemy/Rangoon_run.fbx");
			converter->ExportAnimationData(L"Enemy/Rangoon_run");
		}
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/pistol_mesh.fbx");
            converter->ExportMaterialData(L"Enemy/pistol");
            converter->ExportModelData(L"Enemy/pistol");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/pistol_idle.fbx");
            converter->ExportAnimationData(L"Enemy/pistol_idle");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/pistol_shoot.fbx");
            converter->ExportAnimationData(L"Enemy/pistol_shoot");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/pistol_aggro.fbx");
            converter->ExportAnimationData(L"Enemy/pistol_aggro");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/bullet.fbx");
            converter->ExportMaterialData(L"Enemy/bullet");
            converter->ExportModelData(L"Enemy/bullet");
        }
	}
	// MapObject
	{
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/recyclingBox01.fbx");
			converter->ExportMaterialData(L"MapObject/recyclingBox01");
			converter->ExportModelData(L"MapObject/recyclingBox01");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/WallLeft.fbx");
			converter->ExportMaterialData(L"MapObject/WallLeft");
			converter->ExportModelData(L"MapObject/WallLeft");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/flagpole.fbx");
			converter->ExportMaterialData(L"MapObject/flagpole");
			converter->ExportModelData(L"MapObject/flagpole");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/pigeon.fbx");
			converter->ExportMaterialData(L"MapObject/pigeon");
			converter->ExportModelData(L"MapObject/pigeon");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/StarFish.fbx");
			converter->ExportMaterialData(L"MapObject/StarFish");
			converter->ExportModelData(L"MapObject/StarFish");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/PencilHedgehog.fbx");
			converter->ExportMaterialData(L"MapObject/PencilHedgehog");
			converter->ExportModelData(L"MapObject/PencilHedgehog");
		}

		//
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Bottle.fbx");
			converter->ExportMaterialData(L"MapObject/Bottle");
			converter->ExportModelData(L"MapObject/Bottle");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/CardHouseEntity.fbx");
			converter->ExportMaterialData(L"MapObject/CardHouseEntity");
			converter->ExportModelData(L"MapObject/CardHouseEntity");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Cylinder01.fbx");
			converter->ExportMaterialData(L"MapObject/Cylinder01");
			converter->ExportModelData(L"MapObject/Cylinder01");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Cylinder07.fbx");
			converter->ExportMaterialData(L"MapObject/Cylinder07");
			converter->ExportModelData(L"MapObject/Cylinder07");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/KelpGround.fbx");
			converter->ExportMaterialData(L"MapObject/KelpGround");
			converter->ExportModelData(L"MapObject/KelpGround");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/road.fbx");
			converter->ExportMaterialData(L"MapObject/road");
			converter->ExportModelData(L"MapObject/road");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/rock.fbx");
			converter->ExportMaterialData(L"MapObject/rock");
			converter->ExportModelData(L"MapObject/rock");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Umbrella.fbx");
			converter->ExportMaterialData(L"MapObject/Umbrella");
			converter->ExportModelData(L"MapObject/Umbrella");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/wall01.fbx");
			converter->ExportMaterialData(L"MapObject/wall01");
			converter->ExportModelData(L"MapObject/wall01");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/wall02.fbx");
			converter->ExportMaterialData(L"MapObject/wall02");
			converter->ExportModelData(L"MapObject/wall02");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/moonjelly.fbx");
			converter->ExportMaterialData(L"MapObject/moonjelly");
			converter->ExportModelData(L"MapObject/moonjelly");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/rock2.fbx");
			converter->ExportMaterialData(L"MapObject/rock2");
			converter->ExportModelData(L"MapObject/rock2");
		}
		//
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Urock01.fbx");
			converter->ExportMaterialData(L"MapObject/Urock01");
			converter->ExportModelData(L"MapObject/Urock01");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Urock02.fbx");
			converter->ExportMaterialData(L"MapObject/Urock02");
			converter->ExportModelData(L"MapObject/Urock02");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Urock03.fbx");
			converter->ExportMaterialData(L"MapObject/Urock03");
			converter->ExportModelData(L"MapObject/Urock03");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Urock04.fbx");
			converter->ExportMaterialData(L"MapObject/Urock04");
			converter->ExportModelData(L"MapObject/Urock04");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Urock05.fbx");
			converter->ExportMaterialData(L"MapObject/Urock05");
			converter->ExportModelData(L"MapObject/Urock05");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Udrock01.fbx");
			converter->ExportMaterialData(L"MapObject/Udrock01");
			converter->ExportModelData(L"MapObject/Udrock01");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Udrock02.fbx");
			converter->ExportMaterialData(L"MapObject/Udrock02");
			converter->ExportModelData(L"MapObject/Udrock02");
		}
		{
			shared_ptr<Converter> converter = make_shared<Converter>();
			converter->ReadAssetFile(L"MapObject/Udrock04.fbx");
			converter->ExportMaterialData(L"MapObject/Udrock04");
			converter->ExportModelData(L"MapObject/Udrock04");
		}

        //
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/Chips.fbx");
            converter->ExportMaterialData(L"MapObject/Chips");
            converter->ExportModelData(L"MapObject/Chips");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/FishCar.fbx");
            converter->ExportMaterialData(L"MapObject/FishCar");
            converter->ExportModelData(L"MapObject/FishCar");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/Milk Carton.fbx");
            converter->ExportMaterialData(L"MapObject/Milk Carton");
            converter->ExportModelData(L"MapObject/Milk Carton");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/Prop_ChineseTakeout.fbx");
            converter->ExportMaterialData(L"MapObject/Prop_ChineseTakeout");
            converter->ExportModelData(L"MapObject/Prop_ChineseTakeout");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/80s_radio.fbx");
            converter->ExportMaterialData(L"MapObject/80s_radio");
            converter->ExportModelData(L"MapObject/80s_radio");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/BottomFeeders_Barrels.fbx");
            converter->ExportMaterialData(L"MapObject/BottomFeeders_Barrels");
            converter->ExportModelData(L"MapObject/BottomFeeders_Barrels");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/can_crushed_03.fbx");
            converter->ExportMaterialData(L"MapObject/can_crushed_03");
            converter->ExportModelData(L"MapObject/can_crushed_03");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/CastleKit_Tower_01.fbx");
            converter->ExportMaterialData(L"MapObject/CastleKit_Tower_01");
            converter->ExportModelData(L"MapObject/CastleKit_Tower_01");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"MapObject/Urock06.fbx");
            converter->ExportMaterialData(L"MapObject/Urock06");
            converter->ExportModelData(L"MapObject/Urock06");
        }


	}
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
