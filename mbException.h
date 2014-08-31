// ============================================================================
//  mbException
// ============================================================================
#pragma once


class mbException
{
public: 
	mbException( const wxChar *pszFormat, ... )
	{
		va_list argptr;
		va_start(argptr, pszFormat);
		m_errorMsg.PrintfV(pszFormat, argptr);
		va_end(argptr);
	}

	mbException( const mbException& rhs ) : m_errorMsg(rhs.m_errorMsg)
	{
	}

	virtual ~mbException()
	{
	}

	mbException& operator=( const mbException& rhs )
	{
		m_errorMsg = rhs.m_errorMsg; 
		return *this; 
	}

	wxString What() const
	{
		return m_errorMsg;
	}

private:
	wxString m_errorMsg;
};


// ============================================================================
//  EOF
// ============================================================================