#include "common.h"
#include "json.h"

UniqueId UniqueId::make()
{
	static Int64 generator = 0;
	UniqueId id;
	id._id = ++generator;
	return id;
}

Json& operator<< (Json& left, const UniqueId& right)
{
	return left = right._id, left;
}

Json& operator>> (Json& left, UniqueId& right)
{
	return right._id = left.get<Int64>(), left;
}
