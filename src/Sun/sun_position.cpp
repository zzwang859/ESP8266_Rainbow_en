/*****************************************************************************************************************************************
* Global variable definition
*****************************************************************************************************************************************/
#include <Arduino.h>
#include "sun_position.h"

/******** Variables used in sun position calculations *********************/
SunPositionModule SunPosition;


static const double L0_terms[][3] ICACHE_RODATA_ATTR  = {
{175347046.0,0,0},{3341656.0,4.6692568,6283.07585},{34894.0,4.6261,12566.1517},{3497.0,2.7441,5753.3849},{3418.0,2.8289,3.5231},
{3136.0,3.6277,77713.7715},{2676.0,4.4181,7860.4194},{2343.0,6.1352,3930.2097},{1324.0,0.7425,11506.7698},{1273.0,2.0371,529.691},
{1199.0,1.1096,1577.3435},{990,5.233,5884.927},{902,2.045,26.298},{857,3.508,398.149},{780,1.179,5223.694},{753,2.533,5507.553},
{505,4.583,18849.228},{492,4.205,775.523},{357,2.92,0.067},{317,5.849,11790.629},{284,1.899,796.298},{271,0.315,10977.079},{243,0.345,5486.778},
{206,4.806,2544.314},{205,1.869,5573.143},{202,2.4458,6069.777},{156,0.833,213.299},{132,3.411,2942.463},{126,1.083,20.775},{115,0.645,0.98},
{103,0.636,4694.003},{102,0.976,15720.839},{102,4.267,7.114},{99,6.21,2146.17},{98,0.68,155.42},{86,5.98,161000.69},{85,1.3,6275.96},
{85,3.67,71430.7},{80,1.81,17260.15},{79,3.04,12036.46},{71,1.76,5088.63},{74,3.5,3154.69},{74,4.68,801.82},{70,0.83,9437.76},
{62,3.98,8827.39},{61,1.82,7084.9},{57,2.78,6286.6},{56,4.39,14143.5},{56,3.47,6279.55},{52,0.19,12139.55},{52,1.33,1748.02},{51,0.28,5856.48},
{49,0.49,1194.45},{41,5.37,8429.24},{41,2.4,19651.05},{39,6.17,10447.39},{37,6.04,10213.29},{37,2.57,1059.38},{36,1.71,2352.87},
{36,1.78,6812.77},{33,0.59,17789.85},{30,0.44,83996.85},{30,2.74,1349.87},{25,3.16,4690.48},
};

static const double L1_terms[][3]  ICACHE_RODATA_ATTR = {
{628331966747.0,0,0},{206059.0,2.678235,6283.07585},{4303.0,2.6351,12566.1517},{425.0,1.59,3.523},{119.0,5.796,26.298},{109.0,2.966,1577.344},
{93,2.59,18849.23},{72,1.14,529.69},{68,1.87,398.15},{67,4.41,5507.55},{59,2.89,5223.69},{56,2.17,155.42},{45,0.4,796.3},{36,0.47,775.52},
{29,2.65,7.11},{21,5.34,0.98},{19,1.85,5486.78},{19,4.97,213.3},{17,2.99,6275.96},{16,0.03,2544.31},{16,1.43,2146.17},{15,1.21,10977.08},
{12,2.83,1748.02},{12,3.26,5088.63},{12,5.27,1194.45},{12,2.08,4694},{11,0.77,553.57},{10,1.3,3286.6},{10,4.24,1349.87},{9,2.7,242.73},
{9,5.64,951.72},{8,5.3,2352.87},{6,2.65,9437.76},{6,4.67,4690.48},
};


static const double L2_terms[][3]  ICACHE_RODATA_ATTR  = {
{52919.0,0,0},{8720.0,1.0721,6283.0758},{309.0,0.867,12566.152},{27,0.05,3.52},{16,5.19,26.3},{16,3.68,155.42},{10,0.76,18849.23},
{9,2.06,77713.77},{7,0.83,775.52},{5,4.66,1577.34},{4,1.03,7.11},{4,3.44,5573.14},{3,5.14,796.3},{3,6.05,5507.55},{3,1.19,242.73},
{3,6.12,529.69},{3,0.31,398.15},{3,2.28,553.57},{2,4.38,5223.69},{2,3.75,0.98},
};


static const double L3_terms[][3]  ICACHE_RODATA_ATTR = {
{289.0,5.844,6283.076},{35,0,0},{17,5.49,12566.15},{3,5.2,155.42},{1,4.72,3.52},{1,5.3,18849.23},{1,5.97,242.73},
};

static const double L4_terms[][3]  ICACHE_RODATA_ATTR = {
{114.0,3.142,0},{8,4.13,6283.08},{1,3.84,12566.15},
};

