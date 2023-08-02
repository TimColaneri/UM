//Timothy Colaneri
//Universal Machine Memory Segment Implementation

/**********************************************************/
#include<stdlib.h>
#include<stdio.h>   //Output/input instructions
//#include"stack.h"   //Used to handle unmapped segments
#include<seq.h>     //Used to represent memory segment
#include<array.h>   //
#include<assert.h>  //Assertions
#include"um_mem.h"  //Own header
/**********************************************************/
//A Memseg_T structure is a representation of a universal
//machine memory segment. The sequence segemnts holds each
//segmented piece of the memory. The Queue unmapped will
//contain a list of each memory space which has been 
//released and is availible for reuse.
//At any point in execution of a program, the Memseg_T's
//segments sequence will contain all memory segments
//currently stored in the program
//At any point in execution of a program, the unmapped
//stack will contain a list of all of the memory spaces
//which have been unmapped and not reused.

void Stack_resize(lwStack stack);
uint32_t * copyArray(uint32_t* arr,unsigned size);
void Memseg_resize(Memseg_T memSpace);
/**********************************************************/
//Memseg_init creates a new Memseg_T memory segment,
//initializes all of its values to empty, and returns the
//new memory segment.
extern Memseg_T Memseg_init(){

    //Create the new memory space
    Memseg_T memSpace = malloc(sizeof(*memSpace));
    assert(memSpace);

    //Initialize the memory structs members  
    memSpace->array = calloc(40000,sizeof(struct lwArray));
    //printf("%li %li\n",sizeof(lwArray),sizeof(struct lwArray));
    //memSpace->unmapped = Stack_new();
    memSpace->size = 40000;
    memSpace->index = -1;
    lwStack unmapped = malloc(sizeof(struct lwStack));
    unmapped->index = 0;
    unmapped->size = 2000;
    unmapped->stack = calloc(2000,sizeof(uint32_t));
    memSpace->unmapped = unmapped;
    //Assert that the memory space was availible
    assert(memSpace->array);
    assert(memSpace->unmapped);    
    return memSpace;
}
/**********************************************************/
//Memseg_store stores a new value 'elem' into the memory segment
//located at 'seg'. It is placed into this word(memory segment)
// at offset 'offset' 
extern void Memseg_store(Memseg_T memSpace,uint32_t elem,int seg, int offset){
    //printf("got in store with word %i for %i\n",elem,seg);
    //lwArray memSeg = ((memSpace->array)[seg]);
    (((memSpace->array)[seg])->array)[offset] = elem;
    //printf("got out of store with set word %i\n",(memSeg->array)[offset]);
    //*val = elem;
}
/**********************************************************/
//Memseg_load loads a value from the memory space 'memSpace'
//found in the segment 'seg' at offset 'offset'. This 
//function then returns that value
extern uint32_t Memseg_load(Memseg_T memSpace,int seg,int offset){
    //printf("got in load with %i %i for %i\n",memSpace->size,seg,memSpace->index);
    //lwArray memSeg = ((memSpace->array)[seg]);
    //printf("got memSeg %i %i\n",memSeg->size,offset);
    return (((memSpace->array)[seg])->array)[offset];
}
/**********************************************************/
//Memseg_map creates a new segment with a number of words
//equal to 'size'. A pointer to this new segment is then
//returned from the function
extern uint32_t Memseg_map(Memseg_T memSpace, int size){

    //Array_T newSeg = Array_new(size,sizeof(uint32_t));
    lwArray newSeg = malloc(sizeof(*newSeg));
    //printf("%li\n",sizeof(*newSegment));
    newSeg->array = calloc(size,sizeof(uint32_t));
    newSeg->size = size;
    //newSeg->array[0] = 0;
    //(void)newSegment;
    //assert(newSeg);

    //Determine if any memory spaces have been previously
    //freed, if so use the freed up memory space
    //else, add a new memory space

    lwStack unmapped = memSpace->unmapped;

    if (unmapped->index == 0){
        if (memSpace->index == memSpace->size){
            Memseg_resize(memSpace);
        }
        (memSpace->array)[++(memSpace->index)] = newSeg;
        return (memSpace->index);
    }
    else{
        uint32_t index = unmapped->stack[ ((unmapped->index--) - 1) ];
        (memSpace->array)[index] = newSeg;
        return index;
    }
}
/**********************************************************/
//Memseg_unmap unmaps the memery segment 'seg' found in the
//memory space memSpace.
extern void Memseg_unmap(Memseg_T memSpace, uint32_t seg){
    lwArray oldSeg = (memSpace->array)[seg];
    free(oldSeg->array);
    free(oldSeg);
    lwStack unmapped = memSpace->unmapped;
    if (unmapped->index >= unmapped->size){
        Stack_resize(unmapped);
    }
    (unmapped->stack)[(unmapped->index)++] = seg; 
}
/**********************************************************/
//Memseg_load_prog duplicates the memory segment found in
//'memSpace' at 'seg'. This segment is then loaded into 
//'memSpace' at postion 0, and the former code at postion 0
//is abandoned.
extern void Memseg_load_prog(Memseg_T memSpace,int seg){
    //printf("got in load prog\n");
    lwArray segment = (memSpace->array)[seg];
    lwArray newSeg = malloc(sizeof(*newSeg));
    (newSeg->array) = copyArray((segment->array),segment->size);
    newSeg->size = segment->size;
    //lwArray oldSeg = (memSpace->array)[0];
    (memSpace->array)[0] = newSeg;
    //free(oldSeg->array);
    //free(oldSeg);
    //printf("out of load prog\n");
}
/**********************************************************/
//Memspace_free frees the memory space used up by the passed
//in 'memSpace' struct.
extern void Memseg_free(Memseg_T memSpace){
    //printf("got into free\n");

    //int totalSpace = (memSpace->index);
    //lwArray segment;

    //Iterate through all of the segment in the memory space
    //and free them.
    /*
    for (int i = 0;i < (totalSpace);++i){
        segment = (memSpace->array)[i];
        if (segment != NULL){
            //if (segment->array != NULL)
                //free(segment->array);
            //printf("freed array\n");
            free(segment);

        }
    }*/
    //printf("halfway\n");
    //Free the actual memory space stucture
    free((memSpace->array));
    free((memSpace->unmapped));
    free(memSpace);
    //printf("Got out of free\n");
}
/**********************************************************/
uint32_t * copyArray(uint32_t* arr,unsigned size){
    uint32_t* newArr = calloc(size,sizeof(uint32_t));

    for (unsigned i = 0; i < size;++i){
        newArr[i] = arr[i];
    }
    return newArr;
}
/***************************************************************/
void Memseg_resize(Memseg_T memSpace){

    memSpace->size = (memSpace->size) * 2;
    memSpace->array = realloc(memSpace->array,memSpace->size * (sizeof(struct lwArray)));
}
void Stack_resize(lwStack stack){
    stack->size = (stack->size) * 2;
    stack->stack = realloc(stack->stack,stack->size * (sizeof(uint32_t)));
}