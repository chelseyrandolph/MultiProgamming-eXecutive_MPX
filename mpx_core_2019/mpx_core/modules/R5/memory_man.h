
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



u32int init_heap(u32int heap_size_param);

void *alloc_mem(u32int num_bytes);

int free_mem(void *addr);

void merge_free_blocks(CMCB *freeblock);

int isEmpty();

void show_free_mem();

void show_alloc_mem();

void unlink(CMCB* mcb);

void insert_mem(CMCB* mcb);

void function();
