// ============================================================================
//  mbCmd_XML
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbException.h"
#include "mbVariables.h"
#include "mbSandbox.h"
#include "mbUtil.h"
#include "mbXmlNodes.h"
#include "mbCmdXml.h"


// ============================================================================
//  mbCmdXml
// ============================================================================

mbCmdXml::mbCmdXml() : m_xmlDoc(NULL)
{
}

mbCmdXml::~mbCmdXml()
{
	if( m_xmlDoc )
	{
		delete m_xmlDoc;
		m_xmlDoc = NULL;
	}
}

TiXmlDocument* mbCmdXml::GetXmlDocument(const wxString& file)
{
	if( m_xmlDoc && (m_xmlFile.IsSameAs(file,false) || file.empty()) )
	{
		return m_xmlDoc;
	}
	else
	{
		return NULL;
	}
}

TiXmlDocument* mbCmdXml::LoadDocument(mbCommands& stack, const wxString& file, bool bForWriting)
{
	// Find in stack matching xmlDocument if exists
	for( mbCommands::reverse_iterator it=stack.rbegin(); it!=stack.rend(); ++it )
	{
		mbCmd* cmd = *it;
		if( cmd != this && wxString(wxT("xmlDocument")).IsSameAs(cmd->GetName()) )
		{
			mbCmdXml* cmdXml = (mbCmdXml*)cmd;

			TiXmlDocument* xmlDoc = cmdXml->GetXmlDocument(file);
			if( xmlDoc )
				return xmlDoc;
		}
	}

	// Otherwise open it yourself
	if( file.empty() )
		ThrowError(wxT("No file specified!"));

	PrintResult(stack, wxT("Loading: %s"), file.c_str());

	GmbSandbox().Validate(file);

	// Load file
	m_xmlFile = file;
	m_xmlDoc = new TiXmlDocument();
	if( !m_xmlDoc->LoadFile(file.mb_str()) )
		ThrowError(wxT("File could not be opened:\n%s\n%s"), file.c_str(), mbGetXMLError(*m_xmlDoc).c_str());

	return m_xmlDoc;
}

void mbCmdXml::SaveDocument(mbCommands& stack, const wxString& file, TiXmlDocument* xmlDoc)
{
	if( !xmlDoc )
		ThrowError(wxT("Invalid xml document object!"));

	// If we opened it then we save and delete it
	if( m_xmlDoc )
	{
		if( m_xmlDoc != xmlDoc )
			ThrowError(wxT("XmlDoc object mismatch!"));

		if( m_xmlFile != file )
			ThrowError(wxT("XmlDoc file mismatch!"));

		if( file.empty() )
			ThrowError(wxT("No file specified!"));

		PrintResult(stack, wxT("Saving: %s"), file.c_str());

		if( !xmlDoc->SaveFile(file.mb_str()) )
			ThrowError(wxT("File could not be saved: %s"), file.c_str());

		delete m_xmlDoc;
		m_xmlDoc = NULL;
		m_xmlFile = wxEmptyString;
	}
	else
	{
		// Otherwise it doesn't belong to us and we shouldn't mess with it
	}
}

std::vector<TiXmlElement*> mbCmdXml::GetNodeSet(const wxString& file, const wxString& xpath, const wxString& xnode, const TiXmlDocument& xmlDoc)
{
	// Get XPath result as nodeset
	if( !xnode.empty() )
	{
		TiXmlElement* elem = NULL;
		if( !GmbXmlNodes().Get(xnode,elem) )
			ThrowError(wxT("XNode has no result! %s %s"), file.c_str(), xnode.c_str());

		//wxLogMessage(wxT("Found XNode: %s -> %s"), xnode.c_str(), mbGetXPathReadable(elem).c_str() );

		std::vector<TiXmlElement*> elements;
		elements.push_back(elem);
		return elements;
	}
	else
	{	
		return GetXPathNodeSet( file, xpath, xmlDoc );
	}
}


std::vector<TiXmlElement*> mbCmdXml::GetXPathNodeSet(const wxString& file, const wxString& xpath, const TiXmlDocument& xmlDoc)
{
	std::vector<TiXmlElement*> elements;

	// Compute XPath result
	wxStopWatch xpath_timer;
	TinyXPath::xpath_processor proc(xmlDoc.FirstChildElement(), xpath.ToUTF8());
	int nodecount = proc.u_compute_xpath_node_set();
	//wxLogMessage(wxT("XPath took %ldms - %s"), xpath_timer.Time(), xpath.c_str());
	if( nodecount == 0 )
		ThrowError(wxT("No result! %s %s"), file.c_str(), xpath.c_str());

	// Iterate XPath result
	for( int i=0; i<nodecount; ++i)
	{
		// Get XML base node
		const TiXmlBase* base = NULL;
		bool isattrib = false;
		proc.v_get_xpath_base(i, base, isattrib);
		if( isattrib )
			ThrowError(wxT("XPath result contains an attribute!\n%s\n%s"), file.c_str(), xpath.c_str());

		// Get XML element
		TiXmlElement* elem = ((TiXmlNode*)base)->ToElement();
		if( !elem )
			ThrowError(wxT("XPath result is not an element!\n%s\n%s"), file.c_str(), xpath.c_str());
		else
			elements.push_back(elem);
	}

	return elements;
}

std::vector<TiXmlAttribute*> mbCmdXml::GetXPathAttributes(const wxString& file, const wxString& xpath, const TiXmlDocument& xmlDoc)
{
	std::vector<TiXmlAttribute*> attribs;

	// Compute XPath result
	TinyXPath::xpath_processor proc(xmlDoc.FirstChildElement(), xpath.ToUTF8() );
	int nodecount = proc.u_compute_xpath_node_set();
	if( nodecount == 0 )
		ThrowError(wxT("XPath result is not a node set!\n%s\n%s"), file.c_str(), xpath.c_str());

	// Iterate XPath result
	for( int i=0; i<nodecount; ++i)
	{
		// Get XML base node
		const TiXmlBase* base = NULL;
		bool isattrib = false;
		proc.v_get_xpath_base(i, base, isattrib);
		if( !isattrib )
			ThrowError(wxT("XPath result contains an element!\n%s\n%s"), file.c_str(), xpath.c_str());

		// Get XML element
		TiXmlAttribute* attrib = ((TiXmlAttribute*)base);
		if( !attrib )
			ThrowError(wxT("XPath result is not an attribute!\n%s\n%s"), file.c_str(), xpath.c_str());
		else
			attribs.push_back(attrib);
	}

	return attribs;
}

wxString mbCmdXml::GetXPathString(const wxString& WXUNUSED(file), const wxString& xpath, const TiXmlDocument& xmlDoc)
{
	wxString result;

	// Compute XPath result
	TinyXPath::xpath_processor proc(xmlDoc.FirstChildElement(), xpath.ToUTF8() );
	result = wxString::FromUTF8(proc.S_compute_xpath().c_str());

	return result;
}

// ============================================================================
//  EOF
// ============================================================================