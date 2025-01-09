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

#define READ 0
#define WRITE 1

int err(){
  printf("errno %d\n",errno);
  printf("%s\n",strerror(errno));
  exit(1);
}


//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client = 0;
  char * WKP = "WKP";
  char line[256];
  int piper;
  unlink(WKP);
  piper = mkfifo(WKP, 0644);
  if (piper == -1){
    err();
  }
  int fdw;
  fdw = open(piper, O_RDONLY);
  if (fdw < 0) {
    perror("not able to read wkp ");
    err();
  }
  int r = read (WKP, line, sizeof(line));
  if (r == -1){
    err();
  }
  from_client = r;
  //remove WKP
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup();
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char * priv = getpid();
  int privat = mkfifo(priv);

  int fdw;
  fdw = open(piper, O_WRONLY);
  if (fdw == -1){
    perror("couldn't write to wkp");
    err();
  }
  int w = write(fdw, fds, sizeof(fds));
  int rr;
  rr = open(priv, O_RDONLY);
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
