#include "golf.h"

/*
**************************************************************************
validate.c  Brett A. Morrison

This file contains the routines that call edit validation routines.
*************************************************************************
*/
extern char szAppName[];

BOOL get_player(HWND hwnd, player_array *players, int p_index, int r_index)
{
  if ((*players).player_numb == -1)
    return TRUE;

  GetDlgItemText(hwnd, IDD_PFIRST, (*players).player[p_index].p_first, FIRST_NAME_LENGTH+1);
  GetDlgItemText(hwnd, IDD_PLAST, (*players).player[p_index].p_last, LAST_NAME_LENGTH+1);
  GetDlgItemText(hwnd, IDD_PSTREET, (*players).player[p_index].p_street, STREET_LENGTH+1);
  GetDlgItemText(hwnd, IDD_PCITY, (*players).player[p_index].p_city, CITY_LENGTH+1);
  GetDlgItemText(hwnd, IDD_PSTATE, (*players).player[p_index].p_state, STATE_LENGTH+1);
  GetDlgItemText(hwnd, IDD_PPHONE, (*players).player[p_index].p_phone, PHONE_LENGTH+1);
  GetDlgItemText(hwnd, IDD_PZIP, (*players).player[p_index].p_zip, ZIP_LENGTH+1);

  if (!get_round(hwnd, &(*players), p_index, r_index))
    return FALSE;
  return TRUE;
} /* get_player */

BOOL get_course(HWND hwnd, course_array *courses, int c_index)
{
  if ((*courses).course_numb == -1)
    return TRUE;

  GetDlgItemText(hwnd, IDD_CNAME, (*courses).course[c_index].c_name, COURSE_NAME_LENGTH+1);
  GetDlgItemText(hwnd, IDD_CSTREET, (*courses).course[c_index].c_street, STREET_LENGTH+1);
  GetDlgItemText(hwnd, IDD_CCITY, (*courses).course[c_index].c_city, CITY_LENGTH+1);
  GetDlgItemText(hwnd, IDD_CSTATE, (*courses).course[c_index].c_state, STATE_LENGTH+1);
  GetDlgItemText(hwnd, IDD_CZIP, (*courses).course[c_index].c_zip, ZIP_LENGTH+1);
  GetDlgItemText(hwnd, IDD_CPHONE, (*courses).course[c_index].c_phone, PHONE_LENGTH+1);

  if (!get_par(hwnd, &(*courses), c_index) || !get_handicap(hwnd, &(*courses), c_index))
    return FALSE;
  if (!get_rating(hwnd, &(*courses), c_index))
    return FALSE;
  return TRUE;
} /* get_course */

BOOL get_round(HWND hwnd, player_array *players, int p_index, int r_index)
{
  int  i;
  int  iItemId;
  char szTemp[20];
  char szTemp2[4];

  if ((*players).player[p_index].p_scores.round_numb == -1)
    return TRUE;

  for (i=1, iItemId=IDD_SC_1; i<=NUM_HOLES; i++, iItemId++)
  {
    lstrcpy(szTemp, "Score #");
    lstrcat(szTemp, itoa(i, szTemp2, 10));
    if (!CheckEditShort(hwnd, iItemId, szTemp, &(*players).player[p_index].p_scores.history[r_index].score[i], 1, 15))
      return FALSE;
  }
  return TRUE;
} /* get_round */

BOOL get_par(HWND hwnd, course_array *courses, int c_index)
{
  int  i;
  int  iItemId;
  char szTemp[12];
  char szTemp2[12];

  for (i=1, iItemId=IDD_P_1; i<=NUM_HOLES; i++, iItemId++)
  {
    lstrcpy(szTemp, "Par #");
    lstrcat(szTemp, itoa(i, szTemp2, 10));
    if (!CheckEditShort(hwnd, iItemId, szTemp, &(*courses).course[c_index].hole_par[i], 1, 6))
      return FALSE;
  }
  return TRUE;
} /* get_par */

BOOL get_rating(HWND hwnd, course_array *courses, int c_index)
{
  float ftemp;

  GetDlgItemText(hwnd, IDD_CRATING, (*courses).course[c_index].c_rating, RATING_LENGTH+1);
  ftemp = atof((*courses).course[c_index].c_rating);
  if ((ftemp <= 0) || (ftemp > 99)) /* force error- return FALSE */
  {
    MessageBeep(0);
    MessageBox(hwnd, "Range must be between 1 and 99.", szAppName, MB_OK | MB_ICONSTOP);
    SetFocus(GetDlgItem(hwnd, IDD_CRATING));
    return FALSE;
  }
  return TRUE;
} /* get_rating */

BOOL get_handicap(HWND hwnd, course_array *courses, int c_index)
{
  int  i;
  int  iItemId;
  char szTemp[12];
  char szTemp2[12];

  for (i=1, iItemId=IDD_H_1; i<=NUM_HOLES; i++, iItemId++)
  {
    lstrcpy(szTemp, "Handicap #");
    lstrcat(szTemp, itoa(i, szTemp2, 10));
    if (!CheckEditShort(hwnd, iItemId, szTemp, &(*courses).course[c_index].hole_handicap[i], 1, 18))
      return FALSE;
  }
  return TRUE;
} /* get_handicap */
