// ============================================================================
//  mbUtil
// ============================================================================

#include "mbPrecompile.h"
#include "mbUtil.h"
#include "mbException.h"


// ============================================================================
//  File
// ============================================================================
wxFileName mbGetAppDir()
{
	return wxFileName::DirName(wxFileName(wxStandardPaths::Get().GetExecutablePath()).GetPath());
}


// ============================================================================
//  Logging
// ============================================================================
void mbLogHeader( const wxChar *pszFormat, ... )
{
	va_list argptr;
	va_start(argptr, pszFormat);

	wxString s;
	s.PrintfV(pszFormat, argptr);

	va_end(argptr);

	wxLogMessage(wxT(""));
	wxLogMessage(wxT("------------------------------------------------------------------------------"));
	wxLogMessage(wxString(wxT(" ")) + s);
	wxLogMessage(wxT("------------------------------------------------------------------------------"));
	wxLogMessage(wxT(""));
}

void mbLogError( const wxChar *pszFormat, ... )
{
	va_list argptr;
	va_start(argptr, pszFormat);

	wxString s;
	s.PrintfV(pszFormat, argptr);

	va_end(argptr);

	wxLogMessage(wxT(""));
	wxLogMessage(wxT(""));
	wxLogMessage(wxT("------------------------------------------------------------------------------"));
	wxLogMessage(wxString(wxT(" ")) + s);
	wxLogMessage(wxT("------------------------------------------------------------------------------"));
	wxLogMessage(wxT(""));
}


// ============================================================================
//  Printing
// ============================================================================
wxString mbPrintStack(const mbCommands& stack)
{
	wxString result;

	for( mbCommands::const_iterator it=stack.begin(); it!=stack.end(); ++it )
	{
		if( it != stack.begin() )
			result += wxString(L".");

		result += (*it)->GetName();
	}

	return result;
}


wxString mbPrintStackSymbolic(const mbCommands& stack)
{
	wxString result;

	for( mbCommands::const_iterator it=stack.begin(); it!=stack.end(); ++it )
	{
		result += wxT("-");
	}

	if( !stack.empty() )
	{
		result += wxT(" ");
		result += stack.back()->GetName();
	}

	return result;
}

wxString mbPrintStackResult(const mbCommands& stack)
{
	wxString result;

	for( mbCommands::const_iterator it=stack.begin(); it!=stack.end(); ++it )
	{
		result += wxT(" ");
	}

	return result;
}

wxString mbPrintStackDetailed(const mbCommands& stack)
{
	wxString result;

	for( mbCommands::const_iterator it=stack.begin(); it!=stack.end(); ++it )
	{
		if( it != stack.begin() )
			result += wxString(L"\n");

		result += wxT("\t");
		result += (*it)->GetName();
		result += wxT("\t");
		result += (*it)->PrintProps();
	}

	return result;
}

// ============================================================================
//  XML related
// ============================================================================
wxString mbGetXMLError(const TiXmlDocument& doc)
{
	return wxString::Format(wxT("Ln %d, Col %d, (%d) %s")
		, doc.ErrorRow(), doc.ErrorCol(), doc.ErrorId(), wxString::FromUTF8(doc.ErrorDesc()).c_str() );
}

wxString mbGetXPath(const TiXmlNode* node)
{
	wxString result;

	while(node)
	{
		const TiXmlNode* parent = node->Parent();
		if( parent )
		{
			unsigned long count = 1;
			for( const TiXmlNode* child = parent->FirstChildElement(); child; child=child->NextSiblingElement() )
			{
				if( child == node )
				{
					node = parent;
					result = wxString::Format(wxT("/*[%lu]"), count) + result;
					break;
				}
				++count;
			}

			if( node != parent )
				throw mbException(wxT("mbGetXPath stuck"));
		}
		else
		{
			return result;
		}
	}

	throw mbException(wxT("mbGetXPath no node"));
}

wxString mbGetXPathReadable(const TiXmlNode* node)
{
	wxString result;

	while(node)
	{
		const TiXmlNode* parent = node->Parent();
		if( parent )
		{
			unsigned long count = 1;
			for( const TiXmlNode* child = parent->FirstChildElement(node->Value()); child; child=child->NextSiblingElement(node->Value()) )
			{
				if( child == node )
				{
					node = parent;
					result = wxString::Format(wxT("/%s[%lu]"), wxString::FromUTF8(child->Value()).c_str(), count) + result;
					break;
				}
				++count;
			}

			if( node != parent )
				throw mbException(wxT("mbGetXPathReadable stuck"));
		}
		else
		{
			return result;
		}
	}

	throw mbException(wxT("mbGetXPathReadable no node"));
}

// ============================================================================
//  EOF
// ============================================================================