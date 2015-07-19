#include "golf.h"

/*
**************************************************************************
load.c	 Brett A. Morrison

Contains the routines for loading the character buffer into the course
and player arrays.
**************************************************************************
*/

/*
--------------------------------------------------------------------------
LoadCourses

Load one course at a time until end of the buffer is reached.

Inputs:
  lpBuffer - the character buffer which stores all the information
  *courses - the course array

Outputs:
  None
--------------------------------------------------------------------------
*/
void LoadCourses(LPSTR lpBuffer, course_array *courses)
{
  long pointer=3;	 /* pointer to position in buffer array */

  (*courses).course_numb = -1;  /* counts number of courses in array */

  while (lpBuffer[pointer] != '\0')  /* do search until the end of buffer */
  {
    (*courses).course_numb++;  /* add one to the counter */
    /* load each element of the record */
    load_string(&pointer, lpBuffer, (*courses).course[(*courses).course_numb].c_name);
    load_string(&pointer, lpBuffer, (*courses).course[(*courses).course_numb].c_street);
    load_string(&pointer, lpBuffer, (*courses).course[(*courses).course_numb].c_city);
    load_string(&pointer, lpBuffer, (*courses).course[(*courses).course_numb].c_state);
    load_string(&pointer, lpBuffer, (*courses).course[(*courses).course_numb].c_zip);
    load_string(&pointer, lpBuffer, (*courses).course[(*courses).course_numb].c_phone);
    load_string(&pointer, lpBuffer, (*courses).course[(*courses).course_numb].c_rating);
    load_holes (&pointer, lpBuffer, (*courses).course[(*courses).course_numb].hole_par);
    load_holes (&pointer, lpBuffer, (*courses).course[(*courses).course_numb].hole_handicap);
  } /* while */
} /* LoadCourses */

/*
--------------------------------------------------------------------------
LoadPlayers

Load one player at a time until end of the buffer is reached.

Inputs:
  lpBuffer - the character buffer which stores all the information
  *players - the player array

Outputs:
  None
--------------------------------------------------------------------------
*/
void LoadPlayers(LPSTR lpBuffer, player_array *players)
{
  long pointer=3;	       /* pointer to position in buffer array */

  (*players).player_numb = -1;

  while (lpBuffer[pointer] != '\0')  /* do search until the end of buffer */
  {
    (*players).player_numb++;
    /* get the data */
    load_string(&pointer,  lpBuffer, (*players).player[(*players).player_numb].p_first);
    load_string(&pointer,  lpBuffer, (*players).player[(*players).player_numb].p_last);
    load_string(&pointer,  lpBuffer, (*players).player[(*players).player_numb].p_street);
    load_string(&pointer,  lpBuffer, (*players).player[(*players).player_numb].p_city);
    load_string(&pointer,  lpBuffer, (*players).player[(*players).player_numb].p_state);
    load_string(&pointer,  lpBuffer, (*players).player[(*players).player_numb].p_zip);
    load_string(&pointer,  lpBuffer, (*players).player[(*players).player_numb].p_phone);
    load_history(&pointer, lpBuffer, &(*players).player[(*players).player_numb].p_scores); /* load the list */
  } /* while */
} /* LoadPlayers */

/*
--------------------------------------------------------------------------
load_string

Gets the next item from the buffer

Inputs:
  *pointer - pointer to the current character in the buffer
  lpbuffer - the character buffer

Outputs:
  string   - the string that is extracted
--------------------------------------------------------------------------
*/
void load_string(long *pointer, LPSTR lpBuffer, char string[])
{
  int x;  /* loop counter */

  /* search for eoln, and copy values */
  for (x=0; (lpBuffer[*pointer] != '\r'); x++, (*pointer)++)
    string[x] = lpBuffer[*pointer];

  string[x]   = '\0';   /* terminate the string */
  (*pointer) += 2;      /* set the pointer to the start of the next string */

} /* load_string */

/*
--------------------------------------------------------------------------
load_holes

Loads the array with the appropriate numerical value for each hole.

Inputs:
  *pointer - pointer to the current character in the buffer
  lpbuffer - the character buffer

Outputs:
  holes    - the array to store the data
--------------------------------------------------------------------------
*/
void load_holes(long *pointer, LPSTR lpBuffer, score_array holes)
{
  int  i;    /* loop counter */
  char temp[SCORE_LENGTH+1];

  /* get the next line it is of length 1 */
  /* add 2 to pointer to account for the '\n' */
  for (i=1; i<=NUM_HOLES; i++)
  {
    load_string(pointer, lpBuffer, temp);
    holes[i] = (short)atoi(temp);
  } /* for */
} /* load_holes */

/*
--------------------------------------------------------------------------
load_history

Loads the scores from a players previous golf rounds.

Inputs:
  *pointer - pointer to the current character in the buffer
  lpbuffer - the character buffer
  p_scores - the array of rounds list for the current player

Outputs:
  None
--------------------------------------------------------------------------
*/
void load_history(long *pointer, LPSTR lpBuffer, history_array *p_scores)
{
  char         date[DATE_LENGTH+1];

  (*p_scores).round_numb = -1;

  while (lpBuffer[*pointer] != END_RECORDS_MARKER)
  {
    (*p_scores).round_numb++;
    load_string(pointer, lpBuffer, date);
    (*p_scores).history[(*p_scores).round_numb].date = atol(date);
    load_string(pointer, lpBuffer,
		(*p_scores).history[(*p_scores).round_numb].course_name);
    load_holes(pointer, lpBuffer,
		(*p_scores).history[(*p_scores).round_numb].score);
  } /* for */
  (*pointer) += 3;  /* set the pointer to the start of the next string */
} /* load_history */
