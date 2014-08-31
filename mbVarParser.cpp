// ============================================================================
//  mbVarParser
// ============================================================================

#include "mbPrecompile.h"
#include "mbVariables.h"
#include "mbVarParser.h"


// ============================================================================
//	mbVarParser
// ============================================================================
mbVarParser::mbVarParser()
{
}


bool mbVarParser::Parse( wxString& text )
{
	// substitute variables in string with variable values
	// iterative, doesn't support nested variable definitions

	// find first opening bracket
	size_t lpos = text.find_first_of(wxT("["), 0);
	if( lpos != std::string::npos )
	{
		// find first closing bracket
		size_t rpos = text.find_first_of(wxT("]"), lpos);
		if( rpos != std::string::npos )
		{
			// find opening bracket in between
			size_t cpos = text.find_first_of(wxT("["), lpos+1);
			if( cpos != std::string::npos && cpos < rpos )
			{
				// if exists ignore first bracket

				wxString more = text.Mid(cpos);
				Parse(more);
				text = text.Left(cpos) + more;
				return true;
			}


			// try to parse what's between
			wxString fragment = text.Mid(lpos+1,rpos-lpos-1);
			wxString more = text.Mid(rpos+1);
			wxString value;
			if( GmbVariables().Parse( fragment, value) )
			{
				//wxLogMessage(wxT("Parsed [%s] to [%s]"), fragment.c_str(), value.c_str());
				text = text.Left(lpos) + value;
			}
			else
			{
				text = text.Left(rpos+1);
			}

			// if there is more, parse it again
			if( !more.empty() )
			{
				Parse(more);
			}

			// concatenate results and return
			text += more;
			return true;
		}
	}

	return false;
}

// ============================================================================
//  EOF
// ============================================================================