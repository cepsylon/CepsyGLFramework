#include "Base.h"

#ifdef _WIN64
unsigned __int64 Base::ID() const
{
	return reinterpret_cast<unsigned __int64>(this);
}
#else
unsigned __int32 Base::ID() const
{
	return reinterpret_cast<unsigned __int32>(this);
}
#endif
