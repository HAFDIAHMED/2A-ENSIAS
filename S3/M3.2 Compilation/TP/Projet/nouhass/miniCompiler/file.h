#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED


/*
typedef struct slist_s {
    size_t length;
    struct slist_node_s* first;
} slist;

// A node of a singly-linked list.
typedef struct slist_node_s {
    struct slist_node_s* next;
    void* data;
} slist_node;
*/

typedef struct element_file{
    void *data;/* User data.    */
    struct element_f *next;/* Next element. */
}_element_file;


typedef struct file{
    int taille;/* List length. */
    _element_file * debut_file;/* List head.   */
    _element_file * queue_file;
}_file;

void _initialier_file(_file *);
void _emfiler(_file *, _element_file *);
void _defiler(_file *, int index);
void _rechercher_f(_file *,void*);
void _afficher_file(_file *);
#endif // LIST_H_INCLUDED
