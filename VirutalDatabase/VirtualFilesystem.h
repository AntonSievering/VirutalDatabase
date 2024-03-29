#pragma once

#include <map>
#include <string>


class VirtualFilesystem
{
	std::map<std::string, std::string> m_mapFiles{};
	std::string m_sFilename{};

public:
	VirtualFilesystem() noexcept {}
	VirtualFilesystem(const std::string &sFilename) noexcept;
	~VirtualFilesystem() noexcept;

private:
	void loadFiles() noexcept;
	void saveFiles() const noexcept;

public:
	std::string &getFile(const std::string &name) noexcept;
	std::string getFile(const std::string &name) const noexcept;
	void addFile(const std::string &name, const std::string &sContent) noexcept;
	void addFile(const std::string &name, std::string &&sContent) noexcept;
	void addFile(std::string &&name, std::string &&sContent) noexcept;
	void setFilename(const std::string &sFilename) noexcept;

	std::map<std::string, std::string>::iterator begin() noexcept;
	std::map<std::string, std::string>::const_iterator begin() const noexcept;
	std::map<std::string, std::string>::iterator end() noexcept;
	std::map<std::string, std::string>::const_iterator end() const noexcept;
};
