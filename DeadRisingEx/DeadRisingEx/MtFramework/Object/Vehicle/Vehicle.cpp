
#include "Vehicle.h"
#include <MtFramework/Physics/sHavok.h>
#include <MtFramework/Archive/sResource.h>
#include <MtFramework/Graphics/rMotionList.h>
#include <MtFramework/Sound/sSoundSystem.h>
#include <MtFramework/Object/sUnit.h>
#include "DeadRisingEx/MtFramework/Player/uPlayerImpl.h"
#include <locale>
#include <codecvt>

MtDTI* uDrexVehicleDTI = new uDrexVehicle::MyDTI();

inline static void (__stdcall* _uVehicleOm09_Initialize)(uVehicleOm09* thisptr) = (void(__stdcall*)(uVehicleOm09*))GetModuleAddress(0x1401C6C70);


// Forward declarations for command functions.
__int64 SpawnVehicle(WCHAR** argv, int argc);

// Table of commands.
const int g_uDrexVehicleCommandsLength = 1;
const ConsoleCommandInfo g_uDrexVehicleCommands[g_uDrexVehicleCommandsLength] =
{
    { L"spawn_vehicle", L"Spawns the specified vehicle", SpawnVehicle },
};


// Fake destructor function to bypass not being able to take the address of a destructor:
void uDrexVehicle_dtor(uDrexVehicle* thisptr)
{
    thisptr->~uDrexVehicle();
}

//void* uDrexVehicle_vtable[70];

uDrexVehicle::uDrexVehicle()
{
    DECLARE_STATIC_VTABLE(uDrexVehicle, 70);
    BEGIN_VTABLE()

        // Copy the vtable from uVehicleOm09 and replace functions we want to override.
        memcpy(uDrexVehicle_vtable, this->vtable, sizeof(uDrexVehicle_vtable));
        VTABLE_ENTRY(uDrexVehicle, 0, uDrexVehicle_dtor)
        VTABLE_ENTRY(uDrexVehicle, 3, &uDrexVehicle::RegisterDebugOptions)
        VTABLE_ENTRY(uDrexVehicle, 4, &uDrexVehicle::GetDTI);
        VTABLE_ENTRY(uDrexVehicle, 5, &uDrexVehicle::LoadUnitResources);
    END_VTABLE_ASSIGN(uDrexVehicle)
}

uDrexVehicle::~uDrexVehicle()
{
    // Update vtable pointer.
    this->vtable = uVehicleOm09::_vtable;
}

void uDrexVehicle::RegisterDebugOptions(MtPropertyList* pPropList)
{

}

