#include "golf.h"

/*
**************************************************************************
display.c  Brett A. Morrison

This file contains the routines for displaying a player or course to the
screen.
**************************************************************************
*/

/*
--------------------------------------------------------------------------
show_player

Displays the player in the array indicated by p_index.

Inputs:
  hwnd      - handle to main window
  hwndList  - handle to list box displaying the rounds
  *players  - player array
  *p_index  - index user selects
  *r_index  - the round index
  bNew      - if FALSE then need to error check

Outputs:
  none
--------------------------------------------------------------------------
*/
void show_player(HWND hwnd, HWND hwndList, player_array *players, int *p_index,
		 int *r_index, int *r_last)
{
  if ((*players).player_numb == -1)       /* no records exist */
  {
    clear_player(hwnd, hwndList);
    clear_score(hwnd);
    return;
  } /* if */

  if (*p_index == -1)      /* at the beginning of array, set to end */
    *p_index = (*players).player_numb;
  else if (*p_index > (*players).player_numb)  /* at the end, set to beginning */
    *p_index = 0;

  /* display record */
  SetDlgItemText(hwnd, IDD_PFIRST,  (*players).player[*p_index].p_first);
  SetDlgItemText(hwnd, IDD_PLAST,   (*players).player[*p_index].p_last);
  SetDlgItemText(hwnd, IDD_PSTREET, (*players).player[*p_index].p_street);
  SetDlgItemText(hwnd, IDD_PCITY,   (*players).player[*p_index].p_city);
  SetDlgItemText(hwnd, IDD_PSTATE,  (*players).player[*p_index].p_state);
  SetDlgItemText(hwnd, IDD_PZIP,    (*players).player[*p_index].p_zip);
  SetDlgItemText(hwnd, IDD_PPHONE,  (*players).player[*p_index].p_phone);

  PrintIndex(hwnd, IDD_PINDEX, *p_index, (*players).player_numb);

  show_rounds(hwnd, hwndList, &(*players).player[*p_index].p_scores, &(*r_index), &(*r_last), TRUE);
} /* show_player */

/*
--------------------------------------------------------------------------
show_course

Displays the course in the array indicated by c_index.

Inputs:
  hwnd      - handle to main window
  *courses  - course array
  *c_index  - index user selects

Outputs:
  none
--------------------------------------------------------------------------
*/
void show_course(HWND hwnd, course_array *courses, int *c_index)
{
  char temp[SCORE_LENGTH+1];  /* used for ltoa */
  int  i;
  int  iItemId;

  if ((*courses).course_numb == -1)  /* no courses in array */
  {
    clear_course(hwnd);
    return;
  } /* if */

  if (*c_index == -1)                   /* at beginning, set to end */
    *c_index = (*courses).course_numb;
  else if (*c_index > (*courses).course_numb)  /* at end, set to beginning */
    *c_index = 0;

  SetDlgItemText(hwnd, IDD_CNAME,   (*courses).course[*c_index].c_name);
  SetDlgItemText(hwnd, IDD_CSTREET, (*courses).course[*c_index].c_street);
  SetDlgItemText(hwnd, IDD_CCITY,   (*courses).course[*c_index].c_city);
  SetDlgItemText(hwnd, IDD_CSTATE,  (*courses).course[*c_index].c_state);
  SetDlgItemText(hwnd, IDD_CZIP,    (*courses).course[*c_index].c_zip);
  SetDlgItemText(hwnd, IDD_CPHONE,  (*courses).course[*c_index].c_phone);
  SetDlgItemText(hwnd, IDD_CRATING, (*courses).course[*c_index].c_rating);

  PrintIndex(hwnd, IDD_CINDEX, *c_index, (*courses).course_numb);

  for (i=1, iItemId=IDD_P_1; i<=NUM_HOLES; i++, iItemId++)
    SetDlgItemText(hwnd, iItemId, itoa((*courses).course[*c_index].hole_par[i], temp, 10));

  for (i=1, iItemId=IDD_H_1; i<=NUM_HOLES; i++, iItemId++)
    SetDlgItemText(hwnd, iItemId, itoa((*courses).course[*c_index].hole_handicap[i], temp, 10));
} /* show_course */

