/*

*/

#include "rModelImpl.h"

void rModelImpl::InitializeTypeInfo()
{
    // Register type info:
    RegisterTypeInfo(&JointTypeInfo);
    RegisterTypeInfo(&PrimitiveTypeInfo);
    RegisterTypeInfo(&MaterialTypeInfo);
    RegisterTypeInfo(&rModelTypeInfo);
}