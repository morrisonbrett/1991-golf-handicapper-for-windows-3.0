/*
**************************************************************************
golf.c  Brett A. Morrison

This is the main program for The Complete Golf Handicapper.  It loads
a window, and processes each command as it occurs in the message loop.
Rescources for this program are located in "golf.rc".  Headers and
definitions are located in "golf.h". "windows.h", contains standard
windows definitions and data structures and was written by Microsoft.
"boxes.h" contains ID numbers for each message.
**************************************************************************
*/

#include "golf.h"

char szAppName    [] = "Complete Golf Handicapper";
char szUntitled   [] = "(untitled)";

HANDLE          hInst;
HWND            hWnd;
HANDLE          hHourGlass;  /* Handle to hourglass cursor */
HWND            hWndRList;   /* Handle for list box containing the courses played */
HWND            hWndCScroll; /* Handle for course scroll bar */
HWND            hWndPScroll; /* Handle for player scroll bar */
player_array    players;  /* stores all player element data */
course_array    courses;  /* variable to store the courses in memory */
int             r_index;  /* keeps track of current record */
int             p_index;  /* keeps track of current record */
int             c_index;  /* keeps track of current record */
int             r_last;
char            szDelete[LONG_MESSAGE_LENGTH+1];
char            szDate[DATE_LENGTH+1];
char            szCsearch_for [SEARCH_LENGTH+1];
char            szPsearch_for [SEARCH_LENGTH+1];
char            scCNTitle[MAX_CAPTION+1];
char            szc_name[COURSE_NAME_LENGTH+1];
char            szNDTitle[COURSE_NAME_LENGTH+1];
char            szmm[3];
char            szdd[3];
char            szyy[3];
BOOL            bYesNo;
BOOL            bDeleteRounds;
BOOL            bNameDate;
WORD            wWhich;
int             iPkey = IDD_S_LASTNAME;
int             iCkey = IDD_S_COURSE;
char            szFileP[FILE_NAME_SIZE];     /* Player Filename without extension */
char            szFileC[FILE_NAME_SIZE];     /* Course Filename without extension */
char            szFileTitleP[FILE_NAME_SIZE]; /* Players Filename with extension */
char            szFileTitleC[FILE_NAME_SIZE]; /* Courses Filename with extension */
static BOOL     bNeedSaveP = FALSE;              /* Checks if file has been modified */
static BOOL     bNeedSaveC = FALSE;              /* Checks if file has been modified */
static FARPROC  lpfnGenericDlgProc;              /* Stores value returned from About proc */
static OFSTRUCT of;                              /* Structure holding file read/write parameters */
OPENFILENAME    of_course;
OPENFILENAME    of_player;
/*
--------------------------------------------------------------------------
WinMain

The main function of the program.  This function calls initialization functions
which register the window and display it on the screen.

Inputs:
  hInstance      -  Stores ID# of program occurance in memory
  hPrevInstance  -  If no other version of loaded, this value is set to NULL
  lpCmdLine      -  Stores any commmand line parameters
  nCmdShow       -  Value determines how windows is initially shown

Outputs:
  Returns the current message.
--------------------------------------------------------------------------
*/
int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine,
		   int nCmdShow)
{
  static FARPROC lpfnMainWndProc;

  hInst = hInstance;

  hHourGlass = LoadCursor(NULL, IDC_WAIT);   /* get hourglass cursor */

/*  if (hPrevInstance)  /* Uncomment this to run only one version of the program */
      return NULL; */

  lpfnMainWndProc = MakeProcInstance(MainWndProc, hInst);
  DialogBox(hInst, "GOLFMAIN", NULL, lpfnMainWndProc);
  FreeProcInstance(lpfnMainWndProc);

  return NULL;
} /* WinMain */

