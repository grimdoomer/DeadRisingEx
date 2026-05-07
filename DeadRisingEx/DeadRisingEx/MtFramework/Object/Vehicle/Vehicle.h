
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Object/Vehicle/uVehicleOm09.h>
#include "VehicleDefinition.h"
#include <string>

struct uDrexVehicle : public uVehicleOm09
{
    struct MyDTI : public MtDTI
    {
        void* _vtable[2];

        MyDTI() : MtDTI("uDrexVehicle", uVehicleOm09::DebugTypeInfo, sizeof(uDrexVehicle), 0, 0)
        {
            // Setup the vtable.
            BuildVtableLayout(_vtable, ARRAYSIZE(_vtable),
                MtDTI::_dtor,
                &MyDTI::_CreateInstance
            );

            this->vtable = _vtable;
        }

        MtObject* _CreateInstance()
        {
            // Allocate and initialize the vehicle definition.
            void* pVehicleDefAlloc = (*g_pUnitHeapAllocator)->Alloc(this->ObjectSize, 0x20);
            if (pVehicleDefAlloc != nullptr)
            {
                return new(pVehicleDefAlloc) uDrexVehicle();
            }

            return nullptr;
        }
    };

    inline static MyDTI DebugTypeInfo;


    // Padding to match the size of uVehicleOm09
    BYTE _padding2[0x3380 - sizeof(uVehicle)];

    std::string VehicleDataFilePath;            // File path of the vehicle data file
    VehicleDefinition* pVehicleData;            // Vehicle data

    uDrexVehicle();
    ~uDrexVehicle();

    void RegisterDebugOptions(MtPropertyList* pPropList);

    void LoadUnitResources();

    static void RegisterCommands();

    MtDTI* GetDTI()
    {
        return &DebugTypeInfo;
    }
};