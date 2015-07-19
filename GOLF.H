/*
**************************************************************************
golf.h  Brett A. Morrison

This file contains all preprocessor statements, type definitions, and
function prototypes.
**************************************************************************
*/
#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include "boxes.h"

#define swap(A,B) ((A)=^(B)=^(A)=^(B))
#define convert_to_digit(A) ((A) - '0')
#define range(A, min, max) (((A) >= (min) ) && ((A) <=(max)))
#define focus(A) (((A) >= IDD_PFIRST) && ((A) <= IDD_SC_18))

#define LESS                  -1
#define EQUAL                  0
#define GREATER                1

#define MAX_CAPTION            60
#define FILE_NAME_SIZE         256
#define FILE_EXTENSION_SIZE    5
#define FULL_NAME_SIZE         256
#define FULL_PATH_SIZE         256
#define COLUMN_WIDTH           80
#define MAX_FILE_SIZE          32000
#define BYTE_SIZE              32767
#define NORMAL_FILE_ATTRIBUTES 0x4010
#define TAB_PIXEL_SETTING      144

#define COURSE_NAME_LENGTH     19
#define FIRST_NAME_LENGTH      20
#define LAST_NAME_LENGTH       20
#define STREET_LENGTH          30
#define CITY_LENGTH            20
#define STATE_LENGTH           2
#define ZIP_LENGTH             10
#define PHONE_LENGTH           14
#define RATING_LENGTH          6
#define DATE_LENGTH            8
#define SCORE_LENGTH           2
#define LONG_MESSAGE_LENGTH    50
#define SEARCH_LENGTH          50
#define NUM_HOLES              18
#define MAX_COURSES            30
#define MAX_PLAYERS            10
#define MAX_ROUNDS             30
#define COURSE_MARKER          'C'
#define PLAYER_MARKER          'P'
#define END_RECORDS_MARKER     '~'

typedef short score_array[NUM_HOLES+1];  /* Stores score at each hole */

typedef struct {                              /* Course record definition */
  char        c_name   [COURSE_NAME_LENGTH+1];
  char        c_street [STREET_LENGTH+1];
  char        c_city   [CITY_LENGTH+1];
  char        c_state  [STATE_LENGTH+1];
  char        c_zip    [ZIP_LENGTH+1];
  char        c_phone  [PHONE_LENGTH+1];
  char        c_rating [RATING_LENGTH+1];
  score_array hole_par;
  score_array hole_handicap;
} course_record;

typedef course_record course_data[MAX_COURSES];

typedef struct {
  course_data course;
  int         course_numb;
} course_array;

typedef struct {            /* Player history record */
  long        date;
  char        course_name [COURSE_NAME_LENGTH+1];
  score_array score;
} score_record;

typedef score_record history_data[MAX_ROUNDS];

typedef struct {
  history_data history;
  int          round_numb;
} history_array;

typedef struct {           /* Player record definition */
  char           p_first  [FIRST_NAME_LENGTH+1];
  char           p_last   [LAST_NAME_LENGTH+1];
  char           p_street [STREET_LENGTH+1];
  char           p_city   [CITY_LENGTH+1];
  char           p_state  [STATE_LENGTH+1];
  char           p_zip    [ZIP_LENGTH+1];
  char           p_phone  [PHONE_LENGTH+1];
  history_array  p_scores;
} player_record;

typedef player_record player_data[MAX_PLAYERS];

typedef struct {
  player_data player;
  int         player_numb;
} player_array;

/* golf.c */
int  PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
int  FAR  PASCAL MainWndProc(HWND, WORD, WORD, LONG);
int  FAR  PASCAL DefaultDlg(HWND, unsigned, WORD, LONG);
int  NEAR PASCAL CommandHandler(WORD, LONG);

