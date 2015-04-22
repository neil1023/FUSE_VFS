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
	double (*func) (double a, double b);
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

double factor(double a, double b)
{
	return 0;
}

double fib(double a, double b)
{
	return 0;
}

double add(double a, double b)
{
	return a + b;
}

double sub(double a, double b)
{
	return a - b;
}

double mul(double a, double b)
{
	return a * b;
}

double div1(double a, double b)
{
	return a / b;
}

double exp1(double a, double b)
{
	return pow(a, b);
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

	fileDescriptions[5]->name = "div1";
	fileDescriptions[5]->func = &div1;

	fileDescriptions[6]->name = "exp1";
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
	int i = 0;

	while (token != NULL) {
		tokens[i] = token;
		token = strtok(NULL, "/");
		i++;
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

	if (cmd == NULL) {
		res = -ENOENT;
		return res;
	}
	
	if (tokens[1] != NULL) {
		// do nothing
	} else {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;	// . and ..
		return res;
	}

	if (tokens[2] != NULL) {
		// do nothing
	} else if (strcmp(cmd, "factor") == 0) {
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
		stbuf->st_nlink = 3;
		return res;
	}

	if (tokens[3] != NULL) {
		res = -ENOENT;
		return res;
	}

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
