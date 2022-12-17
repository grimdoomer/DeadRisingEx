
#pragma once
#include "DeadRisingEx.h"
#include <MtFramework/MtObject.h>

extern bool(__stdcall *sUnit_AddObject)(void *thisptr, DWORD Unk, void *pObject);

struct ObjectInfo
{
    const char *ClassName;
    const char *ArchivePath;
};

#define OBJECT_COUNT        132
const ObjectInfo ObjectInfoTable[OBJECT_COUNT] =
{
    { "uEm00", "arc\\rom\\em\\em00\\em00" },
    { "uEm01", "arc\\rom\\em\\em01\\em01" },
    { "uEm03", "arc\\rom\\em\\em02\\em02" },
    { "uEm04", "arc\\rom\\em\\em03\\em03" },
    { "uEm04", "arc\\rom\\em\\em04\\em04" },
    { "uEm08", "arc\\rom\\em\\em08\\em08" },
    { "uEm40", "arc\\rom\\em\\em40\\em40" },
    { "uEm41", "arc\\rom\\em\\em41g\\em41g" },
    { "uEm43", "arc\\rom\\em\\em43\\em43" },
    { "uEm44", "arc\\rom\\em\\em44g\\em44g" },
    { "uEm45", "arc\\rom\\em\\em45g\\em45g" },
    { "uEm46", "arc\\rom\\em\\em46g\\em46g" },
    { "uEm47", "arc\\rom\\em\\em47g\\em47g" },
    { "uEm48", "arc\\rom\\em\\em48g\\em48g" },
    { "uEm49", "arc\\rom\\em\\em49g\\em49g" },
    { "uEm4a", "arc\\rom\\em\\em4ag\\em4ag" },
    { "uEm4b", "arc\\rom\\em\\em4bg\\em4bg" },
    { "uEm4d", "arc\\rom\\em\\em4dg\\em4dg" },
    { "uEm4e", "arc\\rom\\em\\em4eg\\em4eg" },
    { "uEm4f", "arc\\rom\\em\\em4fg\\em4fg" },
    { "uEm50", "arc\\rom\\em\\em50\\em50" },
    { "uEm53", "arc\\rom\\em\\em53\\em53" },
    { "uEm54", "arc\\rom\\em\\em54\\em54" },
    { "uEm55", nullptr },
    { "uEm56", "arc\\rom\\em\\em56g\\em56g" },
    { "uEm5a", "arc\\rom\\em\\em5a\\em5a" },

    { "uEvh000", "arc\\rom\\ev\\evh000\\evh000" },
    { "uEvh001", "arc\\rom\\ev\\evh001\\evh001" },
    { "uEvh002", "arc\\rom\\ev\\evh002\\evh002" },
    { "uEvh003", "arc\\rom\\ev\\evh003\\evh003" },
    { "uEvh004", "arc\\rom\\ev\\evh004\\evh004" },
    { "uEvh005", "arc\\rom\\ev\\evh005\\evh005" },
    { "uEvh006", "arc\\rom\\ev\\evh006\\evh006" },
    { "uEvh007", "arc\\rom\\ev\\evh007\\evh007" },
    { "uEvh008", "arc\\rom\\ev\\evh008\\evh008" },
    { "uEvh009", "arc\\rom\\ev\\evh009\\evh009" },

    { "uEvo000", "arc\\rom\\ev\\evo000\\evo000" },
    { "uEvo001", "arc\\rom\\ev\\evo001\\evo001" },
    { "uEvo002", "arc\\rom\\ev\\evo002\\evo002" },
    { "uEvo003", "arc\\rom\\ev\\evo003\\evo003" },
    { "uEvo004", "arc\\rom\\ev\\evo004\\evo004" },
    { "uEvo005", "arc\\rom\\ev\\evo005\\evo005" },
    { "uEvo006", "arc\\rom\\ev\\evo006\\evo006" },

    { "uNpc00", "arc\\rom\\npc\\npc00\\npc00" },
    { "uNpc01", "arc\\rom\\npc\\npc01\\npc01" },
    { "uNpc02", "arc\\rom\\npc\\npc02\\npc02" },
    { "uNpc03", "arc\\rom\\npc\\npc03\\npc03" },
    { "uNpc04", "arc\\rom\\npc\\npc04\\npc04" },
    { "uNpc05", "arc\\rom\\npc\\npc05\\npc05" },
    { "uNpc06", "arc\\rom\\npc\\npc06\\npc06" },
    { "uNpc07", "arc\\rom\\npc\\npc07\\npc07" },
    { "uNpc08", "arc\\rom\\npc\\npc08\\npc08" },
    { "uNpc09", "arc\\rom\\npc\\npc09\\npc09" },
    { "uNpc0a", "arc\\rom\\npc\\npc0a\\npc0a" },
    { "uNpc0b", "arc\\rom\\npc\\npc0b\\npc0b" },
    { "uNpc0c", "arc\\rom\\npc\\npc0c\\npc0c" },
    { "uNpc0d", "arc\\rom\\npc\\npc0d\\npc0d" },
    { "uNpc0e", "arc\\rom\\npc\\npc0e\\npc0e" },
    { "uNpc0f", "arc\\rom\\npc\\npc0f\\npc0f" },
    { "uNpc10", "arc\\rom\\npc\\npc10\\npc10" },
    { "uNpc11", "arc\\rom\\npc\\npc11\\npc11" },
    { "uNpc12", "arc\\rom\\npc\\npc12\\npc12" },
    { "uNpc13", "arc\\rom\\npc\\npc13\\npc13" },
    { "uNpc14", "arc\\rom\\npc\\npc14\\npc14" },
    { "uNpc15", "arc\\rom\\npc\\npc15\\npc15" },
    { "uNpc16", "arc\\rom\\npc\\npc16\\npc16" },
    { "uNpc17", "arc\\rom\\npc\\npc17\\npc17" },
    { "uNpc18", "arc\\rom\\npc\\npc18\\npc18" },
    { "uNpc19", "arc\\rom\\npc\\npc19\\npc19" },
    { "uNpc1a", "arc\\rom\\npc\\npc1a\\npc1a" },
    { "uNpc1b", "arc\\rom\\npc\\npc1b\\npc1b" },
    { "uNpc1d", "arc\\rom\\npc\\npc1d\\npc1d" },
    { "uNpc1e", "arc\\rom\\npc\\npc1e\\npc1e" },
    { "uNpc1f", "arc\\rom\\npc\\npc1fg\\npc1fg" },
    { "uNpc20", "arc\\rom\\npc\\npc20g\\npc20g" },
    { "uNpc21", "arc\\rom\\npc\\npc21\\npc21" },
    { "uNpc2a", "arc\\rom\\npc\\npc2a\\npc2a" },
    { "uNpc2b", "arc\\rom\\npc\\npc2b\\npc2b" },
    { "uNpc2c", "arc\\rom\\npc\\npc2c\\npc2c" },
    { "uNpc2d", "arc\\rom\\npc\\npc2d\\npc2d" },
    { "uNpc30", "arc\\rom\\npc\\npc30\\npc30" },
    { "uNpc31", "arc\\rom\\npc\\npc31\\npc31" },
    { "uNpc33", "arc\\rom\\npc\\npc33\\npc33" },
    { "uNpc34", "arc\\rom\\npc\\npc34\\npc34" },
    { "uNpc40", "arc\\rom\\npc\\npc40g\\npc40g" },
    { "uNpc42", "arc\\rom\\npc\\npc42g\\npc42g" },
    { "uNpc44", "arc\\rom\\npc\\npc44g\\npc44g" },
    { "uNpc45", "arc\\rom\\npc\\npc45\\npc45" },
    { "uNpc46", "arc\\rom\\npc\\npc46g\\npc46g" },
    { "uNpc4c", "arc\\rom\\npc\\npc4c\\npc4c" },
    { "uNpc4d", "arc\\rom\\npc\\npc4d\\npc4d" },
    { "uNpc4e", "arc\\rom\\npc\\npc4e\\npc4e" },
    { "uNpc4f", "arc\\rom\\npc\\npc4fg\\npc4fg" },
    { "uNpc50", "arc\\rom\\npc\\npc50g\\npc50g" },
    { "uNpc52", "arc\\rom\\npc\\npc52g\\npc52g" },
    { "uNpc54", "arc\\rom\\npc\\npc54\\npc54" },
    { "uNpc56", "arc\\rom\\npc\\npc56g\\npc56g" },
    { "uNpc57", "arc\\rom\\npc\\npc57\\npc57" },
    { "uNpc59", "arc\\rom\\npc\\npc59\\npc59" },
    { "uNpc5a", "arc\\rom\\npc\\npc5a\\npc5a" },
    { "uNpc70", "arc\\rom\\npc\\npc70\\npc70" },
    { "uNpc80", "arc\\rom\\npc\\npc80g\\npc80g" },
    { "uNpc81", "arc\\rom\\npc\\npc81g\\npc81g" },
    { "uNpc82", "arc\\rom\\npc\\npc82g\\npc82g" },
    { "uNpc83", "arc\\rom\\npc\\npc83g\\npc83g" },
    { "uNpc84", "arc\\rom\\npc\\npc84g\\npc84g" },
    { "uNpc85", "arc\\rom\\npc\\npc85g\\npc85g" },
    { "uNpc86", "arc\\rom\\npc\\npc86g\\npc86g" },
    { "uNpc87", "arc\\rom\\npc\\npc87g\\npc87g" },
    { "uNpc88", "arc\\rom\\npc\\npc88\\npc88" },
    { "uNpc8a", "arc\\rom\\npc\\npc8a\\npc8a" },
    { "uNpc8b", "arc\\rom\\npc\\npc8b\\npc8b" },
    { "uNpc8c", "arc\\rom\\npc\\npc8cg\\npc8cg" },
    { "uNpc8e", "arc\\rom\\npc\\npc8eg\\npc8eg" },
    { "uNpc90", "arc\\rom\\npc\\npc90g\\npc90g" },
    { "uNpc91", "arc\\rom\\npc\\npc91g\\npc91g" },
    { "uNpc92", "arc\\rom\\npc\\npc92\\npc92" },
    { "uNpc93", "arc\\rom\\npc\\npc93\\npc93" },
    { "uNpc94", "arc\\rom\\npc\\npc94\\npc94" },
    { "uNpc95", "arc\\rom\\npc\\npc95\\npc95" },
    { "uNpc96", "arc\\rom\\npc\\npc96\\npc96" },
    { "uNpc97", "arc\\rom\\npc\\npc97\\npc97" },
    { "uNpc98", "arc\\rom\\npc\\npc98\\npc98" },
    { "uNpc9c", "arc\\rom\\npc\\npc9cg\\npc9cg" },
    { "uNpc9d", "arc\\rom\\npc\\npc9dg\\npc9dg" },
    { "uNpc9e", "arc\\rom\\npc\\npc9eg\\npc9eg" },

    //{ "uVehicleOm02", "arc\\rom\\om\\om0002\\om0002" },   // Missing havok xml file
    { "uVehicleOm09", "arc\\rom\\om\\om0009\\om0009" },
    { "uVehicleOm98", "arc\\rom\\om\\om0098\\om0098" },
    { "uVehicleOm99", "arc\\rom\\om\\om0099\\om0099" },
    { "uVehicleOm9a", "arc\\rom\\om\\om009a\\om009a" },
    { "uVehicleOm9b", "arc\\rom\\om\\om009b\\om009b" },
    { "uVehicleOm9d", "arc\\rom\\om\\om009d\\om009d" },
};

class uItemImpl
{
protected:

public:
    static void RegisterTypeInfo();

    static void InstallHooks();
};