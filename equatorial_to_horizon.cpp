#include <iostream>
#include <math.h>
#define PI 3.14159265

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time_adjustor.hpp"
#include <boost/date_time/gregorian/greg_date.hpp>

using namespace boost::posix_time;
using boost::gregorian::date;

//Conversion of Julian Day to J2000
long double j2000_format(date a)
{
    date j2000(2000, 01, 01);
    return a.julian_day() - j2000.julian_day();
}

/**
 *Equatorial to Horizon Coordinate Conversion
 *
 * Symbols used
 * H - Hour Angle
 * δ - Declination
 * A - azimuth
 * A'- Temp azimuth
 * a - altitude
 * φ -  observer’s geographical latitude
 *
 * STEPS involved
   1. Convert hour angle to decimal hours
   2. Multiply by 15 to convert H to degrees
   3. Convert δ into decimal degrees
   4. Find sina = sinδ sinφ +cosδ cosφ cosH.
   5. Take inverse sin to find a.
   6. Find cosA = sinδ−sinφ sina / cosφcosa
   7. Take inverse cos to find A'
   8. Find sinH
   If negative, the true azimuth is A = A′
   If positive, the true azimuth is A = 360 − A′
   9. Convert a and A to degrees, minutes and seconds
 *
 */

typedef long double ld;

//1. Convert hour angle to decimal hours
ld hour_angle_to_decimal_hours(ld h, ld m, ld s)
{
    /**
      Assumption 24hour format
      1. Take the number of seconds and divide by 60.
      2. Add this to the number of minutes and divide by 60.
      3. Add the number of hours.
     */
    return h + (m+(s/60))/60;
}

//2. Multiply by 15 to convert H to degrees
ld H_to_degree(ld H)
{
    return H*15;
}

//3. Convert δ into decimal degrees
ld declination_angle_to_decimal_degree(ld degree, ld minutes, ld seconds)
{
    return degree + (minutes + (seconds/60)) / 60;
}

//4. Find sina = sinδ sinφ +cosδ cosφ cosH
ld sina(ld declination, ld latitude, ld H)
{
    //sina = sinδ sinφ +cosδ cosφ cosH

    //Radian to degree conversion
    declination = declination*PI/180;
    latitude = latitude*PI/180;
    H = H*PI/180;
    return sin(declination) * sin(latitude) + cos(declination) * cos(latitude) * cos(H);
}

//5. Take inverse sin to find a
ld altitude(ld sina_val)
{
    return asin(sina_val) * 180.0 / PI;
}

//6. Find cosA = sinδ−sinφ sina / cosφcosa
ld cosA(ld declination, ld latitude, ld H, ld altitude)
{
    //Radian to degree conversion
    declination = declination*PI/180;
    latitude = latitude*PI/180;
    H = H*PI/180;
    altitude = altitude*PI/180;
    return ( sin(declination) - sin(latitude) * sin(altitude) ) / ( cos(latitude) * cos(altitude) );
}

//7. Take inverse cos to find A'
ld temp_azimuth(ld cosa_value)
{
    return acos(cosa_value) * 180.0 / PI;
}

//8. Find sinH
ld sinH(ld H)
{
    H = H*PI/180;
    return sin(H);
}

//8.5.
//If negative, the true azimuth is A = A′(positive).
//If positive, the true azimuth is A = 360 − A′
ld azimuth(ld sinH_value, ld temp_azimuth)
{
    if(sinH_value < 0)
        return abs(temp_azimuth);
    else
        return (360 - temp_azimuth);
}

struct DMS
{
    DMS(double value)
    {
        degrees = std::floor(value);
        double rem = (value-degrees) * 60;
        minutes = std::floor(rem);
        seconds = (rem-minutes) * 60;
        std::cout<<degrees<<"◦ "<<minutes<<"' "<<seconds<<"''"<<std::endl;
    }

    operator double() const
    {
        return degrees + minutes/60 + seconds/3600;
    }

    double degrees;
    double minutes;
    double seconds;
};

//9. Convert a and A to degrees, minutes and seconds.
void printHorizonCoordinates(ld azimuth, ld altitude)
{
    //A - azimuth
    //A'- Temp azimuth
    //a - altitude
    std::cout<<"Azimuth: ";
    DMS printAzimuth(azimuth);
    std::cout<<"\nAltitude: ";
    DMS printAltitude(altitude);
}

//Wrapping
void equatorial_to_horizon()
{
    ld hours,min,sec;
    ld arc_degrees,arc_minutes,arc_seconds;
    ld latitude;
    std::cout<<"Enter Equatorial Coordinates \nHour Angle H (in HMS): ";
    std::cin>>hours>>min>>sec;
    std::cout<<"\nDeclination δ (in DMS): ";
    std::cin>>arc_degrees>>arc_minutes>>arc_seconds;
    std::cout<<"\nObserver’s Geographical Latitude φ (in degrees): ";
    std::cin>>latitude;

    ld hourAngle_in_degrees = H_to_degree(hour_angle_to_decimal_hours(hours,min,sec));
    ld declination_in_degree = declination_angle_to_decimal_degree(arc_degrees,arc_minutes,arc_seconds);

    ld altitude_val = altitude( sina ( declination_in_degree,
            latitude,
            hourAngle_in_degrees) );

    ld azimuth_val = azimuth( sinH(hourAngle_in_degrees),
            temp_azimuth( cosA(declination_in_degree, latitude, hourAngle_in_degrees, altitude_val) ));

    printHorizonCoordinates(azimuth_val,altitude_val);

}

int main()
{
    equatorial_to_horizon();

    /**
     * Examples
     *
     * Example 1
     *
     * Input
     * Hour Angle H (in HMS): 5 51 44
     * Declination δ (in DMS): 23 13 10
     * Observer’s Geographical Latitude φ (in degrees): 52
     *
     * Output
     * 283◦ 16' 15.6976''
     * 19◦ 20' 3.64298''
     *
     * Verified from (Practical_Astronomy_with_your_Calculator_or_Spreadsheet_4th_edition_by_Peter_Duffett-Smith_Jonathan_Zwart Page 47)
     *
     * Example 2
     *
     * Input
     * Hour Angle H (in HMS): 16 29 45
     * Declination δ (in DMS): -0 30 30
     * Observer’s Geographical Latitude φ (in degrees): 25
     *
     * Output
     * 79◦ 33' 26.8251''
     * -21◦ 52' 49.8622''
     *
     * Verified from (lawrence_j_l_celestial_calculations_a_gentle_introduction_to Page 90)
     */
}