static const double L5_terms[][3]  ICACHE_RODATA_ATTR = {
{1,3.14,0},
};

static const double B0_terms[][3]  ICACHE_RODATA_ATTR = {
{280.0,3.199,84334.662},{102.0,5.422,5507.553},{80,3.88,5223.69},{44,3.7,2352.87},{32,4,1577.34},
};

static const double B1_terms[][3]  ICACHE_RODATA_ATTR = {
{9,3.9,5507.55},{6,1.73,5223.69},
};

static const double R0_terms[][3]  ICACHE_RODATA_ATTR = {
{100013989.0,0,0},{1670700.0,3.0984635,6283.07585},{13956.0,3.05525,12566.1517},{3084.0,5.1985,77713.7715},{1628.0,1.1739,5753.3849},
{1576.0,2.8469,7860.4194},{925.0,5.453,11506.77},{542.0,4.564,3930.21},{472.0,3.661,5884.927},{346.0,0.964,5507.553},{329.0,5.9,5223.694},
{307.0,0.299,5573.143},{243.0,4.273,11790.629},{212.0,5.847,1577.344},{186.0,5.022,10977.079},{175.0,3.012,18849.228},{110.0,5.055,5486.778},
{98,0.89,6069.78},{86,5.69,15720.84},{86,1.27,161000.69},{85,0.27,17260.15},{63,0.92,529.69},{57,2.01,83996.85},{56,5.24,71430.7},
{49,3.25,2544.31},{47,2.58,775.52},{45,5.54,9437.76},{43,6.01,6275.96},{39,5.36,4694},{38,2.39,8827.39},{37,0.83,19651.05},{37,4.9,12139.55},
{36,1.67,12036.46},{35,1.84,2942.46},{33,0.24,7084.9},{32,0.18,5088.63},{32,1.78,398.15},{28,1.21,6286.6},{28,1.9,6279.55},{26,4.59,10447.39},
};

static const double R1_terms[][3]  ICACHE_RODATA_ATTR = {
{103019.0,1.10749,6283.07585},{1721.0,1.0644,12566.1517},{702.0,3.142,0},{32,1.02,18849.23},{31,2.84,5507.55},{25,1.32,5223.69},
{18,1.42,1577.34},{10,5.91,10977.08},{9,1.42,6275.96},{9,0.27,5486.78},
};

static const double R2_terms[][3]  ICACHE_RODATA_ATTR = {
{4359.0,5.7846,6283.0758},{124.0,5.579,12566.152},{12,3.14,0},{9,3.63,77713.77},{6,1.87,5573.14},{3,5.47,18849},
};

static const double R3_terms[][3]  ICACHE_RODATA_ATTR = {
{145.0,4.273,6283.076},{7,3.92,12566.15},
};

static const double R4_terms[][3]  ICACHE_RODATA_ATTR = {
{4,2.56,6283.08},
};

static const double Y_terms[][5]  ICACHE_RODATA_ATTR = {
{0,0,0,0,1},{-2,0,0,2,2},{0,0,0,2,2},{0,0,0,0,2},{0,1,0,0,0},{0,0,1,0,0},{-2,1,0,2,2},{0,0,0,2,1},{0,0,1,2,2},{-2,-1,0,2,2},{-2,0,1,0,0},
{-2,0,0,2,1},{0,0,-1,2,2},{2,0,0,0,0},{0,0,1,0,1},{2,0,-1,2,2},{0,0,-1,0,1},{0,0,1,2,1},{-2,0,2,0,0},{0,0,-2,2,1},{2,0,0,2,2},
{0,0,2,2,2},{0,0,2,0,0},{-2,0,1,2,2},{0,0,0,2,0},{-2,0,0,2,0},{0,0,-1,2,1},{0,2,0,0,0},{2,0,-1,0,1},{-2,2,0,2,2},{0,1,0,0,1},{-2,0,1,0,1},
{0,-1,0,0,1},{0,0,2,-2,0},{2,0,-1,2,1},{2,0,1,2,2},{0,1,0,2,2},{-2,1,1,0,0},{0,-1,0,2,2},{2,0,0,2,1},{2,0,1,0,0},{-2,0,2,2,2},
{-2,0,1,2,1},{2,0,-2,0,1},{2,0,0,0,1},{0,-1,1,0,0},{-2,-1,0,2,1},{-2,0,0,0,1},{0,0,2,2,1},{-2,0,2,0,1},{-2,1,0,2,1},{0,0,1,-2,0},
{-1,0,1,0,0},{-2,1,0,0,0},{1,0,0,0,0},{0,0,1,2,0},{0,0,-2,2,2},{-1,-1,1,0,0},{0,1,1,0,0},{0,-1,1,2,2},{2,-1,-1,2,2},{0,0,3,2,2},
{2,-1,0,2,2},
};

