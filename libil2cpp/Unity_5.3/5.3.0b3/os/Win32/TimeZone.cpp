#include "il2cpp-config.h"

#if IL2CPP_PLATFORM_WIN32

#include "os/TimeZone.h"
#include <cassert>

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
/*
 * Magic number to convert FILETIME base Jan 1, 1601 to DateTime - base Jan, 1, 0001
 */
const uint64_t FILETIME_ADJUST = ((uint64_t)504911232000000000LL);

namespace il2cpp
{
namespace os
{

	
static void
convert_to_absolute_date(SYSTEMTIME *date)
{
#define IS_LEAP(y) ((y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0))
	static int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static int leap_days_in_month[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	/* from the calendar FAQ */
	int a = (14 - date->wMonth) / 12;
	int y = date->wYear - a;
	int m = date->wMonth + 12 * a - 2;
	int d = (1 + y + y/4 - y/100 + y/400 + (31*m)/12) % 7;

	/* d is now the day of the week for the first of the month (0 == Sunday) */

	int day_of_week = date->wDayOfWeek;

	/* set day_in_month to the first day in the month which falls on day_of_week */    
	int day_in_month = 1 + (day_of_week - d);
	if (day_in_month <= 0)
		day_in_month += 7;

	/* wDay is 1 for first weekday in month, 2 for 2nd ... 5 means last - so work that out allowing for days in the month */
	date->wDay = day_in_month + (date->wDay - 1) * 7;
	if (date->wDay > (IS_LEAP(date->wYear) ? leap_days_in_month[date->wMonth - 1] : days_in_month[date->wMonth - 1]))
		date->wDay -= 7;
}

bool TimeZone::GetTimeZoneData (int32_t year, int64_t data[4], std::string names[2])
{
	TIME_ZONE_INFORMATION tz_info;
	FILETIME ft;
	int i;
	int err, tz_id;
	// TODO: Get proper unicode conversion routines
	char daylightName[33] = {0};
	char standardName[33] = {0};

	tz_id = GetTimeZoneInformation (&tz_info);
	if (tz_id == TIME_ZONE_ID_INVALID)
		return 0;

	for (i = 0; i < 32; ++i)
	{
		daylightName[i] = tz_info.DaylightName [i];
		if (!tz_info.DaylightName [i])
			break;
	}
	names [1] = daylightName; // use i

	for (i = 0; i < 32; ++i)
	{
		standardName[i] = tz_info.StandardName [i];
		if (!tz_info.StandardName [i])
			break;
	}
	names [0] = standardName; // use i

	if ((year <= 1601) || (year > 30827)) {
		/*
		 * According to MSDN, the MS time functions can't handle dates outside
		 * this interval.
		 */
		return 1;
	}

	/* even if the timezone has no daylight savings it may have Bias (e.g. GMT+13 it seems) */
	if (tz_id != TIME_ZONE_ID_UNKNOWN) {
		tz_info.StandardDate.wYear = year;
		convert_to_absolute_date(&tz_info.StandardDate);
		err = SystemTimeToFileTime (&tz_info.StandardDate, &ft);
		//g_assert(err);
		if (err == 0)
			return 0;
		
		data[1] = FILETIME_ADJUST + (((uint64_t)ft.dwHighDateTime<<32) | ft.dwLowDateTime);
		tz_info.DaylightDate.wYear = year;
		convert_to_absolute_date(&tz_info.DaylightDate);
		err = SystemTimeToFileTime (&tz_info.DaylightDate, &ft);

		if (err == 0)
			return 0;
		
		data[0] = FILETIME_ADJUST + (((uint64_t)ft.dwHighDateTime<<32) | ft.dwLowDateTime);
	}
	data[2] = (tz_info.Bias + tz_info.StandardBias) * -600000000LL;
	data[3] = (tz_info.DaylightBias - tz_info.StandardBias) * -600000000LL;

	return 1;
}


}
}

#endif 