void uDrexVehicle::LoadUnitResources()
{
    // Check some havok system flag.
    if (*(__int64*)((BYTE*)sHavok::Instance() + 0x1000C0) == 0)
    {
        if (this->mAttr.mUnitAttr.Fix != 0 || this->mAttr.Move > 2)
            return;

        this->mAttr.Move = 3;
        return;
    }

    // Check if the vehicle data file path has been set, if not we need to come up with some default bail out scenario...
    if (this->VehicleDataFilePath.length() == 0)
    {
        DbgPrint(__FUNCTION__ ": vehicle data file path is empty\n");
        DebugBreak();
    }

    // Load the vehicle data file.
    this->pVehicleData = sResource::Instance()->LoadGameResource<VehicleDefinition>(VehicleDefinitionDTI, this->VehicleDataFilePath.c_str(), RLF_SYNCHRONOUS);
    if (this->pVehicleData == nullptr)
    {
        DbgPrint(__FUNCTION__ ": failed to load vehicle data file '%s'\n", this->VehicleDataFilePath.c_str());
        DebugBreak();
    }

    // Load havok vehicle data:
    *(rHavokVehicleData**)((BYTE*)this + 0x2F30) = sResource::Instance()->LoadGameResource<rHavokVehicleData>(rHavokVehicleData::DebugTypeInfo, this->pVehicleData->AssetPaths.HavokVehicleData, RLF_SYNCHRONOUS);

    // Load havok vertex layout:
    *(rHavokVertexLayout**)((BYTE*)this + 0x2F20) = sResource::Instance()->LoadGameResource<rHavokVertexLayout>(rHavokVertexLayout::DebugTypeInfo, this->pVehicleData->AssetPaths.HavokVertexLayout, RLF_SYNCHRONOUS);

    // Load vehicle model:
    LoadAndAssignModel(this->pVehicleData->AssetPaths.Model);

    // Load and assign animation:
    rMotionList* pAnimation = sResource::Instance()->LoadGameResource<rMotionList>(rMotionList::DebugTypeInfo, this->pVehicleData->AssetPaths.Animation, RLF_SYNCHRONOUS);
    AssignAnimation(pAnimation, 0);
    if (pAnimation != nullptr)
        pAnimation->DecrementRefCount();

    // Setup havok state.
    (void)ThisPtrCallNoFixup(this->vtable[54], this);

    // Load sound files:
    *(rSoundSnd**)((BYTE*)this + 0x1090) = sSoundSystem::Instance()->LoadSoundSnd(this->pVehicleData->AssetPaths.SoundSnd1, 0);
    *(rSoundSnd**)((BYTE*)this + 0x10A0) = sSoundSystem::Instance()->LoadSoundSnd(this->pVehicleData->AssetPaths.SoundSnd2, 0);
    *(rSoundRrd**)((BYTE*)this + 0x2DD8) = sSoundSystem::Instance()->LoadSoundRrd(this->pVehicleData->AssetPaths.SoundRrd);
    *(rSoundWed**)((BYTE*)this + 0x2DD0) = sSoundSystem::Instance()->LoadSoundWed(this->pVehicleData->AssetPaths.SoundWed);

    // Initialize more object state.
    _uVehicleOm09_Initialize(this);

    // Set initial parts display values.
    int partMask = 1;
    DWORD* pPartsDisplay = (DWORD*)((BYTE*)this + 0xBC4);
    for (int i = 0; i < 0x80; i++)
    {
        // Mask out the specified bit position.
        pPartsDisplay[i / 32] &= ~partMask;

        // Setup next bit mask value.
        partMask = 1 << (i % 32);
    }

    pPartsDisplay[0] |= 0xFFFFF;

    // More parts display setup.
    for (int i = 0; i < 12; i++)
    {
        (void)ThisPtrCallNoFixup(this->vtable[69], this, *(DWORD*)((BYTE*)this + 0x3000 + (i * 4)), i);
    }

    // Call base class implementation.
    uVehicle::_LoadUnitResources(this);
}

void uDrexVehicle::RegisterCommands()
{
    ImGuiConsole::Instance()->RegisterCommands(g_uDrexVehicleCommands, g_uDrexVehicleCommandsLength);
}

__int64 SpawnVehicle(WCHAR** argv, int argc)
{
    // Make sure at least 1 argument was passed.
    if (argc < 1)
    {
        // Invalid syntax.
        ImGuiConsole::Instance()->ConsolePrint(L"Invalid command syntax!\n");
        return 0;
    }

    if (uPlayerInstance == nullptr)
    {
        ImGuiConsole::Instance()->ConsolePrint(L"Player instance not set!\n");
        return 0;
    }

    // Setup the unicode converter.
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> unicConvert;

    // Convert the vehicle name to ascii.
    std::string sVehicleName = unicConvert.to_bytes(argv[0]);

    // TODO: lookup vehicle name

    // Create a new drex vehicle object.
    uDrexVehicle* pVehicle = uDrexVehicleDTI->CreateInstance<uDrexVehicle>();
    if (pVehicle == nullptr)
    {
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to create new drex vehicle\n");
        return 0;
    }

    // Set the file path of the vehicle data file.
    pVehicle->VehicleDataFilePath = "warthog";

    // Get the uPlayer instance from sUnit.
    BYTE* pPlayerInst = (BYTE*)uPlayerInstance;

    // Get the player's position and rotation.
    Vector3 position = *(Vector3*)(pPlayerInst + 0x40);
    position.y += 50.0f;
    Vector4 rotation = { 0 };

    // Set the item position.
    pVehicle->mPos.AsVec3 = position;

    // Spawn the object.
    if (sUnit::Instance()->AddObjectToMoveLine(9, pVehicle) == false)
    {
        // Failed to add object to sUnit.
        ImGuiConsole::Instance()->ConsolePrint(L"Failed to add vehicle to sUnit move line\n");
        return 0;
    }

    // TODO: destroy pArchive if it was loaded.

    return 0;
}