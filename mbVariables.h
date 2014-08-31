// ============================================================================
//  mbVariables
// ============================================================================
#pragma once




// ============================================================================
//	mbVariableContainer
// ============================================================================
class mbVariableContainer
{
private:
	WX_DECLARE_STRING_HASH_MAP( wxString, mbVariableMap );
	mbVariableMap m_map;
	wxString m_id;

public:
	mbVariableContainer(const wxString& id);
	wxString GetId() const { return m_id; }

public:
	void Set( const wxString& name, const wxString& value );
	bool Get( const wxString& name, wxString& value );
	bool Parse( const wxString& name, wxString& value );

};


// ============================================================================
//	mbVariables 
// ============================================================================
class mbVariables
{
private:
	typedef std::vector<mbVariableContainer> mbVariablesStack;
	mbVariablesStack m_stack;

public:
	mbVariables();
	static mbVariables& Global();

public:
	void PushContainer(const wxString& id);
	void PopContainer(const wxString& id);
	wxString PrintStack() const;

public:
	void SetGlobal( const wxString& name, const wxString& value );
	bool GetGlobal( const wxString& name, wxString& value );
	bool ParseGlobal( const wxString& name, wxString& value );

	bool Get( const wxString& name, wxString& value );
	bool Parse( const wxString& name, wxString& value );

	void PushLocal( const wxString& id, const wxString& name, const wxString& value );
	void PopLocal( const wxString& id );
};


// ============================================================================
//	mbVariables global
// ============================================================================

inline mbVariables& GmbVariables(){
	return mbVariables::Global();
}


// ============================================================================
//  EOF
// ============================================================================