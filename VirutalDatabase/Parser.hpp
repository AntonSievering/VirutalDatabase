#pragma once

#include <string>
#include "Serializable.hpp"


class ParseResult
{
	friend class Parser;

	std::string *m_pData   = nullptr;
	std::size_t  m_nOffset = 0;
	std::size_t  m_nSize   = 0;
	
private:
	ParseResult() noexcept = default;
	ParseResult(std::string *data, const std::size_t nOffset, const std::size_t nSize) noexcept;

public:
	std::string as_string() noexcept;
	operator std::string() noexcept;

	// exponent not supported
	double as_fixed_fp_number() noexcept;
	operator double() noexcept;

	uint64_t parse_as_uint64_t() noexcept;
	operator uint64_t() noexcept;

	bool operator==(const std::string &rhs) noexcept
	{
		if (rhs.size() != m_nSize) return false;
		for (std::size_t i = 0; i < m_nSize; i++)
			if (rhs.at(i) != m_pData->at(i + m_nOffset))
				return false;
		return true;
	}
};


class Parser
{
private:
	std::string m_sData{};
	std::size_t m_nOffset = 0;

public:
	Parser() noexcept = default;
	Parser(const std::string &str) noexcept;

private:
	char get_char(const std::size_t index) noexcept;

public:
	void setContent(const std::string &str) noexcept;
	const std::string &getContent() const noexcept;

	std::size_t size() const noexcept;
	std::size_t offset() const noexcept;
	std::size_t content_size() const noexcept;

public:
	template <class T>
	T parse() noexcept;

	ParseResult parse(uint32_t nBytes) noexcept;
	ParseResult parse_until(const char token, const bool bCufOffToken = true) noexcept;
	ParseResult get_remaining() noexcept;
};

template<class T>
inline T Parser::parse() noexcept
{
	Serializable<T> s{};

	for (std::size_t i = 0; i < s.size(); i++)
		s.data[i] = get_char(i);
	m_nOffset += s.size();

	return s.object;
}
