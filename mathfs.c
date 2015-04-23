#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <fuse/fuse.h>
#include <math.h>

typedef struct a {
	char *(*func) (double a, double b);
	char *name;		// name of file/directory
} file_descr;

file_descr *fileDescriptions[7];

static const char *factor_path = "/factor";
static const char *fib_path = "/fib";
static const char *add_path = "/add";
static const char *sub_path = "/sub";
static const char *mul_path = "/mul";
static const char *div_path = "/div";
static const char *exp_path = "/exp";

static const char *hello_str = "Hello World!\n";

char *factor(double a, double b)
{
	struct linkedlist{
		int value; 
		struct linkedlist *next;
		struct linkedlist *prev; 
	};

	typedef struct linkedlist linkedlist_t; 
	
	
	linkedlist_t *factors = (linkedlist_t *)calloc(1, sizeof(linkedlist_t)); 
	factors->next = NULL; 
	factors->prev = NULL; 

	int first = 0; 

	linkedlist_t *ptr = factors; 

	int number = (int)a; 
	
	while(number % 2 == 0){
		if(first == 0){
			ptr->value = 2; 
			first = 1; 
		}
		else{
			ptr->next = (linkedlist_t *)calloc(1, sizeof(linkedlist_t)); 
			ptr->next->prev = ptr; 
			ptr = ptr->next;
			ptr->value = 2;
			ptr->next = NULL;
		}

		number = number / 2;
	}

	int possible_factor; 

	for(possible_factor = 3; possible_factor < sqrt(number); possible_factor += 2){
	
		while(number % possible_factor == 0){
			if(first == 0){
				ptr->value = possible_factor; 
				first = 1; 
			}
			else{
				ptr->next = (linkedlist_t *)calloc(1, sizeof(linkedlist_t)); 
				ptr->next->prev = ptr; 
				ptr = ptr->next; 
				ptr->value = possible_factor; 
				ptr->next = NULL; 
			}
			
			number = number / possible_factor; 
		}

	}

	if(number > 2){ 
		if(first == 0){
			ptr->value = number; 
			first = 1; 
		}
		else{
			ptr->next = (linkedlist_t *)calloc(1, sizeof(linkedlist_t)); 
			ptr->next->prev = ptr; 
			ptr = ptr->next; 
			ptr->value = number; 
			ptr->next = NULL;
		}
	} 


	char *buf = (char *)calloc(1024, sizeof(char)); 
	char *c_ptr = buf; 
	ptr = factors; 
	for(; ptr != NULL; ptr = ptr->next){
		c_ptr += sprintf(c_ptr, "%d, ", ptr->value); 	
	}

	c_ptr -= 2; 
	*c_ptr = '\0'; 

	return buf; 
}

char *fib(double a, double b)
{
	int *fib_nums = (int *)calloc((int)a, sizeof(int)); 

	fib_nums[0] = 1; 
	fib_nums[1] = 1; 
	

	int i;
	for(i=2; i<a; i++){
		fib_nums[i] = fib_nums[i-1] + fib_nums[i-2]; 
	}

	char *buf = (char *)calloc(1024, sizeof(char)); 
	char *ptr = buf; 

	for(i=0; i<a; i++){
		ptr += sprintf(ptr, "%d, ", fib_nums[i]); 
	}

	ptr -= 2; 
	*ptr = '\0';

	return buf; 
}

char *add(double a, double b)
{
//	return a + b;
	return NULL;
}

char *sub(double a, double b)
{
//	return a - b;
	return NULL;
}

char *mul(double a, double b)
{
	//return a * b;
	return NULL;
}

char *div1(double a, double b)
{
	//return a / b;
	return NULL;
}

char *exp1(double a, double b)
{
	//return pow(a, b);
	return NULL;
}

static void initFileDescriptions()
{
	int i = 0;
	for (; i < 7; i++) {
		fileDescriptions[i] =
		    (file_descr *) malloc(sizeof(file_descr *));
	}

	fileDescriptions[0]->name = "factor";
	fileDescriptions[0]->func = &factor;

	fileDescriptions[1]->name = "fib";
	fileDescriptions[1]->func = &fib;

	fileDescriptions[2]->name = "add";
	fileDescriptions[2]->func = &add;

	fileDescriptions[3]->name = "sub";
	fileDescriptions[3]->func = &sub;

	fileDescriptions[4]->name = "mul";
	fileDescriptions[4]->func = &mul;

	fileDescriptions[5]->name = "div";
	fileDescriptions[5]->func = &div1;

	fileDescriptions[6]->name = "exp";
	fileDescriptions[6]->func = &exp1;
}

