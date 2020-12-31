#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZEOFSTRING 5

char* readline();
char** split_string(char*);

struct node {
    struct node *next;
    char word[SIZEOFSTRING+1];
    int count;
};
/**
 * @brief free the memory that was saved for individual nodes.
 * @param head is a pointer to the first node of the linked list.
 */
void list_kill(struct node *head) {
	struct node *cursor;
	struct node *cursor_copy;
	cursor = head;
	/* free every node in the linked listd */
	while (cursor) {
		cursor_copy = cursor;
		cursor = cursor->next;
		free(cursor_copy);
	}
}


/**
 * @brief takes a word from the magazine and inserting to a linked list.
 * @param head is a pointer to the first node of the linked list.
 * @param word is a pointer to the word we add to the linked list.
 * @return 0 if the insert was a success or 1 if it failed.
 */
int insert_node(struct node *head , char *word) {
	struct node *curr_node = head;
	/* check if word was already discovered  */
	while (curr_node->next) {
		if(!strcmp(curr_node->word, word)) {
			curr_node->count++;
			return 0;
		}
		curr_node=curr_node->next;
	}
	struct node *new_node = malloc(sizeof(struct node) );
	/* malloc succesful ? */
	if(new_node == NULL) {
		return 1;
	}
	/* if the word has yet to appear' add it to the end of the list*/
	strcpy(new_node->word, word);
	new_node->count=1;
	curr_node->next=new_node;
	new_node->next=NULL;
	return 0;
}


/**
 * @brief check if a word appearers in the list and wasn't used yet.
 * @param head is a pointer to the first node of the linked list.
 * @param word  is a pointer to the word we want to check.
 * @return value 0 if the word was found and used, 1 if not.
 */
int check_word(struct node *head , char *word) {
	struct node *current_node = head;
	/* if the word is not included in the list-continue searching*/
	while (strcmp(current_node->word, word)) {
		/* if done searching all nodes in the linked list*/
		if(!current_node->next) {
			return 1;
		}
		current_node=current_node->next;
	}
	/* check if specific word found was already used  */
	if(!(current_node->count)) {
		return 1;
	}
	(current_node->count)--;
	return 0;
}




/**
 * @brief goes over note, word by word and checks if it appeared in the magazine.
 * @param magazine - array of words in the magazine.
 * @param note - array of words in the note.
 * @param magazine_count -number of words in magazine text.
 * @param note_count -number of words in note text.
 * @return value 0 if all words from note truly exist in Magazine, 1 if not.
 */
void checkMagazine(int magazine_count,
				   char** magazine,
				   int note_count,
				   char** note) {
	/* check if there are enough words in the magazine */
	if(magazine_count<note_count) {
	    printf("No");
		exit(1);
	}
	/* create the first node */
	struct node *head = malloc(sizeof(struct node));
	head->next=NULL;
	strcpy(head->word, *(magazine));
	head->count=1;
	/* insert all words in the magazine one by one to linked list */
	for (int i=1; i<magazine_count; i++) {
		/* checks if allocation failed */
		if(insert_node(head, *(magazine + i))){
			printf("No");
			exit(1);
		}
    }
	/* checks if all the words in the note appeared in the linked list */
	for (int i = 0; i < note_count; i++) {
		/* checks if word was missing */
	    if(check_word(head, *(note + i))==1){
			printf("No");
			exit(1);
		}
    }
	printf("Yes");
	/* release the saved memory*/
	list_kill(head);
}



int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
