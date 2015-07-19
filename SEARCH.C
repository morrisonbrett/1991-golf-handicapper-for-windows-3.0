#include "golf.h"

extern player_array   players;  /* stores all player element data */
extern course_array   courses;  /* variable to store the courses in memory */
extern int            p_index;  /* keeps track of current record */
extern int            c_index;  /* keeps track of current record */
extern int            r_index;
extern int            r_last;
extern HWND           hWndRList;

/*
**************************************************************************
search.c  Brett A. Morrison

This file contains the routines for searching for a player
**************************************************************************
*/

void SearchFor(HWND hwnd, BOOL bWhich, int iKey, char szSearch_for[], BOOL bBeginning)
{
  int  i;
  BOOL bFound = FALSE;
  char szTemp[SEARCH_LENGTH+1];

  switch(bWhich)
  {
    case IDM_P_SEARCH:
      for ((bBeginning) ? (i = 0) : (i = p_index+1); i<=(players).player_numb && !bFound; i++)
      {
	switch(iKey)
	{
	  case IDD_S_FIRSTNAME:
	    if (lstrcmp(szSearch_for, (players).player[i].p_first) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_LASTNAME:
	    if (lstrcmp(szSearch_for, (players).player[i].p_last) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_STREET:
	    if (lstrcmp(szSearch_for, (players).player[i].p_street) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_CITY:
	    if (lstrcmp(szSearch_for, (players).player[i].p_city) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_STATE:
	    if (lstrcmp(szSearch_for, (players).player[i].p_state) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_ZIP:
	    if (lstrcmp(szSearch_for, (players).player[i].p_zip) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_PHONE:
	    if (lstrcmp(szSearch_for, (players).player[i].p_phone) == 0)
	      bFound = TRUE;
	    break;
	} /* switch iKey */
      } /* for */
      if (bFound)
      {
	if (p_index != i-1) /* need to show - else already on screen */
	{
	  p_index = i-1;
	  show_player(hwnd, hWndRList, &players, &p_index, &r_index, &r_last);
	} /* if */
      } /* if */
      else
      {
	lstrcpy(szTemp, szSearch_for);
	lstrcat(szTemp, " Not Found");
	MessageBeep(0);
	MessageBox(hwnd, szTemp, "Search Player", MB_OK | MB_ICONINFORMATION);
      } /* else */
      break;

    case IDM_C_SEARCH:
      for ((bBeginning) ? (i = 0) : (i = c_index+1); i<=(courses).course_numb && !bFound; i++)
      {
	switch(iKey)
	{
	  case IDD_S_COURSE:
	    if (lstrcmp(szSearch_for, (courses).course[i].c_name) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_STREET:
	    if (lstrcmp(szSearch_for, (courses).course[i].c_street) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_CITY:
	    if (lstrcmp(szSearch_for, (courses).course[i].c_city) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_STATE:
	    if (lstrcmp(szSearch_for, (courses).course[i].c_state) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_ZIP:
	    if (lstrcmp(szSearch_for, (courses).course[i].c_zip) == 0)
	      bFound = TRUE;
	    break;
	  case IDD_S_PHONE:
	    if (lstrcmp(szSearch_for, (courses).course[i].c_phone) == 0)
	      bFound = TRUE;
	    break;
	} /* switch iKey */
      } /* for */
      if (bFound)
      {
	if (c_index != i-1) /* need to show - else already on screen */
	{
	  c_index = i-1;
	  show_course(hwnd, &courses, &c_index);
	} /* if */
      } /* if */
      else
      {
	lstrcpy(szTemp, szSearch_for);
	lstrcat(szTemp, " Not Found");
	MessageBeep(0);
	MessageBox(hwnd, szTemp, "Search Course", MB_OK | MB_ICONINFORMATION);
      } /* else */
      break;
  } /* switch bWhich */
} /* SearchFor */
