/*
**************************************************************************
delete.c  Brett A. Morrison

This file contains the routines for deleteing a round, player, and course.
**************************************************************************
*/
#include "golf.h"

void delete_player(HWND hwnd, HWND hwndList, player_array *players, int *p_index,
		   int *r_index, int *r_last)
{
  /* copy the last element in the array to the deletion element */
  (*players).player[*p_index] = (*players).player[(*players).player_numb];

  (*players).player_numb--;
  (*p_index)--;
  show_player(hwnd, hwndList, &(*players), &(*p_index), &(*r_index), &(*r_last));
} /* delete_player */

void delete_course(course_array *courses, int *c_index)
{
  /* copy the last element in the array to the deletion element */
  (*courses).course[*c_index] = (*courses).course[(*courses).course_numb];

  (*courses).course_numb--;
  (*c_index)--;
} /* delete_course */

void delete_round(HWND hwnd, HWND hwndList, player_array *players, int *p_index,
		  int *r_index, int *r_last)
{
  /* copy the last element in the array to the deletion element */
  (*players).player[*p_index].p_scores.history[*r_index] =
    (*players).player[*p_index].p_scores.history[(*players).player[*p_index].p_scores.round_numb];

  (*players).player[*p_index].p_scores.round_numb--;

  show_rounds(hwnd, hwndList, &(*players).player[*p_index].p_scores, &(*r_index), &(*r_last), TRUE);
} /* delete_round */

BOOL delete_rounds(player_array *players, char szc_name[])
{
  int  i, j;
  BOOL bDeleteRounds = FALSE;

  for (i=0; i<=(*players).player_numb; i++)
  {
    for (j=0; j<=(*players).player[i].p_scores.round_numb; j++)
    {
      if (lstrcmp(szc_name, (LPSTR)(*players).player[i].p_scores.history[j].course_name) == 0)
      {  /* need to delete */
	(*players).player[i].p_scores.history[j] =
	  (*players).player[i].p_scores.history[(*players).player[i].p_scores.round_numb];
	(*players).player[i].p_scores.round_numb--;
	j=0;  /* need to start over from the beginning */
	bDeleteRounds = TRUE;
      }  /* if */
    } /* for j */
  } /* for i */
  return bDeleteRounds;
} /* delete_rounds */