/*
--------------------------------------------------------------------------
MainWndProc

This is the main procedure which processes each message.

Inputs:
  hwnd     - The handle to the parent window
  message  - Message sent from MainWndProc
  wParam   - Command from MainWndProc
  lParam   - Additional message information

Outputs:
  Used as an boolean for the current message
--------------------------------------------------------------------------
*/
int FAR PASCAL MainWndProc(HWND hwnd, WORD message, WORD wParam, LONG lParam)
{
  WORD           wTabStop;

  switch (message)
  {
    case WM_INITDIALOG:
      hWnd = hwnd;
      hWndPScroll = GetDlgItem(hWnd, IDD_PSCROLL);
      hWndCScroll = GetDlgItem(hWnd, IDD_CSCROLL);
      hWndRList   = GetDlgItem(hWnd, IDD_RLIST);
      of_course.lStructSize       = sizeof(OPENFILENAME);
      of_player.lStructSize       = sizeof(OPENFILENAME);
      of_course.hwndOwner         = hWnd;
      of_player.hwndOwner         = hWnd;
      of_course.lpstrFilter       = "Course(*.GLC)\0*.glc\0";
      of_player.lpstrFilter       = "Player(*.GLP)\0*.glp\0";
      of_course.lpstrCustomFilter = (LPSTR) NULL;
      of_player.lpstrCustomFilter = (LPSTR) NULL;
      of_course.nMaxCustFilter    = 0L;
      of_player.nMaxCustFilter    = 0L;
      of_course.nFilterIndex      = 0L;
      of_player.nFilterIndex      = 0L;
      of_course.lpstrFile         = szFileC;
      of_player.lpstrFile         = szFileP;
      of_course.nMaxFile          = sizeof(szFileC);
      of_player.nMaxFile          = sizeof(szFileP);
      of_course.lpstrFileTitle    = szFileTitleC;
      of_player.lpstrFileTitle    = szFileTitleP;
      of_course.nMaxFileTitle     = sizeof(szFileTitleC);
      of_player.nMaxFileTitle     = sizeof(szFileTitleP);
      of_course.lpstrInitialDir   = NULL;
      of_player.lpstrInitialDir   = NULL;
      of_course.nFileOffset       = 0;
      of_player.nFileOffset       = 0;
      of_course.nFileExtension    = 0;
      of_player.nFileExtension    = 0;
      of_course.lpstrDefExt       = "GLC";
      of_player.lpstrDefExt       = "GLP";
      p_index = r_index = r_last = c_index = 0;
      players.player_numb = courses.course_numb = -1;
      SetClassWord(hWnd, GCW_HICON, LoadIcon(hInst, "GOLFICON"));
      wTabStop = 4 + TAB_PIXEL_SETTING / (LOWORD(GetDialogBaseUnits()) / 4);
      SendMessage(hWndRList, LB_SETTABSTOPS, 1, (LONG) (WORD FAR *) &wTabStop);
      DoCaption(hWnd, szFileTitleP, szFileTitleC);
      return TRUE;

    case WM_INITMENU:  /* Set the menu items */
      DoMenu(wParam, &players, &courses, p_index, hWndRList);
      return TRUE;

    case WM_HSCROLL:  /* move to next or previous record */
      if (HIWORD(lParam) == hWndPScroll)
	move_player(hWnd, hWndRList, wParam, &players, &p_index, &r_index, &r_last);
      else if (HIWORD(lParam) == hWndCScroll)
	move_course(hWnd, wParam, &courses, &c_index);
      return TRUE;

    case WM_COMMAND:
      return CommandHandler(wParam, lParam);

    case WM_QUERYENDSESSION:
      if (bNeedSaveP && IDCANCEL == AskAboutSave(hWnd, szFileTitleP, PLAYER_MARKER))
	return 1;
      if (bNeedSaveC && IDCANCEL == AskAboutSave(hWnd, szFileTitleC, COURSE_MARKER))
	return 1;
      EndDialog(hWnd, NULL);
      return 0;

    case WM_CLOSE:  /* Check if file has been saved and close window */
      if (bNeedSaveP && IDCANCEL == AskAboutSave(hWnd, szFileTitleP, PLAYER_MARKER))
	return 0;
      if (bNeedSaveC && IDCANCEL == AskAboutSave(hWnd, szFileTitleC, COURSE_MARKER))
	return 0;
      EndDialog(hWnd, NULL);
      return 1;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    default:
      return FALSE;
  } /* switch (message) */
} /* MainWndProc */

