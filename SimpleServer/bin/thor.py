#!/usr/bin/env python3

import concurrent.futures
import os
import requests
import sys
import time

# Functions

def usage(status=0):
    progname = os.path.basename(sys.argv[0])
    print(f'''Usage: {progname} [-h HAMMERS -t THROWS] URL
    -h  HAMMERS     Number of hammers to utilize (1)
    -t  THROWS      Number of throws per hammer  (1)
    -v              Display verbose output
    ''')
    sys.exit(status)

def hammer(url, throws, verbose, hid):
    ''' Hammer specified url by making multiple throws (ie. HTTP requests).

    - url:      URL to request
    - throws:   How many times to make the request
    - verbose:  Whether or not to display the text of the response
    - hid:      Unique hammer identifier

    Return the average elapsed time of all the throws.
    '''
    
    # For each throw, perform request and print results, while returning average time
    for throw in range(throws):
        begin = time.time()
        response = requests.get(url)

        if verbose:
            print(response.text)
        
        print(f'Hammer: {hid}, Throw: {throw}, Elapsed Time: {time.time() - begin}')

    return (time.time() - begin) / throws

def do_hammer(args):
    ''' Use args tuple to call `hammer` '''
    return hammer(*args)

def main():
    arguments = sys.argv[1:]
    hammers   = 1
    throws    = 1
    verbose   = False
    url       = ''

    # Check if there are any arguments
    if len(sys.argv) == 1:
        usage(1)
    
    # Parse command line arguments
    while arguments and arguments[0].startswith('-'):
        arg = arguments.pop(0)
        if arg == '-h':
            hammers = int(arguments.pop(0))
        elif arg == '-t':
            throws = int(arguments.pop(0))
        elif arg == '-v':
            verbose = True
        else:
            usage(1)

    # Last argument is url, which should start with [h]ttps
    if arguments and arguments[0].startswith('h'):
        url = arguments.pop(0)

    # Create pool of workers and perform throws
    hammerThrows = [(url, throws, verbose, hid) for hid in range(hammers)]
    begin = 0

    with concurrent.futures.ProcessPoolExecutor() as executor:
        workers = zip(range(hammers), executor.map(do_hammer, hammerThrows))
        for hid, worker in workers:
            begin = begin + worker
            print(f'Hammer: {hid}, AVERAGE , Elapsed Time: {begin}')

    # Print final average time
    print(f'TOTAL AVERAGE ELAPSED TIME: {begin / throws * hammers}')
    

# Main execution

if __name__ == '__main__':
    main()

# vim: set sts=4 sw=4 ts=8 expandtab ft=python:
