#include "FilePath.h"

#include <algorithm>

void vg::FilePath::to_unix_format()
{
#ifdef _WIN32
	std::replace(path.begin(), path.end(), '\\', '/');
#endif
}

vg::FilePath vg::FilePath::operator/(const FilePath& relative) const
{
	if (path.ends_with("/"))
		return path + relative.path;
	else
		return path + "/" + relative.path;
}

vg::FilePath& vg::FilePath::operator+=(const std::string& addon)
{
	path += addon;
	return *this;
}

vg::FilePath vg::FilePath::operator+(const std::string& addon) const
{
	return FilePath(path.c_str()) += addon;
}

std::string vg::FilePath::native_path() const
{
#ifdef _WIN32
	std::string native_filepath = path;
	std::replace(native_filepath.begin(), native_filepath.end(), '/', '\\');
	return native_filepath;
#else
	return path;
#endif
}

std::string vg::FilePath::filename() const
{
	return std::filesystem::path(path).filename().string();
}

vg::FilePath vg::FilePath::extension() const
{
	for (long long i = path.size() - 1; i >= 0; --i)
	{
		if (path[i] == '.')
			return path.c_str() + i;
	}
	return "";
}

bool vg::FilePath::has_extension(const char* ext) const
{
	auto actual = extension();
	size_t i = 0;
	while (i < actual.path.size())
	{
		if (ext[i] == '\0')
			return false;
		if (toupper(ext[i]) != toupper(actual.path[i]))
			return false;
		++i;
	}
	return ext[i] == '\0';
}

bool vg::FilePath::has_any_extension(const char* const* exts, size_t num_exts) const
{
	for (size_t i = 0; i < num_exts; ++i)
		if (has_extension(exts[i]))
			return true;
	return false;
}
