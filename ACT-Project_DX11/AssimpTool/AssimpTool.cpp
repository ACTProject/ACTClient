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
    // Shell
    {
        shared_ptr<Converter> converter = make_shared<Converter>();
        converter->ReadAssetFile(L"Shell/Shell_SodaCan.fbx");
        converter->ExportMaterialData(L"Shell/Shell_SodaCan");
        converter->ExportModelData(L"Shell/Shell_SodaCan");
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
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Player/Crab_Jump.fbx");
            converter->ExportAnimationData(L"Player/Crab_Jump");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Player/Crab_Dodge.fbx");
            converter->ExportAnimationData(L"Player/Crab_Dodge");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Player/Crab_BlockingIdle.fbx");
            converter->ExportAnimationData(L"Player/Crab_BlockingIdle");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Player/Crab_BlockingCrawl.fbx");
            converter->ExportAnimationData(L"Player/Crab_BlockingCrawl");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Player/Crab_Death.fbx");
            converter->ExportAnimationData(L"Player/Crab_Death");
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
            converter->ReadAssetFile(L"Enemy/pistol_hit.fbx");
            converter->ExportAnimationData(L"Enemy/pistol_hit");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/pistol_die.fbx");
            converter->ExportAnimationData(L"Enemy/pistol_die");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/bullet.fbx");
            converter->ExportMaterialData(L"Enemy/bullet");
            converter->ExportModelData(L"Enemy/bullet");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/bubble.fbx");
            converter->ExportMaterialData(L"Enemy/bubble");
            converter->ExportModelData(L"Enemy/bubble");
        }
        {
            shared_ptr<Converter> converter = make_shared<Converter>();
            converter->ReadAssetFile(L"Enemy/money.fbx");
            converter->ExportMaterialData(L"Enemy/money");
            converter->ExportModelData(L"Enemy/money");
        }
        //MR_Krab
        {
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/mrKRAB.fbx");
                converter->ExportMaterialData(L"Enemy/FinalBoss/mrKrab");
                converter->ExportModelData(L"Enemy/FinalBoss/mrKrab");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/00_mrKRAB_idle.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/00_mrKRAB_idle");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/01_boss_walk.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/01_boss_walk");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/02_boss_roar.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/02_boss_roar");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/03_warming_up.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/03_warming_up");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/04_fight_idle.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/04_fight_idle");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/05_run_forward.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/05_run_forward");
            } {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/06_sprint.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/06_sprint");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/07_back_sprint.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/07_back_sprint");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/08_jumping.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/08_jumping");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/09_hit_1.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/09_hit_1");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/10_hit_2.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/10_hit_2");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/11_boxing_1.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/11_boxing_1");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/12_boxing_2.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/12_boxing_2");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/13_boxing_3.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/13_boxing_3");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/14_boxing_4.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/14_boxing_4");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/15_getUp_behind.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/15_getUp_behind");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/16_getUp_front.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/16_getUp_front");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/17_knockedDown_back.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/17_knockedDown_back");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/18_knockedDown_forward.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/18_knockedDown_forward");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/19_choke_lift.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/19_choke_lift");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/20_fireball.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/20_fireball");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/21_magic_spell.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/21_magic_spell");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/22_sweep.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/22_sweep");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/23_thrust_slash.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/23_thrust_slash");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/24_uppercut.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/24_uppercut");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/25_gettingThrown.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/25_gettingThrown");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/26_grabSlam.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/26_grabSlam");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/27_hurricane.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/27_hurricane");
            }
            {
                shared_ptr<Converter> converter = make_shared<Converter>();
                converter->ReadAssetFile(L"Enemy/FinalBoss/28_dying.fbx");
                converter->ExportAnimationData(L"Enemy/FinalBoss/28_dying");
            }
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
