/* handler.c: HTTP Request Handlers */

#include "spidey.h"

#include <errno.h>
#include <limits.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

/* Internal Declarations */
Status handle_browse_request(Request *request);
Status handle_file_request(Request *request);
Status handle_cgi_request(Request *request);
Status handle_error(Request *request, Status status);

/**
 * Handle HTTP Request.
 *
 * @param   r           HTTP Request structure
 * @return  Status of the HTTP request.
 *
 * This parses a request, determines the request path, determines the request
 * type, and then dispatches to the appropriate handler type.
 *
 * On error, handle_error should be used with an appropriate HTTP status code.
 **/
Status  handle_request(Request *r) {
    Status result;

    /* Parse request */
    int status = parse_request(r);
    if(status < 0) {
      debug("Unable to parse request: %s", strerror(errno));
      return handle_error(r, HTTP_STATUS_BAD_REQUEST);
    }
    debug("Request was parsed successfully");

    /* Determine request path */
    r->path = determine_request_path(r->uri);
    if(r->path == NULL) {
      debug("Unable to determine request path: %s", strerror(errno));
      return handle_error(r, HTTP_STATUS_NOT_FOUND);
    }
    debug("HTTP REQUEST PATH: %s", r->path);

    /* Dispatch to appropriate request handler type based on file type */
    struct stat s;
    if(stat(r->path, &s) < 0) {
      debug("Unable to stat the file information: %s", strerror(errno));
      return handle_error(r, HTTP_STATUS_NOT_FOUND);
    }

    if(S_ISDIR(s.st_mode)) {
      debug("The request path is a directory");
      result = handle_browse_request(r);
    }
    else if(access(r->path, X_OK) == 0) {
      debug("The request path is a CGI");
        result = handle_cgi_request(r);
    }
    else if(access(r->path, R_OK) == 0) {
      debug("The request path is a file");
        result = handle_file_request(r);
    }
    else {
      debug("Unable to determine the request path type");
      result = HTTP_STATUS_BAD_REQUEST;
    }

    log("HTTP REQUEST STATUS: %s", http_status_string(result));
    return result;
}

/**
 * Handle browse request.
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP browse request.
 *
 * This lists the contents of a directory in HTML.
 *
 * If the path cannot be opened or scanned as a directory, then handle error
 * with HTTP_STATUS_NOT_FOUND.
 **/
Status  handle_browse_request(Request *r) {
    struct dirent **entries;
    int n;

    /* Open a directory for reading or scanning */
    n = scandir(r->path, &entries, NULL, alphasort);
    if(n < 0) {
      debug("Unable to open directory: %s", strerror(errno));
      return HTTP_STATUS_BAD_REQUEST;
    }
    debug("The directory was scanned sucessfully");

    /* Write HTTP Header with OK Status and text/html Content-Type */
    fprintf(r->stream, "HTTP/1.0 200 OK\r\n");
    fprintf(r->stream, "Content-Type: text/html\r\n");
    fprintf(r->stream, "\r\n");

    /* For each entry in directory, emit HTML list item */
    fprintf(r->stream, "<ul>\r\n");

    for(unsigned int i = 1; i < n; i++) {
      if(strcmp(&r->uri[strlen(r->uri) - 1], "/")) {
        fprintf(r->stream, "<li><a href=\"%s/%s\">%s</a></li>\r\n", r->uri, entries[i]->d_name, entries[i]->d_name);
      }
      else {
        fprintf(r->stream, "<li><a href=\"%s%s\">%s</a></li>\r\n", r->uri, entries[i]->d_name, entries[i]->d_name);
      }
    }
    fprintf(r->stream, "</ul>\r\n");

    for(unsigned int i = 0; i < n; i++) {
      free(entries[i]);
    }
    free(entries);
    debug("Entries in directory freed successfully");

    /* Return OK */
    return HTTP_STATUS_OK;
}

/**
 * Handle file request.
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP file request.
 *
 * This opens and streams the contents of the specified file to the socket.
 *
 * If the path cannot be opened for reading, then handle error with
 * HTTP_STATUS_NOT_FOUND.
 **/
