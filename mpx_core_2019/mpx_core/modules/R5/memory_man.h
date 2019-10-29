
typedef struct CMCB {
	int size;
	char name[16];
	int type; //1 for alloc, 0 for free
	struct CMCB* next;
	struct CMCB* prev;
	char startAddr[];

}CMCB;


typedef struct LMCB {
	int size;
	int type; //1 for alloc, 0 for free
}LMCB;

typedef struct mem_queue {
	int count;
	struct CMCB* head;
	struct CMCB* tail;
}mem_queue;



int init_heap(u32int heap_size_param);

void alloc_mem(int num_bytes);

void free_mem(char addr[]);

int isEmpty();

void show_free_mem();

void show_alloc_mem();
