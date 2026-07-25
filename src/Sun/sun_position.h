#ifndef __SUN_POSITION_H__
#define __SUN_POSITION_H__

#include <math.h>
#define PI 3.14159265358979323846

/******** time data structure *********************/
typedef struct
{
    double year;              /* year */
    double month;             /* month */
    double day;               /* day */
    double hour;              /* time */
    double min;               /* points */
    double sec;               /* seconds */
    double UTC;               /* time zone */
} _YOUR_TIME;

/******** location structure *********************/
typedef struct
{
    double longitude;         /* Longitude */
    double latitude;          /* Latitude */
    double altitude;          /* altitude */
} _YOUR_LOCATION;

typedef struct
{
    double day;
    double ephemeris_day;
    double century;
    double ephemeris_century;
    double ephemeris_millenium;
    double JME[5];
    double JCE[3];
} _JULIAN;

typedef struct {
    double longitude;
    double latitude;
    double radius;
}_EARTH_HELI_POSITION;

typedef struct {
    double longitude;
    double latitude;
}_SUN_GEO_POSITION;

typedef struct {
    double longitude;
    double obliquity;
}_NUTATION;

typedef struct {
    double rigth_ascension_parallax;
    double rigth_ascension;
    double declination;
} _TOPCENTER_SUN_POSITION;

typedef struct {
    double zenith;
    double azimuth;
}_SUN_VIEW_LOCATION;

typedef struct {
    unsigned int start;
    unsigned int end;
}_SUN_SHINE;


class SunPositionModule
{
public:
    _SUN_VIEW_LOCATION sun_pos;
    _YOUR_TIME time_earth;
    _YOUR_LOCATION location_earth;
    _SUN_SHINE Shine;

    /*****************************************************************************************************************************************
    * Step-by-step sub-function declaration
    *****************************************************************************************************************************************/
    double set_to_range(double var, double min_interval, double  max_interval);
    /*****************************************************************************************************************************************/
    /*  This function compute the julian day and julian century from the local */
    /*  time and timezone information. Ephemeris are calculated with a delta_t=0 */
    /*  seconds.  */
    /*  If time input is a Matlab time string, extract the information from */
    /*  this string and create the structure as defined in the main header of */
    /*  this script. */
    /*****************************************************************************************************************************************/
    void julian_calculation(_JULIAN* julian, _YOUR_TIME* time);
    /*****************************************************************************************************************************************
    * This function compute the earth position relative to the sun, using
    * tabulated values
    *****************************************************************************************************************************************/
    void earth_heliocentric_position_calculation( _EARTH_HELI_POSITION* earth_heliocentric_position, _JULIAN* julian);
    /*****************************************************************************************************************************************
    * This function compute the sun position relative to the earth. 
    ******************************************************************************************************************************************/
    void sun_geocentric_position_calculation(_SUN_GEO_POSITION* sun_geocentric_position, _EARTH_HELI_POSITION* earth_heliocentric_position);
    /*****************************************************************************************************************************************
    *  This function compute the nutation in longtitude and in obliquity, in degrees. 
    ******************************************************************************************************************************************/
    void nutation_calculation(_NUTATION* nutation, _JULIAN* julian);
    /*****************************************************************************************************************************************
    *   This function compute the true obliquity of the ecliptic.
    ******************************************************************************************************************************************/
    double true_obliquity_calculation(_NUTATION* nutation, _JULIAN* julian);
    /*****************************************************************************************************************************************
    *  This function compute the aberration_correction, as a function of the  earth-sun distance
    ******************************************************************************************************************************************/
    double abberation_correction_calculation(_EARTH_HELI_POSITION* earth_heliocentric_position);
    /*****************************************************************************************************************************************
    *  This function compute the sun apparent longitude
    ******************************************************************************************************************************************/
    double apparent_sun_longitude_calculation(_SUN_GEO_POSITION* sun_geocentric_position, _NUTATION* nutation, double aberration_correction);
    /*****************************************************************************************************************************************
    *  This function compute the apparent sideral time at Greenwich.
    ******************************************************************************************************************************************/
    double apparent_stime_at_greenwich_calculation(_JULIAN* julian, _NUTATION* nutation, double true_obliquity);
    /*****************************************************************************************************************************************
    *  This function compute the sun rigth ascension.
    ******************************************************************************************************************************************/
    double sun_rigth_ascension_calculation(double apparent_sun_longitude, double true_obliquity, _SUN_GEO_POSITION* sun_geocentric_position);
    /*****************************************************************************************************************************************
    *  
    ******************************************************************************************************************************************/
    double sun_geocentric_declination_calculation(double apparent_sun_longitude, double true_obliquity, _SUN_GEO_POSITION* sun_geocentric_position);
    /*****************************************************************************************************************************************
    *  
    ******************************************************************************************************************************************/
    double observer_local_hour_calculation(double apparent_stime_at_greenwich, _YOUR_LOCATION* location, double sun_rigth_ascension);
    /*****************************************************************************************************************************************
    *This function compute the sun position(rigth ascension and declination)
    * with respect to the observer local position at the Earth surface.
    ******************************************************************************************************************************************/
    void topocentric_sun_position_calculate(_TOPCENTER_SUN_POSITION* topocentric_sun_position, _EARTH_HELI_POSITION* earth_heliocentric_position,
        _YOUR_LOCATION* location, double sun_geocentric_declination, double sun_rigth_ascension, double observer_local_hour);
    /*****************************************************************************************************************************************
    *This function compute the topocentric local jour angle in degrees
    ******************************************************************************************************************************************/
    double topocentric_local_hour_calculate(double observer_local_hour, _TOPCENTER_SUN_POSITION* topocentric_sun_position);
    /*****************************************************************************************************************************************
    * % This function compute the sun zenith angle, taking into account the
    * % atmospheric refraction.A default temperature of 283K and a
    * % default pressure of 1010 mbar are used.
    ******************************************************************************************************************************************/
    void sun_topocentric_zenith_angle_calculate(_SUN_VIEW_LOCATION* sun, _YOUR_LOCATION* location, 
                                                   _TOPCENTER_SUN_POSITION* topocentric_sun_position, double topocentric_local_hour);
    /*****************************************************************************************************************************************
    * Main program entrance
    *****************************************************************************************************************************************/
    void sun_position( _SUN_VIEW_LOCATION* sun, _YOUR_TIME*  time,  _YOUR_LOCATION* location);
    /*****************************************************************************************************************************************
    * Calculate daylight start and end times
    *****************************************************************************************************************************************/
    void sun_shine(_SUN_SHINE* shine, _SUN_VIEW_LOCATION* sun, _YOUR_TIME* time, _YOUR_LOCATION* location, double windir);
};

extern SunPositionModule SunPosition;

#endif // __SUN_POSITION_H__

/*****************************************************************************************************************************************
* end of file
*****************************************************************************************************************************************/

