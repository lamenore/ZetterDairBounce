#include <Mod/CppUserModBase.hpp>
#include <UE4SSProgram.hpp>

#include <Unreal/Hooks.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UObjectGlobals.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/UActorComponent.hpp>

#include <SigScanner/SinglePassSigScanner.hpp>
#include <safetyhook.hpp>
#include <utility/Scan.hpp>
#include <utility/Module.hpp>

#include <funcs.hpp>
#include <Rivals2.hpp>

using enum Rivals2SDK::ERivalsCharacterStateFlag;

SafetyHookInline FSnapNetPropertyInt32Base_SetValue_Detour{};
// SafetyHookInline ZetterVtable_Detour{};
SafetyHookInline ZetterGameplayTick_Detour{};
bool g_ZetterHooked = false;

void FSnapNetPropertyInt32_SetValue_Hook(Rivals2SDK::FSnapNetPropertyInt32Base *prop, int64 Value, Rivals2SDK::ESnapNetInterpolation Interpolation)
{
    FSnapNetPropertyInt32Base_SetValue_Detour.call(prop, Value, Interpolation);
}

constexpr int StateflagBitmask(const Rivals2SDK::ERivalsCharacterStateFlag shift)
{
    return (1 << (uint32)shift);
}

bool Zetterburn_Gameplay_Tick_Hook(RC::Unreal::AActor *actor)
{
    bool result = ZetterGameplayTick_Detour.fastcall<bool>(actor);

    if (!result)
        return result;

    Rivals2SDK::ERivalsCharacterAttack attack = Zetterburn_Get_Attack(actor);

    if (attack == Rivals2SDK::ERivalsCharacterAttack::Dair)
    {
        // RC::Output::send<RC::LogLevel::Verbose>(STR("Zetterburn Dair\n"));
        auto actorBytes = ((uint8_t *)actor);
        Rivals2SDK::FSnapNetPropertyInt32 WindowSnap = *(Rivals2SDK::FSnapNetPropertyInt32 *)(&actorBytes[0x8a8]);
        Rivals2SDK::FSnapNetPropertyInt32 StateFlagsSnap = *(Rivals2SDK::FSnapNetPropertyInt32 *)(&actorBytes[0x29E0]);
        Rivals2SDK::FSnapNetPropertyVector2D VelocitySnap = *(Rivals2SDK::FSnapNetPropertyVector2D *)(&actorBytes[0x840]);
        uint32 Window = WindowSnap.Value;

        // RC::Output::send<RC::LogLevel::Verbose>(STR("Window window: {:d}\n"), Window);

        if (Window == 1 || Window == 2)
        {
            // RC::Output::send<RC::LogLevel::Verbose>(STR("Zetterburn Window 1, StateFlagsSnap: {:#b}\n"), StateFlagsSnap.Value);
            // RC::Output::send<RC::LogLevel::Verbose>(STR("Zetterburn Window 1, StateFlagsSnap HasHitObject: {:#b}\n"), ((StateFlagsSnap.Value & StateflagBitmask(HasHitObject)) != 0));
            // RC::Output::send<RC::LogLevel::Verbose>(STR("Zetterburn Window 1, StateFlagsSnap HasHitShield: {:#b}\n"), ((StateFlagsSnap.Value & StateflagBitmask(HasHitShield)) != 0));

            if ((StateFlagsSnap.Value & (StateflagBitmask(HasHitPlayer) | StateflagBitmask(HasHitObject) | StateflagBitmask(HasHitShield))) != 0)
            {
                // RC::Output::send<RC::LogLevel::Verbose>(STR("Zetterburn HIT 1\n"));
                auto FlagValue = StateFlagsSnap.Value & (~(StateflagBitmask(HasHitPlayer) | StateflagBitmask(HasHitObject) | StateflagBitmask(HasHitShield)));
                FSnapNetPropertyInt32Base_SetValue((Rivals2SDK::FSnapNetPropertyInt32 *)(&actorBytes[0x29E0]), FlagValue, Rivals2SDK::ESnapNetInterpolation::InterpolateToValue);

                auto Velocity = VelocitySnap.Value;
                Velocity.SetY(28.0);
                FSnapNetPropertyVector2DBase_SetValue((Rivals2SDK::FSnapNetPropertyVector2D *)(&actorBytes[0x840]), &Velocity, Rivals2SDK::ESnapNetInterpolation::InterpolateToValue);
                return true;
            }
        }
    }

    // RC::Output::send<RC::LogLevel::Verbose>(STR("Zetterburn attack update\n"));
    return result;
}