static const double nutation_terms[][5]  ICACHE_RODATA_ATTR  = {
{-171996,-174.2,92025,8.9},{-13187,-1.6,5736,-3.1},{-2274,-0.2,977,-0.5},{2062,0.2,-895,0.5},{1426,-3.4,54,-0.1},{712,0.1,-7,0},
{-517,1.2,224,-0.6},{-386,-0.4,200,0},{-301,0,129,-0.1},{217,-0.5,-95,0.3},{-158,0,0,0},{129,0.1,-70,0},{123,0,-53,0},{63,0,0,0},
{63,0.1,-33,0},{-59,0,26,0},{-58,-0.1,32,0},{-51,0,27,0},{48,0,0,0},{46,0,-24,0},{-38,0,16,0},{-31,0,13,0},{29,0,0,0},{29,0,-12,0},
{26,0,0,0},{-22,0,0,0},{21,0,-10,0},{17,-0.1,0,0},{16,0,-8,0},{-16,0.1,7,0},{-15,0,9,0},{-13,0,7,0},{-12,0,6,0},{11,0,0,0},{-10,0,5,0},
{-8,0,3,0},{7,0,-3,0},{-7,0,0,0},{-7,0,3,0},{-7,0,3,0},{6,0,0,0},{6,0,-3,0},{6,0,-3,0},{-6,0,3,0},{-6,0,3,0},{5,0,0,0},{-5,0,3,0},
{-5,0,3,0},{-5,0,3,0},{4,0,0,0},{4,0,0,0},{4,0,0,0},{-4,0,0,0},{-4,0,0,0},{-4,0,0,0},{3,0,0,0},{-3,0,0,0},{-3,0,0,0},{-3,0,0,0},
{-3,0,0,0},{-3,0,0,0},{-3,0,0,0},{-3,0,0,0},
};

static const double p[][4]  ICACHE_RODATA_ATTR  = {
    {  189474,    -0.0019142,    445267.11148,   297.85036 },
    { -300000,    -0.0001603,     35999.05034,   357.52772 },
    {   56250,     0.0086972,   477198.867398,   134.96298 },
    {  327270,    -0.0036825,   483202.017538,    93.27191 },
    {  450000,     0.0020708,    -1934.136261,   125.04452 },
};

static const double  p1[11]  ICACHE_RODATA_ATTR = { 
    2.45, 5.79, 27.87, 7.12, -39.05, -249.67,
    -51.38, 1999.25, -1.55, -4680.93, 84381.448
};

/*****************************************************************************************************************************************
* Step-by-step sub-function definition
*****************************************************************************************************************************************/
double SunPositionModule::set_to_range(double var, double min_interval, double  max_interval)
{
    var = var - max_interval * floor(var / max_interval);

    if (var < min_interval) {
        var = var + max_interval;
    }
    return var;
}

/*****************************************************************************************************************************************/
/*  This function compute the julian day and julian century from the local */
/*  time and timezone information. Ephemeris are calculated with a delta_t=0 */
/*  seconds.  */
/*  If time input is a Matlab time string, extract the information from */
/*  this string and create the structure as defined in the main header of */
/*  this script. */
/*****************************************************************************************************************************************/
void SunPositionModule::julian_calculation(_JULIAN* julian, _YOUR_TIME* time)
{
    double Y;
    double M;
    double B;

    if ((time->month == 1.0) || (time->month == 2.0)) {
        Y = time->year - 1.0;
        M = time->month + 12.0;
    }
    else {
        Y = time->year;
        M = time->month;
    }

    /*  time of day in UT time.  */
    /*  Day of month in decimal time, ex. 2sd day of month at 12:30:30UT, D=2.521180.0556 */
    /*  In 1582, the gregorian calendar was adopted */
    if (time->year == 1582.0) {
        if (time->month == 10.0) {
            if (time->day <= 4.0) {
                /*  The Julian calendar ended on October 4, 1582 */
                B = 0.0;
            }
            else if (time->day >= 15.0) {
                /*  The Gregorian calendar started on October 15, 1582 */
                B = floor(Y / 100.0);
                B = (2.0 - B) + floor(B / 4.0);
            }
            else {
                B = 0.0;
            }
        }
        else if (time->month < 10.0) {
            /*  Julian calendar  */
            B = 0.0;
        }
        else {
            /*  Gregorian calendar */
            B = floor(Y / 100.0);
            B = (2.0 - B) + floor(B / 4.0);
        }
    }
    else if (time->year < 1582.0) {
        /*  Julian calendar */
        B = 0.0;
    }
    else {
        B = floor(Y / 100.0);

        /*  Gregorian calendar */
        B = (2.0 - B) + floor(B / 4.0);
    }

    julian->day = (((floor(365.25 * (Y + 4716.0)) + floor(30.6001 * (M + 1.0))) +
        (time->day + (((time->hour - time->UTC) / 24.0 +
            time->min / 1440.0) + time->sec / 86400.0))) + B) - 1524.5;
    julian->ephemeris_day = julian->day;
    julian->century = (julian->day - 2.451545E+6) / 36525.0;
    julian->ephemeris_century = julian->century;
    julian->ephemeris_millenium = julian->century / 10.0;

    julian->JME[0] = julian->ephemeris_millenium;
    julian->JME[1] = pow(julian->ephemeris_millenium, 2);
    julian->JME[2] = pow(julian->ephemeris_millenium, 3);
    julian->JME[3] = pow(julian->ephemeris_millenium, 4);
    julian->JME[4] = pow(julian->ephemeris_millenium, 5);

    julian->JCE[0] = julian->ephemeris_century;
    julian->JCE[1] = pow(julian->ephemeris_century, 2);
    julian->JCE[2] = pow(julian->ephemeris_century, 3);
}

