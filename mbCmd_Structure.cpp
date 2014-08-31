// ============================================================================
//  mbCmd_Structure
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbException.h"
#include "mbVariables.h"
#include "mbActions.h"


// ============================================================================
//	Action Command
// ============================================================================
void mbCmdAction::Load(mbCommands& stack, TiXmlElement* elem)
{
	// Actions are listed in hash map
	wxString id = GetProp(wxT("Id"));
	GmbActions().Add(id, this);

	LoadChildren(stack, elem);
}

void mbCmdAction::Execute(mbCommands& stack)
{
	wxString id = ParseProp(wxT("Id"));

	ExecuteChildren(stack);
}


// ============================================================================
//	ActionRef Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(ActionRef);

void mbCmdActionRef::Execute(mbCommands& stack)
{
	wxString id = ParseProp(wxT("Id"));
	bool quiet = ParseBool(wxT("Quiet"), true);

	mbCmd* action = GmbActions().Get(id);

	// Extra atributes are local variables
	wxString props;
	bool bPushed = false;
	for( mbCmdPropertiesMap::iterator pit=m_properties.begin(); pit!=m_properties.end(); ++pit )
	{
		if( !wxString(wxT("Id")).IsSameAs((*pit).first)
		&&  !wxString(wxT("Quiet")).IsSameAs((*pit).first)
		&&  !wxString(wxT("Optional")).IsSameAs((*pit).first) )
		{
			wxString value = ParseProp((*pit).first,true);
			if( !quiet )
			{
				props += wxString::Format(wxT("%s='%s' "), (*pit).first.c_str(), value.c_str() );
			}
			GmbVariables().PushLocal(GetDebugId(&stack), id + wxString(wxT("_")) + (*pit).first, value.c_str() );
			bPushed = true;
		}
	}

	if( !quiet )
	{
		if( bPushed )
		{
			PrintExecute(stack, wxT("%s"),  PrintPropsParsed().c_str() );
		}
		else
		{
			wxLogMessage(wxT(""));
			wxLogMessage(wxT(""));
			wxLogMessage(wxT("------------------------------------------------------------------------------"));
			wxLogMessage(wxString::Format(wxT(" Action: %s %s"), id.c_str(), props.c_str()));
			wxLogMessage(wxT("------------------------------------------------------------------------------"));
		}
	}

	// Extra Action atributes are required local variables, fail if they can't be parsed
	wxString varid = id + wxString(wxT("_"));
	for( mbCmdPropertiesMap::iterator pit=action->m_properties.begin(); pit!=action->m_properties.end(); ++pit )
	{
		// Skip Id
		if( wxString(wxT("Id")).IsSameAs((*pit).first) )
			continue;

		// Check variable
		wxString value;
		if( !GmbVariables().Get(varid + (*pit).first, value) )
		{
			ThrowError(wxT("Required Action parameter missing: %s (Desc: %s)"), (*pit).first.c_str(), (*pit).second.c_str());
		}
	}


	MB_CMD_GUARD_OPT;
	mbCmd::StaticExecute(stack, action);
	MB_CMD_UNGUARD_OPT_NORET;

	// Kill local variables
	if( bPushed )
		GmbVariables().PopLocal(GetDebugId(&stack));		

}

// ============================================================================
//	ModBuild Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(ModBuild);

void mbCmdModBuild::Execute(mbCommands& stack)
{
	for( mbCommands::iterator it=m_commands.begin(); it!=m_commands.end(); ++it )
	{
		// Skip actions
		if( wxString(wxT("Action")).IsSameAs((*it)->GetName()) )
			continue;

		// Skip comments
		if( wxString(wxT("Comment")).IsSameAs((*it)->GetName()) )
			continue;

		// Execute other children normally
		mbCmd::StaticExecute(stack, *it);
	}
}

// ============================================================================
//	ModBuildActions Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(ModBuildActions);
MB_CMD_DEFAULT_EXECUTE(ModBuildActions);


// ============================================================================
//	Sequence Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Sequence);
MB_CMD_DEFAULT_EXECUTE(Sequence);


// ============================================================================
//  EOF
// ============================================================================