class ZetterDairBounceMod : public RC::CppUserModBase
{
private:
public:
    ZetterDairBounceMod() : CppUserModBase()
    {
        ModName = STR("ZetterDairBounceMod");
        ModVersion = STR("1.0");
        ModDescription = STR("Extraction powerdrive");
        ModAuthors = STR("lamenor");
    }

    ~ZetterDairBounceMod() override = default;

    auto on_update() -> void override
    {
    }

    auto on_unreal_init() -> void override
    {

        // auto DefaultClairen = RC::Unreal::UObjectGlobals::StaticFindObject<RC::Unreal::AActor *>(nullptr, nullptr, STR("/Script/Rivals2.Default__Clairen"));

        // RC::Output::send<RC::LogLevel::Verbose>(STR("Found default clairen: {}\n"), DefaultClairen->GetFullName());

        RC::SignatureContainer int32_setvalue_container{
            {{"48 89 5C 24 ?? 57 48 83 EC ?? 8B FA 48 8B D9 41 80 F8"}},
            [&](const RC::SignatureContainer &self)
            {
                FSnapNetPropertyInt32Base_SetValue = reinterpret_cast<FSnapNetPropertyInt32Base_SetValue_Func>(self.get_match_address());
                RC::Output::send<RC::LogLevel::Normal>(STR("Found FSnapNetPropertyInt32Base_SetValue: {:d}\n"), (uint64_t)self.get_match_address());
                return true;
            },
            [](RC::SignatureContainer &self) {},
        };

        RC::SignatureContainer vector2Dbase_setvalue_container{
            {{"40 56 57 41 56 48 83 EC ?? 4C 8B F2"}, {"40 57 41 55 41 56 48 83 EC ?? 48 8B FA"}},
            [&](const RC::SignatureContainer &self)
            {
                FSnapNetPropertyVector2DBase_SetValue = reinterpret_cast<FSnapNetPropertyVector2DBase_SetValue_Func>(self.get_match_address());
                RC::Output::send<RC::LogLevel::Normal>(STR("Found FSnapNetPropertyVector2DBase_SetValue: {:d}\n"), (uint64_t)self.get_match_address());
                return true;
            },
            [](SignatureContainer &self) {},
        };

        /*
         auto i32_func_result = utility::scan_string(utility::get_executable(), STR("Value %d assigned above maximum value %d and will be clamped"), true);
         if (!i32_func_result)
         {
             RC::Output::send<RC::LogLevel::Error>(STR("ZETTER_BOUNCE: Not Found \"Value %d assigned above maximum value %d and will be clamped\" string, not hooking\n"));
             return;
         }

         auto i32_str_ref = utility::scan_displacement_reference(utility::get_executable(), (*i32_func_result - 0x28));
         if (!i32_str_ref)
         {
             RC::Output::send<RC::LogLevel::Error>(STR("ZETTER_BOUNCE: Not Found Int32Base str displacement function, not hooking\n"));
             return;
         }
         auto i32_func = utility::find_function_start(*i32_str_ref);
         if (!i32_func)
         {
             RC::Output::send<RC::LogLevel::Error>(STR("ZETTER_BOUNCE: Not Found FSnapNetPropertyInt32Base_SetValue function, not hooking\n"));
             return;
         }
         FSnapNetPropertyInt32Base_SetValue = reinterpret_cast<FSnapNetPropertyInt32Base_SetValue_Func>(std::move(*i32_func));

         auto v2d_func_result = utility::scan_string(utility::get_executable(), STR("Value %g assigned above maximum value %g on axis %d and will be clamped"), true);
         if (!v2d_func_result)
         {
             RC::Output::send<RC::LogLevel::Error>(STR("ZETTER_BOUNCE: Not Found \"Value %g assigned above maximum value %g on axis %d and will be clamped\" string, not hooking\n"));
             return;
         }

         auto v2d_str_ref = utility::scan_displacement_reference(utility::get_executable(), (*v2d_func_result - 0x28));
         if (!v2d_str_ref)
         {
             RC::Output::send<RC::LogLevel::Error>(STR("ZETTER_BOUNCE: Not Found Vector2DBase str displacement function, not hooking\n"));
             return;
         }
         auto v2d_func = utility::find_function_start(*v2d_str_ref);
         if (!v2d_func)
         {
             RC::Output::send<RC::LogLevel::Error>(STR("ZETTER_BOUNCE: Not Found FSnapNetPropertyVector2DBase_SetValue function, not hooking\n"));
             return;
         }
         FSnapNetPropertyVector2DBase_SetValue = reinterpret_cast<FSnapNetPropertyVector2DBase_SetValue_Func>(std::move(*v2d_func));
         */

        std::vector<SignatureContainer> signature_containers;
        signature_containers.push_back(int32_setvalue_container);
        signature_containers.push_back(vector2Dbase_setvalue_container);

        SinglePassScanner::SignatureContainerMap signature_containers_map;
        signature_containers_map.emplace(ScanTarget::MainExe, signature_containers);

        SinglePassScanner::start_scan(signature_containers_map);

        RC::Unreal::Hook::RegisterStaticConstructObjectPostCallback(
            [](const RC::Unreal::FStaticConstructObjectParameters &, RC::Unreal::UObject *constructed_object)
            {
                if (g_ZetterHooked)
                    return constructed_object;

                Unreal::UStruct *object_class = constructed_object->GetClassPrivate();

                if (object_class->GetName() == STR("Zetterburn"))
                {
                    // RC::Output::send<RC::LogLevel::Verbose>(STR("Found zetterburn\n"));
                    // ??_R4AZetterburn@@6B@
                    auto pZetterVtable = *reinterpret_cast<uintptr_t **>(constructed_object);
                    // RC::Output::send<RC::LogLevel::Error>(STR("Vtable: {0:x}\n"), (uint64_t)pZetterVtable);
                    // RC::Output::send<RC::LogLevel::Error>(STR("Vtable Func address: {0:x}\n"), (uint64_t)pZetterVtable[0]);
                    auto inline_result = SafetyHookInline::create(pZetterVtable[436], &Zetterburn_Gameplay_Tick_Hook);
                    // RC::Output::send<RC::LogLevel::Error>(STR("Created vmt hook\n"));

                    if (!inline_result.has_value())
                    {
                        RC::Output::send<RC::LogLevel::Error>(STR("Failed to create hook: {:d}\n"), 0);
                        return constructed_object;
                    }

                    ZetterGameplayTick_Detour = std::move(*inline_result);
                    Zetterburn_Get_Attack = (Zetterburn_Get_Attack_Func)(pZetterVtable[235]);

                    g_ZetterHooked = true;
                }
                return constructed_object;
            });
    }
};

#define ZETTER_DAIR_BOUNCE_MOD_API __declspec(dllexport)
extern "C"
{
    ZETTER_DAIR_BOUNCE_MOD_API RC::CppUserModBase *start_mod()
    {
        return new ZetterDairBounceMod();
    }

    ZETTER_DAIR_BOUNCE_MOD_API void uninstall_mod(RC::CppUserModBase *mod)
    {
        delete mod;
    }
}