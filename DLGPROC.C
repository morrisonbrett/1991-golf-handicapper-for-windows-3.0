/*
**************************************************************************
dlgproc.c       Brett A. Morrison

This file contains several call back routines for processing dialog boxes.
**************************************************************************
*/

#include "golf.h"

extern char szAppName[];
extern char szUntitled[];
extern char szc_name[];
extern char szmm[];
extern char szdd[];
extern char szyy[];
extern char szCsearch_for[];
extern char szNDTitle[];
extern char szPsearch_for[];
extern BOOL bYesNo;
extern BOOL bDeleteRounds;
extern BOOL bNameDate;
extern WORD wWhich;
extern int  iPkey;
extern int  iCkey;
/*
--------------------------------------------------------------------------
DoCaption

Sets the caption of the window by getting szAppName, which is defined
globally, and appending the current filename to it.

Inputs:
  hwnd        - The handle of the main window
  szFileNameP - The current filename of the player file in memory
  szFileNameC - The current filename of the course file in memory

Outputs:
  None
--------------------------------------------------------------------------
*/
void DoCaption(HWND hwnd, char *szFileTitleP, char *szFileTitleC)
{ /* DoCaption */
  char szCaption[MAX_CAPTION];  /* String to hold caption */

  /* Set the buffer for the caption, and determine if a file is in memory,
     if not, append szAppName with szUntitled */

  wsprintf(szCaption, "%s - %s %s", (LPSTR)szAppName,
	  (LPSTR)(szFileTitleC[NULL] ? szFileTitleC : szUntitled),
	  (LPSTR)(szFileTitleP[NULL] ? szFileTitleP : szUntitled));

  SetWindowText(hwnd, szCaption);  /* Display caption */
} /* DoCaption */

/*
--------------------------------------------------------------------------
AskAboutSave

Asks user to save changes.

Inputs:
  hwnd       - The handle of the main window
  szFileName - The current filename of the file in memory
  which      - checks if the file is a course or player file

Outputs:
  TRUE if user selects YES
  FALSE if user cancels box
--------------------------------------------------------------------------
*/
short AskAboutSave(HWND hwnd, char *szFileTitle, char which)
{ /* AskAboutSave */
  char  szBuffer[FILE_NAME_SIZE];  /* holds filename and path */
  short nReturn;                   /* stores boolean output */

  /* Set the buffer for the dialog box, with a prompt and filename */
  wsprintf(szBuffer, "Save current changes: %s",
	  (LPSTR)(szFileTitle[NULL] ? szFileTitle : szUntitled));

  /* then send a save message */
  if (IDYES == (nReturn = MessageBox(hwnd, szBuffer, szAppName,
		MB_YESNOCANCEL | MB_ICONQUESTION)))
    switch(which)
    {
      case PLAYER_MARKER:
	 if (!SendMessage(hwnd, WM_COMMAND, IDM_SAVE_P, 0L)) /* then message failed */
	    return IDCANCEL;
	 break;
      case COURSE_MARKER:
	 if (!SendMessage(hwnd, WM_COMMAND, IDM_SAVE_C, 0L)) /* then message failed */
	    return IDCANCEL;
	 break;
    } /* switch */

  return nReturn;
} /* AskAboutSave */

