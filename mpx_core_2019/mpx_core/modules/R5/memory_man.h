
typedef struct CMCB {
	int size;
	char name[16];
	int type; //1 for alloc, 0 for free
	struct CMCB* next;
	struct CMCB* prev;
	void* startAddr;

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

u32int alloc_mem(u32int num_bytes);

int free_mem(void *addr);

int isEmpty();

void show_free_mem();

void show_alloc_mem();

void unlink(CMCB* mcb);

void insert(CMCB* mcb);

void function();