/*
--------------------------------------------------------------------------
show_round

Displays the round in the array indicated by r_index.

Inputs:
  hwnd      - handle to main window
  score     - scores

Outputs:
  none
--------------------------------------------------------------------------
*/
void show_round(HWND hwnd, score_array score)
{
  char temp[SCORE_LENGTH+1];  /* used for ltoa */
  int  i;
  int  iItemId;

  for (i=1, iItemId=IDD_SC_1; i<=NUM_HOLES; i++, iItemId++)
    SetDlgItemText(hwnd, iItemId, itoa(score[i], temp, 10));
} /* show_round */
/*
--------------------------------------------------------------------------
show_rounds

Displays the rounds of the current player

Inputs:
  hwnd      - handle to the main window
  hwndList  - handle to list box displaying the rounds
  *history  - the array containing the dates, scores, and courses of player
Outputs:
  none
--------------------------------------------------------------------------
*/
void show_rounds(HWND hwnd, HWND hwndList, history_array *p_scores, int *r_index,
		 int *r_last, BOOL bReset)
{
  int  i;
  char cname_date[COURSE_NAME_LENGTH+DATE_LENGTH+1];
  char cdate[DATE_LENGTH+1];

  SendMessage(hwndList, WM_SETREDRAW, FALSE, 0L);  /* turn display off */
  SendMessage(hwndList, LB_RESETCONTENT, 0, 0L);   /* empty the listbox */

  if (bReset)
    *r_index = *r_last= 0;

  for (i=0; i<=(*p_scores).round_numb; i++)   /* add each round to listbox */
  {
    convert_date(&(*p_scores).history[i].date, cdate);
    lstrcpy((LPSTR)cname_date, (LPSTR)(*p_scores).history[i].course_name);
    lstrcat((LPSTR)cname_date, "\t");
    lstrcat((LPSTR)cname_date, (LPSTR)cdate);
    SendMessage(hwndList, LB_ADDSTRING, 0, (LONG)cname_date);
  } /* for */

  SendMessage(hwndList, WM_SETREDRAW, TRUE, 0L);
  InvalidateRect(hwndList, NULL, TRUE);

  if ((*p_scores).round_numb > -1)
  {
    SendMessage(hwndList, LB_SETCURSEL, *r_index, 0L);
    if (bReset)
      show_round(hwnd, (*p_scores).history[*r_index].score);
  }
  else
    clear_score(hwnd);
} /* show_rounds */

/*
--------------------------------------------------------------------------
clear_player

Clears the player information area by setting the items to nothing

Inputs:
  hwnd      - handle to main window
  hwndList  - handle to list box displaying the rounds

Outputs:
  none
--------------------------------------------------------------------------
*/
void clear_player(HWND hwnd, HWND hwndList)
{
  ClearItems(hwnd, IDD_PFIRST, IDD_PPHONE);
  SendMessage(hwndList, LB_RESETCONTENT, 0, 0L);   /* empty the listbox */
  InvalidateRect(hwndList, NULL, TRUE);  /* this is new */
  clear_score(hwnd);

  PrintIndex(hwnd, IDD_PINDEX, -1, -1);
} /* clear_player */

void clear_course(HWND hwnd)
{
  ClearItems(hwnd, IDD_CNAME, IDD_CRATING);
  PrintIndex(hwnd, IDD_CINDEX, -1, -1);
  ClearItems(hwnd, IDD_P_1, IDD_P_18);
  ClearItems(hwnd, IDD_H_1, IDD_H_18);
} /* clear_course */

void clear_score(HWND hwnd)
{
  ClearItems(hwnd, IDD_SC_1, IDD_SC_18);
} /* clear_score */

/*
--------------------------------------------------------------------------
move_player

Moves the index forwards or backwords, depending on the message, calls
show_player.

Inputs:
  hwnd      - handle to main window
  hwndList  - handle to list box displaying the rounds
  wParam    - message determining whether to increment or decrement
  *players  - the player array
  *p_index  - index of current player

Outputs:
  none
--------------------------------------------------------------------------
*/
void move_player(HWND hwnd, HWND hwndList, WORD wParam, player_array *players,
		 int *p_index, int *r_index, int *r_last)
{
  switch(wParam)
  {
    case SB_LINEUP:
      if (!get_player(hwnd, &(*players), *p_index, *r_index))
	return;
      (*p_index)--;
      break;
    case SB_LINEDOWN:
      if (!get_player(hwnd, &(*players), *p_index, *r_index))
	return;
      (*p_index)++;
      break;
    default:
      return;
  } /* switch */
  show_player(hwnd, hwndList, players, &(*p_index), &(*r_index), &(*r_last));
} /* move_player */

/*
--------------------------------------------------------------------------
move_course

Moves the index forwards or backwords, depending on the message, calls
show_course.

Inputs:
  hwnd      - handle to main window
  wParam    - message determining whether to increment or decrement
  *courses  - the course array
  *c_index  - index of current player

Outputs:
  none
--------------------------------------------------------------------------
*/
void move_course(HWND hwnd, WORD wParam, course_array *courses, int *c_index)
{
  switch(wParam)
  {
    case SB_LINEUP:
      if (!get_course(hwnd, &(*courses), *c_index))
	return;
      (*c_index)--;
      break;
    case SB_LINEDOWN:
      if (!get_course(hwnd, &(*courses), *c_index))
	return;
      (*c_index)++;
      break;
    default:
      return;
  } /* switch */
  show_course(hwnd, courses, c_index);
} /* move_course */

void PrintIndex(HWND hwnd, int iDialogid, int iMin, int iMax)
{
  char temp[10];
  char temp2[3];

  lstrcpy(temp, itoa(iMin+1, temp2, 10));
  lstrcat(temp, " of ");
  lstrcat(temp, itoa(iMax+1, temp2, 10));

  SetDlgItemText(hwnd, iDialogid, temp);
} /* PrintIndex */

void ClearItems(HWND hWnd, int min, int max)
{
  int i;

  for (i=min; i<=max; i++)
    SetDlgItemText(hWnd, i, "");
} /* ClearItems */