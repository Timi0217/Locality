/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 = HW 3: PART D: PPMTRANS
* 02/20/2018
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "cputiming.h"
#include "pnm.h"

#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}


/*
Function: transform
Parameters: a pointer of type A2Methods_mapfun that determines the type of
mapping to be done when writing the rotated image, and A2Methods_T instance that
lets us know whether to use plain or blocked format, the file containing the 
image to be rotated, an integer containing the degree of rotation to be 
performed on the image, a variable containing the name of the file to write
the time information to, a variable that is either 0 or 1 that let's us know
whether we have received the required parameters to check the time 
performance of the rotation, a variable that lets us know whether the image is
to be flipped horizontally, vertically, or transposed, and an integer that is
either 0 or 1 and lets us know whether to flip/transpose or rotate by x degrees
Purpose: This kick starts the transformation process by calling needed functions
*/
void transform(A2Methods_mapfun *mapType, A2Methods_T mapMethod, 
        FILE *sourceImage, int rotation, char *time_file_name, int timeParam, 
        char *transformation, int whichTransform);

/*
Function: rotate
Parameters: This function takes in a pointer to a struct containing the image
to be rotated, a pointer to a struct that will contain the rotated image, an 
integer that takes in the degree of rotation, a pointer of type A2Methods_mapfun
that determines the type of mapping to be done when writing the rotated image, 
and A2Methods_T instance that lets us know whether to use plain or blocked 
format, a pointer to a file for the time info to be written to, and an int 
variable that saves how long the rotation took
Purpose: This function handles rotating the image 90, 180, or 270 degrees. It 
calls the functions that performs these operations as necessary and returns a
pointer to a struct that contains the rotated image.
*/
Pnm_ppm rotate(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, int rotation, 
        A2Methods_mapfun *mapType, A2Methods_T mapMethod, char *time_file_name,
        int timeParam);
/*
Function: flip
Parameters: This function takes in a pointer to a struct containing the image
to be flipped or transposed, a pointer to a struct that will contain the 
flipped or transposed image, an integer that takes in the degree of rotation, a
pointer of type A2Methods_mapfun that determines the type of mapping to be done
when writing the rotated image, and A2Methods_T instance that lets us know 
whether to use plain or blocked format, a pointer to a file for the time info to
be written to, and an int variable that saves how long the rotation took
Purpose: This function handles flipping the image vertically or horizontally. It
calls the functions that performs these operations as necessary and returns a
pointer to a struct that contains the flipped image.
*/
Pnm_ppm flip(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, char *transformation, 
        A2Methods_mapfun *mapType, A2Methods_T mapMethod, char *time_file_name,
        int timeParam);

/*
Function: createFinalimage
Parameters: This function takes in a pointer to a struct containing the image
to be the rotated image, a pointer to a struct that will contain the rotated 
image, A2Methods_T instance that lets us know whether to use plain or blocked 
format, and a variable that contains the requested degree of rotation
Purpose: It creates the image structure in rows and columns that the original
image will be rotated into
*/
Pnm_ppm createFinalimage(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, 
        A2Methods_T mapMethod, int rotation);

/*
Function: createFinalFlipimage
Parameters: This function takes in a pointer to a struct containing the image
to be the flipped or transposed image, a pointer to a struct that will contain 
the rotated image, A2Methods_T instance that lets us know whether to use plain 
or blocked format, and a variable that contains whether the image is to be 
flipped horizontally, vertically, or transposed.
Purpose: It creates the image structure in rows and columns that the original
image will be rotated into
*/
Pnm_ppm createFinalFlipimage(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, 
        A2Methods_T mapMethod, char *transformation);


/*
Function: rotateImage90
Parameters: The column and row location of a pixel, an instance of an 
A2Methods_UArray2 that contains the original source image, and two void
pointers. The first void pointer points to the address of the element at the 
specified row and col in the source image, and the second pointer points to
to the struct that contains the final image.
Purpose: This function rotates the source image 90 degrees clockwise and
stores its new orientation in the final image 
*/
void rotateImage90(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage);


/*
Function: rotateImage180
Parameters: The column and row location of a pixel, an instance of an 
A2Methods_UArray2 that contains the original source image, and two void
pointers. The first void pointer points to the address of the element at the 
specified row and col in the source image, and the second pointer points to
to the struct that contains the final image.
Purpose: This function rotates the source image 180 degrees clockwise and
stores its new orientation in the final image 
*/
void rotateImage180(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage);


/*
Function: rotateImage270
Parameters: The column and row location of a pixel, an instance of an 
A2Methods_UArray2 that contains the original source image, and two void
pointers. The first void pointer points to the address of the element at the 
specified row and col in the source image, and the second pointer points to
to the struct that contains the final image.
Purpose: This function rotates the source image 270 degrees clockwise and
stores its new orientation in the final image 
*/
void rotateImage270(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage);


