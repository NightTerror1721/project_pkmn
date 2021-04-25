#pragma once

#include <native_json/json.hpp>
#include "common.h"

typedef nlohmann::json Json;

namespace utils
{
	class JsonException : public std::exception
	{
		inline JsonException(const char* msg) : exception{ msg } {}
		inline JsonException(const std::string& msg) : exception{ msg.c_str() } {}
	};

	class JsonSerializable
	{
	public:
		virtual Json serialize() const = 0;
		virtual void deserialize(const Json& json) = 0;

		inline JsonSerializable& operator<< (const Json& right) { return deserialize(right), *this; }
		inline JsonSerializable& operator>> (Json& right) { return right = std::move(serialize()), *this; }
	};

	template<typename _Ty>
	concept JsonSerializableOnly = std::derived_from<_Ty, JsonSerializable>;

	Json read(std::istream& input);
	Json read(const Path& path);
	Json read(const String& path);

	void write(std::ostream& output, const Json& json);
	void write(const Path& path, const Json& json);
	void write(const String& path, const Json& json);

	inline Json extract(const JsonSerializable& obj) { return obj.serialize(); }

	inline void inject(JsonSerializable& obj, const Json& json) { obj.deserialize(json); }

	inline void read(std::istream& input, JsonSerializable& obj) { return obj.deserialize(read(input)); }
	inline void read(const Path& path, JsonSerializable& obj) { return obj.deserialize(read(path)); }
	inline void read(const String& path, JsonSerializable& obj) { return obj.deserialize(read(path)); }

	inline void write(std::ostream& output, const JsonSerializable& obj) { write(output, obj.serialize()); }
	inline void write(const Path& path, const JsonSerializable& obj) { write(path, obj.serialize()); }
	inline void write(const String& path, const JsonSerializable& obj) { write(path, obj.serialize()); }

	inline bool has(const Json& json, const char& key) { return json.find(key) != json.end(); }
	inline bool has(const Json& json, const String& key) { return json.find(key) != json.end(); }

	template<typename _Ty>
	const _Ty& opt(const Json& json, const String& key, const _Ty& default_value)
	{
		const auto it = json.find(key);
		return it == json.end() ? default_value : it.value().get_ref<_Ty>();
	}

	template<typename _Ty>
	const _Ty* opt(const Json& json, const String& key)
	{
		const auto it = json.find(key);
		if (it == json.end())
			return nullptr;
		return std::addressof<_Ty>(it.value().get_ref<_Ty>());
	}
}

std::ostream& operator<< (std::ostream& left, const utils::JsonSerializable& right);
std::istream& operator>> (std::istream& left, utils::JsonSerializable& right);
