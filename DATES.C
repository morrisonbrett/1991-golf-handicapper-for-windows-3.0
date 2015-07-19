#include "golf.h"

/*
**************************************************************************
dates.c	 Brett A. Morrison

Contains date validation, and conversion functions
**************************************************************************
*/

/*
--------------------------------------------------------------------------
convert_date

Converts a long date in the format 9102025, where 91 is the year, 02 is the
month, and 25 is the day to a readable format 02/25/91.

Inputs:
  *date - the long form of the date
  cdate - the string containing the formatted date

Outputs
  none
--------------------------------------------------------------------------
*/
void convert_date(long *date, char cdate[])
{
  long  remainder;
  short mo, dy, yr;
  char  temp[3];

  yr = (short)floor(*date / 10000);
  remainder = *date % 10000;
  mo = (short)floor(remainder / 100);
  dy = remainder % 100;

  lstrcpy((LPSTR)cdate, "");
  if (mo < 10)
    lstrcat((LPSTR)cdate, "0");
  lstrcat((LPSTR)cdate, (LPSTR)itoa((int)mo, temp, 10));
  lstrcat((LPSTR)cdate, "/");
  if (dy < 10)
    lstrcat((LPSTR)cdate, "0");
  lstrcat((LPSTR)cdate, (LPSTR)itoa((int)dy, temp, 10));
  lstrcat((LPSTR)cdate, "/");
  if (yr < 10)
    lstrcat((LPSTR)cdate, "0");
  lstrcat((LPSTR)cdate, (LPSTR)itoa((int)yr, temp, 10));
} /* convert_date */

void get_date_elements(char szDate[], char szmm[], char szdd[], char szyy[])
{
  szmm[0] = szDate[0];
  szmm[1] = szDate[1];
  szmm[2] = '\0';

  szdd[0] = szDate[3];
  szdd[1] = szDate[4];
  szdd[2] = '\0';

  szyy[0] = szDate[6];
  szyy[1] = szDate[7];
  szyy[2] = '\0';
} /* get_date_elements */

void GetSystemDate(char szmm[], char szdd[], char szyy[])
{
  struct date d;

  getdate(&d);

  itoa(d.da_mon, szmm, 10);
  itoa(d.da_day, szdd, 10);
  itoa(d.da_year % 1900, szyy, 10);
} /* GetSystemDate */

long string_to_date(char szmm[], char szdd[], char szyy[])
{
  char sztemp[DATE_LENGTH+1];

  lstrcpy(sztemp, szyy);
  lstrcat(sztemp, szmm);
  lstrcat(sztemp, szdd);

  return atol(sztemp);
} /* string_to_date */

void add0(char szvalue[])
{
  char temp[3];

  if (lstrlen(szvalue) == 1)
  {
    lstrcpy((LPSTR)temp, (LPSTR)szvalue);
    lstrcpy((LPSTR)szvalue, "0");
    lstrcat((LPSTR)szvalue, (LPSTR)temp);
  } /* if */
} /* add0 */

void GraphData(history_array *p_scores, course_array *courses)
{
} /* GraphData */

/* Calculate the Julian day number for the specified day, month, and year */
long JulianDate(int imm, int idd, int iyy)
{
  int   a, b;
  float year_corr;

  /* Correct for negative year */
  year_corr = (iyy > 0 ? 0.0 : 0.75);
  if (imm <= 2)
  {
    iyy--;
    imm += 12;
  } /* if */
  b = 0;
  /* Handle the Gregorian calendar reform */
  if (iyy * 10000.0 + imm * 100.0 + idd >= 15821015.0)
  {
    a = iyy / 100;
    b = 2 - a + a / 4;
  } /* if */
  return (long) (long) (365.25 * iyy - year_corr) +
	 (long) (30.6001 * (imm + 1)) + idd + 1720994L + b;
} /* JulianDate */

/* Calculate the day, month, and year corresponding to a Julian day number */
void CalendarDate(long lJulian, int *imm, int *idd, int *iyy)
{
  long a, b, c, d, e, z, alpha;

  z = lJulian + 1;

  /* Handle the Gregorian calendar reform */
  if (z < 2299161L)
    a = z;
  else
  {
    alpha = (long) ((z - 1867216.25) / 36524.25);
    a = z + 1 + alpha - alpha / 4;
  } /* else */
  b = a + 1524;
  c = (long) ((b - 122.1) / 365.25);
  d = (long) (365.25 * c);
  e = (long) ((b - d) / 30.6001);
  *idd = (int) b - d - (long) (30.6001 * e);
  *imm = (int) (e < 13.5) ? e - 1 : e - 13;
  *iyy = (int) (*imm > 2.5) ? (c - 4716) : c - 4715;
} /* CalendarDate */

/* Validate the specified day, month, and year.  For example, the following
   Dates are not valid : 2/29/69, 4/30/91 */
BOOL GoodDate(int imm, int idd, int iyy)
{
  int cal_day, cal_month, cal_year;
  /* Convert date to Julian day number and back */
  CalendarDate(JulianDate(imm, idd, iyy), &cal_month, &cal_day, &cal_year);
  /* Date is valid if day, month, and year did not change */
  return ((imm == cal_month) && (idd == cal_day) && (iyy == cal_year));
} /* GoodDate */