/*****************************************************************************************************************************************
* This function compute the earth position relative to the sun, using
* tabulated values
*****************************************************************************************************************************************/
void SunPositionModule::earth_heliocentric_position_calculation(_EARTH_HELI_POSITION* earth_heliocentric_position, _JULIAN * julian )
{
    double L0, L1, L2, L3, L4, L5;
    uint32_t  i = 0;
  
    // Compute the Earth Heliochentric longitude from the tabulated values. 
    L0 = 0; for (i = 0; i < sizeof(L0_terms)/sizeof(double)/3; i++) {L0 += L0_terms[i][0] * cos(L0_terms[i][1] +  julian->JME[0] * L0_terms[i][2]);}
    L1 = 0; for (i = 0; i < sizeof(L1_terms)/sizeof(double)/3; i++) {L1 += L1_terms[i][0] * cos(L1_terms[i][1] +  julian->JME[0] * L1_terms[i][2]);}
    L2 = 0; for (i = 0; i < sizeof(L2_terms)/sizeof(double)/3; i++) {L2 += L2_terms[i][0] * cos(L2_terms[i][1] +  julian->JME[0] * L2_terms[i][2]);}
    L3 = 0; for (i = 0; i < sizeof(L3_terms)/sizeof(double)/3; i++) {L3 += L3_terms[i][0] * cos(L3_terms[i][1] +  julian->JME[0] * L3_terms[i][2]);}
    L4 = 0; for (i = 0; i < sizeof(L4_terms)/sizeof(double)/3; i++) {L4 += L4_terms[i][0] * cos(L4_terms[i][1] +  julian->JME[0] * L4_terms[i][2]);}
    L5 = 0; for (i = 0; i < sizeof(L5_terms)/sizeof(double)/3; i++) {L5 += L5_terms[i][0] * cos(L5_terms[i][1] +  julian->JME[0] * L5_terms[i][2]);}


    earth_heliocentric_position->longitude = (L0 + (L1 * julian->JME[0]) + (L2 * julian->JME[1]) + (L3 * julian->JME[2]) + (L4 * julian->JME[3]) + (L5 * julian->JME[4])) / 1e8;
    // Convert the longitude to degrees. 
    earth_heliocentric_position->longitude = earth_heliocentric_position->longitude * 180.0/PI;
    // Limit the range to [0,360[;
    earth_heliocentric_position->longitude = set_to_range(earth_heliocentric_position->longitude, 0, 360);


    L0 = 0; for (i = 0; i < sizeof(B0_terms) / sizeof(double) / 3; i++) { L0 += B0_terms[i][0] * cos(B0_terms[i][1] + julian->JME[0] * B0_terms[i][2]); }
    L1 = 0; for (i = 0; i < sizeof(B1_terms) / sizeof(double) / 3; i++) { L1 += B1_terms[i][0] * cos(B1_terms[i][1] + julian->JME[0] * B1_terms[i][2]); }


    earth_heliocentric_position->latitude = (L0 + (L1 * julian->JME[0])) / 1e8;
    // Convert the latitude to degrees
    earth_heliocentric_position->latitude = earth_heliocentric_position->latitude * 180.0 / PI;
    // Limit the range to[0, 360];
    earth_heliocentric_position->latitude = set_to_range(earth_heliocentric_position->latitude, 0, 360);

    L0 = 0; for (i = 0; i < sizeof(R0_terms) / sizeof(double) / 3; i++) { L0 += R0_terms[i][0] * cos(R0_terms[i][1] +  julian->JME[0] * R0_terms[i][2]); }
    L1 = 0; for (i = 0; i < sizeof(R1_terms) / sizeof(double) / 3; i++) { L1 += R1_terms[i][0] * cos(R1_terms[i][1] +  julian->JME[0] * R1_terms[i][2]); }
    L2 = 0; for (i = 0; i < sizeof(R2_terms) / sizeof(double) / 3; i++) { L2 += R2_terms[i][0] * cos(R2_terms[i][1] +  julian->JME[0] * R2_terms[i][2]); }
    L3 = 0; for (i = 0; i < sizeof(R3_terms) / sizeof(double) / 3; i++) { L3 += R3_terms[i][0] * cos(R3_terms[i][1] +  julian->JME[0] * R3_terms[i][2]); }
    L4 = 0; for (i = 0; i < sizeof(R4_terms) / sizeof(double) / 3; i++) { L4 += R4_terms[i][0] * cos(R4_terms[i][1] +  julian->JME[0] * R4_terms[i][2]); }

    // Units are in AU
    earth_heliocentric_position->radius = (L0 + (L1 * julian->JME[0]) + (L2 * julian->JME[1]) + (L3 * julian->JME[2]) + (L4 * julian->JME[3])) / 1e8;
}


