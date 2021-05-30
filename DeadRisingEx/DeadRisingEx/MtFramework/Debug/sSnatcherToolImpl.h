
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/Utils/MtPropertyList.h>
#include <vector>
#include <string>

class sSnatcherToolImpl
{
    friend __int64 OpenDebugMenu(WCHAR **argv, int argc);

protected:

    bool                        sysMenuOpen = false;
    int                         sysMenuIndex;
    std::vector<std::string>    sysMenuNavList;
    std::vector<MtPropertyList*>    sysMenuPropertyList;

    void DrawSection(MtPropertyListEntry *pEntry);

public:
    static void RegisterTypeInfo();

    static sSnatcherToolImpl * Instance()
    {
        static sSnatcherToolImpl * pInstance = nullptr;

        // If the singleton hasn't been initialized create it now.
        if (pInstance == nullptr)
        {
            // Create the singleton instance.
            pInstance = new sSnatcherToolImpl();
        }

        // Return the singleton instance.
        return pInstance;
    }

    void Draw();
};