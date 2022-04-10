/*
 * Helper methods
 */

int DaysInMonth(int m)
{
  int dim = 0;

  if ( m < 1 )
    m = 12;

  if ( m == 2 )
    dim = 28;
  else if ( m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12 )
    dim = 31;
  else
    dim = 30;
    
  return dim;
}

int DaysInYear(int year)
{
  return (year % 4) ? 366 : 365;
}
 
time_t OneYearAgo(time_t t)
{
    int diy = 365;
    if ( year(t) % 4 == 0 && month(t) > 2 )
      diy = 366;
    if ( year(t) % 4 == 1 && month(t) < 3 )
      diy = 366;

    return t - diy*secondsInDay;
}

time_t OneMonthAgo(time_t t)
{
    int dim = DaysInMonth(month(t)-1);
    dim = dim < day(t) ? day(t) : dim;
    return t - dim*secondsInDay;
}

time_t FirstDayOfMonth(time_t t)
{
  return t - (day(t)-1)*secondsInDay;
}