/*****************************************************************************************************************************************
* This function compute the sun position relative to the earth. 
******************************************************************************************************************************************/
void SunPositionModule::sun_geocentric_position_calculation(_SUN_GEO_POSITION* sun_geocentric_position,_EARTH_HELI_POSITION* earth_heliocentric_position)
{
    sun_geocentric_position->longitude = earth_heliocentric_position->longitude + 180.0;
    // Limit the range to[0, 360];
    sun_geocentric_position->longitude = set_to_range(sun_geocentric_position->longitude, 0, 360);

    sun_geocentric_position->latitude = -earth_heliocentric_position->latitude;
    // Limit the range to[0, 360]
    sun_geocentric_position->latitude = set_to_range(sun_geocentric_position->latitude, 0, 360);
}



/*****************************************************************************************************************************************
*  This function compute the nutation in longtitude and in obliquity, in degrees. 
******************************************************************************************************************************************/
void  SunPositionModule::nutation_calculation(_NUTATION* nutation, _JULIAN* julian)
{
    double  X0, X1, X2, X3, X4;
    double tabulated_argument;
    uint32_t i;

    // All Xi are in degrees.
    X0 = julian->JCE[2] / p[0][0] + p[0][1] * julian->JCE[1] + p[0][2] * julian->JCE[0] + p[0][3]; // 1. Mean elongation of the moon from the sun
    X1 = julian->JCE[2] / p[1][0] + p[1][1] * julian->JCE[1] + p[1][2] * julian->JCE[0] + p[1][3]; // 2. Mean anomaly of the sun(earth)   
    X2 = julian->JCE[2] / p[2][0] + p[2][1] * julian->JCE[1] + p[2][2] * julian->JCE[0] + p[2][3]; // 3. Mean anomaly of the moon   
    X3 = julian->JCE[2] / p[3][0] + p[3][1] * julian->JCE[1] + p[3][2] * julian->JCE[0] + p[3][3]; // 4. Moon argument of latitude            
    X4 = julian->JCE[2] / p[4][0] + p[4][1] * julian->JCE[1] + p[4][2] * julian->JCE[0] + p[4][3]; // 5. Longitude of the ascending node of the moon's mean orbit on the ecliptic, measured from the mean equinox of the date

    nutation->longitude = 0;
    nutation->obliquity = 0;
    
    for (i = 0; i < sizeof(Y_terms) / sizeof(double) / 5; i++) {
        tabulated_argument = (X0 * Y_terms[i][0] + X1 * Y_terms[i][1] + X2 * Y_terms[i][2] + X3 * Y_terms[i][3] + X4 * Y_terms[i][4]) * PI / 180.0;
        nutation->longitude += (nutation_terms[i][0] + nutation_terms[i][1] * julian->JCE[0]) * sin(tabulated_argument);
        nutation->obliquity += (nutation_terms[i][2] + nutation_terms[i][3] * julian->JCE[0]) * cos(tabulated_argument);
    }

    nutation->longitude /= 36E6;
    nutation->obliquity /= 36E6;
}

