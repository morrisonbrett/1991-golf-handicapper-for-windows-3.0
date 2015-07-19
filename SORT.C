/*
**************************************************************************
sort.c	Brett A. Morrison

Contains procedures called by qsort.
**************************************************************************
*/

#include "golf.h"

/*
--------------------------------------------------------------------------
compare_course

Returns the value of lstrcmp on two course names.

Inputs:
  *a - course name 1
  *b - course name 2

Outputs:
  The value of lstrcmp
--------------------------------------------------------------------------
*/
int compare_course(const void *a, const void *b)
{
  return lstrcmp((LPSTR)((course_record *)a)->c_name,
		 (LPSTR)((course_record *)b)->c_name);
} /* compare_course */

/*
--------------------------------------------------------------------------
compare_player

Compares two sets of character string keys.  The first key is the last name
if they are equal, the next key is compared, first name.

Inputs:
  *a - player 1
  *b - player 2

Outputs:
  LESS, GREATER, or EQUAL to the value compared.
--------------------------------------------------------------------------
*/
int compare_player(const void *a, const void *b)
{
  int value = lstrcmp((LPSTR)((player_record *)a)->p_last,
		      (LPSTR)((player_record *)b)->p_last);  /* compare last names */
  if (value < 0)
    return LESS;
  else if (value > 0)
    return GREATER;

  /* Last names are equal, need to compare first */
  value = lstrcmp((LPSTR)((player_record *)a)->p_first,
		  (LPSTR)((player_record *)b)->p_first); /* compare first names */
  if (value < 0)
    return LESS;
  else if (value > 0)
    return GREATER;
  return EQUAL;  /* last and first are equal */
} /* compare_player */

/*
--------------------------------------------------------------------------
compare_round

Compares two dates. Sort descending.

Inputs:
  *a - round 1
  *b - round 2

Outputs:
  LESS, GREATER, or EQUAL to the value compared.
--------------------------------------------------------------------------
*/
int compare_round(const void *a, const void *b)
{
  if (((score_record *)a)->date < ((score_record *)b)->date)
    return GREATER;
  else if (((score_record *)a)->date > ((score_record *)b)->date)
    return LESS;
  return EQUAL;
} /* compare_round */
