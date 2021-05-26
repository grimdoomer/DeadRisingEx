
#pragma once
#include "MtStream.h"

// sizeof = 0x10
struct BinaryXmlHeader
{
    /* 0x00 */ DWORD    Magic;
    /* 0x04 */ WORD     Version;
    /* 0x06 */ WORD     SubVersion;
    /* 0x08 */ DWORD    NodeCount;
    /* 0x0C */ DWORD    NodeDataSize;
};
static_assert(sizeof(BinaryXmlHeader) == 0x10, "BinaryXmlHeader incorrect struct size");

// sizeof = 0x10
struct NodeEntry
{
    union
    {
        /* 0x00 */ DWORD    FileType;       // File type ID
        /* 0x00 */ MtDTI    *pFileTypeDTI;  // Type info for the xml node type
    };
    /* 0x08 */ DWORD NodeCount;
};
static_assert(sizeof(NodeEntry) == 0x10, "NodeEntry incorrect struct size");

// sizeof = 0x30
struct NodeDescriptor
{
    /* 0x00 */ void     *pName;             // Offset of the node name relative to the header
    union
    {
        /* 0x08 */ BYTE     PropertyType;       // Node type, see MT_PROP_TYPE_*
        /* 0x08 */ DWORD    Flags;
    };

    /* 0x10 */ void                             *pFieldGetterFunc;      // Offset of the field relative to the start of the object (runtime only)
    /* 0x18 */ MtPropertyEntry_GetArrayLength   pGetArrayLengthFunc;    // Function to get the length of the field if it is an array (runtime only)
    /* 0x20 */ void                             *pUnknownFunc1;         // pUnkFunc1 from the MtPropertyListEntry (runtime only)
    /* 0x28 */ void                             *pUnknownFunc2;         // pUnkFunc2 from the MtPropertyListEntry (runtime only)
};
static_assert(sizeof(NodeDescriptor) == 0x30, "NodeDescriptor incorrect struct size");

// sizeof = 0x280?
struct MtBinaryXmlReader
{
    /* 0x08 */ cResource    **ppParsedObjects;      // Array of resource pointers that represent the parsed node data
    /* 0x10 */ // void*
    /* 0x18 */ NodeEntry    **ppNodes;
    /* 0x20 */ DWORD        NodeCount;

    inline static MtBinaryXmlReader * (__stdcall *_ctor)(MtBinaryXmlReader *thisptr, MtStream *pStream) =
        (MtBinaryXmlReader*(__stdcall*)(MtBinaryXmlReader*, MtStream*))GetModuleAddress(0x140001000);
};