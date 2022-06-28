
#include "q3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

request_item pending_reqs[REQUESTS_SIZE];
request_item current_request = 0;
request_item previous_request = 0;
int arrayPointer = 0;

/*******************************************************************************
Double Linked List Functions
*******************************************************************************/
struct node {
    int data;
    int key;
    bool visited;

    struct node *next;
    struct node *prev;
};

struct node *head = NULL;
struct node *last = NULL;

bool is_empty() {
   return head == NULL;
}

//insert link at the first location
void insert_at_head(int key, int data) {

   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   link->key = key;
   link->data = data;
   link->visited = false;
	
   if(is_empty()) {
      //make it the last link
      last = link;
   } else {
      //update first prev link
      head->prev = link;
   }

   //point it to old first link
   link->next = head;
	
   //point first to new first link
   head = link;
}

//insert link at the end of the list location
void insert_at_end(int key, int data) {

   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   link->key = key;
   link->data = data;
	
   if(is_empty()) {
      //make it the last link
      last = link;
      head = link;
   } else {
      //make link a new last link
      last->next = link;     
      
      //mark old last node as prev of new link
      link->prev = last;
   }

   //point last to new last node
   last = link;
}

bool insert_after(int key, int newKey, int data) {
   //start from the first link
   struct node *current = head; 
	
   //if list is empty
   if(head == NULL) {
      return false;
   }

   //navigate through list
   while(current->key != key) {
	
      //if it is last node
      if(current->next == NULL) {
         return false;
      } else {           
         //move to next link
         current = current->next;
      }
   }
	
   //create a link
   struct node *newLink = (struct node*) malloc(sizeof(struct node));
   newLink->key = newKey;
   newLink->data = data;

   if(current == last) {
      newLink->next = NULL; 
      last = newLink; 
   } else {
      newLink->next = current->next;         
      current->next->prev = newLink;
   }
	
   newLink->prev = current; 
   current->next = newLink; 
   return true; 
}

//display the list in from first to last
void display_forward() {

   //start from the beginning
   struct node *ptr = head;
	
   //navigate till the end of the list
   printf("\n[ ");
	
   while(ptr != NULL) {        
      printf("(%d,%d) ",ptr->key,ptr->data);
      ptr = ptr->next;
   }
	
   printf(" ]\n");
}


void add_requests_in_order(int item) {
    struct node *ptr = head;
    bool flag = false;
	if (is_empty()){
        insert_at_end(arrayPointer, item);
    } else {
        while(ptr->next != NULL) {
            if (item >= ptr->data) {
                ptr = ptr->next;
                flag = true;
            } else {
                break;
            }
        }
        if(item >= ptr->data) {
            insert_after(ptr->key, arrayPointer, item);
        } else {
            if (flag) {
                insert_after(ptr->prev->key, arrayPointer, item);
                flag = false;
            } else {
                insert_at_head(arrayPointer, item);
            }
        }
    }
    arrayPointer++;
}

/*******************************************************************************
End of Double Linked List Functions
*******************************************************************************/

void FCFS(int *result){
    struct node *ptr = head;
    int curr_key = 0;
    int prev_data = 0;

    while(curr_key != ptr->key) {
            ptr = ptr->next;
    }
    prev_data = ptr->data;
    curr_key += 1;
    *result += abs(current_request - prev_data);

    while(curr_key < REQUESTS_SIZE){
        ptr = head;
        while(curr_key != ptr->key) {
            ptr = ptr->next;
        }
        *result += abs(ptr->data - prev_data);
        prev_data = ptr->data;
        curr_key++;
    }
}

void get_request_behind(struct node *ptr, int *request_behind, struct node **behind_ptr) {
    while(ptr->prev != NULL && ptr->visited == true) {
        ptr = ptr->prev;
    }
    *behind_ptr = ptr;
    *request_behind = ptr->visited == true ? -1 :  ptr->data;
}

void get_request_ahead(struct node *ptr, int *request_ahead, struct node **ahead_ptr) {
    while(ptr->next != NULL && ptr->visited == true) {
        ptr = ptr->next;
    }
    *ahead_ptr = ptr;
    *request_ahead = ptr->visited == true ? -1 : ptr->data;
}

