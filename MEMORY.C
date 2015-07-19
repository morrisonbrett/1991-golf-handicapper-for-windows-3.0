/*
**************************************************************************
memory.c	Brett A. Morrison

Contains procedures used for allocating and dealocating memory.
**************************************************************************
*/

#include "golf.h"

BOOL AllocatePlayer(player_array *players)
{
  return (*players).player_numb < MAX_PLAYERS-1;
} /* AllocatePlayer */

BOOL AllocateRound(history_array *history)
{
  return (*history).round_numb < MAX_ROUNDS-1;
} /* AllocateRound */

BOOL AllocateCourse(course_array *courses)
{
  return (*courses).course_numb < MAX_COURSES-1;
} /* AllocateCourse */
