#pragma once

#include <filesystem>

namespace vg
{
	class FilePath
	{
		std::string path;
		void to_unix_format();

	public:
		FilePath() = default;
		FilePath(const char* path) : path(path ? path : "") { to_unix_format(); }
		FilePath(std::string&& path) : path(std::move(path)) { to_unix_format(); }
		FilePath& operator=(const char* path_) { path = path_ ? path_ : ""; to_unix_format(); return *this; }
		FilePath& operator=(std::string&& path_) { path = std::move(path_); to_unix_format(); return *this; }
		FilePath(const FilePath&) = default;
		FilePath(FilePath&&) noexcept = default;
		FilePath& operator=(const FilePath&) = default;
		FilePath& operator=(FilePath&&) = default;

		bool operator==(const FilePath& other) const = default;
		FilePath operator/(const FilePath& relative) const;
		FilePath& operator+=(const std::string& addon);
		FilePath operator+(const std::string& addon) const;

		bool is_absolute() const { return std::filesystem::path(path).is_absolute(); }
		bool is_relative() const { return std::filesystem::path(path).is_relative(); }
		std::string native_path() const;

		void clear() { path.clear(); }
		bool empty() const { return path.empty(); }
		const char* c_str() const { return path.c_str(); }
		std::string string() const { return path; }
		std::string& path_ref() { return path; }
		const std::string& path_ref() const { return path; }
		std::string filename() const;
		FilePath extension() const;
		bool has_extension(const char* ext) const;
		bool has_any_extension(const char* const* exts, size_t num_exts) const;
		size_t hash() const { return std::hash<std::filesystem::path>{}(path); }
	};
}

template<>
struct std::hash<vg::FilePath>
{
	size_t operator()(const vg::FilePath& fp) const { return fp.hash(); }
};