/*****************************************************************************************************************************************
*   This function compute the true obliquity of the ecliptic.
******************************************************************************************************************************************/
double SunPositionModule::true_obliquity_calculation(_NUTATION* nutation, _JULIAN* julian)
{
    double U, mean_obliquity;

    U = julian->ephemeris_millenium / 10;
    mean_obliquity = p1[0] * pow(U,10) + p1[1] * pow(U,9) + p1[2] * pow(U,8) + p1[3] * pow(U,7) + p1[4] * pow(U,6) +
                     p1[5] * pow(U,5)  + p1[6] * pow(U,4) + p1[7] * pow(U,3) + p1[8] * pow(U,2) + p1[9] * U + p1[10];

    return ((mean_obliquity / 3600.0) + nutation->obliquity);
}

/*****************************************************************************************************************************************
*  This function compute the aberration_correction, as a function of the  earth-sun distance
******************************************************************************************************************************************/
double SunPositionModule::abberation_correction_calculation(_EARTH_HELI_POSITION* earth_heliocentric_position )
{
   return( -20.4898/(3600.0 * earth_heliocentric_position->radius));
}

/*****************************************************************************************************************************************
*  This function compute the sun apparent longitude
******************************************************************************************************************************************/
double  SunPositionModule::apparent_sun_longitude_calculation(_SUN_GEO_POSITION* sun_geocentric_position, _NUTATION* nutation, double aberration_correction)
{
    return sun_geocentric_position->longitude + nutation->longitude + aberration_correction;
}

/*****************************************************************************************************************************************
*  This function compute the apparent sideral time at Greenwich.
******************************************************************************************************************************************/
double SunPositionModule::apparent_stime_at_greenwich_calculation(_JULIAN* julian, _NUTATION* nutation, double true_obliquity)
{
    double mean_stime ;

    // Mean sideral time, in degrees
    mean_stime = 280.46061837 + (360.98564736629 * (julian->day - 2451545)) + (0.000387933 * pow(julian->century,2)) - (pow(julian->century,3) / 38710000);
    // Limit the range to[0 - 360];
    mean_stime = set_to_range(mean_stime, 0, 360);

    return ( mean_stime + (nutation->longitude * cos(true_obliquity * PI / 180.0)));
}

/*****************************************************************************************************************************************
*  This function compute the sun rigth ascension.
******************************************************************************************************************************************/
double SunPositionModule::sun_rigth_ascension_calculation(double apparent_sun_longitude, double true_obliquity, _SUN_GEO_POSITION* sun_geocentric_position)
{
    double argument_numerator, sun_rigth_ascension, argument_denominator;

    argument_numerator = (sin(apparent_sun_longitude * PI / 180.0) * cos(true_obliquity * PI / 180.0)) - 
        (tan(sun_geocentric_position->latitude * PI / 180.0) * sin(true_obliquity * PI / 180.0));
    argument_denominator = cos(apparent_sun_longitude * PI / 180.0);

    sun_rigth_ascension = atan2(argument_numerator, argument_denominator) * 180.0 / PI;
    // Limit the range to[0, 360];
    sun_rigth_ascension = set_to_range(sun_rigth_ascension, 0, 360);

    return sun_rigth_ascension;
}

/*****************************************************************************************************************************************
*  
******************************************************************************************************************************************/
double SunPositionModule::sun_geocentric_declination_calculation(double apparent_sun_longitude, double true_obliquity, _SUN_GEO_POSITION* sun_geocentric_position)
{
    double argument;
    argument = (sin(sun_geocentric_position->latitude * PI / 180.0) * cos(true_obliquity * PI / 180.0)) + 
        (cos(sun_geocentric_position->latitude * PI / 180.0) * sin(true_obliquity * PI / 180.0) * sin(apparent_sun_longitude * PI / 180.0));

    return asin(argument) * 180.0 / PI;
}

/*****************************************************************************************************************************************
*  
******************************************************************************************************************************************/
double SunPositionModule::observer_local_hour_calculation(double apparent_stime_at_greenwich, _YOUR_LOCATION* location, double sun_rigth_ascension)
{
    double observer_local_hour;

    observer_local_hour = apparent_stime_at_greenwich + location->longitude - sun_rigth_ascension;
    // Set the range to[0 - 360]
    observer_local_hour = set_to_range(observer_local_hour, 0, 360);

    return observer_local_hour;
}