// Find pointer using data value and desired direction
void find_pointer(int desired_value, struct node **result_ptr) {
    struct node *ptr = head;

    while (ptr->next != NULL) {
        if (ptr->data >= desired_value) {
            *result_ptr = ptr;
            break;
        }
        ptr = ptr->next;
    }

}

void reset_visited(struct node **ptr_result) {
    struct node *ptr = head;
    enum direction dir = Right;
    while(ptr->visited == true){
        ptr->visited = false;
        if(dir == Right) {
            if(ptr->next != NULL) {
                ptr = ptr->next;
            } else {
                ptr = ptr->prev;
                dir = Left;
            } 
        } else {
            if(ptr->prev != NULL) {
                ptr = ptr->prev;
            } else {
                ptr = ptr->next;
                dir = Right;
            }
        }   
    }
    *ptr_result = ptr;
}


void SSTF(int *result) {
    struct node *ptr = head;
    struct node *request_behind_ptr = NULL;
    struct node *request_ahead_ptr = NULL;
    int request = current_request;
    int request_ahead = current_request;
    int request_behind = current_request;
    int request_count = 1;
    int abs_behind = 0;
    int abs_ahead = 0;

    // Find current request's pointer
    find_pointer(current_request, &ptr);
    request_ahead = ptr->data;
    request_behind = ptr->prev != NULL ? ptr->prev->data : -1;

    // Find the requests before and after the current request
    abs_behind = request_behind != -1 ?abs(request-request_behind): NUM_CYLINDERS+1; 
    abs_ahead = request_ahead != -1 ? abs(request-request_ahead):NUM_CYLINDERS+1;

    // If previous request's distance is shorter, set the pointer to 
    // the previous request's pointer and mark it as visited
    // Else, set the pointer to the next request's pointer and mark it as
    // visited
    if (abs_behind < abs_ahead) {
        *result += abs(request-request_behind);
        request = request_behind;
        ptr = ptr->prev;
        ptr->visited = true;
    } else {
        *result += abs(request - request_ahead);
        request = request_ahead;
        ptr = ptr->next;
        ptr->visited = true;
    }

    // While not all requests have been read
    while(request_count < REQUESTS_SIZE){

        // Get the previous and next requests from the current pointer
        get_request_behind(ptr, &request_behind, &request_behind_ptr);
        get_request_ahead(ptr, &request_ahead, &request_ahead_ptr);

        abs_behind = request_behind != -1 ?abs(request-request_behind): NUM_CYLINDERS+1;
        abs_ahead = request_ahead != -1 ? abs(request-request_ahead):NUM_CYLINDERS+1;

        // If previous request's distance is shorter, set the pointer to 
        // the previous request's pointer and mark it as visited
        // Else, set the pointer to the next request's pointer and mark it as
        // visited
        if (abs_behind < abs_ahead){
            *result += abs(request-request_behind);
            request = request_behind;
            ptr = request_behind_ptr;
            ptr->visited = true;
        } else {
            *result += abs(request - request_ahead);
            request = request_ahead;
            ptr = request_ahead_ptr;
            ptr->visited = true;
        }
        request_count += 1;
    }

    //remember to return all visited to false
    reset_visited(&ptr);
}

