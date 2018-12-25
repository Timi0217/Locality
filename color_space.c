/*
*Joshusa Tso
*Favour Okereke-Mba
*HW4: Arith
*/
#include "color_space.h"

typedef A2Methods_UArray2 A2;

static void rgb2ypp_convert(int col, int row, A2 array2, 
                        A2Methods_Object *ptr, void *cl){
        (void) array2;
        Pnm_rgb elem = ptr;     
        Ypp_ppm closure = cl;   
        unsigned red = elem->red, green = elem->green, blue = elem->blue;
        Color_ypp pixel_info = malloc(sizeof(struct Color_ypp));
        /* TO DO: DO SOMETHING ABOUT THE LITERALS*/
        pixel_info->Y = 0.299 * red + 0.587 * green + 0.144 * blue;
        pixel_info->Pb = -0.168736 * red - 0.331264 * green + 0.5 * blue;
        pixel_info->Pr = 0.5 * red - 0.418688 * green - 0.081312 * blue;
        Color_ypp destination = closure->methods->at(closure->pixels, col, row);
        *destination = *pixel_info;
}

extern Ypp_ppm rgb2ypp(Pnm_ppm source_image, A2Methods_T ypp_methods, int size){
        assert(source_image != NULL && ypp_methods != NULL);
        unsigned width = source_image->width, height = source_image->height;
        Ypp_ppm output_image = malloc(sizeof(struct Ypp_ppm));
        output_image->width = width;
        output_image->height = height;
        /* TO DO: ASK WHAT THE DENOMINATOR IS FOR*/
        output_image->denominator = source_image->denominator;
        output_image->methods = ypp_methods;
        output_image->pixels = ypp_methods->new_with_blocksize(width, height, 
                                sizeof(struct Color_ypp), size);
        source_image->methods->map_default(source_image->pixels, 
                                rgb2ypp_convert, output_image);
        /* TO DO: Free Pnm_ppm after this function is called in compress40*/
        return output_image;
}