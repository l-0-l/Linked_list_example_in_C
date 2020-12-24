/* This is a linked list example in C language. It is
 * quite specific, as the data is not placed directly
 * in the node, but in an additional struct inside it.
 * There are functions for creating the list and adding
 * nodes, but the actual goal is adding a new word in
 * a sorted way. There's an extensive use of double
 * pointers, this is the main reason for creating the
 * example.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct word_data {
    char *data;
    int occurrences;
} s_word_data;

typedef struct word_node {
    s_word_data word;
    struct word_node *next;
} s_word_node;

typedef struct word_list {
    s_word_node *head;
} s_word_list;


char *create_word_data(char *word)
{
    unsigned int word_len = strlen(word);
    char *new_data = malloc(word_len+1); // Add one additional character for null-termination
    strncpy(new_data, word, word_len);
    new_data[word_len] = '\0'; // Add a null-termination character at the end of the string
    return new_data;
}

void delete_word_data(char **word)
{
    if(*word)
    {
        free(*word);
        *word = NULL;
    }
}

s_word_node *create_node(char *word, s_word_node *next)
{
    s_word_node *new_node = malloc(sizeof(s_word_node));
    new_node->word.data = create_word_data(word);
    new_node->word.occurrences = 1;
    new_node->next = next;
    return new_node;
}

void delete_node(s_word_node **node)
{
    if(node && *node)
    {
        s_word_node *next = (*node)->next;
        delete_word_data(&((*node)->word.data));
        free(*node);
        *node = next;
    }
}

s_word_list *create_word_list()
{
    s_word_list *list = malloc(sizeof(s_word_list));
    list->head = NULL;
    return list;
}

void delete_word_list(s_word_list **list)
{
    if(*list)
    {
        while((*list)->head)
        {
            delete_node(&((*list)->head));
        }
        free(*list);
        *list = NULL;
    }
}

void insert_word(char *word, s_word_node **node)
{
    s_word_node *next = *node;
    *node = create_node(word, next);
}

/* Pay attention to the difference between the functions insert_word
 * and insert_word_sorted: the former inserts a new node before the
 * given node, and therefore doesn't handle occurrances numbering,
 * and the latter receives a list instead, and handles the rest
 * internally (through using the insert_word function).
 */
void insert_word_sorted(char *word, s_word_list *list)
{
    if(list)
    {
        s_word_node **node = NULL;
        node = &(list->head);
        while(*node && strcmp(word, (*node)->word.data) >= 0)
        {
            if(0 == strcmp((*node)->word.data, word))
            {
                (*node)->word.occurrences++;
                return;
            }
            node = &((*node)->next);
        }
        insert_word(word, node);
    }
}

void print_word_list(s_word_list *list)
{
    if(list && list->head)
    {
        printf("Printing the list:\n");
        s_word_node *node = list->head;
        unsigned int count = 1;
        while(node)
        {
            printf("Node [%d], addr [%p], data [%s], occurrences [%d], next [%p]\n",
                   count++, (void*)node, node->word.data, node->word.occurrences, (void *)node->next);
            node = node->next;
        }
        printf("Done.\n");
    }
    else
    {
        printf("The list is empty.\n");
    }
}

/* In the main here we just experiment with the
 * lists and the functions, showing off the abilities.
 */

int main()
{
    s_word_list *list = create_word_list();
    char* strings[] = {"bbb", "ddd", "aaa", "bbb", "ccc", "eee", "bbb", "aaa", "bbb"};

    printf("Test one - inserting the words.\n");
    for(int i = 0; i < sizeof(strings)/sizeof(strings[0]); i++)
    {
        insert_word(strings[i], &(list->head));
    }
    print_word_list(list);

    printf("\nTest two - deleting the third word.\n");
    delete_node(&(list->head->next->next));
    print_word_list(list);

    printf("\nTest three - deleting the word list.\n");
    delete_word_list(&list);
    print_word_list(list);

    list = create_word_list();

    printf("\nTest four - adding sorted words.\n");
    for(int i = 0; i < sizeof(strings)/sizeof(strings[0]); i++)
    {
        insert_word_sorted(strings[i], list);
    }
    print_word_list(list);

    printf("\nTest five - deleting the word list for final cleanup.\n");
    delete_word_list(&list);
    print_word_list(list);

    return 0;
}
