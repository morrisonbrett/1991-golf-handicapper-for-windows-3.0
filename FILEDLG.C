/*
**************************************************************************
filedlg.c  Brett A. Morrison

This file contains the dialog box routines for reading and writing files.
files.c contains the actual read functions.  It is called by messages
deciphered in golf.c.  It performs thorough error checking for input
of the file names.
**************************************************************************
*/

#include "golf.h"

static char	 szDefExt  [FILE_EXTENSION_SIZE]; /* Default extension */
static char	 szFileName[FULL_NAME_SIZE];	  /* File name to display */
static char	 szFileSpec[FILE_NAME_SIZE];	  /* Type of file */
static POFSTRUCT pof;				  /* Characteristics of file */
static WORD	 wFileAttr;			  /* Stores attributes */
static WORD	 wStatus;			  /* Stores read/write status */
static char      szWhich;                         /* Determines if course or player */
/*
--------------------------------------------------------------------------
DoFileOpenDlg

Initializes parameters for the FileOpenDlgProc.

Inputs:
  hInst 	 - Instance of the parent window
  hwnd		 - Handle to the parent window
  *szFileSpecIn  - Pointer to filename passed to procedure
  *szDefExtIn	 - Pointer to default extension
  wFileAttrIn	 - File attribute types to be displayed
  *szFileNameOut - Filename returned
  pofIn 	 - Structure containing characteristics of file
  which          - determines if the file is a player or course file

Outputs:
  TRUE if OK is selected, FALSE if user ends box with cancel.
--------------------------------------------------------------------------
*/
BOOL DoFileOpenDlg(HANDLE hInst, HWND hwnd, char *szFileSpecIn,
		  char *szDefExtIn, WORD wFileAttrIn,
		  char *szFileNameOut, POFSTRUCT pofIn, char which)
{ /* DoFileOpenDlg */
   FARPROC lpfnFileOpenDlgProc; /* Procedure handle */
   BOOL	   iReturn;		/* Return value	*/

   lstrcpy(szFileSpec, szFileSpecIn);	 /* Set initial names */
   lstrcpy(szDefExt, szDefExtIn);
   wFileAttr = wFileAttrIn;		 /* Set initial attributes */
   pof = pofIn; 			 /* Set initial characteristics	*/
   szWhich = which;

   /* Set procedure */
   lpfnFileOpenDlgProc = MakeProcInstance((FARPROC)FileOpenDlgProc, hInst);

   /* Call procedure */
   iReturn = DialogBox(hInst, "OPEN", hwnd, lpfnFileOpenDlgProc);

   /* Free memory allocated for dialog box */
   FreeProcInstance(lpfnFileOpenDlgProc);

   lstrcpy(szFileNameOut, szFileName);
   return iReturn;
} /* DoFileOpenDlg */

/*
--------------------------------------------------------------------------
DoFileSaveDlg

Initializes parameters for the FileSaveDlgProc.

Inputs:
  hInst 	 - Instance of the parent window
  hwnd		 - Handle to the parent window
  *szFileSpecIn  - Pointer to filename passed to procedure
  *szDefExtIn	 - Pointer to default extension
  *pwStatusOut	 - File attribute types to be displayed
  *szFileNameOut - Filename returned
  pofIn 	 - Structure containing characteristics of file
  which          - determines if the file is a player or course file

Outputs:
  TRUE if OK is selected, FALSE if user ends box with cancel.
--------------------------------------------------------------------------
*/
BOOL DoFileSaveDlg(HANDLE hInst, HWND hwnd, char *szFileSpecIn,
		  char *szDefExtIn, WORD wFileAttrIn, WORD *pwStatusOut,
		  char *szFileNameOut, POFSTRUCT pofIn, char which)
{ /* DoFileSaveDlg */
   FARPROC lpfnFileSaveDlgProc;
   BOOL	   iReturn;

   wFileAttr = wFileAttrIn;		 /* Set initial attributes */

   lstrcpy(szFileSpec, szFileSpecIn);
   lstrcpy(szDefExt, szDefExtIn);
   pof = pofIn;
   szWhich = which;

   /* Set procedure */
   lpfnFileSaveDlgProc = MakeProcInstance((FARPROC)FileSaveDlgProc, hInst);

   /* Call procedure */
   iReturn = DialogBox(hInst, "SAVE", hwnd, lpfnFileSaveDlgProc);

   /* Free memory allocated for dialog box */
   FreeProcInstance(lpfnFileSaveDlgProc);

   lstrcpy(szFileNameOut, szFileName);
   *pwStatusOut = wStatus;
   return iReturn;
} /* DoFileSaveDlg */

