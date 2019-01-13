#pragma once

#include "RTTI/RTTI.h"

#include <string>

class Base
{
public:
	RTTI_H;

#ifdef _WIN64
	unsigned __int64 ID() const;
#else
	unsigned __int32 ID() const;
#endif

	// Settor
	void set_name(const std::string & name);

	// Gettor
	const std::string & name() const;


protected:
	std::string mName;
};
