#include "json.h"

namespace utils
{
	Json read(std::istream& input)
	{
		try
		{
			Json json;
			input >> json;
			return json;
		}
		catch (const std::exception& ex) { throw JsonException{ ex.what() }; }
	}

	Json read(const Path& path)
	{
		std::fstream f{ path, std::ios::in };
		return read(f);
	}

	Json read(const String& path)
	{
		std::fstream f{ path, std::ios::in };
		return read(f);
	}

	void write(std::ostream& output, const Json& json)
	{
		try
		{
			output << json;
		}
		catch (const std::exception& ex) { throw JsonException{ ex.what() }; }
	}

	void write(const Path& path, const Json& json)
	{
		std::fstream f{ path, std::ios::out };
		write(f, json);
	}

	void write(const String& path, const Json& json)
	{
		std::fstream f{ path, std::ios::out };
		write(f, json);
	}
}

std::ostream& operator<< (std::ostream& left, const utils::JsonSerializable& right)
{
	utils::write(left, right);
	return left;
}

std::istream& operator>> (std::istream& left, utils::JsonSerializable& right)
{
	utils::read(left, right);
	return left;
}
