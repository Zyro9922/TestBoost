#include <iostream>
#include <math.h>
#include <tuple>
#include <cmath>
#include <math.h>

#define PI 3.14159265

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time_adjustor.hpp"
#include <boost/date_time/gregorian/greg_date.hpp>

using namespace boost::posix_time;
using boost::gregorian::date;

typedef long double ld;

void print( ) { ; } //Used like a base case in recursive functions

template<typename T, typename... Types> //Normal template
void print(const T& first, const Types&... args)
{
    std::cout<<first<<" ";
    print(args...);
}

class day_month_year
{
public:
    ld day, month, year;

    day_month_year(ld d, ld m, ld y)
    {
        day = d;
        month = m;
        year = y;
    }
};

ld get_Julian_Day_Number(ld d, ld m, ld y)
{
    ld _m,_y;
    if(m == 1 || m == 2)
    {
        _m = m + 12;
        _y = y - 1;
    } else {
        _m = m;
        _y = y;
    }

    ld A = 0,B = 0,C = 0,D = 0;

    if(y >= 1582)
    {
        A = trunc(_y/100);
        B = 2 - A + trunc(A/4);
    }
    else
        B = 0;

    if(_y < 0)
        C = trunc((365.25*_y)-0.75);
    else
        C = trunc(365.25*_y);

    D = trunc(30.6001 * (_m + 1));

    ld JD = B + C + D + d + 1720994.5;
    return JD;
}

day_month_year Julian_Date_to_GreenWhich(ld JD)
{
    JD = JD + 0.5;

    ld I;
    ld F = modf(JD, &I);

    ld B,C,D,E,G;
    if(I > 2299160)
    {
        ld A = trunc( (I - 1867216.25) / 36524.25);
        B = I + A - trunc(A/4) + 1;
    } else  B = I;

    C = B + 1524;
    D = trunc( (C - 122.1)/365.25 );
    E = trunc(365.25 * D);
    G = trunc( (C - E) / 30.6001 );

    ld day = C - E + F - trunc(30.6001 * G);

    ld m;

    if( G < 13.5 )
        m = G - 1;
    else
        m = G - 13;

    ld y;

    if( m > 2.5 )
        y = D - 4716;
    else
        y = D - 4715;

    return day_month_year(day,m,y);
}

ld to_decimal_hours(ld hours, ld min, ld sec)
{
    return hours + (min + (sec/60))/60;
}

ld to_zone_time(ld hours, ld min, ld sec, ld DST)
{
    hours = hours - DST;
    return to_decimal_hours(hours, min, sec);
}

ld get_UT(ld hours, ld time_zone)
{
    return hours - time_zone;
}

ld get_greenwhich_calendar_day(ld local_calendar_day, ld UT)
{
    return local_calendar_day + (UT/24.0);
}

day_month_year GD_day_month_year(ld greenwhich_calendar_day,ld LCTday, ld LCTmonth, ld LCTyear)
{
    date d(LCTyear, LCTmonth, LCTday);
    ld JD = get_Julian_Day_Number(greenwhich_calendar_day,LCTmonth,LCTyear);

    std::cout<<std::endl<<"JD: "<<JD<<std::endl;


    return Julian_Date_to_GreenWhich(2456474.442);

}

ld UT_Final(day_month_year GD)
{
    ld I;
    ld F = modf(GD.day, &I);
    return F * 24;
}

//Conversion of Julian Day to J2000
ld j2000_format(date a)
{
    date j2000(2000, 01, 01);
    return a.julian_day() - j2000.julian_day();
}


void get_UT_GD(ld LCT_hour,ld LCT_min,ld LCT_sec, ld DST,ld time_zone,
        ld local_calendar_day, ld local_calendar_month, ld local_calendar_year)
{
    ld zone_time = to_zone_time(LCT_hour,LCT_min,LCT_sec,DST);
    std::cout<<std::endl<<"ZoneTime: "<<zone_time<<std::endl;
    ld temp_UT = get_UT(zone_time,time_zone);
    std::cout<<std::endl<<"TempUT: "<<temp_UT<<std::endl;
    ld GCD = get_greenwhich_calendar_day(local_calendar_day,temp_UT);
    std::cout<<std::endl<<"GCD: "<<GCD<<std::endl;
    day_month_year dmy1 = GD_day_month_year(GCD,local_calendar_day,local_calendar_month,local_calendar_year);
    ld ut_final_value = UT_Final(dmy1);

    std::cout<<std::endl<<std::endl<<std::cout<<"GDAY: "<<dmy1.day<<" GMONTH: "<<dmy1.month<<" GYEAR: "<<dmy1.year<<std::endl;
    std::cout<<"UT: "<<ut_final_value;
}

void take_input()
{
    ld LCT_hour, LCT_min, LCT_sec, DST, time_zone,
            local_calendar_day, local_calendar_month, local_calendar_year;

    std::cout<<"Enter Hour: ";
    std::cin>>LCT_hour>>LCT_min>>LCT_sec;
    std::cout<<std::endl;
    std::cout<<"Enter DST: ";
    std::cin>>DST;
    std::cout<<std::endl;
    std::cout<<"Enter time_zone: ";
    std::cin>>time_zone;
    std::cout<<"Enter DMY: ";
    std::cin>>local_calendar_day>>local_calendar_month>>local_calendar_year;

    get_UT_GD(LCT_hour, LCT_min, LCT_sec, DST, time_zone,
              local_calendar_day, local_calendar_month, local_calendar_year);
}

int main()
{
    freopen("input.txt","r",stdin);

    take_input();
}