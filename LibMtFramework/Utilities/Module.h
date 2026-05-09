/*

*/

#pragma once
#include <Windows.h>

// Helper macro to expand and stringify a parameter:
#define STRINGIFY_(x)   #x
#define STRINGIFY(x)    STRINGIFY_(x)

#ifdef SHIMDLL

// When linking from the shim dll declare APIs as dllexport:
#define SHIM_API __declspec(dllexport)

// Helper macro to declare an empty function body when compiling from the shim dll. The first linker comment
// creates an export alias for the function with the name "snatcher_0xXXXXXXXXXXX__N" where "XXXXX" is replaced
// with the address of the function in the game executable. The second linker comment forces an include to the
// function even if it's not used in code (the shim dll has no code).
//
// In the event the function is not redirected to the game executable the code in this stub will report an error.
#define SHIM_BODY(addr)                                                                                                 \
    {                                                                                                                   \
__pragma(comment(linker, "/EXPORT:snatcher_" #addr "__" STRINGIFY(__COUNTER__) "=" __FUNCDNAME__))                      \
__pragma(comment(linker, "/INCLUDE:" __FUNCDNAME__))                                                                    \
                                                                                                                        \
        MessageBoxW(NULL, L"SnatcherShim.dll function '" __FUNCDNAME__ "' failed to redirect. Report this to grim.",    \
            L"DeadRisingEx", MB_OK | MB_ICONERROR | MB_APPLMODAL);                                                      \
        TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);                                                              \
    }

#define SHIM_BODY_SCALAR_DTOR(addr)                                                                                     \
    {                                                                                                                   \
__pragma(comment(linker, "/EXPORT:snatcher_scalar_dtor_" #addr "__" STRINGIFY(__COUNTER__) "=" __FUNCDNAME__))          \
__pragma(comment(linker, "/INCLUDE:" __FUNCDNAME__))                                                                    \
                                                                                                                        \
        MessageBoxW(NULL, L"SnatcherShim.dll function '" __FUNCDNAME__ "' failed to redirect. Report this to grim.",    \
            L"DeadRisingEx", MB_OK | MB_ICONERROR | MB_APPLMODAL);                                                      \
        TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);                                                              \
    }

#define SHIM_BODY_VCALL(ordinal)                                                                                        \
    {                                                                                                                   \
__pragma(comment(linker, "/EXPORT:snatcher_vcall_" #ordinal "__" STRINGIFY(__COUNTER__) "=" __FUNCDNAME__))             \
__pragma(comment(linker, "/INCLUDE:" __FUNCDNAME__))                                                                    \
                                                                                                                        \
        MessageBoxW(NULL, L"SnatcherShim.dll function '" __FUNCDNAME__ "' failed to redirect. Report this to grim.",    \
            L"DeadRisingEx", MB_OK | MB_ICONERROR | MB_APPLMODAL);                                                      \
        TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);                                                              \
    }

#define SHIM_BODY_DTOR_VCALL()                                                                                          \
    {                                                                                                                   \
__pragma(comment(linker, "/EXPORT:snatcher_dtor_vcall__" STRINGIFY(__COUNTER__) "=" __FUNCDNAME__))                     \
__pragma(comment(linker, "/INCLUDE:" __FUNCDNAME__))                                                                    \
                                                                                                                        \
        MessageBoxW(NULL, L"SnatcherShim.dll function '" __FUNCDNAME__ "' failed to redirect. Report this to grim.",    \
            L"DeadRisingEx", MB_OK | MB_ICONERROR | MB_APPLMODAL);                                                      \
        TerminateProcess(GetCurrentProcess(), 0xBAD0C0DE);                                                              \
    }

#else

// When linking from outside the shim dll declare APIs as dllimport:
#define SHIM_API __declspec(dllimport)

#define SHIM_BODY(addr)                 ;
#define SHIM_BODY_SCALAR_DTOR(addr)     ;
#define SHIM_BODY_VCALL(ordinal)        ;
#define SHIM_BODY_DTOR_VCALL()          ;
#endif


#define ASSERT_STRUCT_SIZE(type, size)        static_assert(sizeof(type) == size, STRINGIFY(type) " incorrect struct size")

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
extern "C" __int64 ThisPtrCall(void *functionPtr, void *thisPtr, ...);

/*
    Performs a this ptr call for an object without adjusting functionPtr for relocation.
*/
extern "C" __int64 ThisPtrCallNoFixup(void *functionPtr, void *thisPtr, ...);


// Function to assign a member function to a vtable entry:
extern "C" void AssignVTableEntry(void** vtable, int index, ...);


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


/*
    Implements a singleton pattern for the object.
*/
#define IMPLEMENT_SINGLETON(type, instanceAddr) \
inline static type **_Instance = (type**)GetModuleAddress(instanceAddr); \
\
inline static type * Instance() \
{ \
    return *type::_Instance; \
}


/*
    Implements operator new/delete overrides for the class.
*/
#define IMPLEMENT_OPERATOR_NEW_DELETE(allocator, alignment)             \
    void* operator new(size_t size)                                     \
    {                                                                   \
        return allocator->Alloc(size, alignment);                       \
    }                                                                   \
                                                                        \
    void operator delete(void* ptr)                                     \
    {                                                                   \
        allocator->Free(ptr);                                           \
    }

/*
    Implements a fake scalar deleting destructor used when extending existing game object classes
    with custom code.
*/
#define SCALAR_DELETING_DTOR(type)                                      \
    void type##_scalar_deleting_dtor(type* thisptr, unsigned int flags) \
    {                                                                   \
        if ((flags & 1) != 0)                                           \
            delete thisptr;                                             \
        else                                                            \
            thisptr->~type();                                           \
    }


/*
    Helper class to wrap a global pointer variable in the game executable and remove a layer of indirection.
*/
template<typename T>
class WrappedPtr
{
protected:
    T** pointer;

public:

    WrappedPtr(T** pointer)
    {
        this->pointer = pointer;
    }

    T* operator*()
    {
        return (*this->pointer);
    }

    const T* operator*() const
    {
        return (*this->pointer);
    }

    T* operator->()
    {
        return (*this->pointer);
    }

    WrappedPtr<T>& operator=(T* rhs)
    {
        (*this->pointer) = rhs;

        return *this;
    }

    bool operator==(const T* rhs) const
    {
        return (*this->pointer) == rhs;
    }

    bool operator!=(const T* rhs) const
    {
        return (*this->pointer) != rhs;
    }

    operator T* () const
    {
        return (*this->pointer);
    }
};