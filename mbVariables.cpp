// ============================================================================
//  mbVariables
// ============================================================================

#include "mbPrecompile.h"
#include "mbVariables.h"
#include "mbException.h"


// ============================================================================
//	mbVariables
// ============================================================================
mbVariables::mbVariables()
{
	PushContainer(wxT("Global"));
}

void mbVariables::PushContainer( const wxString& id )
{
	m_stack.push_back(mbVariableContainer(id));
}

void mbVariables::PopContainer( const wxString& id )
{
	if( m_stack.back().GetId().IsSameAs(id) )
		m_stack.pop_back();
	else
		throw mbException( wxT("PopContainer stack corruption!\nRequested: %s\nGot: %s"), id.c_str(), PrintStack().c_str() );
}

wxString mbVariables::PrintStack() const
{
	wxString result;
	for( mbVariablesStack::const_iterator it=m_stack.begin(); it!=m_stack.end(); ++it )
	{
		if( it != m_stack.begin() )
			result += wxString(L".");

		result += (*it).GetId();
	}

	return wxEmptyString;
}

// ============================================================================
//	mbVariables - local variables
// ============================================================================
void mbVariables::PushLocal( const wxString& id, const wxString& name, const wxString& value )
{
	if( !m_stack.back().GetId().IsSameAs(id) )
		PushContainer(id);

	m_stack.back().Set(name, value);
}

void mbVariables::PopLocal( const wxString& id )
{
	PopContainer(id);
}


// ============================================================================
//	mbVariables - global variables
// ============================================================================
void mbVariables::SetGlobal( const wxString& name, const wxString& value )
{
	m_stack.front().Set(name, value);
}

bool mbVariables::GetGlobal( const wxString& name, wxString& value )
{
	return m_stack.front().Get(name, value);
}

bool mbVariables::ParseGlobal( const wxString& name, wxString& value )
{
	return m_stack.front().Parse(name, value);
}


// ============================================================================
//	mbVariables - global and local variables
// ============================================================================
bool mbVariables::Get( const wxString& name, wxString& value )
{
	for( mbVariablesStack::reverse_iterator it=m_stack.rbegin(); it!=m_stack.rend(); ++it )
	{
		if( (*it).Get(name, value) )
			return true;
	}
	return false;
}

bool mbVariables::Parse( const wxString& name, wxString& value )
{
	for( mbVariablesStack::reverse_iterator it=m_stack.rbegin(); it!=m_stack.rend(); ++it )
	{
		if( (*it).Parse(name, value) )
			return true;
	}
	return false;
}


// ============================================================================
//	mbVariables - Static
// ============================================================================
mbVariables& mbVariables::Global()
{
	// Create on first use
	static mbVariables p;
	return p;
}


// ============================================================================
//	mbVariableContainer
// ============================================================================
mbVariableContainer::mbVariableContainer( const wxString& id ) : m_id(id)
{
}

void mbVariableContainer::Set( const wxString& name, const wxString& value )
{
	m_map[name] = value;
}

bool mbVariableContainer::Get( const wxString& name, wxString& value )
{
	mbVariableMap::iterator it = m_map.find(name);
	if( it != m_map.end() )
	{
		value = (*it).second;
		return true;
	}

	return false;
}

bool mbVariableContainer::Parse( const wxString& name, wxString& value )
{
	mbVariableMap::iterator it = m_map.find(name);
	if( it != m_map.end() )
	{
		value = (*it).second;
		return true;
	}

	return false;
}


// ============================================================================
//  EOF
// ============================================================================