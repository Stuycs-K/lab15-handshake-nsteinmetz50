#include "pipe_networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#define READ 0
#define WRITE 1

static void sighandler(int signo) {
  if ( signo == SIGINT ){
    remove(WKP);
  }
  if (signo == SIGPIPE){
    remove(WKP);
  }
}

int main() {
  int to_client;
  int from_client;
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  while (1){
    from_client = server_handshake( &to_client );
    close(to_client);
    close(from_client);
  }
}
