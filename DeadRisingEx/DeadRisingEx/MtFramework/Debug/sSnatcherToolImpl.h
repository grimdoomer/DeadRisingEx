
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

    DWORD       lastAreaId;     // Last area id the sysmenu was opened at

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

    /*
        Returns true if the sysmenu is visible, false otherwise.
    */
    bool IsVisible()
    {
        return this->sysMenuOpen;
    }

    void Draw();
};