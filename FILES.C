/*
**************************************************************************
files.c  Brett A. Morrison

This file contains the routines for reading in a file.	It serves as an
intermediary between golf.c and the functions in filedlg.c
**************************************************************************
*/

#include "golf.h"

/*
--------------------------------------------------------------------------
FileLength

Determines the size of the file before reading.

Inputs:
  hFile - The handle of the file to be checked

Outputs:
  The length of the file
--------------------------------------------------------------------------
*/
long FileLength(HANDLE hFile)
{ /* FileLength */
  long lCurrentPos = _llseek(hFile, 0L, 1);  /* Set the pointer to the top */
  long lFileLength = _llseek(hFile, 0L, 2);  /* Seek to the bottom */

  _llseek(hFile, lCurrentPos, 0);	     /* Reset pointer */

  return lFileLength;			     /* Exit with length */
} /* FileLength */

/*
--------------------------------------------------------------------------
ReadFile

Calls DoFileOpenDlgProc and reads the file into a global memory block.
Checks to see if the file is too large or if memory can't be allocated
for the file.

Inputs:
  hInstance   - The instance ID# of the parent window
  hwnd	      - The handle to the parent window
  pof	      - The characteristics of the current file
  *szFileName - The current file name
  bAskName    - Determines whether or not to prompt for filename
  *players    - player array
  *courses    - course array
  which       - determines if file is course or player

Outputs:
  TRUE if file can be read, else FALSE
--------------------------------------------------------------------------
*/
BOOL ReadFile(HWND hwnd, char *szFile, char *szFileTitleP,
	      char *szFileTitleC, BOOL bNeedSave, player_array *players,
	      course_array *courses, char which)
{ /* ReadFile */
  DWORD    dwLength;	     /* checks length of file */
  HANDLE   hFile;	     /* handle to filename */
  HANDLE   hTextBuffer;	     /* handle to buffer which stores text */
  LPSTR    lpTextBuffer;	     /* buffer which stores text */
  HCURSOR  hCursor;	     /* stores handle for hourglass cursor */
  OFSTRUCT pof;
  BOOL     bInvalid = FALSE;

  if (bNeedSave && IDCANCEL ==
      AskAboutSave(hwnd, which == PLAYER_MARKER ? szFileTitleP : szFileTitleC,
		   which))
     return FALSE;

  /* Check to see if file can be opened */
  if (-1 == (hFile = OpenFile(szFile, &pof, OF_READ)))
  { /* if */
    OkMessageBox(hwnd, "Cannot open file %s", szFile);
    return FALSE;
  } /* if */

  if ((dwLength = FileLength(hFile)) >= MAX_FILE_SIZE) /* too big */
  { /* if */
    _lclose(hFile);
    OkMessageBox(hwnd, "File %s too large", szFile);
    return FALSE;
  } /* if */

  if (NULL == (hTextBuffer = GlobalAlloc(GHND, (DWORD)dwLength + 1))) /* not enough memory */
  { /* if */
    _lclose(hFile);
    OkMessageBox(hwnd, "Cannot allocate memory for %s", szFile);
    return FALSE;
  } /* if */

  hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
  ShowCursor(TRUE);

  lpTextBuffer = GlobalLock(hTextBuffer);	/* Set the handle for the buffer */
  _lread(hFile, lpTextBuffer, (WORD)dwLength);	/* Load the buffer */
  _lclose(hFile);				/* Close the file */
  lpTextBuffer[(WORD)dwLength] = '\0';		/* Terminate the buffer	*/

  if ((lpTextBuffer[0] == COURSE_MARKER) && (which == COURSE_MARKER))
    LoadCourses(lpTextBuffer, &(*courses));
  else if ((lpTextBuffer[0] == PLAYER_MARKER) && (which == PLAYER_MARKER))
    LoadPlayers(lpTextBuffer, &(*players));
  else
    bInvalid = TRUE;

  GlobalUnlock(hTextBuffer);		 /* Release the handle to the buffer */
  GlobalFree(hTextBuffer);		 /* Dispose of the allocated memory */

  ShowCursor(FALSE);
  SetCursor(hCursor);

  if (bInvalid)
  {
    OkMessageBox(hwnd, "Invalid File %s", szFile);
    return FALSE;
  } /* if */

  return TRUE;
} /* ReadFile */

/*
--------------------------------------------------------------------------
WriteFile

Calls DoFileSaveDlgProc to obtain the name of the file.  It then obtains
the handle to the edit control's buffer, locks it and writes the file
to the disk directly from that buffer.

Inputs:
  hwnd	      - The handle to the parent window
  pof	      - The characteristics of the current file
  *szFileName - The current file name
  bAskName    - Determines whether or not to prompt for filename
  *players    - player array
  *courses    - course array
  which       - determines if file is course or player

Outputs:
  TRUE if file can be written, else FALSE
--------------------------------------------------------------------------
*/
BOOL WriteFile(HWND hwnd, char *szFile, player_array *players,
	       course_array *courses, char which, int p_index, int r_index,
	       int c_index)
{
  HANDLE   hFile;		    /* Handle to the current file */
  HCURSOR  hCursor;	            /* stores handle for hourglass cursor */
  OFSTRUCT pof;

  if (-1 == (hFile = OpenFile(szFile, &pof, OF_WRITE | OF_CREATE)))
  { /* if */
    OkMessageBox(hwnd, "Cannot create file %s", szFile);
    return FALSE;
  } /* if */

  hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
  ShowCursor(TRUE);

  if (which == PLAYER_MARKER)
  {
    if (!get_player(hwnd, &(*players), p_index, r_index))
      return FALSE;
    unload_string(hFile, "P");
    UnloadPlayers(hFile, &(*players));
  } /* if */
  else
  {
    if (!get_course(hwnd, &(*courses), c_index))
      return FALSE;
    unload_string(hFile, "C");
    UnloadCourses(hFile, &(*courses));
  } /* else */

  _lclose(hFile);

  ShowCursor(FALSE);
  SetCursor(hCursor);

  return TRUE;
} /* WriteFile */