Status  handle_file_request(Request *r) {
    FILE *fs;
    char buffer[BUFSIZ];
    char *mimetype = NULL;
    size_t nread;

    /* Open file for reading */
    fs = fopen(r->path, "r");

    if(!fs) {
      debug("Unable to open file: %s", strerror(errno));
      goto fail;
    }

    /* Determine mimetype */
    mimetype = determine_mimetype(r->path);

    if(!mimetype){
      debug("Unable to find mimetype, setting mimetype to DefaultMimeType");
      mimetype = strdup(DefaultMimeType);
    }

    /* Write HTTP Headers with OK status and determined Content-Type */
    fprintf(r->stream, "HTTP/1.0 200 OK\r\n");
    fprintf(r->stream, "Content-Type: %s\r\n", mimetype);
    fprintf(r->stream, "\r\n");

    /* Read from file and write to socket in chunks */
    nread = fread(buffer, 1, BUFSIZ, fs);
    while(nread > 0) {
      fwrite(buffer, 1, nread, r->stream);
      nread = fread(buffer, 1, BUFSIZ, fs);
    }
    debug("Write to the socket was successful");

    /* Close file, deallocate mimetype, return OK */
    fclose(fs);
    free(mimetype);
    return HTTP_STATUS_OK;

fail:
    /* Close file, free mimetype, return INTERNAL_SERVER_ERROR */
    debug("Unable to perform handle_file_request");
    fclose(fs);
    free(mimetype);
    return HTTP_STATUS_INTERNAL_SERVER_ERROR;
}

/**
 * Handle CGI request
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP file request.
 *
 * This popens and streams the results of the specified executables to the
 * socket.
 *
 * If the path cannot be popened, then handle error with
 * HTTP_STATUS_INTERNAL_SERVER_ERROR.
 **/
Status  handle_cgi_request(Request *r) {
    FILE *pfs;
    char buffer[BUFSIZ];

    /* Export CGI environment variables from request:
     * http://en.wikipedia.org/wiki/Common_Gateway_Interface */
    setenv("DOCUMENT_ROOT", RootPath, 1);
    setenv("QUERY_STRING", r->query, 1);
    setenv("REMOTE_ADDR", r->host, 1);
    setenv("REMOTE_PORT", r->port, 1);
    setenv("REQUEST_METHOD", r->method, 1);
    setenv("REQUEST_URI", r->uri, 1);
    setenv("SCRIPT_FILENAME", r->path, 1);
    setenv("SERVER_PORT", Port, 1);

    /* Export CGI environment variables from request headers */
    Header *curr = r->headers;
    while(curr != NULL) {
      if(streq(curr->name, "Host")) {
        setenv("HTTP_HOST", curr->data, 1);
      }
      else if(streq(curr->name, "Accept")) {
        setenv("HTTP_ACCEPT", curr->data, 1);
      }
      else if(streq(curr->name, "Accept-Language")) {
        setenv("HTTP_ACCEPT_LANGUAGE", curr->data, 1);
      }
      else if(streq(curr->name, "Accept-Encoding")) {
        setenv("HTTP_ACCEPT_ENCODING", curr->data, 1);
      }
      else if(streq(curr->name, "Connection")) {
        setenv("HTTP_CONNECTION", curr->data, 1);
      }
      else if(streq(curr->name, "User-Agent")) {
        setenv("HTTP_USER_AGENT", curr->data, 1);
      }
      curr = curr->next;
    }

    /* POpen CGI Script */
    pfs = popen(r->path, "r");

    if(!pfs) {
      debug("Unable to open CGI script");
      return HTTP_STATUS_NOT_FOUND;
    }

    /* Copy data from popen to socket */
    while(fgets(buffer, BUFSIZ, pfs)) {
      fputs(buffer, r->stream);
    }

    /* Close popen, return OK */
    pclose(pfs);
    return HTTP_STATUS_OK;
}

/**
 * Handle displaying error page
 *
 * @param   r           HTTP Request structure.
 * @return  Status of the HTTP error request.
 *
 * This writes an HTTP status error code and then generates an HTML message to
 * notify the user of the error.
 **/
Status  handle_error(Request *r, Status status) {
    const char *status_string = http_status_string(status);

    /* Write HTTP Header */
    fprintf(r->stream, "HTTP/1.0 %s\r\n", status_string);
    fprintf(r->stream, "Content-Type: text/html\r\n");
    fprintf(r->stream, "\r\n");

    /* Write HTML Description of Error*/
    char *oof = "https://t4.rbxcdn.com/d9d279ac6a67e5288624a2f2dc5782ee";

    fprintf(r->stream,"<h1>%s</h1>\r\n", status_string);
    fprintf(r->stream,"<center>\r\n");
    fprintf(r->stream,"<img src=\"%s\">\r\n", oof);
    fprintf(r->stream,"</center>\r\n");

    /* Return specified status */
    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
