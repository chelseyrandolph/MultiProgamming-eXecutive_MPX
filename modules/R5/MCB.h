
typedef struct CMCB {
	int size;
	char name[];
	int type; //1 for alloc, 0 for free
	CMCB* next;
	CMCB* prev;
	char startAddr[];

}


typedef struct LMCB {
	int size;
	int type; //1 for alloc, 0 for free
}
