# GROUPME ANALYSIS

Project for fall 2020 programming paradigms. Project serves as a groupMe analysis API. 

The server has a dictionary of groupMe entities that contain both a file about the chat, and one containing all the messages in the chat. Both of these files can be downloaded bu anyone from groupme to be used in the server

OO API:

GET/STATS - gets groupchat json and information about a groupchat from a group key
GET/USER - gets information about that user (messages, likes, ect)
GET/SEARCH - searches groupMe for phrases and returns a list of the messages with that phrase
GET/COUNT - returns the number of time a phrase has been sent in a groupchat
PUT - puts group message into server
POST - updates group message in server
DELETE - removes group message info from the server

Complexity:

This project makes use of the ability to use the GroupMe website to download your groupMes as a Json file. 
Users downlaod that file, and upload the JSON as a PUT request (and updates with a post). 
The server python file connects to a controller python file and preforms the function specified above (and in the docs) through the use of a library with functions to parse the json that represents the groupme, and returns information based on the request. 

To run the test script, run the server with python3 server.py in one terminal window and run python3 test_GroupMe_Gets.py in another terminal window.

Our server uses localhost and an objectively randomly chosen port (8080), which can be changed to whatever host and port preferred. They must be changed in the server.py, test_GroupMe_Gets.py, and interaction.js files to work.

The JS page then provides a front end and website to allow users easy access to this functionality, as well as provides information on how to use the website, and also provides some options to parse the data and navigate what the server and controller can do. The options are the same as what the backend specifies. To run the frontend, simply open another window with server.py running and enter information into the fields according to the How To page of the website.
