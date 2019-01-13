#include "rtti.h"

RTTI::RTTI()
{	}

RTTI::RTTI(const RTTI & base)
{
	mParent = &base;
}

const RTTI & RTTI::get_parent() const
{
	return *mParent;
}

bool RTTI::operator==(const RTTI & rhs) const
{
	return this == &rhs;
}

bool RTTI::operator!=(const RTTI & rhs) const
{
	return this != &rhs;
}
