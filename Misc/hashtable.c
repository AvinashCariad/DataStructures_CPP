# include <stdio.h>
# include <stdlib.h>


// Using hash table - Time: O(n)

#define SIZE 10000

// define node:
typedef struct
{
    int key;
    int value;
} HashNode;


// define struct:
typedef struct
{
    HashNode **array;
} HashMap;

HashMap *createHashMap()
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->array = (HashNode **)calloc(SIZE, sizeof(HashNode *));   // array of pointers to the hashnode
    return map;
}


// define insertion:
void insert(HashMap *map, int key, int value)
{
    int index = abs(key) % SIZE;
    while (map->array[index] != NULL && map->array[index]->key != key)
    {
        index = (index + 1) % SIZE;      // note how the iteration is bounded
    }
    if (map->array[index] == NULL)
    {
        map->array[index] = (HashNode *)malloc(sizeof(HashNode));
    }
    map->array[index]->key = key;
    map->array[index]->value = value;
}


// search routine:
int search(HashMap *map, int key)
{
    int index = abs(key) % SIZE;
    while (map->array[index] != NULL)
    {
        if (map->array[index]->key == key)
        {
            return map->array[index]->value;
        }
        index = (index + 1) % SIZE;
    }
    return -1;
}


// define two-sum:
int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    HashMap *map = createHashMap();
    for (int i = 0; i < numsSize; i++)
    {
        int t = target - nums[i];
        int searchIndex = search(map, t);

        //If the hash table contains the key, return the index and the current index i
        if (searchIndex != -1)
        {
            int *result = (int *)malloc(2 * sizeof(int));
            result[0] = searchIndex;
            result[1] = i;
            *returnSize = 2;
            return result;
        }
        insert(map, nums[i], i);
    }

    *returnSize = 0;
    return NULL; 
}





int main(){
    printf("===SUCCESS===\n");
    return 1;    

}
