#pragma once

#include "common.h"
#include "json.h"

class ResourceFolder
{
private:
	Path _path;

public:
	ResourceFolder() = default;
	ResourceFolder(const ResourceFolder&) = default;
	ResourceFolder(ResourceFolder&&) noexcept = default;
	~ResourceFolder() = default;

	ResourceFolder& operator= (const ResourceFolder&) = default;
	ResourceFolder& operator= (ResourceFolder&&) noexcept = default;

	bool operator== (const ResourceFolder&) const = default;
	auto operator<=> (const ResourceFolder&) const = default;

	ResourceFolder(const Path& path);
	ResourceFolder(const ResourceFolder& parent, const Path& path);

	bool openInput(const String& filename, std::ifstream& input) const;
	bool openInput(const Path& path, std::ifstream& input) const;
	bool openInput(const String& filename, const Function<void(std::istream&)>& action) const;
	bool openInput(const Path& path, const Function<void(std::istream&)>& action) const;

	bool openOutput(const String& filename, std::ofstream& output) const;
	bool openOutput(const Path& path, std::ofstream& output) const;
	bool openOutput(const String& filename, const Function<void(std::ostream&)>& action) const;
	bool openOutput(const Path& path, const Function<void(std::ostream&)>& action) const;

	bool readJson(const String& filename, Json& json) const;
	bool readJson(const Path& path, Json& json) const;

	bool writeJson(const String& filename, const Json& json) const;
	bool writeJson(const Path& path, const Json& json) const;

	inline bool openInput(const char* filename, std::ifstream& input) const { return openInput(String{ filename }, input); }
	inline bool openInput(const char* filename, const Function<void(std::istream&)>& action) const { return openInput(String{ filename }, action); }

	inline bool openOutput(const char* filename, std::ofstream& output) const { return openOutput(String{ filename }, output); }
	inline bool openOutput(const char* filename, const Function<void(std::ostream&)>& action) const { return openOutput(String{ filename }, action); }

	inline bool readJson(const char* filename, Json& json) const { return readJson(String{ filename }, json); }

	inline bool writeJson(const char* filename, const Json& json) const { return writeJson(String{ filename }, json); }

	template<utils::JsonSerializableOnly _Ty>
	inline _Ty& readAndInject(const String& filename, _Ty& obj) const
	{
		return openInput(filename, [&obj](std::istream& in) { utils::read(in, obj); }), obj;
	}

	template<utils::JsonSerializableOnly _Ty>
	inline _Ty& readAndInject(const Path& path, _Ty& obj) const
	{
		return openInput(path, [&obj](std::istream& in) { utils::read(in, obj); }), obj;
	}

	template<utils::JsonSerializableOnly _Ty>
	inline _Ty& readAndInject(const char* filename, _Ty& obj) const
	{
		return readAndInject(String{ filename }, obj);
	}

	template<utils::JsonSerializableOnly _Ty>
	inline void extractAndWrite(const String& filename, _Ty& obj) const
	{
		openOutput(filename, [&obj](std::ostream& os) { utils::write(os, obj); });
	}

	template<utils::JsonSerializableOnly _Ty>
	inline void extractAndWrite(const Path& path, _Ty& obj) const
	{
		openOutput(path, [&obj](std::ostream& os) { utils::write(os, obj); });
	}

	template<utils::JsonSerializableOnly _Ty>
	inline void extractAndWrite(const char* filename, _Ty& obj) const
	{
		extractAndWrite(String{ filename }, obj);
	}

	inline Path pathOf(const String& filename) const { return _path / filename; }
	inline Path pathOf(const Path& path) const { return _path / path; }
	inline Path pathOf(const char* filename) const { return _path / filename; }

	inline ResourceFolder folder(const String& filename) const { return { *this, filename }; }
	inline ResourceFolder folder(const Path& path) const { return { *this, path }; }
	inline ResourceFolder folder(const char* path) const { return { *this, String{ path } }; }

	inline const Path& path() const { return _path; }

private:
	bool _open(const String& filename, std::ifstream& stream) const;
	bool _open(const Path& path, std::ifstream& stream) const;

	bool _open(const String& filename, std::ofstream& stream) const;
	bool _open(const Path& path, std::ofstream& stream) const;
};
