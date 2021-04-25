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

class UniqueId
{
private:
	Int64 _id = 0;

public:
	UniqueId() = default;
	UniqueId(const UniqueId&) = default;
	~UniqueId() = default;

	UniqueId& operator= (const UniqueId&) = default;

	bool operator== (const UniqueId&) const = default;
	auto operator<=> (const UniqueId&) const = default;

	inline operator bool() const { return _id; }
	inline bool operator! () const { return !_id; }

	static UniqueId make();

	friend inline std::ostream& operator<< (std::ostream& left, const UniqueId& right) { return left << right._id; }
	friend inline std::istream& operator>> (std::istream& left, UniqueId& right) { return left >> right._id; }
};

template<typename _Ty>
class Reference
{
private:
	struct Data
	{
		_Ty* value;
		Size count;

	public:
		inline Data() : value{ nullptr }, count{ 0 } {}
		inline Data(_Ty* value) : value{ value }, count{ value ? 1 : 0 } {}
		~Data()
		{
			if (value)
				delete value;
		}

		Data(const Data&) = delete;
		Data& operator= (const Data&) = delete;

		inline void increase() { ++count; }
		void decrease()
		{
			if (count > 1)
				--count;
			else clear();
		}
		void clear()
		{
			if (value)
				delete value;
			count = 0;
			value = nullptr;
		}

		inline bool empty() const { return !value; }
	};

private:
	Data* _data;
	_Ty* _value;

private:
	inline void _destroy()
	{
		if (_data)
		{
			_data->decrease();
			if (_data->empty())
				delete _data;
		}
	}

public:
	inline Reference() : _data{}, _value{ nullptr } {}
	inline Reference(decltype(nullptr)) : _data{}, _value{ nullptr } {}
	inline Reference(_Ty* ptr) : _data{ ptr }, _value{ ptr } {}

	Reference(const Reference& right) : _data{ right._data }, _value{ _data->value }
	{
		_data->increase();
	}
	Reference(Reference&& right) noexcept : _data{ right._data }
	{
		right._data = nullptr;
		right._value = nullptr;
	}
	inline ~Reference() { _destroy(); }

	Reference& operator= (decltype(nullptr))
	{
		_destroy();
		_data = nullptr;
		_value = nullptr;
		return *this;
	}

	Reference& operator= (_Ty* ptr)
	{
		_destroy();
		_data = new Data{ ptr };
		_value = ptr;
		return *this;
	}

	Reference& operator= (const Reference& right)
	{
		_destroy();
		_data = right._data;
		_value = _data->value;
		_data->increase();
		return *this;
	}
	Reference& operator= (Reference&& right) noexcept
	{
		_destroy();
		_data = right._data;
		_value = _data->value;
		right._data = nullptr;
		return *this;
	}

	inline _Ty& operator* () { return *_value; }
	inline const _Ty& operator* () const { return *_value; }

	inline _Ty* operator-> () { return _value; }
	inline const _Ty* operator-> () const { return _value; }

	inline operator bool() const { return _data; }
	inline bool operator! () const { return !_data; }
};
