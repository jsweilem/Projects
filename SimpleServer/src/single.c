/* single.c: Single User HTTP Server */

#include "spidey.h"

#include <errno.h>
#include <string.h>

#include <unistd.h>

/**
 * Handle one HTTP request at a time.
 *
 * @param   sfd         Server socket file descriptor.
 * @return  Exit status of server (EXIT_SUCCESS).
 **/
int single_server(int sfd) {
    /* Accept and handle HTTP request */
    Request *request;
    Status status;

    while (true) {
    	/* Accept request */
        request = accept_request(sfd);

        if (!request){
            debug("Unable to accept request: %s", strerror(errno));
            free_request(request);
            return EXIT_FAILURE;
        }

	    /* Handle request */
        status = handle_request(request);
        const char *success = http_status_string(status);

        log("Status successfully returned: %s", success);

	    /* Free request */
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
