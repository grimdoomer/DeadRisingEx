
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Archive/cResource.h>
#include <MtFramework/Memory/MtHeapAllocator.h>


#define VEHICLEDEF_HEADER_MAGIC         'vehi'
#define VEHICLEDEF_HEADER_VERSION       1

struct VehicleDefinitionHeader
{
    /* 0x00 */ DWORD Magic;
    /* 0x04 */ DWORD Version;
};

struct VehicleDefinitionAssetPaths
{
    /* 0x00 */ CHAR Model[64];
    /* 0x40 */ CHAR Animation[64];
    /* 0x80 */ CHAR HavokVehicleData[64];
    /* 0xC0 */ CHAR HavokVertexLayout[64];
    /* 0x100 */ CHAR SoundSnd1[64];
    /* 0x140 */ CHAR SoundSnd2[64];
    /* 0x180 */ CHAR SoundRrd[64];
    /* 0x1C0 */ CHAR SoundWed[64];
};


// VehicleDefinition DTI:
extern MtDTI* VehicleDefinitionDTI;

struct VehicleDefinition : public cResource
{
    struct MyDTI : public MtDTI
    {
        void* _vtable[2];

        MyDTI() : MtDTI("VehicleDefinition", cResource::DebugTypeInfo, sizeof(VehicleDefinition), 'vehi', 0)
        {
            // Setup the vtable.
            BuildVtableLayout(_vtable, ARRAYSIZE(_vtable),
                MtDTI::_dtor,
                &MyDTI::CreateInstance
            );

            this->vtable = _vtable;
        }

        MtObject* CreateInstance()
        {
            // Allocate and initialize the vehicle definition.
            void* pVehicleDefAlloc = (*g_pResourceHeapAllocator)->Alloc(this->ObjectSize, 0x10);
            if (pVehicleDefAlloc != nullptr)
            {
                return new(pVehicleDefAlloc) VehicleDefinition();
            }

            return nullptr;
        }
    };


    VehicleDefinitionHeader Header;
    VehicleDefinitionAssetPaths AssetPaths;

    VehicleDefinition();
    ~VehicleDefinition();

    void RegisterDebugOptions(MtPropertyList* pPropList);

    bool LoadResource(MtStream* pStream);
    void CleanupResources();

    MtDTI* GetDTI()
    {
        return VehicleDefinitionDTI;
    }

    const char* GetFileExtension()
    {
        return "vehicle";
    }
};