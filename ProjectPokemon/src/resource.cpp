#include "resource.h"

ResourceFolder::ResourceFolder(const Path& path) :
	_path{ path }
{}

ResourceFolder::ResourceFolder(const ResourceFolder& parent, const Path& path) :
	_path{ parent._path / path }
{}

bool ResourceFolder::_open(const String& filename, std::ifstream& stream) const
{
	stream.open(_path / filename, std::ios::in);
	return !stream.fail();
}

bool ResourceFolder::_open(const Path& path, std::ifstream& stream) const
{
	stream.open(_path / path, std::ios::in);
	return !stream.fail();
}

bool ResourceFolder::_open(const String& filename, std::ofstream& stream) const
{
	stream.open(_path / filename, std::ios::out);
	return !stream.fail();
}

bool ResourceFolder::_open(const Path& path, std::ofstream& stream) const
{
	stream.open(_path / path, std::ios::out);
	return !stream.fail();
}

bool ResourceFolder::openInput(const String& filename, std::ifstream& input) const { return _open(filename, input); }

bool ResourceFolder::openInput(const Path& path, std::ifstream& input) const { return _open(path, input); }

bool ResourceFolder::openInput(const String& filename, const Function<void(std::istream&)>& action) const
{
	std::ifstream stream;
	if (_open(filename, stream))
		return action(stream), true;
	return false;
}

bool ResourceFolder::openInput(const Path& path, const Function<void(std::istream&)>& action) const
{
	std::ifstream stream;
	if (_open(path, stream))
		return action(stream), true;
	return false;
}

bool ResourceFolder::openOutput(const String& filename, std::ofstream& output) const { return _open(filename, output); }

bool ResourceFolder::openOutput(const Path& path, std::ofstream& output) const { return _open(path, output); }

bool ResourceFolder::openOutput(const String& filename, const Function<void(std::ostream&)>& action) const
{
	std::ofstream stream;
	if (_open(filename, stream))
		return action(stream), true;
	return false;
}

bool ResourceFolder::openOutput(const Path& path, const Function<void(std::ostream&)>& action) const
{
	std::ofstream stream;
	if (_open(path, stream))
		return action(stream), true;
	return false;
}

bool ResourceFolder::readJson(const String& filename, Json& json) const { return openInput(filename, [&json](std::istream& is) { json = utils::read(is); }); }

bool ResourceFolder::readJson(const Path& path, Json& json) const { return openInput(path, [&json](std::istream& is) { json = utils::read(is); }); }

bool ResourceFolder::writeJson(const String& filename, const Json& json) const { return openOutput(filename, [&json](std::ostream& os) { utils::write(os, json); }); }

bool ResourceFolder::writeJson(const Path& path, const Json& json) const { return openOutput(path, [&json](std::ostream& os) { utils::write(os, json); }); }
