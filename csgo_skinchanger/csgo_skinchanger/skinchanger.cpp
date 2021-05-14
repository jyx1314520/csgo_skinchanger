#include "skinchanger.h"

extern uintptr_t clientBase;

int modelIndex = 0;


int getKnifeIDef(int knifeID) {
	switch (knifeID) {
	case 0:
		return 500;
	case 2:
		return 505;
	case 4:
		return 507;
	case 5:
		return 508;
	case 6:
		return 509;
	case 7:
		return 512;
	case 14:
		return 520;
	case 16:
		return 522;
	default:
		break;
	}
}


void skinchanger() {
	int knifeIDOffset = knifeID < 10 ? 0 : 1;
	int knifeIDef = getKnifeIDef(knifeID);

	uintptr_t localPlayer = *(uintptr_t*)(clientBase + dwLocalPlayer);

	if (localPlayer) {
		for (int i = 0; i < 8; i++) {
			uintptr_t curWeapon = *(uintptr_t*)(localPlayer + m_hMyWeapons + i * 0x4) & 0xFFF;
			uintptr_t curWeaponBase = *(uintptr_t*)(clientBase + dwEntityList + (curWeapon - 1) * 0x10);

			if (curWeaponBase != 0) {
				short curWeaponID = *(short*)(curWeaponBase + m_iItemDefinitionIndex);

				int curPaintKit = *(int*)(curWeaponBase + m_nFallbackPaintKit);
				int paintKit = 44;;

				if (curWeaponID == 42 || curWeaponID == 59 || curWeaponID == knifeIDef) {
					if (modelIndex > 0) {
						paintKit = getKnifeSkin(knifeID);
						*(short*)(curWeaponBase + m_iItemDefinitionIndex) = knifeIDef;
						*(uintptr_t*)(curWeaponBase + m_nModelIndex) = modelIndex;
						*(uintptr_t*)(curWeaponBase + m_iViewModelIndex) = modelIndex;
					}
				}
				else {
					paintKit = getSkin(curWeaponID);
				}

				if (curPaintKit != paintKit && curPaintKit != -1) {
					*(int*)(curWeaponBase + m_iItemIDHigh) = -1;
					*(int*)(curWeaponBase + m_nFallbackPaintKit) = paintKit;
					*(int*)(curWeaponBase + m_nFallbackSeed) = 661;
					*(float*)(curWeaponBase + m_flFallbackWear) = 0.00001f;
				}
			}
		}

		uintptr_t activeWeapon = *(uintptr_t*)(localPlayer + m_hActiveWeapon) & 0xFFF;
		uintptr_t activeWeaponBase = *(uintptr_t*)(clientBase + dwEntityList + (activeWeapon - 1) * 0x10);

		if (activeWeaponBase != 0) {
			short activeWaeponID = *(short*)(activeWeaponBase + m_iItemDefinitionIndex);
			int weaponViewModelID = *(int*)(activeWeaponBase + m_iViewModelIndex);

			if (activeWaeponID == 42) {
				modelIndex = weaponViewModelID + precache_bayonet_ct + knifeID * 3 + knifeIDOffset;
			}
			else if (activeWaeponID == 59) {
				modelIndex = weaponViewModelID + precache_bayonet_t + knifeID * 3 + knifeIDOffset;
			}

			uintptr_t knifeViewModel = *(uintptr_t*)(localPlayer + m_hViewModel) & 0xFFF;
			uintptr_t knifeViewModelBase = *(uintptr_t*)(clientBase + dwEntityList + (knifeViewModel - 1) * 0x10);

			if (knifeViewModel != 0) {
				if (activeWaeponID == 42 || activeWaeponID == 59 || activeWaeponID == knifeIDef) {
					*(short*)(activeWeaponBase + m_iItemDefinitionIndex) = knifeIDef;
					*(uintptr_t*)(knifeViewModelBase + m_nModelIndex) = modelIndex;
					*(uintptr_t*)(knifeViewModelBase + m_iViewModelIndex) = modelIndex;
				}
			}
		}
	}
	else {
		modelIndex = 0;
	}
}