#pragma once

class RTTI
{
public:
	// Base constructor
	RTTI();

	// Derived class constructor
	RTTI(const RTTI & base);

	// Gets parent
	const RTTI & get_parent() const;

	// Equal operator overload
	bool operator==(const RTTI & rhs) const;
	// Not equal operator overload
	bool operator!=(const RTTI & rhs) const;

private:
	const RTTI * mParent = nullptr;	// Parent if any of the current RTTI
};

// Header rtti
#define RTTI_H static const RTTI mRTTI; \
static const RTTI & class_type(); \
virtual const RTTI & type() const; \
virtual const char * class_name() const; \

// Implementation of rtti
#define RTTI_I(current_class, parent_class) const RTTI current_class::mRTTI{ parent_class::mRTTI }; \
const RTTI & current_class::class_type() { return mRTTI; } \
const RTTI & current_class::type() const { return mRTTI; } \
const char * current_class::class_name() const { return #current_class; } \

// Base class for rtti
#define RTTI_B(base_class) const RTTI base_class::mRTTI; \
const RTTI & base_class::class_type() { return mRTTI; } \
const RTTI & base_class::type() const { return mRTTI; } \
const char * base_class::class_name() const { return #base_class; } \

