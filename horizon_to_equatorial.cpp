#include <iostream>
#include <math.h>
#include <fstream>
#define PI 3.14159265

/**
 * Horizon to Equatorial Coordinates Conversion
 *
 * Symbols used
 * Symbols used
 * H - Hour Angle
 * δ - Declination
 * A - azimuth
 * A'- Temp azimuth
 * a - altitude
 * φ -  observer’s geographical latitude
 *
 * STEPS INVOLVED
   1. Convert azimuth to decimal degrees
   2. Convert altitude to decimal degrees
   3. Find sinδ = sinasinφ +cosacosφ cosA.
   4. Take inverse sin to find δ .
   5. Find cosH = sina−sinφ sinδ . cosφcosδ ′
   6. Take inverse cos to find H .
   7. Find sin A.
      If negative, true hour angle is H = H'
      If positive, true hour angle is H = 360 - H'
   8. Convert H into hours by dividing by 15
   9. Convert H and δ into minutes and seconds form
 */

typedef long double ld;

void print( ) { ; } //Used like a base case in recursive functions

template<typename T, typename... Types> //Normal template
void print(const T& first, const Types&... args)
{
    std::cout<<first<<" ";
    print(args...);
}

//1. Convert azimuth to decimal degrees
ld azimuth_to_decimal_degrees(ld degree, ld minutes, ld seconds)
{
    return degree + (minutes + (seconds/60)) / 60;
}

//2. Convert altitude to decimal degrees
ld altitude_to_decimal_degrees(ld degree, ld minutes, ld seconds)
{
    return degree + (minutes + (seconds/60)) / 60;
}

//3. Find sinδ = sinasinφ +cosacosφ cosA
ld sin_declination(ld altitude, ld latitude, ld azimuth)
{
    //sinδ = sinasinφ +cosacosφ cosA

    //Degree to Radian
    altitude = altitude*PI/180;
    latitude = latitude*PI/180;
    azimuth = azimuth*PI/180;

    //Return in radian
    return ( ( sin(altitude) * sin(latitude) ) + ( cos(altitude) * cos(latitude) * cos(azimuth) ) );
}

//4. Take inverse sin to find δ
ld declination(ld sin_declination_value)
{
    return asin(sin_declination_value) * 180.0 / PI;
}

//5. Find cosH = sina − sinφ sinδ / cosφ cosδ
ld cosH(ld altitude, ld latitude, ld declination)
{
    //cosH = sina − sinφ sinδ / cosφ cosδ

    //Degree to Radian conversion
    altitude = altitude * PI/180;
    latitude = latitude * PI/180;
    declination = declination * PI/180;

    return ( sin(altitude) - ( sin(latitude) * sin(declination) ) ) / ( cos(latitude) * cos(declination) );
}

//6. Take inverse cos to find H'
ld temp_hour_angle(ld cosH_value)
{
    return acos(cosH_value) * 180.0 / PI;
}

//7. Find sin A
ld sinA(ld azimuth)
{
    azimuth = azimuth*PI/180;
    return sin(azimuth);
}

//7.5
//If negative, true hour angle is H = H'
//If positive, true hour angle is H = 360 - H'
ld hour_angle(ld sinA_value, ld temp_hour_angle_value)
{
    if(sinA_value < 0)
        return abs(temp_hour_angle_value);
    else
        return (360 - temp_hour_angle_value);
}

//8. Convert H into hours by dividing by 15
ld h_to_hours(ld hour_angle)
{
    return hour_angle/15;
}

struct DMS
{
    DMS(double value)
    {
        degrees = floor(value);
        double rem = (value-degrees) * 60;
        minutes = floor(rem);
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

//9. Convert H and δ into minutes and seconds form
void printHorizonCoordinates(ld hour_angle, ld declination)
{
    //A - azimuth
    //A'- Temp azimuth
    //a - altitude
    std::cout<<"HourAngle: ";
    DMS printHourAngle(hour_angle);
    std::cout<<"Declination: ";
    DMS printDeclination(declination);
}

//Wrapping
void horizon_to_equatorial()
{
    ld azimuth_degrees, azimuth_minutes, azimuth_seconds;
    ld altitude_degrees, altitude_minutes, altitude_seconds;
    ld latitude;
    std::cout<<"Enter Horizon Coordinates \nAltitude a (in DMS): ";
    std::cin>>altitude_degrees>>altitude_minutes>>altitude_seconds;
    std::cout<<"\nAzimuth A (in DMS): ";
    std::cin>>azimuth_degrees>>azimuth_minutes>>azimuth_seconds;
    std::cout<<"\nObserver’s Geographical Latitude :";
    std::cin>>latitude;
    std::cout<<"\n";

    ld azimuth_degrees_value = azimuth_to_decimal_degrees(azimuth_degrees, azimuth_minutes, azimuth_seconds);
    ld altitude_degrees_value = altitude_to_decimal_degrees(altitude_degrees, altitude_minutes, altitude_seconds);

    ld declination_value = declination(sin_declination(altitude_degrees_value,latitude,azimuth_degrees_value));

    ld h_to_hours_value = h_to_hours(hour_angle(sinA(azimuth_degrees_value),
            temp_hour_angle(
                    cosH(altitude_degrees_value,latitude, declination_value))));

    printHorizonCoordinates(h_to_hours_value,declination_value);
}

int main()
{
//    freopen("input.txt","r",stdin);

    horizon_to_equatorial();

    /**
     * Examples
     *
     * Example 1
     *
     * Input
     * Altitude a (in DMS): 19 20 3.64
     * Azimuth A (in DMS): 283 16 15.7
     * Observer’s Geographical Latitude φ (in degrees): 52
     *
     * Output
     * HourAngle: 5◦ 51' 44.0002''
     * Declination: 23◦ 13' 9.99831''
     *
     * Verified from (Practical_Astronomy_with_your_Calculator_or_Spreadsheet_4th_edition_by_Peter_Duffett-Smith_Jonathan_Zwart Page 49)
     *
     * Example 2
     * Input
     * Altitude a (in DMS): 40 0 0
     * Azimuth A (in DMS): 115 0 0
     * Observer’s Geographical Latitude : 38
     *
     * Output
     * HourAngle: 21◦ 1' 53.615''
     * Declination: 8◦ 5' 2.55896''
     * Verified from (lawrence_j_l_celestial_calculations_a_gentle_introduction_to Page 89)
     */
}