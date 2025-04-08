#include <Unreal/Core/Math/MathFwd.hpp>
#include <Unreal/Core/CoreTypes.hpp>
#include <Unreal/UnrealCoreStructs.hpp>

using namespace RC::Unreal;

namespace Rivals2SDK
{

	enum class ERivalsCharacterAttack : uint8
	{
		None = 0,
		Jab = 1,
		DAttack = 2,
		Ftilt = 3,
		Utilt = 4,
		Dtilt = 5,
		FirstTiltAttack = 3,
		LastTiltAttack = 5,
		Fstrong = 6,
		Ustrong = 7,
		Dstrong = 8,
		FirstStrongAttack = 6,
		LastStrongAttack = 8,
		Nair = 9,
		Fair = 10,
		Bair = 11,
		Uair = 12,
		Dair = 13,
		FirstAerialAttack = 9,
		LastAerialAttack = 13,
		Nspecial = 14,
		Fspecial = 15,
		Uspecial = 16,
		Dspecial = 17,
		FirstSpecial = 14,
		LastSpecial = 17,
		NspecialAir = 18,
		FspecialAir = 19,
		UspecialAir = 20,
		DspecialAir = 21,
		FirstSpecialAir = 18,
		LastSpecialAir = 21,
		Grab = 22,
		DashGrab = 23,
		PivotGrab = 24,
		FirstGrab = 22,
		LastGrab = 24,
		Fthrow = 25,
		Bthrow = 26,
		Uthrow = 27,
		Dthrow = 28,
		FirstThrow = 25,
		LastThrow = 28,
		Pummel = 29,
		PummelSpecial = 30,
		FirstPummel = 29,
		LastPummel = 30,
		GetupAttack = 31,
		GetupSpecial = 32,
		FirstGetupAttack = 31,
		LastGetupAttack = 32,
		LedgeAttack = 33,
		LedgeSpecial = 34,
		FirstLedgeAttack = 33,
		LastLedgeAttack = 34,
		Taunt1 = 35,
		Taunt2 = 36,
		Taunt3 = 37,
		Taunt4 = 38,
		FirstTaunt = 35,
		LastTaunt = 38,
		Extra1 = 39,
		Extra2 = 40,
		Extra3 = 41,
		Extra4 = 42,
		Intro = 43,
		Thrown = 44,
		Count = 45,
		ERivalsCharacterAttack_MAX = 46,
	};

	enum class ERivalsCharacterStateFlag : uint8
	{
		CanAerialDrift = 0,
		CanAirDodge = 1,
		CanAttack = 2,
		CanCrouch = 3,
		CanDash = 4,
		CanFallThrough = 5,
		CanGrabLedge = 6,
		CanJump = 7,
		CanPlatDrop = 8,
		CanShield = 9,
		CanWalk = 10,
		CanWallJump = 11,
		DisableECB = 12,
		DisableGravity = 13,
		FarFromECB = 14,
		ForceParryStun = 15,
		HasHitObject = 16,
		HasHitPlayer = 17,
		HasHitShield = 18,
		PerformedBReverse = 19,
		PreserveMomentumAtLedge = 20,
		PreserveMomentumOnWallCollision = 21,
		PreserveStateOnHit = 22,
		PreventFastfalls = 23,
		ReverseAfterState = 24,
		Rooted = 25,
		SnapToPlatforms = 26,
		StartedAttackWithRightStick = 27,
		SuccessfullyParried = 28,
		SuccessfullyCountered = 29,
		Count = 30,
		ERivalsCharacterStateFlag_MAX = 31,
	};

	enum class ESnapNetInterpolation : uint8
	{
		InterpolateToValue = 0,
		SnapToValue = 1,
		ESnapNetInterpolation_MAX = 2,
	};

	enum class ESnapNetPropertyRelevance : uint8
	{
		Always = 0,
		Never = 1,
		Simulated = 2,
		NotSimulated = 3,
		Spectated = 4,
		NotSpectated = 5,
		SimulatedOrSpectated = 6,
		NeitherSimulatedNorSpectated = 7,
		ESnapNetPropertyRelevance_MAX = 8,
	};

	enum class ESnapNetFloatEncodingType : uint32
	{
		FixedRange = 0,
		FloatingPoint = 1,
		SignedRange = 2,
		ESnapNetFloatEncodingType_MAX = 3,
	};

	struct alignas(0x08) FSnapNetProperty
	{
	public:
		uint8 Pad_0[0x8];					 // 0x0000(0x0008)(Fixing Size After Last Property [ Dumper-7 ])
		ESnapNetPropertyRelevance Relevance; // 0x0008(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		bool UseAsEventId;					 // 0x0009(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8 Pad_A[0x6];					 // 0x000A(0x0006)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct FSnapNetPropertyWithInterpolation : public FSnapNetProperty
	{
	public:
		bool DiscontinuityFlag;		   // 0x0010(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		bool Interpolate;			   // 0x0011(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		bool DiscontinuitiesSupported; // 0x0012(0x0001)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8 Pad_13[0x5];			   // 0x0013(0x0005)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct FSnapNetPropertyInt32Base : public FSnapNetPropertyWithInterpolation
	{
	public:
		bool Clamped;		// 0x0018(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8 Pad_19[0x3];	// 0x0019(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		int32 Value;		// 0x001C(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8 Pad_20[0x10]; // 0x0020(0x0010)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct FSnapNetPropertyInt32 final : public FSnapNetPropertyInt32Base
	{
	public:
		int32 MinValue; // 0x0030(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		int32 MaxValue; // 0x0034(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	};

	struct FSnapNetPropertyVector2DBase : public FSnapNetPropertyWithInterpolation
	{
	public:
		bool Clamped;	   // 0x0018(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8 Pad_19[0x7]; // 0x0019(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
		FVector Value;	   // 0x0020(0x0010)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	};

	struct FSnapNetVector2DEncoding final
	{
	public:
		ESnapNetFloatEncodingType Encoding; // 0x0000(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8 Pad_4[0x4];					// 0x0004(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
		FVector MinValue;					// 0x0008(0x0010)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		FVector MaxValue;					// 0x0018(0x0010)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		double Precision;					// 0x0028(0x0008)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32 ExponentBits;					// 0x0030(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32 SignificandBits;				// 0x0034(0x0004)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct FSnapNetPropertyVector2D final : public FSnapNetPropertyVector2DBase
	{
	public:
		struct FSnapNetVector2DEncoding Encoding; // 0x0030(0x0038)(Edit, DisableEditOnInstance, NoDestructor, Protected, NativeAccessSpecifierProtected)
	};
}