/*
--------------------------------------------------------------------------
FileOpenDlgProc

Processes open file dialog box, and performs error checking.

Inputs:
  hDlg	  - Handle to dialog box
  message - Message from MainWndProc
  wParam  - Additional message information
  lParam  - Additional message information

Outputs:
  TRUE if valid filename is selected, else FALSE.
--------------------------------------------------------------------------
*/
BOOL FAR PASCAL FileOpenDlgProc(HWND hDlg, WORD message, WORD wParam,
				LONG lParam)
{ /* FileOpenDlgProc */
  char	cLastChar;
  short nEditLen;

  switch (message)
  { /* switch (message) */
    case WM_INITDIALOG:  /* Setup box */
      SendDlgItemMessage(hDlg, IDD_FNAME, EM_LIMITTEXT, COLUMN_WIDTH, 0L);
      /* Get current drive and directory */
      DlgDirList(hDlg, szFileSpec, IDD_FLIST, IDD_FPATH, wFileAttr);
      SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
      SetWindowText(hDlg, szWhich == PLAYER_MARKER ? "Open Player File" :
		    "Open Course File");
      return TRUE;

    case WM_COMMAND:  /* Command messages from box */
      switch (wParam)
      {
	case IDD_FLIST: /* Relist files	*/
	  switch (HIWORD (lParam))
	  {
	    case LBN_SELCHANGE:    /* Relist files */
	      if (DlgDirSelect(hDlg, szFileName, IDD_FLIST))
		lstrcat(szFileName, szFileSpec);
	      SetDlgItemText(hDlg, IDD_FNAME, szFileName);
	      return TRUE;

	    case LBN_DBLCLK: /* Process open message */
	      if (DlgDirSelect(hDlg, szFileName, IDD_FLIST))
	      {	/* if */
		lstrcat(szFileName, szFileSpec);
		DlgDirList(hDlg, szFileName, IDD_FLIST, IDD_FPATH, wFileAttr);
		SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
	      } /* if */
	      else /* Send OK message */
	      { /* else */
		SetDlgItemText(hDlg, IDD_FNAME, szFileName);
		SendMessage(hDlg, WM_COMMAND, IDOK, 0L);
	      } /* else */
	      return TRUE;
	  } /* switch (HIWORD (lParam)) */
	  break;

	case IDD_FNAME:  /* Print name to small window */
	  if (HIWORD(lParam) == EN_CHANGE)
	    EnableWindow(GetDlgItem(hDlg, IDOK),
			 (BOOL) SendMessage(LOWORD(lParam),
			 WM_GETTEXTLENGTH, 0, 0L));
	  return TRUE;

	case IDOK:  /* Try to open file */
	/* This routine was written by Charles Petzold */
	  GetDlgItemText(hDlg, IDD_FNAME, szFileName, COLUMN_WIDTH);
	  nEditLen  = lstrlen(szFileName);
	  cLastChar = *AnsiPrev(szFileName, szFileName + nEditLen);

	  if (cLastChar == '\\' || cLastChar == ':')  /* New drive or directory	*/
	    lstrcat(szFileName, szFileSpec);

	  if (lstrchr(szFileName, '*') || lstrchr(szFileName, '?')) /* Wildcards */
	  {
	    if (DlgDirList(hDlg, szFileName, IDD_FLIST, IDD_FPATH, wFileAttr))
	    {
	      lstrcpy(szFileSpec, szFileName);
	      SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
	    } /* if */
	    else
	      MessageBeep(0);

	    return TRUE;
	  } /* if */

	  /* set the selected directory	*/
	  lstrcat(lstrcat(szFileName, "\\"), szFileSpec);

	  /* Reset defaults */
	  if (DlgDirList(hDlg, szFileName, IDD_FLIST, IDD_FPATH, wFileAttr))
	  {
	    lstrcpy(szFileSpec, szFileName);
	    SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
	    return TRUE;
	  } /* if */

	  szFileName[nEditLen] = '\0';	/* Terminate the string */

	  /* check if file can be opened */
	  if (-1 == OpenFile(szFileName, pof, OF_READ | OF_EXIST))
	  {
	    lstrcat(szFileName, szDefExt);
	    if (-1 == OpenFile(szFileName, pof, OF_READ | OF_EXIST))
	    {
	      MessageBeep(0);
	      return TRUE;
	    } /* if */
	  } /* if */

	  /* Change filename to equal full path name */
	  lstrcpy(szFileName, AnsiNext(lstrrchr ((LPSTR)pof->szPathName, '\\')));

	  OemToAnsi(szFileName, szFileName);  /* Copy to ansi equivalent */
	  EndDialog(hDlg, TRUE);
	  return TRUE;

	case IDCANCEL:
	  EndDialog(hDlg, FALSE);
	  return TRUE;
      } /* switch (wParam) */
  } /* switch (mesage) */
  return FALSE;
} /* FileOpenDlgProc */

/*
--------------------------------------------------------------------------
FileSaveDlgProc

Processes save file dialog box, and performs error checking.

Inputs:
  hDlg	  - Handle to parent window
  message - Message from MainWndProc
  wParam  - Additional message information
  lParam  - Additional message information

Outputs:
  TRUE if valid filename is selected, else FALSE.
--------------------------------------------------------------------------
*/

