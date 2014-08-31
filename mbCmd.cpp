// ============================================================================
//  mbCmd
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd.h"
#include "mbUtil.h"
#include "mbCmdFactory.h"
#include "mbException.h"
#include "mbVarParser.h"




// ============================================================================
//	mbStackPusher
// ============================================================================
class mbStackPusher
{
public:
	mbStackPusher(mbCommands& stack, mbCmd* cmd) : m_stack(stack), m_cmd(cmd)
	{
		// Push stack
		m_stack.push_back(m_cmd);
	}

	~mbStackPusher()
	{
		// Pop stack
		if( m_stack.back() == m_cmd )
			m_stack.pop_back();
		else
			throw mbException(wxT("Stack corruption detected!\nStack: %s"), mbPrintStackDetailed(m_stack).c_str());
	}

private:
	mbCommands& m_stack;
	mbCmd* m_cmd;
};


// ============================================================================
//	ModBuildCmd
// ============================================================================

mbCmd::mbCmd() : m_xmlRow(0), m_xmlColumn(0)
{
}

mbCmd::~mbCmd()
{

}

void mbCmd::Init()
{
}

void mbCmd::Execute(mbCommands& WXUNUSED(stack))
{
	ThrowError(wxT("Execute() not implemented!"));
}

void mbCmd::Load(mbCommands& WXUNUSED(stack), TiXmlElement* WXUNUSED(elem))
{
	ThrowError(wxT("Load() not implemented!"));
}

void mbCmd::ExecuteChildren(mbCommands& stack)
{
	// execute subcommands
	for( mbCommands::iterator it=m_commands.begin(); it!=m_commands.end(); ++it )
	{
		mbCmd::StaticExecute(stack, *it);
	}
}

void mbCmd::LoadChildren(mbCommands& stack, TiXmlElement* elem)
{
	// Iterate XML child nodes
	for( TiXmlNode* node = elem->FirstChild(); node; node=node->NextSibling() )
	{
		// Get XML element
		TiXmlElement* childElem = node->ToElement();
		if( childElem )
		{
			// Create command
			mbCmd* cmd = GmbCmdFactory().Get( wxString::FromUTF8(childElem->Value()) );

			// Add command as child
			AddCommand(cmd);

			// Load command 
			mbCmd::StaticLoad(stack, cmd, childElem);
		}
		else
		{
			// If node is a comment create Comment element
			TiXmlComment* childComment = node->ToComment();
			if( childComment )
			{
				// Do not load comment if commented out
				if( !wxString(wxT("//")).IsSameAs(wxString::FromUTF8(childComment->Value()).Left(2)) )
				{
					childElem = elem->InsertAfterChild(node, TiXmlElement(wxString(wxT("Comment")).ToUTF8()))->ToElement();
					childElem->InsertEndChild(TiXmlText(childComment->Value()));
				}
			}
		}
	}
}

void mbCmd::StaticLoad(mbCommands& stack, mbCmd* cmd, TiXmlElement* elem)
{
	// Push stack using object
	mbStackPusher stackPusher(stack, cmd);

	// Load position in XML file
	cmd->m_xmlRow = elem->Row();
	cmd->m_xmlColumn = elem->Column();

	// Load attributes
	for( TiXmlAttribute* atrr = elem->FirstAttribute(); atrr; atrr=atrr->Next() )
	{
		cmd->SetProp( wxString::FromUTF8(atrr->Name()), wxString::FromUTF8(atrr->Value()) );
	}

	// Load inner text
	wxString innerText = wxString::FromUTF8(elem->GetText());
	if( innerText != wxEmptyString )
		cmd->SetProp( wxT("InnerText"), innerText );

	// Print stack
	wxLogMessage(wxT("Load: %s %s"), mbPrintStackSymbolic(stack).c_str(), cmd->PrintProps().c_str() );

	// Execute command
	cmd->Load(stack, elem);
}

void mbCmd::StaticExecute(mbCommands& stack, mbCmd* cmd)
{
	// Push stack using object
	mbStackPusher stackPusher(stack, cmd);

	// Print stack
	//wxLogMessage(wxT("Exec: %s %s"), mbPrintStack(stack, true).c_str(), cmd->PrintProps().c_str() );

	// Execute command
	cmd->Execute(stack);
}

wxString mbCmd::PrintProps(const wxString& separator)
{
	wxString result;
	for( mbCmdPropertiesMap::iterator it=m_properties.begin(); it!=m_properties.end(); ++it )
	{
		result += (*it).first;
		result += wxT("=\"");
		result += (*it).second;
		result += wxT("\"");
		if( separator != wxEmptyString )
			result += separator;
		else
			result +=  wxT(" ");
	}
	return result;
}

