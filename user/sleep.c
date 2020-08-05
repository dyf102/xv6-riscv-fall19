#include "kernel/types.h"
//#include "kernel/sysproc.h"
#include "user/user.h"
//#include "user/ulib.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
		write(0 , "usage: sleep seconds\n", 22);
  	exit();
  }
  int sec = atoi(argv[1]);
	sleep(sec);
  exit();
}
