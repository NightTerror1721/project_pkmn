#pragma once

#include <unordered_map>
#include <type_traits>
#include <functional>
#include <filesystem>
#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <concepts>
#include <sstream>
#include <fstream>
#include <compare>
#include <utility>
#include <chrono>
#include <random>
#include <memory>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <list>
#include <map>
#include <new>

#include <sfml/Graphics.hpp>

typedef std::uint8_t UInt8;
typedef std::uint16_t UInt16;
typedef std::uint32_t UInt32;
typedef std::uint64_t UInt64;

typedef std::int8_t Int8;
typedef std::int16_t Int16;
typedef std::int32_t Int32;
typedef std::int64_t Int64;

typedef std::string String;

typedef std::byte Byte;

typedef std::size_t Size;
typedef std::size_t Offset;

typedef std::filesystem::path Path;

typedef sf::Vector2f Vec2f;
typedef sf::Vector2i Vec2i;
typedef sf::Vector2u Vec2u;

typedef sf::IntRect IntRect;

typedef sf::Color Color;

template<typename _Fty>
using Function = std::function<_Fty>;

namespace filesystem = std::filesystem;

namespace utils
{
	constexpr char path_separator =
#ifdef _WIN32
		'\\';
#else
		'/';
#endif
}

namespace utils
{
	template<typename _Ty>
	concept StringOnly = std::same_as<String, _Ty>;
}

namespace utils
{
	template<typename _Ty>
	inline _Ty* raw_malloc(Size size) { return reinterpret_cast<_Ty*>(::operator new(size)); }

	inline void raw_free(void* ptr) { ::operator delete(ptr); }


	template<typename _Ty, typename... _Args>
	inline _Ty& construct(_Ty& object, _Args&&... args) { return new (&object) _Ty(std::forward<_Args>(args)...), object; }

	template<typename _Ty>
	inline void destroy(_Ty& object) { object.~_Ty(); }

	template<typename _Ty>
	inline _Ty& copy(_Ty& dst, const _Ty& src) { return construct<_Ty, const _Ty&>(dst, src); }

	template<typename _Ty>
	inline _Ty& move(_Ty& dst, _Ty&& src) { return construct<_Ty, _Ty&&>(dst, src); }


	template<typename _ValueTy, typename _MinTy, typename _MaxTy>
	constexpr _ValueTy clamp(_ValueTy value, _MinTy min, _MaxTy max)
	{
		if constexpr (std::same_as< _MinTy, _ValueTy>)
		{
			if constexpr (std::same_as< _MaxTy, _ValueTy>)
				return std::max(std::min(max, value), min);
			else return std::max(std::min(static_cast<_ValueTy>(max), value), min);
		}
		else
		{
			if constexpr (std::same_as< _MaxTy, _ValueTy>)
				return std::max(std::min(max, value), static_cast<_ValueTy>(min));
			else return std::max(std::min(static_cast<_ValueTy>(max), value), static_cast<_ValueTy>(min));
		}
	}

	template<std::same_as<String>... _Args>
	Path make_path(const _Args&... parts)
	{
		std::initializer_list<String*> args{ std::addressof(parts)... };

		bool first = true;
		std::stringstream ss;
		for (String* part : args)
		{
			if (!first)
				ss << path_separator << part;
			else first = false, ss << part;
		}

		return ss.str();
	}

	inline Path operator"" _p(const char* str, Size size) { return String{ str, size }; }
}
