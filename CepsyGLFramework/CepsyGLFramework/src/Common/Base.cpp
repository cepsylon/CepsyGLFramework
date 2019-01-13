#include "Base.h"

RTTI_B(Base);

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

void Base::set_name(const std::string & name) { mName = name; }
const std::string & Base::name() const { return mName; }

