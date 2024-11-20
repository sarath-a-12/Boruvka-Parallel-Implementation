#ifndef SET_H
#define SET_H
class Set {
    public:
        int tot_size;
        int *elems;
        int *parent;
        int *size;
        void *settype;
        void set(){
            elems=parent=size=NULL;
            settype=NULL;
        }
};

#endif

