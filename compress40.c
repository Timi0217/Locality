/*
*Joshusa Tso
*Favour Okereke-Mba
*HW4: Arith
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "compress40.h"
#include "a2methods.h"
#include "a2blocked.h"
#include "a2plain.h"
#include "assert.h"
#include "pnm.h"

/*******************************************************************************
Function: getImage
Parameters: an input stream
Purpose: reads the contents of the ppm file passed in
Returns: a Pnm_ppm struct 
*******************************************************************************/
static Pnm_ppm getImage(FILE *input){
        A2Methods_T methods = uarray2_methods_blocked; 
        assert(methods != NULL);
        Pnm_ppm source_image = Pnm_ppmread(input, methods);
        assert(source_image != NULL);
        /*TO DO: ASK A TA WHAT TO DO WITH 1x1 2x1 1x2*/
        if (source_image->width % 2 != 0)
                source_image->width--;
        if (source_image->height % 2 != 0)
                source_image->height--;
        return source_image;
}

void compress40  (FILE *input){
        getImage(input);
}

void decompress40  (FILE *input){
        getImage(input);
}