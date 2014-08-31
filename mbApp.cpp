// ============================================================================
//  mbApp
// ============================================================================

#include "mbPrecompile.h"
#include "mbApp.h"
#include "mbUtil.h"
#include "mbException.h"
#include "mbCmdFactory.h"
#include "mbVariables.h"


// ----------------------------------------------------------------------------
//	Application Implementation
// ----------------------------------------------------------------------------
IMPLEMENT_APP_CONSOLE(mbApp);


// ----------------------------------------------------------------------------
//	mbApp
// ----------------------------------------------------------------------------
mbApp::mbApp() : m_rootCmd(NULL)
{
}

void mbApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	wxApp::OnInitCmdLine(parser);
	parser.AddParam(wxT("ProjectFile"));
}

bool mbApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	m_configName = parser.GetParam(0);
	m_configName.Normalize();

	m_scriptlib = wxFileName(mbGetAppDir().GetPathWithSep(), wxT("ModBuildActions.xml"));
	m_scriptlib.Normalize();

	return wxApp::OnCmdLineParsed(parser);
}

int mbApp::OnRun()
{
	try
	{
		// Init logging
		wxLog::GetActiveTarget()->SetVerbose(TRUE); 
		wxLog::GetActiveTarget()->SetTimestamp(NULL);

		// Start ModBuild sequence
		MainSequence();

		// Success
		mbLogHeader(wxT("Success!"));
		return 0;
	}
	catch( mbException& e )
	{
		mbLogError(wxT("ERROR! %s"), e.What().c_str());
	}
	catch( ... )
	{
		mbLogError(wxT("ERROR! unknown exception"));
	}

	// Error
	return 1;
}

void mbApp::ValidateScript(const wxString& filename)
{
	// Get schema path
	wxString appdir = mbGetAppDir().GetPathWithSep();
	wxString validatorPath = wxFileName(appdir,wxT("XMLValidate.exe")).GetFullPath();
	wxString schema = wxString::Format(wxT("ModBuild ModBuild.xsd") );

	if( wxExecute(wxString::Format(wxT("%s \"%s\" %s"), validatorPath.c_str(), filename.c_str(), schema.c_str()), wxEXEC_NODISABLE | wxEXEC_SYNC) != 0 )
		throw mbException(wxT("Script file invalid!\n%s"), filename.c_str());
}

void mbApp::MainSequence()
{
	// Load libs
	LoadLibs();

	// Load script
	LoadScript();

	// Execute script
	ExecuteScript();
}

void mbApp::LoadScript()
{
	mbLogHeader(wxT("Loading script... %s"), m_configName.GetFullPath().c_str());

	// Validate
	ValidateScript(m_configName.GetFullPath());

	// Load script file
	TiXmlDocument xmlDoc;
	if( !xmlDoc.LoadFile(m_configName.GetFullPath().ToUTF8()) )
		throw mbException(wxT("Script file could not be opened!\n%s\n%s"), m_configName.GetFullPath().c_str(), mbGetXMLError(xmlDoc).c_str());

	// Get the script root element
	TiXmlElement* rootElem = xmlDoc.RootElement();
	m_rootCmd = GmbCmdFactory().Get( wxString::FromUTF8(rootElem->Value()) );

	// Load script
	mbCommands loadStack;
	mbCmd::StaticLoad(loadStack, m_rootCmd, rootElem);

	mbLogHeader(wxT("Script loaded."));
}

void mbApp::LoadLibs()
{
	mbLogHeader(wxT("Loading script libraries... %s"), m_scriptlib.GetFullPath().c_str());

	// Validate
	ValidateScript(m_scriptlib.GetFullPath());

	// Load script file
	TiXmlDocument xmlDoc;
	if( !xmlDoc.LoadFile(m_scriptlib.GetFullPath().ToUTF8()) )
		throw mbException(wxT("Script file could not be opened!\n%s\n%s"), m_scriptlib.GetFullPath().c_str(), mbGetXMLError(xmlDoc).c_str());

	// Get the script root element
	TiXmlElement* rootElem = xmlDoc.RootElement();
	mbCmd* libelem = GmbCmdFactory().Get( wxString::FromUTF8(rootElem->Value()) );

	// Load script
	mbCommands loadStack;
	mbCmd::StaticLoad(loadStack, libelem, rootElem);

	mbLogHeader(wxT("Script libraries loaded."));
}

void mbApp::ExecuteScript()
{
	mbLogHeader(wxT("Executing commands..."));

	// Set built-in variables
	GmbVariables().SetGlobal(wxT("DirProject"), m_configName.GetPathWithSep());
	GmbVariables().SetGlobal(wxT("ProjectName"), m_configName.GetName());

	// Set current directory to project directory
	if( !wxSetWorkingDirectory(m_configName.GetPathWithSep()) )
		throw mbException(wxT("Failed to set CWD to: %s"), m_configName.GetPathWithSep().c_str() );

	// Print built-in variables
	wxLogMessage(wxT("Current Directory: %s"), m_configName.GetPathWithSep().c_str() );
	wxLogMessage(wxT("Project Name: %s"), m_configName.GetName().c_str() );

	// Execute loaded commands
	mbCommands execStack;
	mbCmd::StaticExecute(execStack, m_rootCmd);

	mbLogHeader(wxT("Commands executed."));
}


// ============================================================================
//  EOF
// ============================================================================