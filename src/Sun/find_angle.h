#ifndef __FIND_ANGLE_H__
#define __FIND_ANGLE_H__

#include <Arduino.h>
#include <math.h>
#define PI 3.14159265358979323846
#define LAMADA 350  /*  Criterion light wavelength nm */

typedef struct {
    double x;
    double y;
    double z;
} _VECTOR3;

typedef struct {
    _VECTOR3 Vec[3];
} _VECTOR3x3;

typedef struct {
    double max_refract2_rotate;
    double max_refract2_inAngle;
    unsigned int flag;
} _BEST_RESULT;

typedef struct {
    double start;
    double end;
    double snap;
} _SCAN_RANGE;

typedef struct {
    double azimuth;
    double altitude;
    double windir;
    double n_zi;           /* Criterion refractive index of light      */
    double linjie;         /* Criterion critical angle of total reflection of light */
    _SCAN_RANGE range;
} _SCAN_PARAMETER;

typedef struct {
    char   id[16];      /* City ID */
    double ew;          /* Longitude   */
    double sn;          /* Latitude   */
} _CITY;

class FindAngleModule
{
public:
    _BEST_RESULT Best;

    /*****************************************************************************************************************
       Get the refractive index of light of a specified wavelength
     *****************************************************************************************************************/
    double get_refract_n(double lamada);
    /*****************************************************************************************************************
      Find the angle between two unit vectors
     *****************************************************************************************************************/
    double angle_of_vector(_VECTOR3* A, _VECTOR3* B);
    /*****************************************************************************************************************
      Generates orthogonal vectors of two non-parallel vectors
     *****************************************************************************************************************/
    void vectgen_cross_vector(_VECTOR3* cross_vector, _VECTOR3* A, _VECTOR3* B);
    /*****************************************************************************************************************
       Generate rotation matrix
     *****************************************************************************************************************/
    void gen_rotate_vector(_VECTOR3x3* rotate_vector, _VECTOR3* vector, double angle);
    /*****************************************************************************************************************
       Rotate a vector using a rotation matrix
    ****************************************************************************************************************/
    void do_rotate_vector(_VECTOR3* out_vec, _VECTOR3x3* rotate_vector, _VECTOR3* in_vec);
    /******************************************************************************************************************
    *  The sun's orientation is converted into a vector with modulus 1
    * Consistent with the right-hand coordinate system of openGL
    *              North y+ 0°
    *               /|\
    *                |
    *                |
    * 270°westx- --------------> East x+ 90°
    *                |
    *                |
    *                |
    *              South y- 180°
    * Input parameters:
    * azimuth solar altitude angle (-90~90 degrees)
    * altitude solar azimuth angle (0~360 degrees) uses the coordinate system above
    * windir window orientation (0~360 angle) uses the coordinate system above
    * range traverse range and traversal step size
    *
    * Output parameters:
    * max_refract2_rotate bubble sorting, the product of the maximum incident angle of the second refraction and the effective illumination area criterion
    * max_refract2_inAngle The maximum incident angle of the second refraction and the corresponding rotation angle
    * flag: 0=Normal 1: It is daytime, but the window has no light, 2: Night
    *****************************************************************************************************************/
    void scan_best_angle(_BEST_RESULT* best, _SCAN_PARAMETER* parm);
    /*****************************************************************************************************************
      Update the range and step size of the iteration traversal
     *****************************************************************************************************************/
    void update_range(_BEST_RESULT* best, _SCAN_PARAMETER* parm, double new_snap);
    /*****************************************************************************************************************
      Find the best prism position at a certain moment
     *****************************************************************************************************************/
    void find_best_angle(_BEST_RESULT* best, double zenith, double  azimuth, double windir);
    /*****************************************************************************************************************
      Find nearest city
     *****************************************************************************************************************/
    unsigned  int  find_city(double ew, double sn);
    /*****************************************************************************************************************
      Get city ID string
     *****************************************************************************************************************/
    const char* get_city_id(unsigned int city);
};

extern FindAngleModule FindAngle;

#endif /* __FIND_ANGLE_H__ */

