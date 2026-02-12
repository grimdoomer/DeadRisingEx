
#include "VehicleDefinition.h"
#include <MtFramework/IO/MtDataReader.h>

MtDTI* VehicleDefinitionDTI = new VehicleDefinition::MyDTI();

// Fake destructor function to bypass not being able to take the address of a destructor:
void VehicleDefinition_dtor(VehicleDefinition* thisptr)
{
    thisptr->~VehicleDefinition();
}

int nullsub_return_0()
{
    return 0;
}

bool nullsub_return_true()
{
    return true;
}

void* VehicleDefinition_vtable[12];

VehicleDefinition::VehicleDefinition()
{
    // I really don't like this pattern but for now it'll do until I figure something else out.
    static bool vtableInitialized = false;
    if (vtableInitialized == false)
    {
        BuildVtableLayout(VehicleDefinition_vtable, ARRAYSIZE(VehicleDefinition_vtable),
            // MtObject:
            VehicleDefinition_dtor,
            nullsub_return_0,
            nullsub_return_true,
            &VehicleDefinition::RegisterDebugOptions,
            &VehicleDefinition::GetDTI,

            // cResource:
            &cResource::GetFileInfo,
            &VehicleDefinition::GetFileExtension,
            nullsub_return_0,
            &VehicleDefinition::LoadResource,
            nullsub_return_0,
            nullsub_return_0,
            &VehicleDefinition::CleanupResources
        );

        vtableInitialized = true;
    }

    // Assign our custom vtable.
    this->vtable = VehicleDefinition_vtable;
}

VehicleDefinition::~VehicleDefinition()
{
    // Update vtable pointer.
    this->vtable = cResource::_vtable;
}

void VehicleDefinition::RegisterDebugOptions(MtPropertyList* pPropList)
{

}

bool VehicleDefinition::LoadResource(MtStream* pStream)
{
    // Wrap the stream in a data reader.
    MtDataReader reader = MtDataReader(pStream, 0x10000);

    // Read the vehicle definition header and check it's valid.
    reader.ReadData(&this->Header, sizeof(this->Header));
    if (this->Header.Magic != VEHICLEDEF_HEADER_MAGIC || this->Header.Version != VEHICLEDEF_HEADER_VERSION)
    {
        DbgPrint("Vehicle definition has invalid header or is unsupported version\n");
        return false;
    }

    // Read the asset path block.
    reader.ReadData(&this->AssetPaths, sizeof(this->AssetPaths));

    return true;
}

void VehicleDefinition::CleanupResources()
{

}