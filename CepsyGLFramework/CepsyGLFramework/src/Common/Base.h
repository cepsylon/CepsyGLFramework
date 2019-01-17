#pragma once

#include "RTTI/RTTI.h"

#include <string>

class Base
{
public:
	RTTI_H;

	Base() = default;

	// Constructor with the name
	Base(const char * name);
	Base(const std::string & name);

	// Copies
	Base(const Base & rhs);
	Base & operator=(const Base & rhs);

	// Moves
	Base(Base && rhs);
	Base & operator=(Base && rhs);

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
