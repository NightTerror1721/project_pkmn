#include "common.h"

UniqueId UniqueId::make()
{
	static Int64 generator = 0;
	UniqueId id;
	id._id = ++generator;
	return id;
}