/*
Function: flipHorizontal
Parameters: The column and row location of a pixel, an instance of an 
A2Methods_UArray2 that contains the original source image, and two void
pointers. The first void pointer points to the address of the element at the 
specified row and col in the source image, and the second pointer points to
to the struct that contains the final image.
Purpose: This function flips the image horizontally and stores its new 
orientation in the final image 
*/
void flipHorizontal(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage);

/*
Function: flipVertical
Parameters: The column and row location of a pixel, an instance of an 
A2Methods_UArray2 that contains the original source image, and two void
pointers. The first void pointer points to the address of the element at the 
specified row and col in the source image, and the second pointer points to
to the struct that contains the final image.
Purpose: This function flips the image vertically and stores its new 
orientation in the final image 
*/
void flipVertical(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage);

/*
Function: transpose
Parameters: The column and row location of a pixel, an instance of an 
A2Methods_UArray2 that contains the original source image, and two void
pointers. The first void pointer points to the address of the element at the 
specified row and col in the source image, and the second pointer points to
to the struct that contains the final image.
Purpose: This function transposes the image and stores its new orientation in 
the final image 
*/
void transpose(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage);

/*
Function: writeToTimeFile
Parameters: The angle of rotation that was performed on the image, the file to
write the information to, the time it took to rotate the image, and the total
number of pixels of the image
Purpose: The function writes the information about the rotation of the image;
time taken, image size in number of pixels, and the degree of rotation to the 
file passed into it as a parameter
*/
void writeToTimeFile(int rotation, char *time_file_name, double timeElapsed, 
                int totalPixels);

/*
Function: writeToTimeFile Flip
Parameters: The operation that was performed on the image; vertical flip, 
horizontal flip, or transpose the file to write the information to, the time it 
took to perform the operation on the image, and the total number of pixels of 
the image
Purpose: The function writes the information about the rotation of the image;
time taken, image size in number of pixels, and the degree of rotation to the 
file passed into it as a parameter
*/
void writeToTimeFileFlip(char *transformation, char *time_file_name, 
        double timeElapsed, int totalPixels);

int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        char *transformation = NULL;
        int rotation = 0;
        int whichTransform = 0;
        int i, timeParam = 0;
        FILE *sourceImage;
        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                        whichTransform = 0;
                         } else if (strcmp(argv[i], "-flip") == 0){
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        transformation = argv[++i];
                        if (!(strcmp(argv[i], "vertical") == 0 || 
                                strcmp(argv[i], "horizontal") == 0)){
                                fprintf(stderr, 
                                    "flip must be horizontal or vertical\n");
                                usage(argv[0]);
                        }
                        whichTransform = 1;
                } else if (strcmp(argv[i], "-transpose") == 0){
                        transformation = "transpose";
                        whichTransform = 1;
                }else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                        timeParam = 1;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                argv[i]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }
        
        if (i < argc){
                sourceImage = fopen(argv[i], "rb");
                assert(sourceImage != NULL);
        }else{
                sourceImage = stdin;
        }

        transform(map, methods, sourceImage, rotation, time_file_name, 
                timeParam, transformation, whichTransform);
}

void transform(A2Methods_mapfun *mapType, A2Methods_T mapMethod, 
        FILE *sourceImage, int rotation, char *time_file_name, int timeParam, 
        char *transformation, int whichTransform)
{
        Pnm_ppm sourceImageppm;
        Pnm_ppm finalImage = NULL;
        sourceImageppm = Pnm_ppmread(sourceImage, mapMethod);
        if (whichTransform == 0){
                finalImage = rotate(sourceImageppm, finalImage, rotation, 
                                mapType, mapMethod, time_file_name, timeParam);
        }
        else if(whichTransform == 1){
                finalImage = flip(sourceImageppm, finalImage, transformation, 
                                mapType, mapMethod, time_file_name, timeParam);
        }
        if (rotation != 0 || transformation != NULL){
                assert(finalImage != NULL);
                Pnm_ppmwrite(stdout, finalImage);
                Pnm_ppmfree(&sourceImageppm);
                Pnm_ppmfree(&finalImage);
        }
}

