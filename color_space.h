/*
*Joshusa Tso
*Favour Okereke-Mba
*HW4: Arith
*/

#ifndef COLOR_SPACE
#define COLOR_SPACE
#include <stdint.h>
#include <stdlib.h>
#include <except.h>
#include <stdio.h>
#include "a2methods.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "assert.h"
#include "pnm.h"


/*
 * Every function in this interface uses the (x, y) coordinate system,
 * which is the same as the (col, row) or (width, height) system.
 */

/*Pixels in component video color space*/
typedef struct Color_ypp {
        float Y, Pb, Pr;
} *Color_ypp;

typedef struct Ypp_ppm {
        unsigned width, height, denominator;
        A2Methods_UArray2 pixels;          /* 2D array with
                                            * element type 'struct Color_ypp'
                                            */
        const struct A2Methods_T *methods; /* to operate on 'pixels' field */
} *Ypp_ppm;

/*******************************************************************************
Function: rgb2ypp
Parameters: Pnm_ppm struct containing 2D array of rgb pixels to be converted,
            A2Methods_T specifying the method to be used to create the 2D array 
            in the Ypp_ppm struct.
            int size which specifies the blocksize if the methods is blocked
Purpose: Converts from rgb color space to ypp color space.
Returns: A Ypp_ppm struct that contains the 2D array Color_ypp structs.
*******************************************************************************/
Ypp_ppm rgb2ypp(Pnm_ppm source_image, A2Methods_T ypp_methods, int size);

#endif