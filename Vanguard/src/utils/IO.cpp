#include "IO.h"

#include "engine/Errors.h"

// LATER use Logger instead
#include <iostream>

std::string vg::io::read_file(const FilePath& filepath)
{
	std::ifstream file(filepath.c_str(), std::ios_base::in);
	if (!file)
		throw Error(ErrorCode::FILE_IO, "Could not open \"" + filepath.string() + "\" for reading");
	std::ostringstream oss;
	oss << file.rdbuf();
	return oss.str();
}

void vg::io::read_file_uc(const FilePath& filepath, unsigned char*& content, size_t& content_length)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filepath.c_str(), "r") != 0 || !file)
		throw Error(ErrorCode::FILE_IO, "Could not open \"" + filepath.string() + "\" for reading");
	fseek(file, 0, SEEK_END);
	content_length = ftell(file);
	fseek(file, 0, SEEK_SET);
	content = new unsigned char[content_length];
	fread(content, content_length, 1, file);
	fclose(file);
}

std::string vg::io::read_template_file(const FilePath& filepath, const std::unordered_map<std::string, std::string>& tmplate)
{
	std::string content = read_file(filepath);
	for (const auto& [placeholder, value] : tmplate)
	{
		size_t pos = 0;
		while ((pos = content.find(placeholder, pos)) != std::string::npos)
		{
			content.replace(pos, placeholder.length(), value);
			pos += value.length();
		}
	}
	return content;
}
