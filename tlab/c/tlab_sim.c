
/* Simulate TLAB throughput in ideal case.
 * Ignore how to do GC properly. Here we just make sure to
 * delete all the objects before the tlab gets full.
 * Just want to see if reducing number of malloc/free syscalls
 * gives a speedup.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAINLOOP_CNT 100000000
// objects are just char arrays
// OBJ_SIZE is the number of chars in each char array
#define OBJ_SIZE            10
// GC will run after this many allocations
#define GC_INTERVAL         10
// TLAB size must be greater than OBJ_SIZE * GC_INTERVAL
#define TLAB_SIZE         OBJ_SIZE * GC_INTERVAL

// set this flag from makefile
// if USE_TLAB is 1, will cache malloc/free using TLAB
// if USE_TLAB is 0, will malloc/free each object directly
#ifndef USE_TLAB
#define USE_TLAB 1
#endif

typedef struct{
    // buffer to allocate objects from
    char buf[TLAB_SIZE];
    // index of the start of the free portion of the buffer
    char top;
} tlab;

typedef struct{
    tlab *tlab;
    // count of how many objects have been allocated
    int alloc_count;
    // Array of pointers to objects (may be located in TLAB)
    // It's size is fixed by GC_INTERVAL because we expect to
    // get rid of all the objects whenever we do GC
    char *objs[GC_INTERVAL];
} context;

// initialize context members
void init_context(context *cxt);
// allocate a new obj and add it to cxt
// may possibly call garbage_collect
char *alloc_new_obj(context *cxt);
// free all the objects in cxt (we assume short-lived objects)
int garbage_collect(context *cxt);

int main (void){

    int i;
    context cxt;
    // object is just a char array of OBJ_SIZE
    char *obj;

    init_context(&cxt);
    
    // allocate and abandon a bunch of objects
    // our fake GC will kick in automatically
    // (called from inside alloc_new_obj)
    for(i=0; i<MAINLOOP_CNT; i++){
        obj = alloc_new_obj(&cxt);
        if(obj == NULL){
            return 1;
        }
        strncpy(obj, "abcdefghi", sizeof(char)*OBJ_SIZE);
    }

    return 0;
}

void init_context(context *cxt){
    int i;

    cxt->tlab = NULL;
    cxt->alloc_count = 0;
    for(i=0; i<GC_INTERVAL; i++){
        cxt->objs[i] = NULL;
    }
}

// return the obj on success; return NULL on failure;
char *alloc_new_obj(context *cxt){
    char *new_obj = NULL;

    // Check to see if we need to GC
    if(cxt->alloc_count >= GC_INTERVAL){
        if(!garbage_collect(cxt)){
            return NULL;
        }
    }

#if USE_TLAB
    // check if a TLAB exists, create it if it dosent exist
    if(cxt->tlab == NULL){
        cxt->tlab = malloc(sizeof(tlab));
        if(cxt->tlab == NULL){
            return NULL;
        }
        // init tlab top
        cxt->tlab->top = 0;
    }

    // check if there is enough space in the TLAB
    // return failure if we don't have enough space
    if(cxt->tlab->top + OBJ_SIZE > TLAB_SIZE){
        return NULL;
    }
    
    // point the new obj to the free space in tlab
    // bump the tlab top
    new_obj = cxt->tlab->buf + cxt->tlab->top;
    cxt->tlab->top += OBJ_SIZE;

#else //USE_TLAB disabled
    // allocate the object directly from malloc
    new_obj = malloc(sizeof(char)*OBJ_SIZE);
    if(new_obj == NULL){
        return NULL;
    }
#endif

    // add the new obj to the obj array and update alloc count
    cxt->objs[cxt->alloc_count] = new_obj;
    cxt->alloc_count++;

    return new_obj;
}

// return 1 on success, 0 on failure
int garbage_collect(context *cxt){
#if USE_TLAB
    // free the tlab
    free(cxt->tlab);
#else //USE_TLAB disabled
    // loop through the array of objects and free them all
    int i;
    for(i=0; i<GC_INTERVAL; i++){
        free(cxt->objs[i]);
    }
#endif
    // re-init the context
    init_context(cxt);
    return 1;
}
