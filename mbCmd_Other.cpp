// ============================================================================
//  mbCmd_Other
// ============================================================================

#include "mbPrecompile.h"
#include "mbCmd_Defs.h"
#include "mbException.h"
#include "mbVariables.h"
#include "mbMD5.h"



// ============================================================================
//	Comment Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Comment);

void mbCmdComment::Execute(mbCommands& stack)
{
	bool quiet = ParseBool(wxT("Quiet"), true);
	bool binline = ParseBool(wxT("Inline"), true);

	if( !quiet )
	{
		wxString text = ParseProp(wxT("InnerText"), true);
		if( !text.empty() )
		{
			if( binline )
				PrintResult(stack, wxT("%s"), text.c_str());
			else
				wxLogMessage(wxT("\n%s"), text.c_str() );
		}
	}
}

// ============================================================================
//	GetSVNVersion Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(GetSVNVersion);

void mbCmdGetSVNVersion::Execute(mbCommands& stack)
{;
	MB_CMD_GUARD_OPT;
	bool allowrange = ParseBool(wxT("AllowRange"), true);
	bool allowmodified = ParseBool(wxT("AllowModified"), true);
	bool allowswitched = ParseBool(wxT("AllowSwitched"), true);
	wxString idversion = ParseProp(wxT("IdVersion"), false);
	wxString idname = ParseProp(wxT("IdName"), false);
	wxString idfile = ParseProp(wxT("IdFile"), false);
	wxString repo = ParseProp(wxT("Repository"), false);
	wxString wc = ParseProp(wxT("WorkingCopy"), false);

	PrintExecute(stack, wxT("%s"), PrintProps().c_str() );

	unsigned long versionMin = 0;
	unsigned long versionMax = 0;
	bool versionModified = false;
	bool versionSwitched = false;

	// Get SVN repo version
	wxArrayString output;
	wxArrayString error;
	output.Clear(); 
	error.Clear();

	wxString cmd = wxString::Format(wxT("svnversion.exe -n %s %s"), wc.c_str(), repo.c_str());
	if( wxExecute(cmd, output, error, wxEXEC_NODISABLE | wxEXEC_SYNC) != 0 )
		ThrowError(wxT("Failed to launch: %s"), cmd.c_str() );
	
	if( output.Count() != 1 )
		ThrowError(wxT("Invalid output count (%d) from: %s"), output.Count(), cmd.c_str() );

	// Parse it
	wxString fullversion = output[0].c_str();
	wxString line = fullversion.Trim().Trim(true).MakeUpper();

	// Is modified?
	int pos = line.Find(wxT("M"));
	if( pos != wxNOT_FOUND )
	{
		versionModified = true;
		line = line.Left(pos) + line.Mid(pos+1);
	}

	// Is switched?
	pos = line.Find(wxT("S"));
	if( pos != wxNOT_FOUND )
	{
		versionSwitched = true;
		line = line.Left(pos) + line.Mid(pos+1);
	}

	if( line.IsEmpty() )
		ThrowError(wxT("Invalid output %s from: %s"), fullversion.c_str(), cmd.c_str() );

	// Parse numbers
	pos = line.Find(wxT(':'));
	if( pos != wxNOT_FOUND )
	{
		if( !line.Left(pos).ToULong(&versionMin) )
			ThrowError(wxT("Invalid output %s from: %s"), fullversion.c_str(), cmd.c_str() );

		if( !line.Mid(pos+1).ToULong(&versionMax) )
			ThrowError(wxT("Invalid output %s from: %s"), fullversion.c_str(), cmd.c_str() );
	}
	else
	{
		if( !line.Left(pos).ToULong(&versionMax) )
			ThrowError(wxT("Invalid output %s from: %s"), fullversion.c_str(), cmd.c_str() );

		versionMin = versionMax;
	}

	// Display version
	wxLogMessage(wxT("Mod version is: [%s] From=%lu To=%lu Modified=%d Switched=%d")
		, fullversion.c_str(), versionMin, versionMax, (int)versionModified, (int)versionSwitched );

	// Verify results
	if( versionMin != versionMax && !allowrange )
		ThrowError(wxT("Local working copy has ranged revision! Update it first!"));

	if( versionModified && !allowmodified )
		ThrowError(wxT("Local working copy is modified! Commit or revert first!"));

	if( versionSwitched && !allowswitched )
		ThrowError(wxT("Local working copy url is switched!"));

	wxString safeversion = fullversion;
	wxString forbiddenchars = wxFileName::GetForbiddenChars();
	for( size_t i=0; i<forbiddenchars.size(); ++i)
	{
		wxString forbiddenChar = forbiddenchars.GetChar(i);
		safeversion.Replace(forbiddenChar, wxT("-"), true);
	}

	GmbVariables().SetGlobal(idversion, wxString::Format(wxT("%lu"), versionMax) );
	GmbVariables().SetGlobal(idname, fullversion);
	GmbVariables().SetGlobal(idfile, safeversion);

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	Launch Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(Launch);

void mbCmdLaunch::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("InnerText"));

	// Trim whitespace from the left side
	id.Trim(false);
	if( id.IsEmpty() )
		ThrowError(wxT("Property InnerText is empty!"));

	PrintExecute(stack, wxT("%s"), id.c_str() );

	if( wxExecute(id, wxEXEC_NODISABLE | wxEXEC_SYNC) != 0 && !optional )
		ThrowError(wxT("Failed to execute %s"), id.c_str());

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	MD5File Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(MD5File);

void mbCmdMD5File::Execute(mbCommands& stack)
{
	MB_CMD_GUARD_OPT;
	wxString id = ParseProp(wxT("Id"));
	wxString value = ParseProp(wxT("Value"));

	PrintExecute(stack, wxT("%s %s"), id.c_str(), value.c_str() );

	wxString digest;
	if( !mbMD5File(value, digest) )
		ThrowError(wxT("Failed to MD5 file: %s"), value.c_str());

	wxLogMessage(wxT("%s"), digest.c_str() );
	GmbVariables().SetGlobal(id, digest);

	MB_CMD_UNGUARD_OPT;
}

// ============================================================================
//	MD5String Command
// ============================================================================
MB_CMD_DEFAULT_LOAD(MD5String);

void mbCmdMD5String::Execute(mbCommands& stack)
{
	wxString id = ParseProp(wxT("Id"));
	wxString value = ParseProp(wxT("Value"));

	PrintExecute(stack, wxT("%s %s"), id.c_str(), value.c_str() );

	wxString digest;
	mbMD5String(value, digest);

	wxLogMessage(wxT("%s"), digest.c_str() );
	GmbVariables().SetGlobal(id, digest);
}


// ============================================================================
//  EOF
// ============================================================================