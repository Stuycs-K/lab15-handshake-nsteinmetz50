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
  }
}

int main() {
  int to_client;
  int from_client;
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);

  while (1){
    from_client = server_handshake( &to_client );
    while (1){
      int num;
      int w;
      num = randomHundred();
      w = write(to_client, &num, sizeof(num)); //send syn ack
      if (w <= 0){
        break;
      }
      sleep(1);
    }
    close(to_client);
    close(from_client);
  }
}
