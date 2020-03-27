#include "kvd.h"
#include <cstring>
#include "format.h"

using namespace ux3d::slimktx2;

uint32_t KeyValueData::computeSize() const
{
	size_t size = 0u; // keyAndValueByteLength member

	const Entry* pEntry = pKeyValues;
	while (pEntry != nullptr)
	{
		size += sizeof(uint32_t);
		if (pEntry->pKeyValue != nullptr)
		{
			size += pEntry->keyAndValueByteLength;
		}
		size += getPadding(size, 4u);

		pEntry = pEntry->pNext;
	};

	return static_cast<uint32_t>(size);
}

KeyValueData::Entry* KeyValueData::getLastEntry() const
{
	Entry* pEntry = pKeyValues;
	while (pEntry != nullptr)
	{
		if (pEntry->pNext == nullptr)
		{
			break;
		}
		pEntry = pEntry->pNext;
	}

	return pEntry;
}

uint32_t KeyValueData::Entry::getKeyLength() const
{
	uint32_t length = 0u;
	if (pKeyValue != nullptr)
	{		
		for (; pKeyValue[length] != 0u && length < keyAndValueByteLength; ++length) {}
	}
	return length;
}

uint32_t KeyValueData::Entry::getValueLength() const
{
	return keyAndValueByteLength - getKeyLength();
}
