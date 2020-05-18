/* forking.c: Forking HTTP Server */

#include "spidey.h"

#include <errno.h>
#include <signal.h>
#include <string.h>

#include <unistd.h>

/**
 * Fork incoming HTTP requests to handle the concurrently.
 *
 * @param   sfd         Server socket file descriptor.
 * @return  Exit status of server (EXIT_SUCCESS).
 *
 * The parent should accept a request and then fork off and let the child
 * handle the request.
 **/
int forking_server(int sfd) {
    /* Accept and handle HTTP request */
    while (true) {
    	/* Accept request */
      Request *request = accept_request(sfd);

      if (!request){
        continue;
      }

	    /* Ignore children */
      signal(SIGCHLD, SIG_IGN);

	    /* Fork off child process to handle request */
      pid_t pid = fork();

      if(pid < 0){
        debug("Unable to create the fork: %s", strerror(errno));
        free_request(request);
        continue;
      }
      else if(pid == 0) {
        Status status = handle_request(request);
        const char *success = http_status_string(status);

        log("Status successfully returned: %s", success);
        exit(0);
      }
      free_request(request);
    }

    /* Close server socket */
    int socket_status = close(sfd);

    if(socket_status < 0) {
      debug("Unable to close socket to server");
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
