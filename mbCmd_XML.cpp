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
//	xmlAttributeSet Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(xmlAttributeSet);

void mbCmdxmlAttributeSet::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), true, GetName());
	wxString name = ParseProp(wxT("Name"));
	wxString value = ParseProp(wxT("Value"));
	wxString file = ParseProp(wxT("File"), true);
	wxString xnode = ParseProp(wxT("XNode"), true);
	wxString xpath = ParseProp(wxT("XPath"), !xnode.empty());
	bool quiet = ParseBool(wxT("Quiet"), true);

	// Display progress
	if( !quiet )
		PrintExecute(stack, wxT("%s"), PrintPropsParsed().c_str());

	// Load file
	TiXmlDocument* xmlDoc = LoadDocument(stack, file, true);

	// Get nodeset
	std::vector<TiXmlElement*> nodes = GetNodeSet(file, xpath, xnode, *xmlDoc);
	for( std::vector<TiXmlElement*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
	{
		TiXmlElement* elem = *it;

		// Set Attribute
		elem->SetAttribute(name.ToUTF8().data(), value.ToUTF8());

		// Update document
		SaveDocument(stack, file, xmlDoc);

		// Store XPath and pointer to element in local variable Id
		GmbVariables().PushLocal(GetDebugId(&stack), id, mbGetXPathReadable(elem) );
		GmbXmlNodes().PushLocal(GetDebugId(&stack), id, elem );

		// Execute children
		ExecuteChildren(stack);

		// Kill local variables	
		GmbXmlNodes().PopLocal(GetDebugId(&stack));	
		GmbVariables().PopLocal(GetDebugId(&stack));		
	}

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	xmlDocument Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(xmlDocument);

void mbCmdxmlDocument::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString file = ParseProp(wxT("File"));
	bool quiet = ParseBool(wxT("Quiet"), true);

	// Display progress
	if( !quiet )
		PrintExecute(stack, wxT("%s"), PrintPropsParsed().c_str());

	// Load file
	TiXmlDocument* xmlDoc = LoadDocument(stack, file, true);

	// Execute children
	ExecuteChildren(stack);

	// Update document
	SaveDocument(stack, file, xmlDoc);

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//	xmlElementAdd Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(xmlElementAdd);

void mbCmdxmlElementAdd::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), true, GetName());
	wxString name = ParseProp(wxT("Name"));
	wxString value = ParseProp(wxT("Value"), true);
	wxString file = ParseProp(wxT("File"), true);
	wxString xnode = ParseProp(wxT("XNode"), true);
	wxString xpath = ParseProp(wxT("XPath"), !xnode.empty());
	bool quiet = ParseBool(wxT("Quiet"), true);

	// Display progress
	if( !quiet )
		PrintExecute(stack, wxT("%s"), PrintPropsParsed().c_str());

	// Load file
	TiXmlDocument* xmlDoc = LoadDocument(stack, file, true);

	// Get nodeset
	std::vector<TiXmlElement*> nodes = GetNodeSet(file, xpath, xnode, *xmlDoc);
	for( std::vector<TiXmlElement*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
	{
		TiXmlElement* elem = *it;

		// Add child element
		TiXmlElement* child = elem->InsertEndChild(TiXmlElement(name.ToUTF8()))->ToElement();
		child->SetUserData(elem);
		if( !value.empty() )
			child->InsertEndChild(TiXmlText(value.ToUTF8()));

		// Update document
		SaveDocument(stack, file, xmlDoc);

		// Store XPath and pointer to element in local variable Id
		GmbVariables().PushLocal(GetDebugId(&stack), id, mbGetXPathReadable(child) );
		GmbXmlNodes().PushLocal(GetDebugId(&stack), id, child );

		// Execute children
		ExecuteChildren(stack);

		// Kill local variable
		GmbVariables().PopLocal(GetDebugId(&stack));		
		GmbXmlNodes().PopLocal(GetDebugId(&stack));	
	}

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//	xmlAttributeRemove Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(xmlAttributeRemove);

void mbCmdxmlAttributeRemove::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString file = ParseProp(wxT("File"), true);
	wxString xnode = ParseProp(wxT("XNode"), true);
	wxString xpath = ParseProp(wxT("XPath"), !xnode.empty());
	wxString name = ParseProp(wxT("Name"));
	bool quiet = ParseBool(wxT("Quiet"), true);

	// Display progress
	if( !quiet )
		PrintExecute(stack, wxT("%s"), PrintPropsParsed().c_str());

	// Load file
	TiXmlDocument* xmlDoc = LoadDocument(stack, file, true);

	// Get nodeset
	std::vector<TiXmlElement*> nodes = GetNodeSet(file, xpath, xnode, *xmlDoc);
	for( std::vector<TiXmlElement*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
	{
		TiXmlElement* elem = *it;

		// Delete attribute
		elem->RemoveAttribute(name.ToUTF8());
	}

	// Display progress
	if( !quiet )
		PrintResult(stack, wxT("Removed attribute from %d elements"), (int)nodes.size());

	// Update document
	SaveDocument(stack, file, xmlDoc);

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//	xmlElementRemove Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(xmlElementRemove);

void mbCmdxmlElementRemove::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString file = ParseProp(wxT("File"), true);
	wxString xnode = ParseProp(wxT("XNode"), true);
	wxString xpath = ParseProp(wxT("XPath"), !xnode.empty());
	bool quiet = ParseBool(wxT("Quiet"), true);

	// Display progress
	if( !quiet )
		PrintExecute(stack, wxT("%s"), PrintPropsParsed().c_str());

	// Load file
	TiXmlDocument* xmlDoc = LoadDocument(stack, file, true);

	// Get nodeset
	std::vector<TiXmlElement*> nodes = GetNodeSet(file, xpath, xnode, *xmlDoc);
	for( std::vector<TiXmlElement*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
	{
		TiXmlElement* elem = *it;

		// Display progress
		//PrintResult(stack, wxT("Removing: %s"), mbGetXPathReadable(elem).c_str());

		// Delete all children
		elem->Clear();

		// Get parent
		TiXmlNode* parent = elem->Parent();
		if( !parent )
			ThrowError(wxT("Node has no parent!\n%s\n%s"), file.c_str(), mbGetXPathReadable(elem).c_str());

		// Delete element 
		parent->RemoveChild(elem);
	}

	// Display progress
	if( !quiet )
		PrintResult(stack, wxT("Removed %d elements"), (int)nodes.size());

	// Update document
	SaveDocument(stack, file, xmlDoc);

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	xmlForEach Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(xmlForEach);

void mbCmdxmlForEach::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"), true, GetName());
	wxString file = ParseProp(wxT("File"), true);
	wxString xnode = ParseProp(wxT("XNode"), true);
	wxString xpath = ParseProp(wxT("XPath"), !xnode.empty());
	bool quiet = ParseBool(wxT("Quiet"), true);

	// Display progress
	if( !quiet )
		PrintExecute(stack, wxT("%s"), PrintPropsParsed().c_str());

	// Load file
	TiXmlDocument* xmlDoc = LoadDocument(stack, file, true);

	// Get nodeset
	std::vector<TiXmlElement*> nodes = GetNodeSet(file, xpath, xnode, *xmlDoc);
	for( std::vector<TiXmlElement*>::iterator it=nodes.begin(); it!=nodes.end(); ++it)
	{
		TiXmlElement* elem = *it;

		// Store XPath and pointer to element in local variable Id
		GmbVariables().PushLocal(GetDebugId(&stack), id, mbGetXPathReadable(elem) );
		GmbXmlNodes().PushLocal(GetDebugId(&stack), id, elem );

		// Store element attributes in local variables
		for( TiXmlAttribute* atrr = elem->FirstAttribute(); atrr; atrr=atrr->Next() )
		{
			GmbVariables().PushLocal(GetDebugId(&stack), id + wxString(wxT("?")) + wxString::FromUTF8(atrr->Name()), wxString::FromUTF8(atrr->Value()) );
		}

		// Store element inner text in local variable
		GmbVariables().PushLocal(GetDebugId(&stack), id + wxString(wxT("?InnerText")), wxString::FromUTF8(elem->GetText()) );


		// Execute children
		ExecuteChildren(stack);

		// Kill local variable
		GmbVariables().PopLocal(GetDebugId(&stack));		
		GmbXmlNodes().PopLocal(GetDebugId(&stack));	
	}

	MB_CMD_UNGUARD_OPT;
}


// ============================================================================
//  EOF
// ============================================================================