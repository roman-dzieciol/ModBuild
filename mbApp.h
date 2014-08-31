// ============================================================================
//  mbApp
// ============================================================================
#pragma once

#include "mbCmd.h"


// ============================================================================
//  mbApp
// ============================================================================
class mbApp : public wxApp
{
public:
	mbApp();

protected:
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
	virtual int OnRun();

protected:
	void MainSequence();
	void LoadScript();
	void LoadLibs();
	void ExecuteScript();

protected:
	void ValidateScript(const wxString& filename);

protected:
	wxFileName m_configName;
	wxFileName m_scriptlib;
	mbCmd* m_rootCmd;
};

DECLARE_APP(mbApp);



// ============================================================================
//  EOF
// ============================================================================