Pnm_ppm rotate(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, int rotation, 
        A2Methods_mapfun *mapType, A2Methods_T mapMethod, char *time_file_name,
        int timeParam)
{
        int totalPixels = sourceImageppm->width * sourceImageppm->width;
        CPUTime_T stopWatch;
        double timeElapsed;
        Pnm_ppm tempImage;
        stopWatch = CPUTime_New();
        if (rotation == 0){
                CPUTime_Start(stopWatch);
                Pnm_ppmwrite(stdout, sourceImageppm);
                timeElapsed = CPUTime_Stop(stopWatch);
                Pnm_ppmfree(&sourceImageppm);
                tempImage = NULL;
        }
        else{
                finalImage = createFinalimage(sourceImageppm, finalImage, 
                                mapMethod, rotation);
                assert(finalImage != NULL);
                if (rotation == 90){
                        CPUTime_Start(stopWatch);
                        mapType(sourceImageppm->pixels, rotateImage90, 
                                finalImage);
                        timeElapsed = CPUTime_Stop(stopWatch);
                }
                else if (rotation == 180){
                        CPUTime_Start(stopWatch);
                        mapType(sourceImageppm->pixels, rotateImage180, 
                                finalImage);
                        timeElapsed = CPUTime_Stop(stopWatch);
                }
                else if (rotation == 270){
                        CPUTime_Start(stopWatch);
                        mapType(sourceImageppm->pixels, rotateImage270, 
                                finalImage);
                        timeElapsed = CPUTime_Stop(stopWatch);
                }
                tempImage = finalImage;
        }
        CPUTime_Free(&stopWatch);
        if(timeParam == 1){
                writeToTimeFile(rotation, time_file_name, timeElapsed, 
                        totalPixels);
        }
        return tempImage;
}

Pnm_ppm flip(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, char *transformation, 
        A2Methods_mapfun *mapType, A2Methods_T mapMethod, char *time_file_name,
        int timeParam)
{
        int totalPixels = sourceImageppm->width * sourceImageppm->width;
        CPUTime_T stopWatch;
        double timeElapsed;
        Pnm_ppm tempImage;
        stopWatch = CPUTime_New();
        finalImage = createFinalFlipimage(sourceImageppm, finalImage, 
                        mapMethod, transformation);
        assert(finalImage != NULL);
        if (strcmp(transformation, "vertical") == 0){
                CPUTime_Start(stopWatch);
                mapType(sourceImageppm->pixels, flipVertical, 
                        finalImage);
                timeElapsed = CPUTime_Stop(stopWatch);
        }
        else if (strcmp(transformation, "horizontal") == 0){
                CPUTime_Start(stopWatch);
                mapType(sourceImageppm->pixels, flipHorizontal, 
                        finalImage);
                timeElapsed = CPUTime_Stop(stopWatch);
        }
        else if (strcmp(transformation, "transpose") == 0){
                CPUTime_Start(stopWatch);
                mapType(sourceImageppm->pixels, transpose, 
                        finalImage);
                timeElapsed = CPUTime_Stop(stopWatch);
        }
        tempImage = finalImage;
        CPUTime_Free(&stopWatch);
        if(timeParam == 1){
                writeToTimeFileFlip(transformation, time_file_name, timeElapsed,
                        totalPixels);
        }
        return tempImage;
}

Pnm_ppm createFinalimage(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, 
        A2Methods_T mapMethod, int rotation)
{
        finalImage = malloc(sizeof(*sourceImageppm));
        finalImage->denominator = sourceImageppm->denominator;
        finalImage->methods = mapMethod;
        int elementSize = sizeof(struct Pnm_rgb);
        if (rotation == 90){
                finalImage->height = sourceImageppm->width;
                finalImage->width = sourceImageppm->height;
                finalImage->pixels = mapMethod->new(finalImage->width, 
                                        finalImage->height, elementSize);
                assert(finalImage != NULL);
                return finalImage;
        }
        else if (rotation == 180){
                finalImage->height = sourceImageppm->height;
                finalImage->width = sourceImageppm->width;
                finalImage->pixels = mapMethod->new(finalImage->width, 
                                        finalImage->height, elementSize);
                assert(finalImage != NULL);
                return finalImage;
        }
        else if (rotation == 270){
                finalImage->height = sourceImageppm->width;
                finalImage->width = sourceImageppm->height;
                finalImage->pixels = mapMethod->new(finalImage->width, 
                                        finalImage->height, elementSize);
                assert(finalImage != NULL);
                return finalImage;
        }else
                return NULL;
}

Pnm_ppm createFinalFlipimage(Pnm_ppm sourceImageppm, Pnm_ppm finalImage, 
        A2Methods_T mapMethod, char *transformation)
{
        finalImage = malloc(sizeof(*sourceImageppm));
        finalImage->denominator = sourceImageppm->denominator;
        finalImage->methods = mapMethod;
        int elementSize = sizeof(struct Pnm_rgb);
        if (strcmp(transformation, "vertical") == 0 || 
                strcmp(transformation, "horizontal") == 0){
                finalImage->height = sourceImageppm->height;
                finalImage->width = sourceImageppm->width;
                finalImage->pixels = mapMethod->new(finalImage->width, 
                                        finalImage->height, elementSize);
                assert(finalImage != NULL);
                return finalImage;
        }
        else if (strcmp(transformation, "transpose") == 0){
                finalImage->height = sourceImageppm->width;
                finalImage->width = sourceImageppm->height;
                finalImage->pixels = mapMethod->new(finalImage->width, 
                                        finalImage->height, elementSize);
                assert(finalImage != NULL);
                return finalImage;
        }else
                return NULL;
}

