#include "golf.h"

/*
**************************************************************************
unload.c	 Brett A. Morrison

Contains the routines for writing the contents of the player and course
arrays to disk.
**************************************************************************
*/

/*
--------------------------------------------------------------------------
UnloadCourses

Unload one course at a time until end of the array is reached.

Inputs:
  hFile    - the handle to the file to be written to
  *courses - the course array

Outputs:
  None
--------------------------------------------------------------------------
*/
void UnloadCourses(HANDLE hFile, course_array *courses)
{
  int i;

  for (i=0; i<=(*courses).course_numb; i++)
  {
    unload_string(hFile, (*courses).course[i].c_name);
    unload_string(hFile, (*courses).course[i].c_street);
    unload_string(hFile, (*courses).course[i].c_city);
    unload_string(hFile, (*courses).course[i].c_state);
    unload_string(hFile, (*courses).course[i].c_zip);
    unload_string(hFile, (*courses).course[i].c_phone);
    unload_string(hFile, (*courses).course[i].c_rating);
    unload_holes (hFile, (*courses).course[i].hole_par);
    unload_holes (hFile, (*courses).course[i].hole_handicap);
  } /* for */
} /* UnloadCourses */

/*
--------------------------------------------------------------------------
UnloadPlayers

Unload one player at a time until end of the array is reached.

Inputs:
  hFile    - the handle to the file to be written to
  *courses - the player array

Outputs:
  None
--------------------------------------------------------------------------
*/
void UnloadPlayers(HANDLE hFile, player_array *players)
{ /* UnloadPlayers */
  int i;

  for (i=0; i<=(*players).player_numb; i++)
  {
    unload_string(hFile, (*players).player[i].p_first);
    unload_string(hFile, (*players).player[i].p_last);
    unload_string(hFile, (*players).player[i].p_street);
    unload_string(hFile, (*players).player[i].p_city);
    unload_string(hFile, (*players).player[i].p_state);
    unload_string(hFile, (*players).player[i].p_zip);
    unload_string(hFile, (*players).player[i].p_phone);
    unload_history(hFile, &(*players).player[i].p_scores);
  } /* for */
} /* UnloadPlayers */

/*
--------------------------------------------------------------------------
unload_string

Write a string to the hFile by calling _lwrite.

Inputs:
  hFile    - the handle to the file to be written to
  string   - the string to be written

Outputs:
  None
--------------------------------------------------------------------------
*/
void unload_string(HANDLE hFile, char string[])
{ /* unload_string */
  _lwrite(hFile, string, lstrlen((LPSTR)string));
  _lwrite(hFile, "\r\n", 2);  /* terminate the string */
} /* unload_string */

/*
--------------------------------------------------------------------------
unload_holes

Unload the array of scores by calling unload_string

Inputs:
  hFile - the handle to the file to be written to
  score - the array of scores

Outputs:
  None
--------------------------------------------------------------------------
*/
void unload_holes(HANDLE hFile, score_array score)
{
  int  i;
  char temp[SCORE_LENGTH+1];

  for (i=1; i<=NUM_HOLES; i++)
  {
    itoa((short)score[i], temp, 10);
    unload_string(hFile, temp);
  } /* for */
} /* unload_holes */

/*
--------------------------------------------------------------------------
unload_history

Unload the history of each player by calling unload_string and unload_holes

Inputs:
  hFile     - the handle to the file to be written to
  *p_scores - array containing all of the player's scores

Outputs:
  None
--------------------------------------------------------------------------
*/
void unload_history(HANDLE hFile, history_array *p_scores)
{
  int  i;
  char temp[DATE_LENGTH+1];

  for (i=0; i<=(*p_scores).round_numb; i++)
  {
    ltoa((*p_scores).history[i].date, temp, 10);
    unload_string(hFile, temp);
    unload_string(hFile, (*p_scores).history[i].course_name);
    unload_holes(hFile, (*p_scores).history[i].score);
  } /* for */
  unload_string(hFile, "~");
} /* unload_history */
