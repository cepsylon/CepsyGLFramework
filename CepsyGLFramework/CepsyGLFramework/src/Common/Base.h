#pragma once
class Base
{
public:
#ifdef _WIN64
	unsigned __int64 ID() const;
#else
	unsigned __int32 ID() const;
#endif

private:
};