/*****************************************************************************************************************************************
*This function compute the sun position(rigth ascension and declination)
 with respect to the observer local position at the Earth surface.
******************************************************************************************************************************************/
void SunPositionModule::topocentric_sun_position_calculate(  _TOPCENTER_SUN_POSITION* topocentric_sun_position,_EARTH_HELI_POSITION* earth_heliocentric_position, 
                                          _YOUR_LOCATION* location, double sun_geocentric_declination, double sun_rigth_ascension, double observer_local_hour)
{
    double eq_horizontal_parallax, u, x, y, nominator, denominator, sun_rigth_ascension_parallax;

    // Equatorial horizontal parallax of the sun in degrees
    eq_horizontal_parallax = 8.794 / (3600 * earth_heliocentric_position->radius);

    // Term u, used in the following calculations(in radians)
    u = atan(0.99664719 * tan(location->latitude * PI / 180.0));

    // Term x, used in the following calculations
    x = cos(u) + ((location->altitude / 6378140) * cos(location->latitude * PI / 180.0));

    // Term y, used in the following calculations
    y = (0.99664719 * sin(u)) + ((location->altitude / 6378140) * sin(location->latitude * PI / 180.0));

    // Parallax in the sun rigth ascension(in radians)
    nominator = -x * sin(eq_horizontal_parallax * PI / 180.0) * sin(observer_local_hour * PI / 180.0);
    denominator = cos(sun_geocentric_declination * PI / 180.0) - (x * sin(eq_horizontal_parallax * PI / 180.0) * cos(observer_local_hour * PI / 180.0));
    sun_rigth_ascension_parallax = atan2(nominator, denominator);
    // Conversion to degrees->
    topocentric_sun_position->rigth_ascension_parallax = sun_rigth_ascension_parallax * 180.0 / PI;

    // Topocentric sun rigth ascension(in degrees)
    topocentric_sun_position->rigth_ascension = sun_rigth_ascension + (sun_rigth_ascension_parallax * 180.0 / PI);

    // Topocentric sun declination(in degrees)
    nominator = (sin(sun_geocentric_declination * PI / 180.0) - (y * sin(eq_horizontal_parallax * PI / 180.0))) * cos(sun_rigth_ascension_parallax);
    denominator = cos(sun_geocentric_declination * PI / 180.0) - (x * sin(eq_horizontal_parallax * PI / 180.0)) * cos(observer_local_hour * PI / 180.0);
    topocentric_sun_position->declination = atan2(nominator, denominator) * 180.0 / PI;
}

/*****************************************************************************************************************************************
*This function compute the topocentric local jour angle in degrees
******************************************************************************************************************************************/
double  SunPositionModule::topocentric_local_hour_calculate(double observer_local_hour, _TOPCENTER_SUN_POSITION* topocentric_sun_position)
{
    return  (observer_local_hour - topocentric_sun_position->rigth_ascension_parallax);
}

/*****************************************************************************************************************************************
% This function compute the sun zenith angle, taking into account the
% atmospheric refraction.A default temperature of 283K and a
% default pressure of 1010 mbar are used.
******************************************************************************************************************************************/
void SunPositionModule::sun_topocentric_zenith_angle_calculate(_SUN_VIEW_LOCATION *sun, _YOUR_LOCATION* location, _TOPCENTER_SUN_POSITION* topocentric_sun_position, double topocentric_local_hour)
{
    double argument, true_elevation, refraction_corr, apparent_elevation;
    double nominator, denominator;

    // Topocentric elevation, without atmospheric refraction
    argument = (sin(location->latitude * PI / 180.0) * sin(topocentric_sun_position->declination * PI / 180.0)) + 
        (cos(location->latitude * PI / 180.0) * cos(topocentric_sun_position->declination * PI / 180.0) * cos(topocentric_local_hour * PI / 180.0));
    true_elevation = asin(argument) * 180.0 / PI;

    // Atmospheric refraction correction(in degrees)
    argument = true_elevation + (10.3 / (true_elevation + 5.11));
    refraction_corr = 1.02 / (60 * tan(argument * PI / 180.0));

    // For exact pressureand temperature correction, use this,
    // with P the pressure in mbar amd T the temperature in Kelvins :
    // refraction_corr = (P / 1010) * (283 / T) * 1.02 / (60 * tan(argument * PI / 180.0));

    // Apparent elevation
    if (true_elevation > -5) {
        apparent_elevation = true_elevation + refraction_corr;
    }
    else {
        apparent_elevation = true_elevation;
    }
           
    sun->zenith = 90 - apparent_elevation;

    // Topocentric azimuth angle.The + 180.0 conversion is to pass from astronomer
    //  notation(westward from south) to navigation notation(eastward from north);
    nominator = sin(topocentric_local_hour * PI / 180.0);
    denominator = (cos(topocentric_local_hour * PI / 180.0) * sin(location->latitude * PI / 180.0)) - 
        (tan(topocentric_sun_position->declination * PI / 180.0) * cos(location->latitude * PI / 180.0));

    sun->azimuth = (atan2(nominator, denominator) * 180.0 / PI) + 180.0;
    // Set the range to[0 - 360]
    sun->azimuth = set_to_range(sun->azimuth, 0, 360);
}


