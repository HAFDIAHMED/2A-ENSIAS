#include <stdio.h>
#include "file.h"

void _initialier_file(_file * f){
    f->taille = 0;
    f->debut_file= NULL;
    f->queue_file= NULL;
}
void _emfiler(_file * f,_element_file* el){
   el->next=f->debut_file;
   f->debut_file=el;
   f->taille++;
}
void _defiler(_file * f, int index){
    int i=0;
    while(i<=index<=f->taille && i!=index)
        i++;
    if(index>0 && index<=f->taille){
       // _element_file * em = em;
    }
}
