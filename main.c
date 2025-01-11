#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define N 10000000
#define SKIP_MAX_HEIGHT 16

typedef struct LLNode
{
	int x;
	struct LLNode* next;
}LLNODE;

typedef struct SkipNode
{
	int x;
	int height;
	struct SkipNode* next[SKIP_MAX_HEIGHT];
}SKIP_NODE;

LLNODE* CreateLinkedList(int x)
{
	LLNODE* node = (LLNODE*)calloc(1, sizeof(LLNODE));
	if (node == NULL)
	{
		perror("Cannot create LL");
		exit(-1);
	}

	node->x = x;
	node->next = NULL;

	return node;
}

void AddToLinkedList(LLNODE** LinkedList, int x)
{
	if (*LinkedList == NULL)
	{
		perror("Passed wrong param. Need PTR** but got PTR*");
		exit(-2);
	}

	LLNODE* node = (LLNODE*)calloc(1, sizeof(LLNODE));
	if (node == NULL)
	{
		perror("Cannot create a new LL node");
		exit(-3);
	}

	node->x = x;
	node->next = *LinkedList;
	*LinkedList = node;
}

LLNODE* SearchLinkedList(LLNODE* head, int wanted)
{
	for (LLNODE* node = head; node != NULL; node = node->next)
	{
		if (node->x == wanted)
			return node;
	}

	return NULL;
}

LLNODE* FreeLinkedList(LLNODE** LinkedList)
{
	if (*LinkedList == NULL)
	{
		perror("Passed wrong param. Need PTR** but got PTR*");
		exit(-4);
	}

	for (LLNODE* node = *LinkedList; node != NULL;)
	{
		LLNODE* previous = node;
		node = node->next;
		free(previous);
	}

	*LinkedList = NULL;
}

void PrintLinkedList(LLNODE* head)
{
	for (LLNODE* node = head; node != NULL; node = node->next)
	{
		printf("%d ", node->x);
	}
}

int GenerateSkipNodeHeight()
{
	/*for (int i = 1; i < SKIP_MAX_HEIGHT + 1; i++)
	{
		if (rand() % 2 == 0)
			return i;
	}

	return SKIP_MAX_HEIGHT;*/

	return rand() % SKIP_MAX_HEIGHT + 1;
}

SKIP_NODE* CreateSkipList(int x)
{
	SKIP_NODE* header = (SKIP_NODE*)calloc(1, sizeof(SKIP_NODE));
	if (header == NULL)
	{
		perror("Cannot allocate memory for skip list.");
		exit(-5);
	}
	header->x = INT_MAX;
	header->height = SKIP_MAX_HEIGHT;

	SKIP_NODE* sentinel = (SKIP_NODE*)calloc(1, sizeof(SKIP_NODE));
	if (sentinel == NULL)
	{
		perror("Cannot allocate memory for skip list.");
		exit(-5);
	}
	sentinel->x = INT_MAX;
	sentinel->height = SKIP_MAX_HEIGHT;

	for (int i = 0; i < SKIP_MAX_HEIGHT; i++)
	{
		header->next[i] = sentinel;
		sentinel->next[i] = NULL;
	}

	SKIP_NODE* first = (SKIP_NODE*)calloc(1, sizeof(SKIP_NODE));
	if (first == NULL)
	{
		perror("Cannot allocate memory for skip list.");
		exit(-5);
	}

	first->x = x;
	first->height = GenerateSkipNodeHeight();
	for (int i = 0; i < first->height; i++)
	{
		first->next[i] = sentinel;
		header->next[i] = first;
	}
	for (int i = first->height; i < SKIP_MAX_HEIGHT; i++)
	{
		first->next[i] = sentinel;
	}

	return header;
}

void AddToSkipList(SKIP_NODE** SkipList, int x)
{
	if (*SkipList == NULL)
	{
		perror("Expected PTR** but got PTR*");
		exit(-6);
	}

	SKIP_NODE* node = (SKIP_NODE*)calloc(1, sizeof(SKIP_NODE));
	if (node == NULL)
	{
		perror("Cannot create a new skip list node");
		exit(-7);
	}

	node->x = x;
	node->height = GenerateSkipNodeHeight();

	SKIP_NODE* previous[SKIP_MAX_HEIGHT];
	for (int i = 0; i < SKIP_MAX_HEIGHT; i++)
	{
		previous[i] = *SkipList;
	}
	SKIP_NODE* currentSearchNode = *SkipList;
	for (int height = SKIP_MAX_HEIGHT - 1; height >= 0; height--)
	{
		while (currentSearchNode->next[height]->x < x)
		{
			currentSearchNode = currentSearchNode->next[height];
		}
		previous[height] = currentSearchNode;
	}

	for (int i = 0; i < node->height; i++)
	{
		node->next[i] = previous[i]->next[i];
		previous[i]->next[i] = node;
	}
}

void PrintSkipList(SKIP_NODE* SkipList)
{
	for (SKIP_NODE* node = SkipList; node != NULL; node = node->next[0])
		printf("value: %d height: %d\n", node->x, node->height);
}

void FreeSkipList(SKIP_NODE** SkipList)
{
	for (SKIP_NODE* node = *SkipList; node != NULL; )
	{
		SKIP_NODE* previous = node;
		node = node->next[0];
		free(previous);
	}

	*SkipList = NULL;
}

SKIP_NODE* SearchSkipList(SKIP_NODE* header, int wanted)
{
	SKIP_NODE* currentSearchNode = header;
	for (int height = SKIP_MAX_HEIGHT - 1; height >= 0; height--)
	{
		while (currentSearchNode->next[height]->x < wanted)
		{
			currentSearchNode = currentSearchNode->next[height];
		}

		if (currentSearchNode->next[height]->x == wanted)
			return currentSearchNode->next[height];
	}

	return NULL;
}

int main(void)
{
	time_t start, end;

	LLNODE* linkedList = CreateLinkedList(0);

	for (int i = 1; i < N; i++)
	{
		AddToLinkedList(&linkedList, i);
	}

	puts("Linked list: ");
	//PrintLinkedList(linkedList);
	start = clock();
	SearchLinkedList(linkedList, -1);
	end = clock();
	printf("Time needed: %dms\n", end - start);

	SKIP_NODE* skipList = CreateSkipList(1);
	for (int i = N; i >= 2; i--)
	{
		AddToSkipList(&skipList, i);
	}

	puts("Skip list: ");
	//PrintSkipList(skipList);
	start = clock();
	SearchSkipList(skipList, N + 1);
	end = clock();
	printf("Time needed: %dms\n", end - start);

	FreeLinkedList(&linkedList);
	FreeSkipList(&skipList);
	return 0;
}