/* dlgproc.c */
BOOL FAR PASCAL AboutDlgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL DeleteCourseDlgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL NameDateDlgProc(HWND, WORD, WORD, LONG);
BOOL FAR PASCAL SearchDlgProc(HWND, WORD, WORD, LONG);
short AskAboutSave(HWND, char *, char);
void DoCaption(HWND, char *, char *);

/* files.c */
long FileLength(HANDLE);
BOOL ReadFile(HWND, char *, char *, char *, BOOL,
	      player_array *, course_array *, char);
BOOL WriteFile(HWND, char *, player_array *, course_array *, char,
	       int, int, int);

/* print.c */
BOOL PrintFile(HANDLE, HWND, char *);
BOOL FAR PASCAL AbortProc(HDC, short);
BOOL FAR PASCAL PrintDlgProc(HWND, WORD, WORD, LONG);

/* sort.c */
int compare_course(const void *, const void *);
int compare_player(const void *, const void *);
int compare_round(const void *, const void *);

/* utils.c */
LPSTR lstrchr(LPSTR, char);
LPSTR lstrrchr(LPSTR, char);
void OkMessageBox(HWND, char *, char *);
void GraphData(history_array *, course_array *);
void DoMenu(WORD, player_array *, course_array *, int, HWND);

/* dates.c */
void convert_date(long *, char []);
void get_date_elements(char [], char [], char [], char []);
void GetSystemDate(char [], char [], char []);
long string_to_date(char [], char [], char []);
void add0(char []);
long JulianDate(int, int, int);
void CalendarDate(long, int *, int *, int *);
BOOL GoodDate(int, int, int);

/* load.c */
void LoadCourses(LPSTR, course_array *);
void LoadPlayers(LPSTR, player_array *);
void load_string(long *, LPSTR , char[]);
void load_holes(long *, LPSTR, score_array);
void load_history(long *, LPSTR, history_array *);

/* unload.c */
void UnloadCourses(HANDLE, course_array *);
void UnloadPlayers(HANDLE, player_array *);
void unload_string(HANDLE, char[]);
void unload_holes(HANDLE, score_array);
void unload_history(HANDLE, history_array *);

/* display.c */
void show_player(HWND, HWND, player_array *, int *, int *, int *);
void clear_player(HWND, HWND);
void show_course(HWND, course_array *, int *);
void clear_score(HWND);
void clear_course(HWND);
void show_rounds(HWND, HWND, history_array *, int *, int *, BOOL);
void move_course(HWND, WORD, course_array *, int *);
void move_player(HWND, HWND, WORD, player_array *, int *, int *, int *);
void show_round(HWND, score_array);
void PrintIndex(HWND, int, int, int);
void ClearItems(HWND, int, int);

/* delete.c */
void delete_round(HWND, HWND, player_array *, int *, int *, int *);
void delete_player(HWND, HWND, player_array *, int *, int *, int *);
void delete_course(course_array *, int *);
BOOL delete_rounds(player_array *, char []);

/* search.c */
void SearchFor(HWND, BOOL, int, char [], BOOL);

/* validate.c */
BOOL get_par(HWND, course_array *, int);
BOOL get_handicap(HWND, course_array *, int);
BOOL get_rating(HWND, course_array *, int);
BOOL get_player(HWND, player_array *, int, int);
BOOL get_course(HWND, course_array *, int);
BOOL get_round(HWND, player_array *, int, int);

/* edit.c */
BOOL CheckEditShort(HWND, int, char [], short *, short, short);

/* add.c */
BOOL AddPlayer(HWND, player_array *, int *, int *);
BOOL AddRound(HWND, player_array *, int *, int *, int *);
BOOL AddCourse(HWND, course_array *, int *);

/* memory.c */
BOOL AllocateCourse(course_array *);
BOOL AllocateRound(history_array *);
BOOL AllocatePlayer(player_array *);

/* handicap.c */
BOOL FAR PASCAL HandicapDlgProc(HWND, WORD, WORD, LONG);
int gen_handicap(history_array *, course_array *);

