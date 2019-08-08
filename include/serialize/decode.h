﻿#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <array>
#include <string>
#include <list>
#include <stack>
#include <deque>
#include <forward_list>
#include <utility>
#include <type_traits>
#include <set>
#include <nlohmann/json.hpp>

#include <variant>
#include <optional>

using json = nlohmann::json;

namespace meta::serialize
{

	bool decode(const json& data, float& dst)
	{
		if (!data.is_number_float())
		{
			return false;
		}
		else
		{
			dst = data.get<float>();
			return true;
		}
	}
	bool decode(const json& data, double& dst)
	{
		if (!data.is_number_float())
		{
			return false;
		}
		else
		{
			dst = data.get<double>();
			return true;
		}
	}
	bool decode(const json& data, bool& dst)
	{
		if (!data.is_boolean())
		{
			return false;
		}
		{
			dst = data.get<bool>();
			return true;
		}
	}
	bool decode(const json& data, std::string& dst)
	{
		if (!data.is_string())
		{
			return false;
		}
		else
		{
			dst = data.get<std::string>();
			return true;
		}
	}
	template <typename T>
	bool decode(const json& data, std::optional<T>& dst)
	{
		if (data.is_null())
		{
			dst.reset();
			return true;
		}
		else
		{
			T temp;
			if (decode(data, temp))
			{
				dst = temp;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	template <typename T>
	inline typename std::enable_if<std::is_integral<T>::value, bool>::type
		decode(const json& data, T& dst)
	{
		if (!data.is_number_integer())
		{
			return false;
		}
		else
		{
			dst = data.get<T>();
			return true;
		}

	}
	template<typename T1, typename T2>
	bool decode(const json& data, std::pair<T1, T2>& dst)
	{
		if (!data.is_array())
		{
			return false;
		}
		if (data.size() != 2)
		{
			return false;
		}
		if (!decode(data[0], dst.first))
		{
			return false;
		}
		if (!decode(data[1], dst.second))
		{
			return false;
		}
		return true;
	}
	template<typename T>
	bool decode(const json& data, std::vector<T>& dst)
	{
		if (!data.is_array())
		{
			return false;
		}
		dst.reserve(data.size());
		for (std::size_t i = 0; i < data.size(); i++)
		{
			T temp;
			if (!decode(data[i], temp))
			{
				dst.clear();
				return false;
			}
			else
			{
				dst.push_back(std::move(temp));
			}
		}
		return true;
	}
	template<typename T, std::size_t N>
	bool decode(const json& data, std::array<T, N>& dst)
	{
		if (!data.is_array())
		{
			return false;
		}
		if (data.size() != N)
		{
			return false;
		}
		for (std::size_t i = 0; i < data.size(); i++)
		{
			if (!decode(data[i], dst[i]))
			{
				return false;
			}
		}
		return true;
	}
	template<typename T>
	bool decode(const json& data, std::forward_list<T>& dst)
	{
		if (!data.is_array())
		{
			return false;
		}
		for (std::size_t i = 0; i < data.size(); i++)
		{
			T temp;
			if (!decode(data[i], temp))
			{
				dst.clear();
				return false;
			}
			else
			{
				dst.push_front(std::move(temp));
			}
		}
		dst.reverse();
		return true;
	}
	template<typename T>
	bool decode(const json& data, std::list<T>& dst)
	{
		if (!data.is_array())
		{
			return false;
		}
		for (std::size_t i = 0; i < data.size(); i++)
		{
			T temp;
			if (!decode(data[i], temp))
			{
				dst.clear();
				return false;
			}
			else
			{
				dst.push_back(std::move(temp));
			}
		}
		dst.reverse();
		return true;
	}

#define DECODE_FOR_SET(SET_TYPE) template<typename T>\
bool decode(const json& data, SET_TYPE<T>& dst)\
{												\
	if (!data.is_array())						\
	{											\
		return false;							\
	}											\
	for (std::size_t i = 0; i < data.size(); i++)\
	{											\
		T temp;									\
		if (!decode(data[i], temp))				\
		{										\
			dst.clear();						\
			return false;						\
		}										\
		else									\
		{										\
			dst.insert(std::move(temp));		\
		}										\
	}											\
	return true;								\
}												\

	DECODE_FOR_SET(std::set)

		DECODE_FOR_SET(std::unordered_set)

		DECODE_FOR_SET(std::multiset)

		DECODE_FOR_SET(std::unordered_multiset)

#define DECODE_FOR_MAP(MAP_TYPE) template <typename T1, typename T2>\
bool decode(const json& data, MAP_TYPE<T1, T2>& dst)				\
{																	\
	if (!data.is_array())											\
	{																\
		return false;												\
	}																\
	for (std::size_t i = 0; i < data.size(); i++)					\
	{																\
		std::pair<T1, T2> temp;										\
		if (!decode(data[i], temp))									\
		{															\
			dst.clear();											\
			return false;											\
		}															\
		else														\
		{															\
			dst.emplace(temp);										\
		}															\
	}																\
	return true;													\
}																	\

		DECODE_FOR_MAP(std::map)

		DECODE_FOR_MAP(std::unordered_map)

		DECODE_FOR_MAP(std::multimap)

		DECODE_FOR_MAP(std::unordered_multimap)



		template <typename... Args>
	bool decode(const json& data, std::tuple<Args...>& dst)
	{
		if (!data.is_array())
		{
			return false;
		}
		if (data.size() != std::tuple_size<std::tuple<Args...>>::value)
		{
			return false;
		}
		return decode_for_tuple(data, dst, std::index_sequence_for<Args...>{});
	}
	template <typename... Args, std::size_t... index>
	bool decode_for_tuple(const json& data, std::tuple<Args...>& dst, std::index_sequence<index...>)
	{
		return (decode(data[index], std::get<index>(dst)) && ...);
	}
	template <std::size_t N, typename... Args>
	bool decode_for_variant(const json& data, std::variant<Args...>& dst)
	{
		if constexpr (N >= std::tuple_size_v<std::tuple<Args...>>)
		{
			return false;
		}
		else
		{
			std::tuple_element_t<N, std::tuple<Args...>> temp;
			if (decode(data, temp))
			{
				dst = temp;
				return true;
			}
			else
			{
				return decode_for_variant<N + 1, Args...>(data, dst);
			}
		}

	}
	template <typename... Args>
	bool decode(const json& data, std::variant<Args...>& dst)
	{
		return decode_for_variant<0, Args...>(data, dst);
	}
	template <typename T1, typename T2>
	typename std::enable_if<
		std::is_same<
		decltype(std::declval<T1>().decode(std::declval<T2>())),
		bool>::value,
		bool>::type decode(const T2& data, T1& dst)
	{
		// for class T1 with function bool decode(const T2& data)
		return dst.decode(data);
	}
}