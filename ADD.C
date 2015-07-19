/*
**************************************************************************
add.c   Brett A. Morrison

Contains procedures used for adding records.
**************************************************************************
*/

#include "golf.h"

extern char   szAppName[];
extern char   szc_name[];
extern char   szNDTitle[];
extern HWND   hWndRList;
extern HANDLE hInst;
extern BOOL   bNameDate;
extern HWND   hWnd;
extern char   szmm[];
extern char   szdd[];
extern char   szyy[];

char szMessage[] = "Not enough space for operation.\nUse a new file to add more data.";

BOOL AddPlayer(HWND hWnd, player_array *players, int *p_index, int *r_index)
{
  if (!AllocatePlayer(&(*players)))
  {
    MessageBeep(0);
    MessageBox(hWnd, szMessage, szAppName, MB_OK | MB_ICONSTOP);
    return FALSE;
  }

  if (!get_player(hWnd, &(*players), *p_index, *r_index))
     return FALSE;

  (*players).player_numb++;
  (*players).player[(*players).player_numb].p_scores.round_numb = -1;
  *p_index = (*players).player_numb;
  clear_player(hWnd, hWndRList);
  SetFocus(GetDlgItem(hWnd, IDD_PFIRST));
  PrintIndex(hWnd, IDD_PINDEX, *p_index, (*players).player_numb);
  return TRUE;
} /* AddPlayer */

BOOL AddRound(HWND hWnd, player_array *players, int *p_index, int *r_index,
	      int *r_last)
{
  static FARPROC lpfnNameDateDlgProc;

  if (!AllocateRound(&(*players).player[*p_index].p_scores))
  {
    MessageBeep(0);
    MessageBox(hWnd, szMessage, szAppName, MB_OK | MB_ICONSTOP);
    return FALSE;
  }

  if (!get_round(hWnd, &(*players), *p_index, *r_index))
    return FALSE;

  lstrcpy(szNDTitle, "Add Round");
  GetDlgItemText(hWnd, IDD_CNAME, szc_name, COURSE_NAME_LENGTH+1);
  GetSystemDate(szmm, szdd, szyy);
  lpfnNameDateDlgProc = MakeProcInstance((FARPROC)NameDateDlgProc, hInst);
  DialogBox(hInst, "NAMEDATE", hWnd, lpfnNameDateDlgProc);
  FreeProcInstance(NameDateDlgProc); /* Release memory */
  if (bNameDate==IDOK)
  {
    (*players).player[*p_index].p_scores.round_numb++;
    (*players).player[*p_index].p_scores.history[(*players).player[*p_index].p_scores.round_numb].date =
      string_to_date(szmm, szdd, szyy);
    lstrcpy((LPSTR)(*players).player[*p_index].p_scores.history[(*players).player[*p_index].p_scores.round_numb].course_name,
	    szc_name);
    *r_index = *r_last = (*players).player[*p_index].p_scores.round_numb;
    show_rounds(hWnd, hWndRList, &(*players).player[*p_index].p_scores, &(*r_index), &(*r_last), FALSE);
    clear_score(hWnd);
    SendMessage(hWndRList, LB_SETCURSEL, *r_index, 0L);
    SetFocus(GetDlgItem(hWnd, IDD_SC_1));
    return TRUE;
  } /* if */
  return FALSE;
} /* AddRound */

BOOL AddCourse(HWND hWnd, course_array *courses, int *c_index)
{
  if (!AllocateCourse(&(*courses)))
  {
    MessageBeep(0);
    MessageBox(hWnd, szMessage, szAppName, MB_OK | MB_ICONSTOP);
    return FALSE;
  }

  if (!get_course(hWnd, &(*courses), *c_index))
    return FALSE;

  (*courses).course_numb++;
  *c_index = (*courses).course_numb;
  clear_course(hWnd);
  SetFocus(GetDlgItem(hWnd, IDD_CNAME));
  PrintIndex(hWnd, IDD_CINDEX, *c_index, (*courses).course_numb);
  return TRUE;
} /* AddCourse */
