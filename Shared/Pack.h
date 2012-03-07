#pragma once

#include <algorithm>
#include <iterator>
#include <stdint.h>

namespace util
{
	template <class val_type>
	void pack(val_type const values[], size_t count, char destination[])
	{
		std::copy(values, values + count, stdext::checked_array_iterator<val_type*>(reinterpret_cast<val_type*>(destination), count));
	}

	template <class val_type>
	void unpack(val_type values[], size_t count, char const source[])
	{
		val_type const* fSource = reinterpret_cast<val_type const*>(source);
		std::copy(fSource, fSource + count, stdext::checked_array_iterator<val_type*>(values, count));
	}

	template <>
	void pack<uint16_t>(uint16_t const values[], size_t count, char destination[])
	{
		uint16_t* destP = reinterpret_cast<uint16_t*>(destination);

		for (size_t i = 0; i < count; ++i)
		{
			destP[i] = htons(values[i]);
		}
	}

	template <>
	void unpack<uint16_t>(uint16_t values[], size_t count, char const source[])
	{
		uint16_t const* sourceP = reinterpret_cast<uint16_t const*>(source);
		
		for (size_t i = 0; i < count; ++i)
		{
			values[i] = ntohs(sourceP[i]);
		}
	}

	template <>
	void pack<uint32_t>(uint32_t const values[], size_t count, char destination[])
	{
		uint32_t* destP = reinterpret_cast<uint32_t*>(destination);

		for (size_t i = 0; i < count; ++i)
		{
			destP[i] = htonl(values[i]);
		}
	}

	template <>
	void unpack<uint32_t>(uint32_t values[], size_t count, char const source[])
	{
		uint32_t const* sourceP = reinterpret_cast<uint32_t const*>(source);
		
		for (size_t i = 0; i < count; ++i)
		{
			values[i] = ntohl(sourceP[i]);
		}
	}
}