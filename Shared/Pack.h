#pragma once

#include <WinSock2.h>
#include <algorithm>
#include <iterator>
#include <stdint.h>

namespace util
{
	template <class val_type>
	size_t pack(val_type const values[], size_t count, char destination[])
	{
		std::copy(values, values + count, stdext::checked_array_iterator<val_type*>(reinterpret_cast<val_type*>(destination), count));
		return count * sizeof(val_type);
	}

	template <class val_type>
	size_t unpack(val_type values[], size_t count, char const source[])
	{
		val_type const* fSource = reinterpret_cast<val_type const*>(source);
		std::copy(fSource, fSource + count, stdext::checked_array_iterator<val_type*>(values, count));
		return count * sizeof(val_type);
	}

	template <> inline
	size_t pack<uint16_t>(uint16_t const values[], size_t count, char destination[])
	{
		uint16_t* destP = reinterpret_cast<uint16_t*>(destination);

		for (size_t i = 0; i < count; ++i)
		{
			destP[i] = htons(values[i]);
		}

		return count * sizeof(uint16_t);
	}

	template <> inline
	size_t unpack<uint16_t>(uint16_t values[], size_t count, char const source[])
	{
		uint16_t const* sourceP = reinterpret_cast<uint16_t const*>(source);
		
		for (size_t i = 0; i < count; ++i)
		{
			values[i] = ntohs(sourceP[i]);
		}

		return count * sizeof(uint16_t);
	}

	template <> inline
	size_t pack<uint32_t>(uint32_t const values[], size_t count, char destination[])
	{
		uint32_t* destP = reinterpret_cast<uint32_t*>(destination);

		for (size_t i = 0; i < count; ++i)
		{
			destP[i] = htonl(values[i]);
		}

		return count * sizeof(uint32_t);
	}

	template <> inline
	size_t unpack<uint32_t>(uint32_t values[], size_t count, char const source[])
	{
		uint32_t const* sourceP = reinterpret_cast<uint32_t const*>(source);
		
		for (size_t i = 0; i < count; ++i)
		{
			values[i] = ntohl(sourceP[i]);
		}

		return count * sizeof(uint32_t);
	}

	template <> inline
	size_t pack<std::string>(std::string const values[], size_t count, char destination[])
	{
		char* currDest = destination;

		for (size_t i = 0; i < count; i++)
		{
			uint16_t stringSize = values[i].size();
			currDest += pack(&stringSize, 1, currDest);

			std::copy(values[i].begin(), values[i].end(), stdext::checked_array_iterator<char*>(currDest, stringSize));
			currDest += stringSize * sizeof(std::string::value_type);
		}

		return currDest - destination;
	}

	template <> inline
	size_t unpack<std::string>(std::string values[], size_t count, char const source[])
	{
		char const* currSource = source;

		for (size_t i = 0; i < count; i++)
		{
			uint16_t stringSize;
			currSource += unpack(&stringSize, 1, currSource);

			values[i].assign(currSource, stringSize);
			currSource += stringSize * sizeof(std::string::value_type);
		}

		return currSource - source;
	}

	inline size_t packedSize(std::string const& str)
	{
		return str.size() + sizeof(uint16_t);
	}
}