void rotateImage90(int col, int row, A2Methods_UArray2 sourceImageppm, 
        void *element, void *finalImage)
{
        Pnm_rgb finalPosition;
        const struct A2Methods_T *mapMethod = ((Pnm_ppm)finalImage)->methods;
        int new_col = ((Pnm_ppm)finalImage)->width - row - 1;
        int new_row = col;
        finalPosition = mapMethod->at(((Pnm_ppm)finalImage)->pixels, new_col, 
                                        new_row);
        *finalPosition = *(Pnm_rgb)element;
        (void)sourceImageppm;
}


void rotateImage180(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage)
{
        Pnm_rgb finalPosition;
        const struct A2Methods_T *mapMethod = ((Pnm_ppm)finalImage)->methods;
        int new_col = mapMethod->width(sourceImageppm) - col - 1;
        int new_row = mapMethod->height(sourceImageppm) - row - 1;
        finalPosition = mapMethod->at(((Pnm_ppm)finalImage)->pixels, new_col, 
                                        new_row);
        *finalPosition = *(Pnm_rgb)element;
        (void)sourceImageppm;
}

void rotateImage270(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage)
{
        Pnm_rgb finalPosition;
        const struct A2Methods_T *mapMethod = ((Pnm_ppm)finalImage)->methods;
        int new_col = row;
        int new_row = mapMethod->width(sourceImageppm) - col - 1;
        finalPosition = mapMethod->at(((Pnm_ppm)finalImage)->pixels, new_col, 
                                        new_row);
        *finalPosition = *(Pnm_rgb)element;
        (void)sourceImageppm;
}

void flipHorizontal(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage)
{
        /*fprintf(stderr, "I get here\n");*/
        Pnm_rgb finalPosition;
        const struct A2Methods_T *mapMethod = ((Pnm_ppm)finalImage)->methods;
        int new_col = mapMethod->width(sourceImageppm) - col - 1;
        int new_row = row;
        finalPosition = mapMethod->at(((Pnm_ppm)finalImage)->pixels, new_col, 
                                        new_row);
        *finalPosition = *(Pnm_rgb)element;
        (void)sourceImageppm;
}

void flipVertical(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage)
{
        Pnm_rgb finalPosition;
        const struct A2Methods_T *mapMethod = ((Pnm_ppm)finalImage)->methods;
        int new_col = col;
        int new_row = mapMethod->height(sourceImageppm) - row - 1;
        finalPosition = mapMethod->at(((Pnm_ppm)finalImage)->pixels, new_col, 
                                        new_row);
        *finalPosition = *(Pnm_rgb)element;
        (void)sourceImageppm;
}

void transpose(int col, int row, A2Methods_UArray2 sourceImageppm, 
                void *element, void *finalImage)
{
        Pnm_rgb finalPosition;
        const struct A2Methods_T *mapMethod = ((Pnm_ppm)finalImage)->methods;
        int new_col = row;
        int new_row = col;
        finalPosition = mapMethod->at(((Pnm_ppm)finalImage)->pixels, new_col, 
                                        new_row);
        *finalPosition = *(Pnm_rgb)element;
        (void)sourceImageppm;
}

void writeToTimeFile(int rotation, char *time_file_name, double timeElapsed, 
        int totalPixels)
{
        FILE *timingFile;
        double timePerPixel = timeElapsed / totalPixels;
        timingFile = fopen(time_file_name, "w+");
        fprintf(timingFile, "Degree of rotation: %d\n", rotation); 
        fprintf(timingFile, "Time Elapsed: %0.0f nanoseconds.\n", timeElapsed);
        fprintf(timingFile, "Number of Pixels: %d\n",totalPixels);
        fprintf(timingFile, "Time per Pixel: %0.0f nanoseconds",timePerPixel);
        fclose(timingFile);
}

void writeToTimeFileFlip(char *transformation, char *time_file_name, 
        double timeElapsed, int totalPixels)
{
        FILE *timingFile;
        double timePerPixel = timeElapsed / totalPixels;
        timingFile = fopen(time_file_name, "w+");
        fprintf(timingFile, "Transformation: %s\n", transformation); 
        fprintf(timingFile, "Time Elapsed: %0.0f nanoseconds.\n", timeElapsed);
        fprintf(timingFile, "Number of Pixels: %d\n",totalPixels);
        fprintf(timingFile, "Time per Pixel: %0.0f nanoseconds",timePerPixel);
        fclose(timingFile);
}