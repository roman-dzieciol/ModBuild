// ============================================================================
//  mbMD5
// ============================================================================

#include "mbPrecompile.h"
#include "mbMD5.h"
#include "MD5.h"




wxString mbMD5Print(void* data)
{
	uchar* charData = (uchar*)data;
	if( charData != NULL )
	{
		wxString digest;
		for( int i=0; i<16; i++ )
			digest.Append(wxString::Format(wxT("%0.2x"), charData[i]));
		return digest;
	}
	return wxEmptyString;
}

bool mbMD5String(const wxString& text, wxString& outDigest)
{
	// Convert string to mb
	const wxWX2MBbuf tmp_buf = wxConvCurrent->cWX2MB(text);
	const char *tmp_str = (const char*) tmp_buf;

	// Hash it
	md5 alg;
	alg.Update((uchar*)tmp_str, text.Len());
	alg.Finalize();

	// Get digest as wxString
	outDigest = mbMD5Print(alg.Digest());
	return true;
}

bool mbMD5Stream(wxInputStream& stream, wxString& outDigest)
{
	if( !stream || !stream.CanRead() )
		return false;

	md5 alg;

	// Parse file
	while( stream.CanRead() )
	{
		// Read chunk
		static const unsigned int chunkSize = 0xFFFF;
		unsigned char chBuffer[chunkSize];
		stream.Read(chBuffer,chunkSize);
		int nLen = stream.LastRead();
		if( nLen > 0 )
		{
			// Hash chunk
			alg.Update(chBuffer, nLen);
		}
	}

	// Finish hashing
	alg.Finalize();

	// Get digest as wxString
	outDigest = mbMD5Print(alg.Digest());
	return true;
}

bool mbMD5File(const wxString& fileName, wxString& outDigest)
{
	wxFileInputStream stream(fileName);
	if( stream.IsOk() )
	{
		return mbMD5Stream(stream,outDigest);
	}

	return false;
}

// ============================================================================
//  EOF
// ============================================================================