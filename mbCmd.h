// ============================================================================
//  mbCmd
// ============================================================================
#pragma once

// ============================================================================
//	Forward declarations
// ============================================================================
class mbCmd;


// ============================================================================
//	mbPropPair
// ============================================================================
struct mbPropPair
{
	wxString first;
	wxString second;

	mbPropPair( const wxString& a, const wxString& b ) : first(a), second(b) {}
};


// ============================================================================
//	typedefs
// ============================================================================
typedef std::vector<mbPropPair> mbCmdPropertiesMap;
typedef std::vector<mbCmd*> mbCommands;


// ============================================================================
//	mbCmd
// ============================================================================
class mbCmd
{
public:
	virtual ~mbCmd();

protected:
	mbCmd();
	void Init();

public:
	virtual void Execute(mbCommands& stack);
	virtual wxString GetName() const {return wxT("Cmd");}
	virtual void Load(mbCommands& stack, TiXmlElement* elem);

public:
	mbCmd* SetProp(const wxString& name, const wxString& value);
	wxString GetProp(const wxString& name, bool allowEmpty=false);
	wxString ParseProp(const wxString& name, bool allowEmpty=false, const wxString& default=wxEmptyString);
	bool ParseBool(const wxString& name, bool allowEmpty, const wxString& default=wxEmptyString);

public:
	wxString PrintProps(const wxString& separator=wxEmptyString);
	wxString PrintPropsParsed(const wxString& separator=wxEmptyString);
	void PrintExecute(mbCommands& stack, const wxChar *pszFormat, ...);
	void PrintResult(mbCommands& stack, const wxChar *pszFormat, ...);
	void PrintWarning(mbCommands& stack, const wxChar *pszFormat, ...);
	void ThrowError(const wxChar *pszFormat, ...);

public:
	void ExecuteChildren(mbCommands& stack);
	void LoadChildren(mbCommands& stack, TiXmlElement* elem);
	mbCmd* AddCommand(mbCmd* cmd);
	wxString GetDebugId(mbCommands* stack=NULL);

public:
	static void StaticExecute(mbCommands& stack, mbCmd* cmd);
	static void StaticLoad(mbCommands& stack, mbCmd* cmd, TiXmlElement* elem);

public:
	mbCommands m_commands;
	mbCmdPropertiesMap m_properties;

protected:
	int m_xmlRow;
	int m_xmlColumn;
};



// ============================================================================
//	mbCmd macros
// ============================================================================

#define MB_CMD_IMPLEMENT(parent, name) \
class mbCmd##name : public parent { \
public: \
	static mbCmd* Create() {mbCmd##name* cmd = new mbCmd##name; cmd->Init(); return cmd;}\
	virtual wxString GetName() const {return L#name;} \
	virtual void Execute(mbCommands& stack); \
	virtual void Load(mbCommands& stack, TiXmlElement* elem); \
};

#define MB_CMD_IMPLEMENT_BASIC(parent, name) \
class mbCmd##name : public parent { \
public: \
	static mbCmd* Create() {mbCmd##name* cmd = new mbCmd##name; cmd->Init(); return cmd;}\
	virtual wxString GetName() const {return L#name;} \
};

#define MB_CMD_DEFAULT_INIT(name)

#define MB_CMD_DEFAULT_EXECUTE(name) \
	void mbCmd##name::Execute(mbCommands& stack) { ExecuteChildren(stack); }

#define MB_CMD_DEFAULT_LOAD(name) \
	void mbCmd##name::Load(mbCommands& stack, TiXmlElement* elem) { LoadChildren(stack,elem); }


#define MB_CMD_GUARD_OPT bool optional = ParseBool(wxT("Optional"), true); \
	try { 

#define MB_CMD_UNGUARD_OPT } \
	catch( mbException& e ) { \
		if( optional ) { \
			if( !ParseBool(wxT("Quiet"), true) ) {PrintWarning(stack, wxT("%s"), e.What().c_str());} \
			return; \
		} else \
			throw e; \
}

#define MB_CMD_UNGUARD_OPT_NORET } \
	catch( mbException& e ) { \
		if( optional ) { \
			if( !ParseBool(wxT("Quiet"), true) ) {PrintWarning(stack, wxT("%s"), e.What().c_str());} \
		} else \
			throw e; \
}


// ============================================================================
//  EOF
// ============================================================================