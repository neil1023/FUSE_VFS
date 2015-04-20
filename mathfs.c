#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <fuse/fuse.h>

typedef struct a{
	int (*func)(int a, int b);
	char *name; // name of file/directory
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

int factor(int a, int b){
	return 0;
}

int fib(int a, int b){
	return 0;
}

int add(int a, int b){
	return 0;
}

int sub(int a, int b){
	return 0;
}

int mul(int a, int b){
	return 0;
}

int div1(int a, int b){
	return 0;
}

int exp1(int a, int b){
	return 0;
}

static void initFileDescriptions()
{
	int i = 0;
	for(; i < 7; i++){
		fileDescriptions[i] = (file_descr *) malloc(sizeof(file_descr *));
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

	fileDescriptions[5]->name = "div1";
	fileDescriptions[5]->func = &div1;

	fileDescriptions[6]->name = "exp1";
	fileDescriptions[6]->func = &exp1;
}

// FUSE function implementations.
static int mathfs_getattr(const char *path, struct stat *stbuf)
{
	printf("getattr(\"%s\"\n", path);
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, factor_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
	} else
		res = -ENOENT;

	return res;

}

static int mathfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			  off_t offset, struct fuse_file_info *fi)
{
	printf("readdir(\"%s\"\n", path);

	(void)offset;
	(void)fi;

}

static int mathfs_open(const char *path, struct fuse_file_info *fi)
{
	printf("mathfs_open(\"%s\"\n", path);

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
	return fuse_main(argc, argv, &mathfs_oper, NULL);
}