// FUSE function implementations.
static int mathfs_getattr(const char *path, struct stat *stbuf)
{
	printf("getattr(\"%s\")\n", path);
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	char *new_path = malloc(sizeof(char) * strlen(path) + 1);
	memcpy(new_path, path, strlen(path) + 1);
	char *token = strtok(new_path, "/");
	char *cmd;
	char **tokens;
	int num_of_tokens = 0;

	while (token != NULL) {
		tokens[num_of_tokens] = token;
		token = strtok(NULL, "/");
		num_of_tokens++;
	}

	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 9;
		return res;
	}

	int z = 0;
	for (; z < 7; z++) {
		if (strcmp(tokens[0], fileDescriptions[z]->name) == 0) {
			cmd = tokens[0];
			break;
		}
	}

	if (z == 7) {
		res = -ENOENT;
		return res;
	}

	if (num_of_tokens == 1) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 3;	// . and .. and doc
		return res;
	}

	if (num_of_tokens == 2) {
		if (strcmp(cmd, "factor") == 0 || strcmp(cmd, "fib") == 0) {
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 3;
			stbuf->st_size = 1024;
			return res;
		} else if (strcmp(tokens[1], "doc") == 0) {
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 3;
			stbuf->st_size = 1024;
			return res;
		} else {
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
			return res;
		}
	}

	if (num_of_tokens == 3) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 3;
		stbuf->st_size = 1024;
	}

	res = -ENOENT;
	return res;

}

static int mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			  off_t offset, struct fuse_file_info *fi)
{
	printf("readdir(\"%s\")\n", path);

	(void)offset;
	(void)fi;

	int i = 0;
	char *curr_dir;

	for (; i < 7; i++) {
		int b = strlen(fileDescriptions[i]->name);
		char a[b + 1];
		strcpy(a, "/");
		strcat(a, fileDescriptions[i]->name);
		if (strcmp(path, a) == 0) {
			curr_dir = fileDescriptions[i]->name;
			break;
		}
	}

	if (i == 7 && strcmp(path, "/") != 0) {
		return -ENOENT;
	}

	if (strcmp(path, "/") == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		for (i = 0; i < 7; i++) {
			int b = strlen(fileDescriptions[i]->name) + 1;
			char a[b + 1];
			strcpy(a, "/");
			strcat(a, fileDescriptions[i]->name);
			filler(buf, a, NULL, 0);
		}
	} else {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "doc", NULL, 0);
	}

	return 0;
}

static int mathfs_open(const char *path, struct fuse_file_info *fi)
{
	
	printf("mathfs_open(\"%s\"\n", path);
	

	char *new_path = malloc(sizeof(char) * strlen(path) + 1);
	memcpy(new_path, path, strlen(path) + 1);
	char *token = strtok(new_path, "/");
	char *cmd;
	char **tokens;
	int num_of_tokens = 0;

	while (token != NULL) {
		tokens[num_of_tokens] = token;
		token = strtok(NULL, "/");
		num_of_tokens++;
	}

	if (strcmp(path, "/") == 0) {
		return -ENOENT;
	}

	int z = 0;
	for (; z < 7; z++) {
		if (strcmp(tokens[0], fileDescriptions[z]->name) == 0) {
			cmd = tokens[0];
			break;
		}
	}

	if (z == 7) {
		return -ENOENT;
	}

	if (num_of_tokens == 1) {
		return -ENOENT;
	}

	if (num_of_tokens == 2) {
		if (strcmp(cmd, "factor") == 0 || strcmp(cmd, "fib") == 0) {
			if((fi->flags & 3) != O_RDONLY){
				return -EACCES;
			}
			return 0;
		} else if (strcmp(tokens[1], "doc") == 0) {	
			if((fi->flags & 3) != O_RDONLY){
				return -EACCES;
			}
			return 0;
		} else {
			return -EACCES;
		}
	}

	if (num_of_tokens == 3) {
		if((fi->flags & 3) != O_RDONLY){
			return -EACCES;
		}
		return 0;
	}

	return -ENOENT;

}

static int mathfs_read(const char *path, char *buf, size_t size, off_t offset,
		       struct fuse_file_info *fi)
{
	printf("mathfs_read(\"%s\"\n", path);
	(void)fi;

}

static struct fuse_operations mathfs_oper = {
	.getattr = mathfs_getattr,
	.readdir = mathfs_readdir,
	.open = mathfs_open,
	.read = mathfs_read,
};

int main(int argc, char **argv)
{
	initFileDescriptions();
	return fuse_main(argc, argv, &mathfs_oper, NULL);
}