void SCAN(int *result){
    struct node *ptr = head;
    struct node *request_behind_ptr = NULL;
    struct node *request_ahead_ptr = NULL;
    int request = current_request;
    int request_count = 1;
    int request_ahead = current_request;
    int request_behind = current_request;
    int abs_behind = 0;
    int abs_ahead = 0;
    enum direction dir;

    // Find current request's pointer
    find_pointer(current_request, &ptr);
    if(previous_request < current_request){
        dir = Right;
    } else {
        dir = Left;
        ptr = ptr->prev;
    }
    request = ptr->data;
    request_ahead = ptr->next != NULL ? ptr->next->data : -1;
    request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
    *result += abs(current_request - request);
    ptr->visited = true;

    // While not all requests have been read
    while(request_count < REQUESTS_SIZE){

        if (dir == Right) {
            if(request_ahead != -1) {
                *result += abs(request_ahead - request);
                ptr->visited = true;
                ptr = ptr->next;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
            } else { // Reached the right end
                *result += abs(NUM_CYLINDERS-1 - request);
                ptr->visited = true;
                request = NUM_CYLINDERS-1;
                request_ahead = -1;
                get_request_behind(ptr, &request_behind, &request_behind_ptr);
                ptr = request_behind_ptr;
                dir = Left;
                request_count -= 1; // Don't count the turn around
            }
        } else {
            if(request_behind != -1) {
                *result += abs(request_behind - request);
                ptr->visited = true;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
                ptr = ptr->prev;
            } else { // Reached the left end
                *result += request;
                ptr->visited = true;
                request = 0;
                request_behind = -1;
                get_request_ahead(ptr, &request_ahead, &request_ahead_ptr);
                ptr = request_ahead_ptr;
                dir = Right;
                request_count -= 1; // Don't count the turn around
            }
        }
        request_count += 1;
    }

    //remember to return all visited to false
    reset_visited(&ptr);
}

void LOOK(int *result) {
    struct node *ptr = head;
    struct node *request_behind_ptr = NULL;
    struct node *request_ahead_ptr = NULL;
    int request = current_request;
    int request_count = 1;
    int request_ahead = current_request;
    int request_behind = current_request;
    int abs_behind = 0;
    int abs_ahead = 0;
    enum direction dir;

    // Find current request's pointer
    find_pointer(current_request, &ptr);
    if(previous_request < current_request){
        dir = Right;
    } else {
        dir = Left;
        ptr = ptr->prev;
    }
    request = ptr->data;
    request_ahead = ptr->next != NULL ? ptr->next->data : -1;
    request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
    *result += abs(current_request - request);
    ptr->visited = true;

    // While not all requests have been read
    while(request_count < REQUESTS_SIZE){

        if (dir == Right) {
            if(request_ahead != -1) {
                *result += abs(request_ahead - request);
                ptr = ptr->next;
                ptr->visited = true;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                get_request_behind(ptr, &request_behind, &request_behind_ptr);
            } else { // Reached the right end
                *result += abs(request_behind - request);
                ptr = request_behind_ptr;
                ptr->visited = true;
                request = ptr-> data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                get_request_behind(ptr, &request_behind, &request_behind_ptr);
                dir = Left;
            }
        } else {
            if(request_behind != -1) {
                *result += abs(request_behind - request);
                ptr = ptr->prev;
                ptr->visited = true;
                request = ptr->data;
                get_request_ahead(ptr, &request_ahead, &request_ahead_ptr);
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
            } else { // Reached the left end
                *result += abs(request_ahead - request);
                ptr = request_ahead_ptr;
                ptr->visited = true;
                request = ptr->data;
                get_request_ahead(ptr, &request_ahead, &request_ahead_ptr);
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
                dir = Right;
            }
        }
        request_count += 1;
    }

    //remember to return all visited to false
    reset_visited(&ptr);
}

void CSCAN(int *result) {
    struct node *ptr = head;
    struct node *ptr_head = head;
    struct node *request_behind_ptr = NULL;
    struct node *request_ahead_ptr = NULL;
    int request = current_request;
    int request_count = 1;
    int request_ahead = current_request;
    int request_behind = current_request;
    int abs_behind = 0;
    int abs_ahead = 0;
    enum direction dir;

    // Find current request's pointer
    find_pointer(current_request, &ptr);
    if(previous_request < current_request){
        dir = Right;
    } else {
        dir = Left;
        ptr = ptr->prev;
    }
    request = ptr->data;
    request_ahead = ptr->next != NULL ? ptr->next->data : -1;
    request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
    *result += abs(current_request - request);
    ptr->visited = true;

    // While not all requests have been read
    while(request_count < REQUESTS_SIZE){

        if (dir == Right) {
            if(request_ahead != -1) {
                *result += abs(request_ahead - request);
                ptr->visited = true;
                ptr = ptr->next;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
            } else { // Reached the right end
                *result += abs(NUM_CYLINDERS-1 - request);
                ptr->visited = true;
                request = 0;
                request_behind = -1;
                get_request_ahead(ptr_head, &request_ahead, &request_ahead_ptr);
                ptr = request_ahead_ptr;
                *result += abs(request_ahead - request);
                ptr->visited = true;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
                dir = Right;
            }
        } else {
            if(request_behind != -1) {
                *result += abs(request_behind - request);
                ptr->visited = true;
                ptr = ptr->prev;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
            } else { // Reached the left end
                *result += request;
                ptr->visited = true;
                request = NUM_CYLINDERS-1;
                request_ahead = -1;
                get_request_behind(ptr_head, &request_behind, &request_behind_ptr);
                ptr = request_behind_ptr;
                *result += abs(request_behind - request);
                ptr->visited = true;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
                dir = Right;
            }
        }
        request_count += 1;
    }

    //remember to return all visited to false
    reset_visited(&ptr);
}

