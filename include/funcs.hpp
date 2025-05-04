#pragma once

#include <Rivals2.hpp>

typedef void (*FSnapNetPropertyInt32Base_SetValue_Func)(Rivals2SDK::FSnapNetPropertyInt32Base *, int64 Value, Rivals2SDK::ESnapNetInterpolation Interpolation);
FSnapNetPropertyInt32Base_SetValue_Func FSnapNetPropertyInt32Base_SetValue = nullptr;

typedef void (*FSnapNetPropertyVector2DBase_SetValue_Func)(Rivals2SDK::FSnapNetPropertyVector2DBase *, RC::Unreal::FVector *NewValue, Rivals2SDK::ESnapNetInterpolation Interpolation);
FSnapNetPropertyVector2DBase_SetValue_Func FSnapNetPropertyVector2DBase_SetValue = nullptr;

typedef Rivals2SDK::ERivalsCharacterAttack (*Zetterburn_Get_Attack_Func)(RC::Unreal::AActor *actor);
Zetterburn_Get_Attack_Func Zetterburn_Get_Attack;