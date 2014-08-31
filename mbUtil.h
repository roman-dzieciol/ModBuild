// ============================================================================
//  mbUtil
// ============================================================================
#pragma once

#include "mbCmd.h"


// ============================================================================
//  File
// ============================================================================
wxFileName mbGetAppDir();


// ============================================================================
//  Logging
// ============================================================================
void mbLogHeader( const wxChar *pszFormat, ... );
void mbLogError( const wxChar *pszFormat, ... );


// ============================================================================
//  Printing
// ============================================================================
wxString mbPrintStack(const mbCommands& stack);
wxString mbPrintStackResult(const mbCommands& stack);
wxString mbPrintStackSymbolic(const mbCommands& stack);
wxString mbPrintStackDetailed(const mbCommands& stack);


// ============================================================================
//  XML related
// ============================================================================
wxString mbGetXPath(const TiXmlNode* elem);
wxString mbGetXPathReadable(const TiXmlNode* elem);
wxString mbGetXMLError(const TiXmlDocument& doc);



// ============================================================================
//  EOF
// ============================================================================