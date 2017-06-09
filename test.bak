#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
	int fread=open(argv[1],O_RDONLY);
	char buff;
	while(read(fread, &buff, 1)){
		printf("%c",buff);
	}
	return 0;
}
