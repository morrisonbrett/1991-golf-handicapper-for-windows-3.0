#include "golf.h"

/*
**************************************************************************
utils.c	 Brett A. Morrison

Contains some utility helper functions
**************************************************************************
*/

extern char szAppName [];  /* located in golf.c */
/*
--------------------------------------------------------------------------
OkMessageBox

This procedure simply displays a ok message box with the current filename.
It is used for replacing existing files and upon exiting.

Inputs:
  hwnd	      - The handle to the parent window
  *szString   - A pointer to the message to be displayed
  *szFileName - A pointer to the current file name

Outputs:
  None
--------------------------------------------------------------------------
*/
void OkMessageBox(HWND hwnd, char *szString, char *szFileName)
{
  char szBuffer[FULL_PATH_SIZE];  /* Stores message */

  /* Set buffer */
  wsprintf(szBuffer, szString, (LPSTR)szFileName);

  /* Display message */
  MessageBox(hwnd, szBuffer, szAppName, MB_OK | MB_ICONEXCLAMATION);
} /* OkMessageBox */

/*
--------------------------------------------------------------------------
lstrchr

Function to parse a filename string, which searches forward for the next
proper ansi value.

Inputs:
  str - string to look at
  ch  - character to search for

Outputs
  lstrchr - modified string
--------------------------------------------------------------------------
*/
LPSTR lstrchr(LPSTR str, char ch)
{
  while (*str)
  {
    if (ch == *str) /* end of string */
      return str;
    str = AnsiNext(str); /* load next character	*/
  } /* while */
  return NULL;
} /* lstrchr */

/*
--------------------------------------------------------------------------
lstrrchr

Function to parse a filename string, which searches backward for the next
proper ansi value.

Inputs:
  str - string to look at
  ch  - character to search for

Outputs
  lstrrchr - modified string
--------------------------------------------------------------------------
*/
LPSTR lstrrchr(LPSTR str, char ch)
{
  LPSTR strl = str + lstrlen(str);   /* Add length of str to end of str	*/

  do
  {
    if (ch == *strl)   /* end of string */
      return strl;
    strl = AnsiPrev(str, strl);  /* load previous character */
  } /* do */
  while(strl > str);

  return NULL;
} /* lstrchr */

void DoMenu(WORD wParam, player_array *players, course_array *courses, int p_index,
	    HWND hWndRList)
{
  LONG           lSelect;                         /* Stores value of highlighted text */
  WORD           wEnable;                         /* Enables grayed menu items */

  if (GetFocus() == hWndRList)
    EnableMenuItem(wParam, IDM_UNDO, MF_GRAYED);
  else
    EnableMenuItem(wParam, IDM_UNDO,
		   SendMessage(GetFocus(), EM_CANUNDO, 0, 0L) ? MF_ENABLED : MF_GRAYED);

  EnableMenuItem(wParam, IDM_PASTE,
		 IsClipboardFormatAvailable(CF_TEXT) ? MF_ENABLED : MF_GRAYED);

  /* check if text is selected */
  lSelect = SendMessage(GetFocus(), EM_GETSEL, 0, 0L);

  if (HIWORD(lSelect) == LOWORD(lSelect))
    wEnable = MF_GRAYED;
  else
    wEnable = MF_ENABLED;
  EnableMenuItem(wParam, IDM_CUT, wEnable);
  EnableMenuItem(wParam, IDM_COPY, wEnable);
  EnableMenuItem(wParam, IDM_CLEAR, wEnable);

  if ((*players).player_numb == -1)
    wEnable = MF_GRAYED;
  else
    wEnable = MF_ENABLED;
  EnableMenuItem(wParam, IDM_P_SEARCH, wEnable);
  EnableMenuItem(wParam, IDM_P_SORT, wEnable);
  EnableMenuItem(wParam, IDM_R_ADD, wEnable);
  EnableMenuItem(wParam, IDM_PRINT_P, wEnable);
  EnableMenuItem(wParam, IDM_P_DELETE, wEnable);
  EnableMenuItem(wParam, IDM_GRAPH, wEnable);

  if ((*courses).course_numb == -1)
    wEnable = MF_GRAYED;
  else
    wEnable = MF_ENABLED;
  EnableMenuItem(wParam, IDM_C_SEARCH, wEnable);
  EnableMenuItem(wParam, IDM_C_SORT, wEnable);
  EnableMenuItem(wParam, IDM_PRINT_C, wEnable);
  EnableMenuItem(wParam, IDM_C_DELETE, wEnable);

  if (((*players).player_numb == -1) || ((*players).player[p_index].p_scores.round_numb == -1))
    wEnable = MF_GRAYED;
  else
    wEnable = MF_ENABLED;
  EnableMenuItem(wParam, IDM_R_DELETE, wEnable);
  EnableMenuItem(wParam, IDM_R_SORT, wEnable);
} /* DoMenu */