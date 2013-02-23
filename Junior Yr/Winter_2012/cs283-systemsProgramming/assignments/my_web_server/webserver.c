#include "csapp.h"

void Send(int fd, char* msg);
void process(int fd);
void parse_uri(int fd, char uri[MAXLINE]);
void parse_json(int fd);
void clienterror(int fd, char *cause, char *errnum,
		char *shortmsg, char *longmsg);



void main(int argc, char *argv[]){
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;

	/* Check command line args */
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(1);
	}   
	port = atoi(argv[1]);

	listenfd = Open_listenfd(port);

	while(1) {
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
		char buf[4096];
		int bytesRead = recv(connfd, buf, 4096, 0);
		Send(connfd,buf);	
		process(connfd);
		
		Close(connfd);

	}
}

void process(int fd){
	struct stat sbuf;
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	rio_t rio;

	// Read request line and headers
	Rio_readinitb(&rio, fd);
	Rio_readlineb(&rio, buf, MAXLINE);

	sscanf(buf, "%s %s %s",method, uri, version);
	
	// chk if uri is present
	if(strcmp(uri,"/"))  // there is something present
		// parse uri and invoke funcs
		parse_uri(fd, uri);
	else
		Send(fd, "No uri params received");


	return;
}

void parse_uri(int fd, char uri[MAXLINE]){
	char *uri_args;

	// parse uri
	uri_args = strtok(uri,"/");
	// double check to make sure something was found
	if(strcmp(uri_args,"") == 0){
		Send(fd,"No function name found");
	}
	else{
		// a function was found, now get the params sent to the 
		// function via json
		Send(fd,uri_args);
	}

}


/*
	 Parses JSON and returns func_detail struct with function name and it's parameteres
*/
void parse_json(int fd){
	char buf[4096];
	int bytesRead = recv(fd, buf, 4096, 0);

	// look for a "param1"
	
}

/*
	 Function to wrap a wrapper, no need to specify strlen
 */
void Send(int fd, char* msg){
	Rio_writen(fd, msg, strlen(msg));
}