int NEAR PASCAL CommandHandler(WORD wParam, LONG lParam)
{
/*  if (HIWORD(lParam) == EN_CHANGE)
  {
    if (focus(GetDlgCtrlID(LOWORD(lParam))) && players.player_numb > -1)
      bNeedSaveP = TRUE;
    else if (courses.course_numb > -1)
      bNeedSaveC = TRUE;
  }
*/
  switch (wParam)
  {
    case IDOK:
      SendMessage(hWnd, WM_COMMAND, IDD_RLIST, MAKELONG(0, LBN_DBLCLK));
      return NULL;

    case IDM_CONTENTS:
    case IDM_SEARCHON:
    case IDM_USE:
      MessageBox(hWnd, "Help to come soon!", szAppName, MB_OK | MB_ICONEXCLAMATION);
      return NULL;

    case IDM_ABOUT:  /* display about dialog box */
      lpfnGenericDlgProc = MakeProcInstance((FARPROC)AboutDlgProc, hInst);
      DialogBox(hInst, "ABOUT", hWnd, lpfnGenericDlgProc);
      FreeProcInstance(lpfnGenericDlgProc); /* Release memory */
      return NULL;

    case IDM_HANDICAP:
      lpfnGenericDlgProc = MakeProcInstance((FARPROC)HandicapDlgProc, hInst);
      DialogBox(hInst, "HANDICAP", hWnd, lpfnGenericDlgProc);
      FreeProcInstance(lpfnGenericDlgProc); /* Release memory */
      return NULL;

    case IDM_P_SEARCH:
      if (!get_player(hWnd, &players, p_index, r_index))
	return NULL;
      /* fall through */

    case IDM_C_SEARCH:
      if (!get_course(hWnd, &courses, c_index))
	return NULL;
	wWhich = wParam;
	lpfnGenericDlgProc = MakeProcInstance((FARPROC)SearchDlgProc, hInst);
	DialogBox(hInst, "SEARCH", hWnd, lpfnGenericDlgProc);
	FreeProcInstance(lpfnGenericDlgProc); /* Release memory */
	return NULL;

    case IDM_GRAPH:
      if (!get_player(hWnd, &players, p_index, r_index))
	return NULL;
      if (!get_course(hWnd, &courses, c_index))
	return NULL;
      if (IDYES == MessageBox(hWnd, "You must have Microsoft Excel\nto use this option.  Continue?",
			      szAppName, MB_YESNO | MB_ICONQUESTION))
	GraphData(&(players).player[p_index].p_scores, &courses);
      return NULL;

    case IDM_NEW_P: /* save current file and clear viewport */
      if (bNeedSaveP && IDCANCEL == AskAboutSave(hWnd, szFileTitleP, PLAYER_MARKER))
	return FALSE;
      szFileTitleP[NULL] = '\0';
      bNeedSaveP = FALSE;
      p_index = r_index = r_last = 0;
      players.player_numb = -1;
      clear_player(hWnd, hWndRList);
      DoCaption(hWnd, szFileTitleP, szFileTitleC);
      return NULL;

    case IDM_NEW_C:
      if (bNeedSaveC && IDCANCEL == AskAboutSave(hWnd, szFileTitleC, COURSE_MARKER))
	return FALSE;
      szFileTitleC[NULL] = '\0';
      bNeedSaveC = FALSE;
      c_index = 0;
      courses.course_numb = -1;
      clear_course(hWnd);
      DoCaption(hWnd, szFileTitleP, szFileTitleC);
      return NULL;

    case IDM_OPEN_P:  /* open an existing file */
      of_player.lpstrTitle = "Open Player";
      of_player.Flags      = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

      if (!GetOpenFileName(&of_player))
	return FALSE;

      if (ReadFile(hWnd, szFileP, szFileTitleP, szFileTitleC, bNeedSaveP,
		   &players, &courses, PLAYER_MARKER))
      {
	bNeedSaveP = FALSE;
	p_index = 0;
	show_player(hWnd, hWndRList, &players, &p_index, &r_index, &r_last);
	DoCaption(hWnd, szFileTitleP, szFileTitleC);
      } /* if */
      return NULL;

    case IDM_OPEN_C:  /* open an existing file */
      of_course.lpstrTitle = "Open Course";
      of_course.Flags      = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

      if (!GetOpenFileName(&of_course))
	return FALSE;

      if (ReadFile(hWnd, szFileC, szFileTitleP, szFileTitleC, bNeedSaveC,
		   &players, &courses, COURSE_MARKER))
      {
	bNeedSaveC = FALSE;
	c_index = 0;
	show_course(hWnd, &courses, &c_index);
	DoCaption(hWnd, szFileTitleP, szFileTitleC);
      } /* if */
      return NULL;

    case IDM_SAVE_P:  /* write current file */
      if (szFileTitleP[NULL])
      {
	if (!get_player(hWnd, &players, p_index, r_index))
	  return NULL;

	if (WriteFile(hWnd, szFileP, &players, &courses, PLAYER_MARKER,
		      p_index, r_index, c_index))
	{
	  bNeedSaveP = FALSE;
	  return 1;
	} /* if */
	return NULL;
      } /* if fall through */

    case IDM_SAVEAS_P:  /* change file name */
      if (!get_player(hWnd, &players, p_index, r_index))
	return NULL;

      of_player.lpstrTitle = "Save Player";
      of_player.Flags      = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

      if (!GetSaveFileName(&of_player))
	return FALSE;

      if (WriteFile(hWnd, szFileP, &players, &courses, PLAYER_MARKER,
		    p_index, r_index, c_index))
      {
	bNeedSaveP = FALSE;
	DoCaption(hWnd, szFileTitleP, szFileTitleC);
      }
      return NULL;

    case IDM_SAVE_C:  /* write current file */
      if (szFileTitleC[NULL])
      {
	if (!get_course(hWnd, &courses, c_index))
	  return NULL;

	if (WriteFile(hWnd, szFileC, &players, &courses, COURSE_MARKER,
		      p_index, r_index, c_index))
	{
	  bNeedSaveC = FALSE;
	  return 1;
	} /* if */
	return NULL;
      } /* if fall through */

    case IDM_SAVEAS_C:  /* change file name */
      if (!get_course(hWnd, &courses, c_index))
	return NULL;

      of_course.lpstrTitle = "Save Course";
      of_course.Flags      = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

      if (!GetSaveFileName(&of_course))
	return FALSE;

      if (WriteFile(hWnd, szFileC, &players, &courses, COURSE_MARKER,
		    p_index, r_index, c_index))
      {
	bNeedSaveC = FALSE;
	DoCaption(hWnd, szFileTitleP, szFileTitleC);
      }
      return NULL;

    case IDM_PRINT_P:  /* Print current file */
      if (!get_player(hWnd, &players, p_index, r_index))
	return NULL;
      PrintFile(hInst, hWnd, szFileTitleP[NULL] ? szFileTitleP : szUntitled);
      return NULL;

    case IDM_PRINT_C:  /* Print current file */
      if (!get_course(hWnd, &courses, c_index))
	return NULL;
      PrintFile(hInst, hWnd, szFileTitleC[NULL] ? szFileTitleC : szUntitled);
      return NULL;

    case IDM_EXIT:  /* Send a close message, and exit the program */
      SendMessage(hWnd, WM_CLOSE, NULL, 0L);
      return NULL;

    case IDM_UNDO:  /* Check for undo status */
      SendMessage(GetFocus(), WM_UNDO, 0, 0L);
      return 0;

    case IDM_CUT:  /* Send cut message */
      SendMessage(GetFocus(), WM_CUT, 0, 0L);
      return 0;

    case IDM_COPY: /* Send copy message */
      SendMessage(GetFocus(), WM_COPY, 0, 0L);
      return 0;

    case IDM_PASTE:  /* Send paste message */
      SendMessage(GetFocus(), WM_PASTE, 0, 0L);
      return 0;

    case IDM_CLEAR:  /* Send clear message */
      SendMessage(GetFocus(), WM_CLEAR, 0, 0L);
      return 0;

    case IDM_P_ADD:
      if (AddPlayer(hWnd, &players, &p_index, &r_index))
	bNeedSaveP = TRUE;
      return NULL;

    case IDM_R_ADD:
      if (AddRound(hWnd, &players, &p_index, &r_index, &r_last))
	bNeedSaveP = TRUE;
      return NULL;

    case IDM_C_ADD:
      if (AddCourse(hWnd, &courses, &c_index))
	bNeedSaveC = TRUE;
      return NULL;

    case IDM_P_SORT:
      if (!get_player(hWnd, &players, p_index, r_index))
	return NULL;
      qsort(players.player, players.player_numb+1, sizeof(players.player[0]),
	    compare_player);
      bNeedSaveP = TRUE;
      p_index = r_index = 0;
      show_player(hWnd, hWndRList, &players, &p_index, &r_index, &r_last);
      return NULL;

    case IDM_R_SORT:
      if (!get_player(hWnd, &players, p_index, r_index))
	return NULL;
      qsort(players.player[p_index].p_scores.history,
	    players.player[p_index].p_scores.round_numb+1,
	    sizeof(players.player[p_index].p_scores.history[0]),
	    compare_round);
      bNeedSaveP = TRUE;
      r_index = 0;
      show_player(hWnd, hWndRList, &players, &p_index, &r_index, &r_last);
      return NULL;

    case IDM_C_SORT:
      if (!get_course(hWnd, &courses, c_index))
	return NULL;
      qsort(courses.course, courses.course_numb, sizeof(courses.course[0]),
	    compare_course);
      bNeedSaveC = TRUE;
      c_index = 0;
      show_course(hWnd, &courses, &c_index);
      return NULL;

    case IDM_P_DELETE:
      if (players.player_numb > -1)
      {
	GetDlgItemText(hWnd, IDD_PLAST, (players).player[p_index].p_last, LAST_NAME_LENGTH+1);
	GetDlgItemText(hWnd, IDD_PFIRST, (players).player[p_index].p_first, FIRST_NAME_LENGTH+1);
	lstrcpy(szDelete, "Delete : ");
	lstrcat(szDelete, (LPSTR)players.player[p_index].p_first);
	lstrcat(szDelete, " ");
	lstrcat(szDelete, (LPSTR)players.player[p_index].p_last);
	if (IDYES == MessageBox(hWnd, szDelete, "Delete Player", MB_YESNO | MB_ICONQUESTION))
	{
	  bNeedSaveP = TRUE;
	  delete_player(hWnd, hWndRList, &players, &p_index, &r_index, &r_last);
	}
      } /* if */
      return NULL;

    case IDM_R_DELETE:
      if (players.player[p_index].p_scores.round_numb > -1)
      {
	convert_date(&players.player[p_index].p_scores.history[r_index].date,
		     szDate);
	lstrcpy(szDelete, (LPSTR)"Delete : ");
	lstrcat(szDelete, (LPSTR)players.player[p_index].p_scores.history[r_index].course_name);
	lstrcat(szDelete, "  ");
	lstrcat(szDelete, szDate);
	if (IDYES == MessageBox(hWnd, szDelete, "Delete Round", MB_YESNO | MB_ICONQUESTION))
	{
	  delete_round(hWnd, hWndRList, &players, &p_index, &r_index, &r_last);
	  bNeedSaveP = TRUE;
	} /* if */
      } /* if */
      return NULL;

    case IDM_C_DELETE:
      if (courses.course_numb > -1)
      {
	GetDlgItemText(hWnd, IDD_CNAME, (courses).course[c_index].c_name, COURSE_NAME_LENGTH+1);
	lstrcpy(szc_name, (courses).course[c_index].c_name);
	lpfnGenericDlgProc = MakeProcInstance((FARPROC)DeleteCourseDlgProc, hInst);
	DialogBox(hInst, "DELETEC", hWnd, lpfnGenericDlgProc);
	FreeProcInstance(DeleteCourseDlgProc); /* Release memory */
	if (bYesNo == IDYES)
	{
	  bNeedSaveC = TRUE;
	  delete_course(&courses, &c_index);
	  if (bDeleteRounds &&  delete_rounds(&players, szc_name))
	  {
	    bNeedSaveP = TRUE;
	    show_rounds(hWnd, hWndRList, &(players).player[p_index].p_scores, &r_index, &r_last, TRUE);
	  } /* if */
	  show_course(hWnd, &courses, &c_index);
	} /* if */
      } /* if */
      return NULL;

    case IDD_RLIST:
      switch(HIWORD(lParam))
      {
	case LBN_DBLCLK:
	  if ((players.player_numb == -1) ||
	      (players.player[p_index].p_scores.round_numb == -1))
	    return NULL;
	  lstrcpy(szNDTitle, "Change Course / Name Date");
	  convert_date(&players.player[p_index].p_scores.history[r_index].date, szDate);
	  get_date_elements(szDate, szmm, szdd, szyy);
	  lstrcpy(szc_name, (LPSTR)players.player[p_index].p_scores.history[r_index].course_name);
	  lpfnGenericDlgProc = MakeProcInstance((FARPROC)NameDateDlgProc, hInst);
	  DialogBox(hInst, "NAMEDATE", hWnd, lpfnGenericDlgProc);
	  FreeProcInstance(NameDateDlgProc); /* Release memory */
	  if (bNameDate == IDOK)
	  {
	    bNeedSaveP = TRUE;
	    players.player[p_index].p_scores.history[r_index].date =
	      string_to_date(szmm, szdd, szyy);
	    lstrcpy((LPSTR)players.player[p_index].p_scores.history[r_index].course_name,
		    szc_name);
	    show_rounds(hWnd, hWndRList, &(players).player[p_index].p_scores, &r_index, &r_last, FALSE);
	  } /* if */
	  return NULL;

	case LBN_SELCHANGE:
	  if ((int)SendMessage(hWndRList, LB_GETCURSEL, 0, 0L) == r_index)
	    break;
	  if (!get_round(hWnd, &players, p_index, r_index))
	  {
	    SendMessage(hWndRList, LB_SETCURSEL, r_index, 0L);
	    return NULL;
	  }
	  r_index = (int)SendMessage(hWndRList, LB_GETCURSEL, 0, 0L);
	  if (r_index != LB_ERR && r_index != r_last)
	  {
	    show_round(hWnd, players.player[p_index].p_scores.history[r_index].score);
	    r_last = r_index;
	  } /* if */
	  return NULL;
      }  /* switch (HIWORD(lParam)) */
      return NULL;

    default:
      return FALSE;
  }  /* switch (wParam) */
} /* CommandHandler */
