// ============================================================================
//  mbFileSystemTraverser
// ============================================================================
#pragma once


// ============================================================================
//  mbFileSystemTraverser
// ============================================================================
class mbFileSystemTraverser : public wxDirTraverser
{
public:
	mbFileSystemTraverser(bool reverse=false) : m_reverse(reverse), m_index(0)
	{
	}

public:
	operator bool() const 
	{ 
		return m_index >= 0 && m_index < m_items.size(); 
	}

	mbFileSystemTraverser& operator++() 
	{ 
		if( m_reverse )
			--m_index; 
		else
			++m_index; 
		return *this; 
	}

	wxFileName FN() const 
	{ 
		return wxFileName(m_items[m_index]); 
	}

	void InitReverse()
	{
		if( m_reverse )
			m_index = m_items.size()-1;
	}

protected:
	virtual wxDirTraverseResult OnFile(const wxString& name);
	virtual wxDirTraverseResult OnDir(const wxString& name);
	virtual wxDirTraverseResult OnOpenError(const wxString& name);

protected:
	bool m_reverse;
	size_t m_index;
	wxArrayString m_items;
};


// ============================================================================
//  mbDirTraverser
// ============================================================================
class mbDirTraverser : public mbFileSystemTraverser
{
public:
	mbDirTraverser(bool reverse=false) : mbFileSystemTraverser(reverse){}
	static mbDirTraverser Init(const wxString& dirName, const wxString& filespec = wxEmptyString, int flags = wxDIR_DEFAULT, bool reverse=false);
	wxString GetPath() const;

protected:
	virtual wxDirTraverseResult OnDir(const wxString& name);
};


// ============================================================================
//  mbFileTraverser
// ============================================================================
class mbFileTraverser : public mbFileSystemTraverser
{
public:
	mbFileTraverser(bool reverse=false) : mbFileSystemTraverser(reverse){}
	static mbFileTraverser Init(const wxString& dirName, const wxString& filespec = wxEmptyString, int flags = wxDIR_DEFAULT, bool reverse=false);

	wxString GetFullPath() const;
	wxString GetFullName() const;
	wxString GetName() const;

protected:
	virtual wxDirTraverseResult OnFile(const wxString& name);
};


// ============================================================================
//  EOF
// ============================================================================