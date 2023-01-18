#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

class FileReader
{
public:
	static inline bool printOnError{ true };

	// returns contents of file if it exists.
	static const std::optional<std::string> getContents(const std::string_view filePath)
	{
		std::ifstream file;

		const std::string filePathStr{ filePath };

		file.open(filePathStr);	// open for read only

		if (file.is_open())
		{
			std::string contents;

			std::string line;
			while (line, std::getline(file, line))
				contents += line + '\n';

			return contents;
		}
		else
		{
			if (printOnError)
				std::cerr << "Error! Couldn't open \"" << filePath << "\"\n";

			return {};	// return empty optional
		}
	}
};