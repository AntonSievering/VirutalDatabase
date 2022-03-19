#include "Parser.hpp"

ParseResult::ParseResult(std::string *data, const std::size_t nOffset, const std::size_t nSize) noexcept
{
	m_pData   = data;
	m_nOffset = nOffset;
	m_nSize   = nSize;
}

std::string ParseResult::as_string() noexcept
{
	return std::string(*m_pData, m_nOffset, m_nSize);
}

ParseResult::operator std::string() noexcept
{
	return as_string();
}

double ParseResult::as_fixed_fp_number() noexcept
{
	bool bFractionalPart = false;
	bool bNegative = false;
	double fNumber = 0.0;
	double e = 0.1;

	for (std::size_t i = 0; i < m_nSize; i++)
	{
		char c = m_pData->at(i + m_nOffset);

		if (i == 0 && c == '-')
		{
			bNegative = true;
		}
		else if (c == '.')
		{
			bFractionalPart = true;
		}
		else if ('0' <= c && c <= '9')
		{
			if (bFractionalPart)
			{
				fNumber += double(c - '0') * e;
				e *= 0.1;
			}
			else
			{
				fNumber = 10.0 * fNumber + double(c - '0');
			}
		}
	}

	return bNegative ? -fNumber : fNumber;
}

ParseResult::operator double() noexcept
{
	return as_fixed_fp_number();
}

uint64_t ParseResult::parse_as_uint64_t() noexcept
{
	uint64_t nNumber = 0;

	for (std::size_t i = 0; i < m_nSize; i++)
	{
		char c = m_pData->at(i + m_nOffset);

		if ('0' <= c && c <= '9')
			nNumber = nNumber * uint64_t(10) + uint64_t(c - '0');
	}

	return nNumber;
}

ParseResult::operator uint64_t() noexcept
{
	return parse_as_uint64_t();
}

Parser::Parser(const std::string &str) noexcept
{
	m_sData = str;
}

char Parser::get_char(const std::size_t index) noexcept
{
	return m_sData.at(index + m_nOffset);
}

void Parser::setContent(const std::string &str) noexcept
{
	m_sData = str;
	m_nOffset = 0;
}

const std::string &Parser::getContent() const noexcept
{
	return m_sData;
}

std::size_t Parser::size() const noexcept
{
	return content_size() - offset();
}

std::size_t Parser::offset() const noexcept
{
	return m_nOffset;
}

std::size_t Parser::content_size() const noexcept
{
	return m_sData.size();
}

ParseResult Parser::parse(uint32_t nBytes) noexcept
{
	uint64_t oldOffset = m_nOffset;
	m_nOffset += nBytes;
	return ParseResult(&m_sData, oldOffset, nBytes);
}

ParseResult Parser::parse_until(const char token, const bool bCufOffToken) noexcept
{
	std::size_t i;
	for (i = 0; i < size(); i++)
		if (get_char(i) == token) break;

	std::size_t ret_offset = m_nOffset;
	m_nOffset += std::min<uint64_t>(i + bCufOffToken, size());

	return ParseResult(&m_sData, ret_offset, i);
}

ParseResult Parser::get_remaining() noexcept
{
	return ParseResult(&m_sData, offset(), size());
}