void CLOOK(int *result) {
    struct node *ptr = head;
    struct node *ptr_head = head;
    struct node *request_behind_ptr = NULL;
    struct node *request_ahead_ptr = NULL;
    int request = current_request;
    int request_count = 1;
    int request_ahead = current_request;
    int request_behind = current_request;
    int abs_behind = 0;
    int abs_ahead = 0;
    enum direction dir;

    // Find current request's pointer
    find_pointer(current_request, &ptr);
    if(previous_request < current_request){
        dir = Right;
    } else {
        dir = Left;
        ptr = ptr->prev;
    }
    request = ptr->data;
    request_ahead = ptr->next != NULL ? ptr->next->data : -1;
    request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
    *result += abs(current_request - request);
    ptr->visited = true;

    // While not all requests have been read
    while(request_count < REQUESTS_SIZE){

        if (dir == Right) {
            if(request_ahead != -1) {
                *result += abs(request_ahead - request);
                ptr->visited = true;
                ptr = ptr->next;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
            } else { // Reached the right end
                request_behind = -1;
                get_request_ahead(ptr_head, &request_ahead, &request_ahead_ptr);
                ptr = request_ahead_ptr;
                ptr->visited = true;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
                dir = Right;
            }
        } else {
            if(request_behind != -1) {
                *result += abs(request_behind - request);
                ptr->visited = true;
                ptr = ptr->prev;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
            } else { // Reached the left end
                request_ahead = -1;
                get_request_behind(ptr_head, &request_behind, &request_behind_ptr);
                ptr = request_behind_ptr;
                ptr->visited = true;
                request = ptr->data;
                request_ahead = ptr->next != NULL ? ptr->next->data : -1;
                request_behind = ptr->prev != NULL ? ptr->prev->data : -1;
                dir = Left;
            }
        }
        request_count += 1;
    }

    //remember to return all visited to false
    reset_visited(&ptr);
}


int main(int argc, char *argv[]){

    if(argc < REQUESTS_SIZE + 3){
		fprintf(stderr, "Useage: <previous request cylinder> <current request cylinder> <queue of pending requests in FIFO order (%d requests)>\n", REQUESTS_SIZE);
		return -1;
	}


    previous_request = atoi(argv[1]);
    current_request = atoi(argv[2]);
    int resulta = 0;
    int resultb = 0;
    int resultc = 0;
    int resultd = 0;
    int resulte = 0;
    int resultf = 0;
    int arg = 3;

    for(arg; arg < argc; arg++){
        add_requests_in_order(atoi(argv[arg]));
    }

    printf("Total amount of head movements for:\n");

    FCFS(&resulta);
    printf("FCFS: %d\n", resulta);
 
    SSTF(&resultb);
    printf("SSTF: %d\n", resultb);

    SCAN(&resultc);
    printf("SCAN: %d\n", resultc);

    LOOK(&resultd);
    printf("LOOK: %d\n", resultd);

    CSCAN(&resulte);
    printf("CSCAN: %d\n", resulte);

    CLOOK(&resultf);
    printf("CLOOK: %d\n", resultf);

	return 0;
}