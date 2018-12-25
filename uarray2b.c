/*
* Favour Okereke-Mba; FOKERE01
* Timi Dayo-Kayode; ODAYOK01
* COMP 40 = HW 3: PART A: UARRAY2B
* 02/20/2018
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "uarray.h"
#include "uarray2.h"
#include "uarray2b.h"


#define T UArray2b_T

/*Struct containing our block array along 
with some useful information about is*/
struct T {
        int col;
        int row;
        int size;
        int blockSize;
        int blockColumns;
        int blockRows;
        UArray2_T blockArray;
};

/*
Function: createArrays
Parameters: An instance of the struct T
Purpose: This function makes every slot in our 2D array a 1D array that
represents a block
*/
void createArrays (T myArray){
        int i, j, blocksize = myArray->blockSize, size = myArray->size;
        int rows = myArray->blockRows;
        int columns = myArray->blockColumns;
        for (i = 0; i < rows; i++){
                for (j = 0; j < columns; j++){
                        UArray_T *oneDarray = UArray2_at(
                                myArray->blockArray, j, i);
                        *oneDarray = UArray_new((blocksize * blocksize), 
                                                        size);  
                                        assert(*oneDarray != NULL);
                }
        }
}

/*
Function: UArray2b_new
Parameters: width height, size, and blocksize
Purpose: This function creates a 2D array of arrays containing blocks
*/
T UArray2b_new (int width, int height, int size, int blocksize){
        int raySize = blocksize * blocksize * size, colMod, rowMod;
        int blockColumns = width / blocksize, blockRows = height / blocksize;
        colMod = width % blocksize;
        rowMod = height % blocksize;
        if (colMod != 0)
                blockColumns++;
        if (rowMod != 0)
                blockRows++;
        T myBlockedarray = malloc(sizeof(struct UArray2b_T));
        myBlockedarray -> col = width;
        myBlockedarray -> row = height;
        myBlockedarray -> size = size;
        myBlockedarray -> blockRows = blockRows;
        myBlockedarray -> blockColumns = blockColumns;
        myBlockedarray -> blockSize = blocksize;
        myBlockedarray -> blockArray = UArray2_new(blockColumns, 
                                                blockRows, raySize);
        createArrays(myBlockedarray);
        assert(myBlockedarray != NULL);
        return myBlockedarray; 
}

/*
Function: UArray2b_new_64K_block
Parameters: A width, height, and size
Purpose: Creates a 2D array of arrays that contains blocks whose sizes are 
         capped at 64K, and if more than 64KB is required for a block of data, 
         it sets the blocksize to 1.
*/
T UArray2b_new_64K_block(int width, int height, int size){
        int blockSize, root, value;
        value = 64000 / size;
        root = pow(value, 0.5);
        if (root * root * size > 64000)
                blockSize = 1;
        else
                blockSize = root;
        return UArray2b_new(width, height, size, blockSize);

}

/*
Function: UArray2b_free
Parameters: A pointer to a struct T 
Purpose: It frees up all the memory used up by the struct pointer parameter
*/
void UArray2b_free( T *array2b){
        assert(array2b != NULL && *array2b != NULL);
        int i, j;
        int row = UArray2_height((*array2b)->blockArray);
        int col = UArray2_width((*array2b)->blockArray);
        for (i = 0; i < row; i++){
                for (j = 0; j < col; j++){
                        UArray_free((UArray2_at((*array2b)->blockArray, j, i)));
                }
        }
        UArray2_free(&(*array2b)->blockArray);
        free(*array2b);
}

/*
Function: UArray2b_width
Parameters: an instance of struct T.
Purpose: Checks that the struct instance is not NULL, and then proceeds to
         return the width of the 2D array
*/
int UArray2b_width(T array2b){
        assert(array2b != NULL);
        return array2b->col;
}

/*
Function: UArray2b_height
Parameters: an instance of struct T.
Purpose: Checks that the struct instance is not NULL, and then proceeds to
         return the height of the 2D array
*/
int UArray2b_height(T array2b){
        assert(array2b != NULL);
        return array2b->row;
}

/*
Function: UArray2b_size
Parameters: an instance of struct T.
Purpose: Checks that the struct instance is not NULL, and then proceeds to
         return the size of the structs instance of a UArray2_T
*/
int UArray2b_size(T array2b){
        assert(array2b != NULL);
        return array2b->size;
}

/*
Function: UArray2b_blocksize 
Parameters: an instance of struct T.
Purpose: Checks that the struct instance is not NULL, and then proceeds to
         return the size of the structs instance of a UArray2_T array
*/
int UArray2b_blocksize(T array2b){
        assert(array2b != NULL);
        return array2b->blockSize;
}

/*
Function: UArray2b_at
Parameters: an instance of struct T, and row and column integer values
Purpose: Takes a row and column from the 2D representation that the client is 
         aware of, and returns a pointer to the address of the location that it
         maps to in our 2D array of arrays containing blocks.
*/
void *UArray2b_at(T array2b, int columns, int rows){
        assert(array2b != NULL);
        assert(columns >= 0);
        assert(rows >= 0);
        int blockSize = array2b->blockSize;
        int col = columns / blockSize;
        int row = rows / blockSize;
        UArray_T *oneDarray = UArray2_at(array2b->blockArray, col, row);
        int pos = blockSize * (rows % blockSize) + (columns % blockSize);
        return UArray_at(*oneDarray, pos);
}

/*
Function: UArray2b_map
Parameters: an instance of struct T, an apply function, and a closure
Purpose: Maps through the UArray2_T member array of the struct T parameter in 
         row major format and applies the passed function to each element in the
         array; in this case, the apply function
*/
void UArray2b_map(T array2b, 
        void apply(int col, int row, T array2b, void *elem, void *cl), 
        void *cl){
        assert(array2b != NULL);
        int rows = UArray2_height(array2b->blockArray);
        int columns = UArray2_width(array2b->blockArray);
        int i, j, k, col, row, length, blockSize = (array2b)->blockSize;
        void *pos2;
        for (i = 0; i < rows; i++){
                for (j = 0; j < columns; j++){
                        UArray_T *oneDarray = UArray2_at(
                                array2b->blockArray, j, i);
                        length = UArray_length(*oneDarray);
                        for (k = 0; k < length; k++){
                                col = (j * blockSize) + (k % blockSize);
                                row = (i * blockSize) + (k / blockSize);
                                if (col < array2b->col && row < array2b->row){
                                        pos2 = UArray_at(*oneDarray, k);
                                        apply(col, row, array2b, pos2, cl);
                                }
                        }
                }
        }
}

#undef T