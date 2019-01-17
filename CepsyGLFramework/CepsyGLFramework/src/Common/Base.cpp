#include "Base.h"

RTTI_B(Base);

Base::Base(const char * name)
	: mName(name)
{ }

Base::Base(const std::string & name)
	: mName(name)
{ }

Base::Base(const Base & rhs)
	: mName(rhs.mName)
{ }

Base & Base::operator=(const Base & rhs)
{
	if (this != &rhs)
		mName = rhs.mName;

	return *this;
}

Base::Base(Base && rhs)
	: mName(std::move(rhs.mName))
{ }

Base & Base::operator=(Base && rhs)
{
	if (this != &rhs)
		mName = std::move(rhs.mName);

	return *this;
}

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