/*****************************************************************************************************************************************
*Main program entrance
*****************************************************************************************************************************************/
void  SunPositionModule::sun_position( _SUN_VIEW_LOCATION* sun, _YOUR_TIME*  time,  _YOUR_LOCATION* location)
{
    _JULIAN  julian;
    _EARTH_HELI_POSITION  earth_heliocentric_position;
    _SUN_GEO_POSITION sun_geocentric_position;
    _NUTATION nutation;
    double true_obliquity;
    double aberration_correction;
    double apparent_sun_longitude;
    double apparent_stime_at_greenwich;
    double sun_rigth_ascension;
    double sun_geocentric_declination;
    double observer_local_hour;
    _TOPCENTER_SUN_POSITION topocentric_sun_position;
    double topocentric_local_hour;

    // /********** Debugging use ************************************************/
    // time->year = 2022;
    // time->month = 11;
    // time->day = 12;
    // time->hour = 10;
    // time->min = 25;
    // time->sec = 0;
    // time->UTC = 8;

    // location->longitude = 113.954266;
    // location->latitude = 22.580521;
    // location->altitude = 80;
    // /********** Debugging use END ********************************************/

    julian_calculation((_JULIAN*)&julian,  time);
    earth_heliocentric_position_calculation((_EARTH_HELI_POSITION*)&earth_heliocentric_position, (_JULIAN*)&julian);
    sun_geocentric_position_calculation((_SUN_GEO_POSITION*)&sun_geocentric_position, (_EARTH_HELI_POSITION*)&earth_heliocentric_position);
    nutation_calculation((_NUTATION*)&nutation, (_JULIAN*)& julian);
    true_obliquity = true_obliquity_calculation((_NUTATION*)&nutation, (_JULIAN*)&julian);
    aberration_correction  = abberation_correction_calculation((_EARTH_HELI_POSITION*)&earth_heliocentric_position);
    apparent_sun_longitude = apparent_sun_longitude_calculation((_SUN_GEO_POSITION*)&sun_geocentric_position, (_NUTATION*)&nutation, aberration_correction);
    apparent_stime_at_greenwich = apparent_stime_at_greenwich_calculation((_JULIAN*)&julian, (_NUTATION*)&nutation, true_obliquity);

    sun_rigth_ascension = sun_rigth_ascension_calculation( apparent_sun_longitude, true_obliquity, (_SUN_GEO_POSITION*)&sun_geocentric_position);
    sun_geocentric_declination = sun_geocentric_declination_calculation(apparent_sun_longitude, true_obliquity, (_SUN_GEO_POSITION*)&sun_geocentric_position);
    observer_local_hour = observer_local_hour_calculation( apparent_stime_at_greenwich,  location, sun_rigth_ascension);
    topocentric_sun_position_calculate((_TOPCENTER_SUN_POSITION*)&topocentric_sun_position, (_EARTH_HELI_POSITION *) &earth_heliocentric_position,
        location, sun_geocentric_declination, sun_rigth_ascension, observer_local_hour);
    topocentric_local_hour = topocentric_local_hour_calculate(observer_local_hour, (_TOPCENTER_SUN_POSITION*)&topocentric_sun_position);
    sun_topocentric_zenith_angle_calculate(sun, location, (_TOPCENTER_SUN_POSITION*) &topocentric_sun_position, topocentric_local_hour);
}

/*****************************************************************************************************************************************
* Find the time of day when there is light in the window
*****************************************************************************************************************************************/
void  SunPositionModule::sun_shine(_SUN_SHINE* shine, _SUN_VIEW_LOCATION* sun, _YOUR_TIME* time, _YOUR_LOCATION* location, double windir)
{
    unsigned char flag_bak = 0;
    unsigned char flag_new = 0;
    unsigned int  i;
    double tmp;

    for (i = 0; i<1440; i += 5) {
        time->hour = floor(i/60.0);   // Change hours
        time->min = (double)(i%60);   // Change minutes
        time->sec = 0;                // Change seconds

        sun_position(sun, time, location);

        tmp = (sun->azimuth - windir);
        if (tmp < 0) { tmp = tmp + 360;} // Constrained to the range of 0~360

        if(sun->zenith < 90 && (tmp < 90 || tmp>270)) {
            flag_new = 1;
        } else {
            flag_new = 0;
        }

        if (flag_bak == 0 && flag_new == 1) { shine->start = i;}
        if (flag_bak == 1 && flag_new == 0) { shine->end = i; }

        flag_bak = flag_new;
        yield(); /* Feed the dog to prevent the program from getting stuck */
    }
}

