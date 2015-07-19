/*
**************************************************************************
handicap.c       Brett A. Morrison

This file contains several routines for generating the handicap.
**************************************************************************
*/

#include "golf.h"

extern player_array players;
extern course_array courses;
extern int          p_index;

/*
--------------------------------------------------------------------------
HandicapDlgProc

Displays the handicap for the currrent player.

Inputs:
  hDlg     - Handle to parent window
  message  - Message sent from MainWndProc
  wParam   - Command from MainWndProc

Outputs:
  TRUE, so the calling program can close the dialog box
  FALSE if control is switched to another window
--------------------------------------------------------------------------
*/
BOOL FAR PASCAL HandicapDlgProc(HWND hDlg, WORD message, WORD wParam, LONG lParam)
{
  char szTemp[FIRST_NAME_LENGTH+LAST_NAME_LENGTH+1];
  char szTemp2[4];

  switch(message)                  /* checks message */
  {
    case WM_INITDIALOG:            /* checks if box has already been loaded in memory */
      GetDlgItemText(GetParent(hDlg), IDD_PLAST, (players).player[p_index].p_last, LAST_NAME_LENGTH+1);
      GetDlgItemText(GetParent(hDlg), IDD_PFIRST, (players).player[p_index].p_first, FIRST_NAME_LENGTH+1);
      lstrcpy(szTemp, (LPSTR)(players).player[p_index].p_first);
      lstrcat(szTemp, " ");
      lstrcat(szTemp, (LPSTR)(players).player[p_index].p_last);
      MessageBox(GetParent(hDlg), "44", szTemp, MB_OK);
      SetWindowText(hDlg, szTemp);
      itoa((players).player[p_index].p_scores.round_numb+1, szTemp2, 10);
      lstrcpy(szTemp, "After ");
      lstrcat(szTemp, szTemp2);
      lstrcat(szTemp, " Rounds");
      SetDlgItemText(hDlg, IDD_HNUMROUNDS, szTemp);
      SetDlgItemInt(hDlg, IDD_HANDICAP,
		    (WORD)gen_handicap(&players.player[p_index].p_scores, &courses),
		    FALSE);
      return TRUE;

    case WM_COMMAND:
      switch(wParam)               /* checks command */
      {
	case IDOK:                 /* do nothing and fall through */
	case IDCANCEL:             /* IDOK is same as cancel, so close box */
	  EndDialog(hDlg, NULL);
	  return TRUE;             /* End procedure */
      } /* switch(wParam) */
      break;
  } /* switch(message) */
  return FALSE;                    /* Control has been switched to another window */
} /* HandicapDlgProc */

int gen_handicap(history_array *p_scores, course_array *courses)
{
  return 69;
} /* gen_handicap */
