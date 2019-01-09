#pragma once
class Base
{
public:
#ifdef _WIN32
	unsigned __int32 ID() const;
#else
	unsigned __int64 ID() const;
#endif

private:
};
