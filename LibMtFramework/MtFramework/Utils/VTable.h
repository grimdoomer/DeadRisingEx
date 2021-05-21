
#pragma once
#include <Windows.h>
#include <cstdarg>

//#define BEGIN_VTABLE_LAYOUT(type) void *type##_vtable[] = {
//
//#define VTABLE_ENTRY(func) (void*)func,
//
//#define END_VTABLE_LAYOUT() }
//
//#define ASSIGN_VTABLE(type) this->vtable = type##_vtable;

static void BuildVtableLayout(void **vtable, int length...)
{
    // Initialize the args list.
    va_list args;
    va_start(args, length);

    // If the vtable has already been initialized skip setting it up.
    if (vtable[0] == nullptr)
    {
        // Loop for the length and initialize the vtable pointers.
        for (int i = 0; i < length; i++)
        {
            // Assign the vtable pointer.
            vtable[i] = va_arg(args, void*);
        }
    }

    // Cleanup the args list.
    va_end(args);
}