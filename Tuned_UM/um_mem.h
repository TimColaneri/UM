//Timothy Colaneri
//Universal Machine Memory interface

/**********************************************************************/
#ifndef MEMSEG_INCLUDED

#define MEMSEG_INCLUDED

#include <inttypes.h>
#include <stack.h>

typedef struct lwArray{ //Light-weight array
    uint32_t * array;
    unsigned size;
}*lwArray;

typedef struct lwStack{ //Light-weigth stack
    uint32_t * stack;
    uint32_t size;
    uint32_t index;
}*lwStack;

#define T Memseg_T
struct T {
    //Seq_T segments;
    lwArray* array;
    unsigned size;  //Max size of array
    unsigned index; //Current size of used elements
    lwStack unmapped;
};
typedef struct T *T;

/**********************************************************************/

extern T Memseg_init();

//Memseg_init creates a new Memseg_T memory segment,

//initializes all of its values to empty, and returns the

//new memory segment.

extern void Memseg_store(T memSpace,uint32_t elem,int seg, int offset);

//Memseg_store stores a new value 'elem' into the memory segment

//located at 'seg'. It is placed into this word(memory segment)

// at offset 'offset' 

extern uint32_t Memseg_load(T memSpace,int seg,int offset);

//Memseg_load loads a value from the memory space 'memSpace'

//found in the segment 'seg' at offset 'offset'. This 

//function then returns that value

extern uint32_t Memseg_map(T memSpace, int size);

//Memseg_map creates a new segment with a number of words

//equal to 'size'. A pointer to this new segment is then

//returned from the function

extern void Memseg_unmap(T memSpace, uint32_t seg);

//Memseg_unmap unmaps the memery segment 'seg' found in the

//memory space memSpace.

extern void Memseg_load_prog(T memSpace,int seg);

//Memseg_load_prog duplicates the memory segment found in

//'memSpace' at 'seg'. This segment is then loaded into 

//'memSpace' at postion 0, and the former code at postion 0

//is abandoned.

extern void Memseg_free(T memSpace);

//Memspace_free frees the memory space used up by the passed

//in 'memSpace' struct.

/**********************************************************************/

#undef T 

#endif

/**********************************************************************/