BOOL FAR PASCAL FileSaveDlgProc(HWND hDlg, WORD message, WORD wParam,
				LONG lParam)
{ /* FileSaveDlgProc */
  char	cLastChar;
  short nEditLen;

  switch(message)
  {
    case WM_INITDIALOG:  /* Setup box */
      SendDlgItemMessage(hDlg, IDD_FNAME, EM_LIMITTEXT, COLUMN_WIDTH, 0L);
      /* Get current drive and directory */
      DlgDirList(hDlg, szFileSpec, IDD_FLIST, IDD_FPATH, wFileAttr);
      SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
      SetWindowText(hDlg, szWhich == PLAYER_MARKER ? "Save Player File" :
		    "Save Course File");
      return TRUE;

    case WM_COMMAND:  /* Command messages from box */
      switch (wParam)
      { /* switch (wParam) */
	case IDD_FLIST: /* Relist files	*/
	  switch (HIWORD (lParam))
	  { /* switch (HIWORD (lParam)) */
	    case LBN_SELCHANGE:    /* Relist files */
	      if (DlgDirSelect(hDlg, szFileName, IDD_FLIST))
		lstrcat(szFileName, szFileSpec);
	      SetDlgItemText(hDlg, IDD_FNAME, szFileName);
	      return TRUE;

	    case LBN_DBLCLK: /* Process open message */
	      if (DlgDirSelect(hDlg, szFileName, IDD_FLIST))
	      {	/* if */
		lstrcat(szFileName, szFileSpec);
		DlgDirList(hDlg, szFileName, IDD_FLIST, IDD_FPATH, wFileAttr);
		SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
	      } /* if */
	      else /* Send OK message */
	      { /* else */
		SetDlgItemText(hDlg, IDD_FNAME, szFileName);
		SendMessage(hDlg, WM_COMMAND, IDOK, 0L);
	      } /* else */
	      return TRUE;
	  } /* switch (HIWORD (lParam)) */
	  break;

	case IDD_FNAME:  /* Print name to small window */
	  if (HIWORD(lParam) == EN_CHANGE)
	    EnableWindow(GetDlgItem(hDlg, IDOK),
			 (BOOL) SendMessage(LOWORD (lParam),
			 WM_GETTEXTLENGTH, 0, 0L));
	  return TRUE;

	case IDOK:  /* Try to save file */
	/* This routine was written by Charles Petzold */
	  GetDlgItemText(hDlg, IDD_FNAME, szFileName, COLUMN_WIDTH);
	  nEditLen  = lstrlen(szFileName);
	  cLastChar = *AnsiPrev(szFileName, szFileName + nEditLen);

	  if (cLastChar == '\\' || cLastChar == ':')  /* New drive or directory	*/
	    lstrcat(szFileName, szFileSpec);

	  if (lstrchr(szFileName, '*') || lstrchr(szFileName, '?')) /* Wildcards */
	  {
	    if (DlgDirList(hDlg, szFileName, IDD_FLIST, IDD_FPATH, wFileAttr))
	    {
	      lstrcpy(szFileSpec, szFileName);
	      SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
	    } /* if */
	    else
	      MessageBeep(0);
	    return TRUE;
	  } /* if */

	  /* set the selected directory	*/
	  lstrcat(lstrcat(szFileName, "\\"), szFileSpec);

	  /* Reset defaults */
	  if (DlgDirList(hDlg, szFileName, IDD_FLIST, IDD_FPATH, wFileAttr))
	  { /* if */
	    lstrcpy(szFileSpec, szFileName);
	    SetDlgItemText(hDlg, IDD_FNAME, szFileSpec);
	    return TRUE;
	  } /* if */

	  szFileName[nEditLen] = '\0';	/* Terminate the string */

	  /* check if file can be opened */
	  if (-1 == OpenFile(szFileName, pof, OF_PARSE))
	  {
	    MessageBeep(0);
	    return TRUE;
	  }

	  if (!lstrchr(AnsiNext(lstrrchr((LPSTR)pof->szPathName, '\\')), '.'))
	    lstrcat(szFileName, szDefExt);

	  if (-1 != OpenFile(szFileName, pof, OF_WRITE | OF_EXIST))
	    wStatus = 1;
	  else if (-1 != OpenFile(szFileName, pof, OF_CREATE | OF_EXIST))
	    wStatus = 0;
	  else
	  {
	    MessageBeep(0);
	    return TRUE;
	  } /* else */

	  /* Change filename to equal full path name */
	  lstrcpy(szFileName, AnsiNext(lstrrchr ((LPSTR)pof->szPathName, '\\')));

	  OemToAnsi(szFileName, szFileName);  /* Copy to ansi equivalent */
	  EndDialog(hDlg, TRUE);
	  return TRUE;

	case IDCANCEL:
	  EndDialog(hDlg, FALSE);
	  return TRUE;
      } /* switch (wParam) */
  } /* switch (mesage) */
  return FALSE;
} /* FileSaveDlgProc */
