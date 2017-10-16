#include "binIO_t.h"
#include <string>
#include <string.h>


binIO_t::binIO_t()
{
	m_commaMode = NONE;
}


binIO_t::binIO_t(const string& _fileName, const string& _mode) : virtIO_t(_fileName, _mode)
{
	m_commaMode = NONE;
}


binIO_t::~binIO_t(){}


bool binIO_t::IsReadMode() const
{
	string mode = GetMode();
	
	if (!strcmp(mode.c_str(), "rb") || !strcmp(mode.c_str(), "r+b") || !strcmp(mode.c_str(), "w+b"))
	{
		return true;
	}
	
	return false;
}


bool binIO_t::IsWriteMode() const
{
	string mode = GetMode();
	
	if (!strcmp(mode.c_str(), "wb") || !strcmp(mode.c_str(), "r+b") || !strcmp(mode.c_str(), "w+b"))
	{
		return true;
	}
	
	return false;
}


binIO_t& binIO_t:: operator>> (char& _char)
{
	return MyRead(_char);
}


binIO_t& binIO_t:: operator<< (const char& _char)
{
	return MyWrite(_char);
}


binIO_t& binIO_t:: operator>> (int& _int)
{
	m_commaMode = READ;	
	
	return MyRead(_int);
}


binIO_t& binIO_t:: operator<< (const int& _int)
{	
	m_commaMode = WRITE;
	
	return MyWrite(_int);
}


binIO_t& binIO_t::operator, (int _len)
{
	if (0 == m_fp || NONE == m_commaMode)
	{
		return *this;
	}
	else if (WRITE == m_commaMode)
	{
		int result = fwrite(m_buf, _len, 1, m_fp);
		
		if (1 != result || ferror(m_fp))
		{
			m_commaMode = NONE;
			throw(result);
		}
	}
	else
	{
		int result = fread(m_buf, _len, 1, m_fp);
	
		if (1 != result || ferror(m_fp))
		{
			m_commaMode = NONE;
			throw(result);
		}
	}
	
	m_commaMode = NONE;
	
	return *this;
}


binIO_t& binIO_t::operator>> (void* _buf)
{
	if (0 == _buf)
	{
		return *this;
	}
	
	m_commaMode = READ;
	
	m_buf = _buf;
	
	return *this;
}


binIO_t& binIO_t::operator<< (const void* _buf)
{
	if (0 == _buf)
	{
		return *this;
	}
	
	m_commaMode = WRITE;
	
	m_buf = (void*)_buf;
	
	return *this;
}













