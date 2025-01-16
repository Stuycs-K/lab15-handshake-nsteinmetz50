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

int randomNum(){
  int x;
  int bytes;
  int r_file = open("/dev/random", O_RDONLY, 0);
  if (r_file == -1)err();
  bytes = read(r_file, &x, 4);
  if (bytes == -1){
      err();
  }
  return x;
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
  char line[256];
  int piper;
  unlink(WKP);
  piper = mkfifo(WKP, 0644); //make pipe
  if (piper == -1){
    err();
  }
  int fdw;
  fdw = open(WKP, O_RDONLY); //open WKP
  if (fdw < 0) {
    perror("not able to read wkp ");
    err();
  }
  from_client = fdw;
  remove(WKP); //remove
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
  char priv[50];
  int fdw = server_setup();
  /*int fdw = open(from_client, O_WRONLY);
  if (fdw == -1){
    err();
  }*/
  int r = read(fdw, priv, sizeof(priv)); //read syn
  if (r == -1){
    err();
  }
  printf("Client PID, received by server: %s\n", priv);
  int down;
  down = open(priv, O_WRONLY); //open pp
  if (down == -1){
    err();
  }
  *to_client = down;
  int num = randomNum();
  printf("Intended SYN: %d\n", num);
  int w = write(down, &num, sizeof(num)); //send syn ack
  if (w == -1){
    err();
  }
  r = read(fdw, &num, sizeof(num));
  if (r == -1){
    err();
  }
  printf("Received ack: %d\n", num);

  int from_client = fdw;
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
  char priv[50]; 
  int num = getpid(); 
  sprintf(priv, "%d", num); 
  int privat = mkfifo(priv, 0644); //make pp
  if (privat == -1){
    err();
  }

  int fdw;
  fdw = open(WKP, O_WRONLY); //open WKP
  if (fdw == -1){
    perror("couldn't write to wkp");
    err();
  }
  *to_server = fdw;
  printf("Intended PID: %s\n", priv);
  int w = write(fdw, priv, sizeof(priv)); //write pp to wkp
  if (w == -1){
    err();
  }
  int rr;
  rr = open(priv, O_RDONLY); //open pp
  if (rr == -1){
    err();
  }
  remove(priv); //delete pp
  int signal;
  int reading = read(rr, &signal, sizeof(signal));
  if (reading == -1){
    err();
  }
  printf("Received syn ack: %d\n", signal);
  signal++;
  w = write(fdw, &signal, sizeof(signal)); //ack
  if (w == -1){
    err();
  }
  
  while (1){
    reading = read(rr, &signal, sizeof(signal));
    if (reading == -1){
      err();
    }
    printf("New Int: %d\n", signal);
  }
  int from_server = rr;
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
