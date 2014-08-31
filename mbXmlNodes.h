// ============================================================================
//  mbXmlNodes
// ============================================================================
#pragma once




// ============================================================================
//	mbXmlNodeContainer
// ============================================================================
class mbXmlNodeContainer
{
private:
	WX_DECLARE_STRING_HASH_MAP( TiXmlElement*, mbXmlNodeMap );
	mbXmlNodeMap m_map;
	wxString m_id;

public:
	mbXmlNodeContainer(const wxString& id);
	wxString GetId() const { return m_id; }

public:
	void Set( const wxString& name, TiXmlElement* value );
	bool Get( const wxString& name, TiXmlElement*& value );
	bool Parse( const wxString& name, TiXmlElement*& value );

};


// ============================================================================
//	mbXmlNodes 
// ============================================================================
class mbXmlNodes
{
private:
	typedef std::vector<mbXmlNodeContainer> mbVariablesStack;
	mbVariablesStack m_stack;

public:
	mbXmlNodes();
	static mbXmlNodes& Global();

public:
	void PushContainer(const wxString& id);
	void PopContainer(const wxString& id);
	wxString PrintStack() const;

public:
	void SetGlobal( const wxString& name, TiXmlElement* value );
	bool GetGlobal( const wxString& name, TiXmlElement*& value );
	bool ParseGlobal( const wxString& name, TiXmlElement*& value );

	bool Get( const wxString& name, TiXmlElement*& value );
	bool Parse( const wxString& name, TiXmlElement*& value );

	void PushLocal( const wxString& id, const wxString& name, TiXmlElement* value );
	void PopLocal( const wxString& id );
};


// ============================================================================
//	mbXmlNodes global
// ============================================================================

inline mbXmlNodes& GmbXmlNodes(){
	return mbXmlNodes::Global();
}


// ============================================================================
//  EOF
// ============================================================================