// ============================================================================
//  mbCmdXml
// ============================================================================
#pragma once

#include "mbCmd.h"

// ============================================================================
//	mbCmdXml
// ============================================================================

class mbCmdXml : public mbCmd 
{
public: 
	mbCmdXml();
	~mbCmdXml();
	static mbCmd* Create() {mbCmdXml* cmd = new mbCmdXml; cmd->Init(); return cmd;}
	virtual wxString GetName() const {return L"Xml";} 

public: 
	TiXmlDocument* LoadDocument(mbCommands& stack, const wxString& file, bool bForWriting);
	void SaveDocument(mbCommands& stack, const wxString& file, TiXmlDocument* xmlDoc);

public: 

	std::vector<TiXmlElement*> GetNodeSet(const wxString& file, const wxString& xpath, const wxString& xnode, const TiXmlDocument& xmlDoc);
	std::vector<TiXmlElement*> GetXPathNodeSet(const wxString& file, const wxString& xpath, const TiXmlDocument& xmlDoc);
	std::vector<TiXmlAttribute*> GetXPathAttributes(const wxString& file, const wxString& xpath, const TiXmlDocument& xmlDoc);
	wxString GetXPathString(const wxString& file, const wxString& xpath, const TiXmlDocument& xmlDoc);

public: 
	TiXmlDocument* GetXmlDocument(const wxString& file);

protected:
	wxString m_xmlFile;
	TiXmlDocument* m_xmlDoc;
};


// ============================================================================
//  EOF
// ============================================================================