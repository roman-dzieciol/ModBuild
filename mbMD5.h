// ============================================================================
//  mbMD5
// ============================================================================
#pragma once

// Print digest to wxString
wxString mbMD5Print(void* data);

// Hash string
bool mbMD5String(const wxString& text, wxString& outDigest);

// Hash stream
bool mbMD5Stream(wxInputStream& stream, wxString& outDigest);

// Hash file
bool mbMD5File(const wxString& fileName, wxString& outDigest);


// ============================================================================
//  EOF
// ============================================================================