wxString mbCmd::PrintPropsParsed(const wxString& separator)
{
	wxString result;
	for( mbCmdPropertiesMap::iterator it=m_properties.begin(); it!=m_properties.end(); ++it )
	{
		result += (*it).first;
		result += wxT("=\"");
		result += ParseProp((*it).first, true);
		result += wxT("\"");
		if( separator != wxEmptyString )
			result += separator;
		else
			result +=  wxT(" ");
	}
	return result;
}

mbCmd* mbCmd::SetProp(const wxString& name, const wxString& value)
{
	for( mbCmdPropertiesMap::iterator it=m_properties.begin(); it!=m_properties.end(); ++it )
	{
		if( (*it).first.IsSameAs(name) )
		{
			(*it).second = value;
			return this;
		}
	}
	m_properties.push_back(mbPropPair(name,value));
	return this;
}

wxString mbCmd::GetProp(const wxString& name, bool allowEmpty)
{
	for( mbCmdPropertiesMap::iterator it=m_properties.begin(); it!=m_properties.end(); ++it )
	{
		if( (*it).first.IsSameAs(name) )
		{
			if( !allowEmpty && (*it).second.empty() )
				ThrowError(wxT("Empty property %s!"), name.c_str());
			else
				return (*it).second;
		}
	}

	if( !allowEmpty )
		ThrowError(wxT("Unknown property %s!"), name.c_str());

	return wxEmptyString;
}

wxString mbCmd::ParseProp(const wxString& name, bool allowEmpty, const wxString& default)
{
	// get property
	wxString str = GetProp(name,allowEmpty);
	
	// substitute variables
	mbVarParser parser;
	parser.Parse(str);

	// remove special characters
	str.Replace(wxT("\\["), wxT("["), true);
	str.Replace(wxT("\\]"), wxT("]"), true);

	if(str.empty())
		str = default;

	return str;
}

bool mbCmd::ParseBool(const wxString& name, bool allowEmpty, const wxString& default)
{
	return wxString(wxT("yes")).IsSameAs( ParseProp(name, allowEmpty, default), false );
}

mbCmd* mbCmd::AddCommand(mbCmd* cmd)
{
	m_commands.push_back(cmd);
	return cmd;
}

wxString mbCmd::GetDebugId(mbCommands* stack)
{
	int stack_size = stack ? stack->size() : -1;
	return wxString::Format(wxT("Ln %d, Col %d, %s %s [%d %p]"), m_xmlRow, m_xmlColumn, GetName().c_str(), PrintProps().c_str(), stack_size, this);
}

void mbCmd::PrintExecute(mbCommands& stack, const wxChar *pszFormat, ...)
{
	va_list argptr;
	va_start(argptr, pszFormat);
	wxString s;
	s.PrintfV(pszFormat, argptr);
	va_end(argptr);

	wxLogMessage(wxT("%s: %s"), mbPrintStackSymbolic(stack).c_str(), s.c_str());
}

void mbCmd::PrintWarning(mbCommands& WXUNUSED(stack), const wxChar *pszFormat, ...)
{
	va_list argptr;
	va_start(argptr, pszFormat);
	wxString s;
	s.PrintfV(pszFormat, argptr);
	va_end(argptr);

	wxLogMessage(wxT(""));
	wxLogMessage(wxT(">>>>>>>>>>>>>>>>>>>>>>>>>>>> ! WARNING ! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"));
	wxLogMessage(wxString(wxT(" ")) + s);
	wxLogMessage(wxT("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"));
	wxLogMessage(wxT(""));
}

void mbCmd::PrintResult(mbCommands& stack, const wxChar *pszFormat, ...)
{
	va_list argptr;
	va_start(argptr, pszFormat);
	wxString s;
	s.PrintfV(pszFormat, argptr);
	va_end(argptr);

	wxLogMessage(wxT("%s %s"), mbPrintStackResult(stack).c_str(), s.c_str());
}

void mbCmd::ThrowError(const wxChar *pszFormat, ...)
{
	va_list argptr;
	va_start(argptr, pszFormat);

	wxString s;
	s.PrintfV(pszFormat, argptr);

	va_end(argptr);

	throw mbException(wxT("%s\n%s"), GetDebugId().c_str(), s.c_str() );
}



// ============================================================================
//  EOF
// ============================================================================