#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <fuse/fuse.h>

typedef struct a{
	int isDir;  // 1 if it is dir, 0 if it is a file
	char *func; // Function to be called
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

static void initFileDescriptions()
{
	int i = 0;
	for(; i < 7; i++){
		fileDescriptions[i] = (file_descr *) malloc(sizeof(file_descr *));
	}

	fileDescriptions[0]->isDir = 1;
	fileDescriptions[0]->name = "factor";
	fileDescriptions
	
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
	(void)offset;
	(void)fi;

}

static int mathfs_open(const char *path, struct fuse_file_info *fi)
{

}

static int mathfs_read(const char *path, char *buf, size_t size, off_t offset,
		       struct fuse_file_info *fi)
{
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
