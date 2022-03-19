#include "VirtualFilesystem.h"
#include <fstream>
#include <sstream>
#include "Parser.hpp"
#include "Writer.hpp"

VirtualFilesystem::VirtualFilesystem(const std::string &sFilename) noexcept
{
	m_sFilename = sFilename;
	loadFiles();
}

VirtualFilesystem::~VirtualFilesystem() noexcept
{
	saveFiles();
}

void VirtualFilesystem::loadFiles() noexcept
{
	std::ifstream file = std::ifstream(m_sFilename, std::ios::binary);

	if (file.is_open())
	{
		std::stringstream ss;
		file >> ss.rdbuf();
		file.close();
		
		Parser parser = Parser(ss.str());
		if (parser.size() < 4) return; // Error

		uint32_t nFiles = parser.parse<uint32_t>();

		for (uint32_t nCurrentFile = 0; nCurrentFile < nFiles && parser.size() >= 8; nCurrentFile++)
		{
			uint32_t nFilenameSize = parser.parse<uint32_t>();
			if (parser.size() < static_cast<uint64_t>(nFilenameSize) + 4) return; // Error
			std::string sFilename = parser.parse(nFilenameSize);

			uint32_t nFileContentSize = parser.parse<uint32_t>();
			if (parser.size() < nFileContentSize) return; // Error
			std::string sFileContent = parser.parse(nFileContentSize);

			addFile(std::move(sFilename), std::move(sFileContent));
		}
	}
}

void VirtualFilesystem::saveFiles() const noexcept
{
	std::ofstream file = std::ofstream(m_sFilename);

	if (file.is_open())
	{
		Writer writer;
		writer.write<uint32_t>(m_mapFiles.size());

		for (const auto &entry : m_mapFiles)
		{
			writer.write<uint32_t>(entry.first.size());
			writer.write_string(entry.first);
			writer.write<uint32_t>(entry.second.size());
			writer.write_string(entry.second);
		}

		file << writer.getContent();
		file.flush();
		file.close();
	}
}

std::string &VirtualFilesystem::getFile(const std::string &name) noexcept
{
	return m_mapFiles.at(name);
}

std::string VirtualFilesystem::getFile(const std::string &name) const noexcept
{
	return m_mapFiles.at(name);
}

void VirtualFilesystem::addFile(const std::string &name, const std::string &sContent) noexcept
{
	m_mapFiles.insert({ name, sContent });
}

void VirtualFilesystem::addFile(const std::string &name, std::string &&sContent) noexcept
{
	m_mapFiles.emplace(name, std::move(sContent));
}

void VirtualFilesystem::addFile(std::string &&name, std::string &&sContent) noexcept
{
	m_mapFiles.emplace(std::move(name), std::move(sContent));
}

void VirtualFilesystem::setFilename(const std::string &sFilename) noexcept
{
	m_sFilename = sFilename;
}

std::map<std::string, std::string>::iterator VirtualFilesystem::begin() noexcept
{
	return m_mapFiles.begin();
}

std::map<std::string, std::string>::const_iterator VirtualFilesystem::begin() const noexcept
{
	return m_mapFiles.begin();
}

std::map<std::string, std::string>::iterator VirtualFilesystem::end() noexcept
{
	return m_mapFiles.end();
}

std::map<std::string, std::string>::const_iterator VirtualFilesystem::end() const noexcept
{
	return m_mapFiles.end();
}
