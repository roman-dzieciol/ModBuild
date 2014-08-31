// ============================================================================
//  mbXmlNodes
// ============================================================================

#include "mbPrecompile.h"
#include "mbXmlNodes.h"
#include "mbException.h"


// ============================================================================
//	mbXmlNodes
// ============================================================================
mbXmlNodes::mbXmlNodes()
{
	PushContainer(wxT("Global"));
}

void mbXmlNodes::PushContainer( const wxString& id )
{
	//wxLogMessage(wxT("PushContainer: %s"), id.c_str());
	m_stack.push_back(mbXmlNodeContainer(id));
}

void mbXmlNodes::PopContainer( const wxString& id )
{
	//wxLogMessage(wxT("PopContainer: %s"), id.c_str());
	if( m_stack.back().GetId().IsSameAs(id) )
		m_stack.pop_back();
	else
		throw mbException( wxT("PopContainer stack corruption!\nRequested: %s\nGot: %s"), id.c_str(), PrintStack().c_str() );
}

wxString mbXmlNodes::PrintStack() const
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
//	mbXmlNodes - local variables
// ============================================================================
void mbXmlNodes::PushLocal( const wxString& id, const wxString& name, TiXmlElement* value )
{
	if( !m_stack.back().GetId().IsSameAs(id) )
		PushContainer(id);

	//wxLogMessage(wxT("PushLocal: %s %p"), id.c_str(), value);
	m_stack.back().Set(name, value);
}

void mbXmlNodes::PopLocal( const wxString& id )
{
	//wxLogMessage(wxT("PopLocal: %s"), id.c_str());
	PopContainer(id);
}


// ============================================================================
//	mbXmlNodes - global variables
// ============================================================================
void mbXmlNodes::SetGlobal( const wxString& name, TiXmlElement* value )
{
	m_stack.front().Set(name, value);
}

bool mbXmlNodes::GetGlobal( const wxString& name, TiXmlElement*& value )
{
	return m_stack.front().Get(name, value);
}

bool mbXmlNodes::ParseGlobal( const wxString& name, TiXmlElement*& value )
{
	return m_stack.front().Parse(name, value);
}


// ============================================================================
//	mbXmlNodes - global and local variables
// ============================================================================
bool mbXmlNodes::Get( const wxString& name, TiXmlElement*& value )
{
	for( mbVariablesStack::reverse_iterator it=m_stack.rbegin(); it!=m_stack.rend(); ++it )
	{
		if( (*it).Get(name, value) )
			return true;
	}
	return false;
}

bool mbXmlNodes::Parse( const wxString& name, TiXmlElement*& value )
{
	for( mbVariablesStack::reverse_iterator it=m_stack.rbegin(); it!=m_stack.rend(); ++it )
	{
		if( (*it).Parse(name, value) )
			return true;
	}
	return false;
}


// ============================================================================
//	mbXmlNodes - Static
// ============================================================================
mbXmlNodes& mbXmlNodes::Global()
{
	// Create on first use
	static mbXmlNodes p;
	return p;
}


// ============================================================================
//	mbXmlNodeContainer
// ============================================================================
mbXmlNodeContainer::mbXmlNodeContainer( const wxString& id ) : m_id(id)
{
}

void mbXmlNodeContainer::Set( const wxString& name, TiXmlElement* value )
{
	m_map[name] = value;
}

bool mbXmlNodeContainer::Get( const wxString& name, TiXmlElement*& value )
{
	mbXmlNodeMap::iterator it = m_map.find(name);
	if( it != m_map.end() )
	{
		value = (*it).second;
		return true;
	}

	return false;
}

bool mbXmlNodeContainer::Parse( const wxString& name, TiXmlElement*& value )
{
	mbXmlNodeMap::iterator it = m_map.find(name);
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