/*
--------------------------------------------------------------------------
AboutDlgProc

Displays about complete golf handicapper dialog box.

Inputs:
  hDlg     - Handle to parent window
  message  - Message sent from MainWndProc
  wParam   - Command from MainWndProc

Outputs:
  TRUE, so the calling program can close the dialog box
  FALSE if control is switched to another window
--------------------------------------------------------------------------
*/
BOOL FAR PASCAL AboutDlgProc(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{
  switch(message)                  /* checks message */
  { /* switch(message) */
    case WM_INITDIALOG:            /* checks if box has already been loaded in memory */
      return TRUE;

    case WM_COMMAND:
      switch(wParam)               /* checks command */
      { /* switch(wParam) */
	case IDOK:                 /* do nothing and fall through */
	case IDCANCEL:             /* IDOK is same as cancel, so close box */
	  EndDialog(hDlg, NULL);
	  return TRUE;             /* End procedure */
      } /* switch(wParam) */
      break;
  } /* switch(message) */
  return FALSE;                    /* Control has been switched to another window */
} /* AboutDlgProc */

BOOL FAR PASCAL DeleteCourseDlgProc(HWND hDlg, WORD message, WORD wParam,
				    LONG lParam)
{
  switch(message)
  {
    case WM_INITDIALOG:
      SetDlgItemText(hDlg, IDD_DC_CNAME, szc_name);
      SendMessage(GetDlgItem(hDlg, IDD_DC_ROUNDS), BM_SETCHECK, 1, 0L);
      return TRUE;

    case WM_COMMAND:
      switch (wParam)
      {
	case IDYES:
	  bYesNo = IDYES;
	  bDeleteRounds =
	    (BOOL)SendMessage(GetDlgItem(hDlg, IDD_DC_ROUNDS), BM_GETCHECK, 0, 0L);
	  EndDialog(hDlg, TRUE);
	  return TRUE;

	case IDNO:
	case IDCANCEL:
	  bYesNo = IDNO;
	  EndDialog(hDlg, FALSE);
	  return TRUE;
      } /* switch (wParam) */
      break;
  } /* switch(wParam) */
  return FALSE;
} /* DeleteCourseDlgProc */

BOOL FAR PASCAL NameDateDlgProc(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{
  switch(message)
  {
    case WM_INITDIALOG:
      SetDlgItemText(hDlg, IDD_ND_CNAME, szc_name);
      SetDlgItemText(hDlg, IDD_ND_MM, szmm);
      SetDlgItemText(hDlg, IDD_ND_DD, szdd);
      SetDlgItemText(hDlg, IDD_ND_YY, szyy);
      SetWindowText(hDlg, szNDTitle);
      return TRUE;

    case WM_COMMAND:
      switch (wParam)
      {
	case IDOK:
	  bNameDate = IDOK;
	  GetDlgItemText(hDlg, IDD_ND_CNAME, szc_name, COURSE_NAME_LENGTH+1);
	  GetDlgItemText(hDlg, IDD_ND_MM, szmm, 3);
	  GetDlgItemText(hDlg, IDD_ND_DD, szdd, 3);
	  GetDlgItemText(hDlg, IDD_ND_YY, szyy, 3);
	  if (!GoodDate(atoi(szmm), atoi(szdd), (atoi(szyy) + 1900)))
	  {
	    MessageBeep(0);
	    MessageBox(hDlg, "Invalid Date", szAppName, MB_OK | MB_ICONSTOP);
	    return FALSE;
	  }
	  add0(szmm);
	  add0(szdd);
	  add0(szyy);
	  EndDialog(hDlg, TRUE);
	  return TRUE;

	case IDCANCEL:
	  bNameDate = IDCANCEL;
	  EndDialog(hDlg, FALSE);
	  return TRUE;
      } /* switch (wParam) */
      break;
  } /* switch(message); */
  return FALSE;
} /* NameDateDlgProc */

BOOL FAR PASCAL SearchDlgProc(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{
  static WORD wPrev;

  switch(message)                  /* checks message */
  {
    case WM_INITDIALOG:            /* checks if box has already been loaded in memory */
      if (wWhich == IDM_P_SEARCH)  /* player search */
      {
	SetWindowText(hDlg, "Search Player");
	EnableWindow(GetDlgItem(hDlg, IDD_S_FIRSTNAME), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDD_S_LASTNAME), TRUE);
	EnableWindow(GetDlgItem(hDlg, IDD_S_COURSE), FALSE);
	SendMessage(GetDlgItem(hDlg, iPkey), BM_SETCHECK, 1, 0L);
	wPrev = iPkey;
	SetDlgItemText(hDlg, IDD_S_SEARCHFOR, szPsearch_for);
      }
      else
      {
	SetWindowText(hDlg, "Search Course");
	EnableWindow(GetDlgItem(hDlg, IDD_S_FIRSTNAME), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDD_S_LASTNAME), FALSE);
	EnableWindow(GetDlgItem(hDlg, IDD_S_COURSE), TRUE);
	SendMessage(GetDlgItem(hDlg, iCkey), BM_SETCHECK, 1, 0L);
	wPrev = iCkey;
	SetDlgItemText(hDlg, IDD_S_SEARCHFOR, szCsearch_for);
      }
      SendMessage(GetDlgItem(hDlg, IDD_S_CURRENT), BM_SETCHECK, 1, 0L);
      return TRUE;

    case WM_COMMAND:
      switch(wParam)               /* checks command */
      {
	case IDD_S_FIRSTNAME:
	case IDD_S_LASTNAME:
	case IDD_S_COURSE:
	case IDD_S_STREET:
	case IDD_S_CITY:
	case IDD_S_STATE:
	case IDD_S_ZIP:
	case IDD_S_PHONE:
	  ((wWhich == IDM_P_SEARCH) ? (iPkey = (int)wParam) : (iCkey = (int)wParam));
	  SendMessage(GetDlgItem(hDlg, wPrev), BM_SETCHECK, 0, 0L);
	  SendMessage(GetDlgItem(hDlg, wParam), BM_SETCHECK, 1, 0L);
	  wPrev = wParam;
	  return NULL;

	case IDOK:
	  (wWhich == IDM_P_SEARCH) ?
	      GetDlgItemText(hDlg, IDD_S_SEARCHFOR, szPsearch_for, SEARCH_LENGTH+1) :
	      GetDlgItemText(hDlg, IDD_S_SEARCHFOR, szCsearch_for, SEARCH_LENGTH+1);
	  EndDialog(hDlg, TRUE);
	  (wWhich == IDM_P_SEARCH) ?
	    SearchFor(GetParent(hDlg), (BOOL)wWhich, iPkey, szPsearch_for,
		      (BOOL)SendMessage(GetDlgItem(hDlg, IDD_S_BEGINNING), BM_GETCHECK, 0, 0L)) :
	    SearchFor(GetParent(hDlg), (BOOL)wWhich, iCkey, szCsearch_for,
		      (BOOL)SendMessage(GetDlgItem(hDlg, IDD_S_BEGINNING), BM_GETCHECK, 0, 0L));
	  return TRUE;

	case IDCANCEL:             /* IDOK is same as cancel, so close box */
	  EndDialog(hDlg, FALSE);
	  return TRUE;             /* End procedure */
      } /* switch(wParam) */
      break;
  } /* switch(message) */
  return FALSE;                    /* Control has been switched to another window */
} /* SearchDlgProc */
