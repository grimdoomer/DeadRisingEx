/*

*/

#pragma once
#include <Windows.h>

extern "C" void *SnatcherModuleHandle;

/*
    Adjusts a module address for relocation.
*/
static void * GetModuleAddress(__int64 address)
{
    static void *GameBaseAddress = GetModuleHandle(NULL);
    return (void*)(address - ((__int64)0x140000000 - (__int64)GameBaseAddress));
}

/*
    Adjusts a module address for relocation.
*/
static void * GetModuleAddress(void * pAddress)
{
    static void *GameBaseAddress = GetModuleHandle(NULL);
    return (void*)((__int64)pAddress - ((__int64)0x140000000 - (__int64)GameBaseAddress));
}

/*
    Gets the T instance at the specified address after adjusting it for module relocation.
*/
template<typename T> T GetModulePointer(void *pAddress)
{
    return *(T*)GetModuleAddress(pAddress);
}

/*
    Gets a pointer value at the specified address after adjusting it for module relocation, adds the
    specified offset, and then returns the new pointer as type S.
*/
template<typename T, typename S> S* GetModulePointer(void *pAddress, int offset = 0)
{
    return (S*)(GetModulePointer<T>(pAddress) + offset);
}

/*
    Performs a this ptr call for an object adjusting functionPtr for relocation.
*/
extern "C" __int64 __stdcall ThisPtrCall(void *functionPtr, void *thisPtr, ...);

/*
    Performs a this ptr call for an object without adjusting functionPtr for relocation.
*/
extern "C" __int64 __stdcall ThisPtrCallNoFixup(void *functionPtr, void *thisPtr, ...);


// Function to assign a member function to a vtable entry:
extern "C" void __stdcall AssignVTableEntry(void** vtable, int index, ...);


// Macros to help build out a custom vtable:
//
//  DECLARE_VTABLE
//      Allocates the vtable, can only be used within the scope of the constructor.
//
//          static void* uDrexVehicle_vtable[N];
//
//  DECLARE_STATIC_VTABLE
//      Allocates the vtable array, used when you want access to the custom vtable outside of the connstructor.
//
//          void* uDrexVehicle_vtable[N];
//
//  BEGIN_VTABLE
//      Used in an objects constructor to start the vtable entry assignments. Uses an execute-once pattern so the vtable
//      is only setup once.
//
//          static bool vtableInitialized = false;
//          if (vtableInitialized == false)
//          {
//
//  VTABLE_ENTRY
//      Assigns a function pointer to the specified vtable entry.
//
//          AssignVTableEntry(uDrexVehicle_vtable, 3, &uDrexVehicle::RegisterDebugOptions);
//
//  END_VTABLE
//      Ends vtable setup block and optionally assigns the vtable to the specified field.
//
//          }
//          this->vtable = uDrexVehicle_vtable;
//

#define DECLARE_STATIC_VTABLE(type, size)                   \
    static void* type##_vtable[size]

#define DECLARE_VTABLE(type, size)                          \
    void* type##_vtable[size]

#define BEGIN_VTABLE()                                      \
    static bool vtableInitialized = false;                  \
    if (vtableInitialized == false)                         \
    {

#define VTABLE_ENTRY(type, index, func)                     \
        AssignVTableEntry(type##_vtable, index, func);

#define END_VTABLE_ASSIGN(type)                             \
    }                                                       \
    this->vtable = type##_vtable;

#define END_VTABLE()                                        \
    }


static void BuildVtableLayout(void** vtable, int length...)
{
    // Initialize the args list.
    va_list args;
    va_start(args, length);

    // Loop for the length and initialize the vtable pointers.
    for (int i = 0; i < length; i++)
    {
        // Assign the vtable pointer.
        vtable[i] = va_arg(args, void*);
    }

    // Cleanup the args list.
    va_end(args);
}