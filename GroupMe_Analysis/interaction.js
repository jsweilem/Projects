console.log('page load - entered main.js');

var sendButton = document.getElementById('analyze-button');
sendButton.onmouseup = getFormInfo;

function getFormInfo(){
    console.log('entered getFormInfo');

    var url_base = "http://localhost";
    var port = "8080";
    var group_me_id = document.getElementById('id-body').value;
    var message = document.getElementById("text-message-body").value;
    var send_message = false;

    var url;
    var request;
    var selindex = document.getElementById('select-server-option').selectedIndex;
    var choice = document.getElementById('select-server-option').options[selindex].value;

    if (choice == "Add GroupMe") {
        console.log('entered add GroupMe');

        request = "PUT";
        url = url_base + ":" + port + "/messages/" + group_me_id;
        send_message = true;
    }
    else if (choice == "Modify GroupMe") {
        console.log('entered modify GroupMe');

        request = "POST"
        url = url_base + ":" + port + "/messages/" + group_me_id;
        send_message = true;
    }
    else if (choice == "Delete GroupMe") {
        console.log('entered delete GroupMe');

        request = "DELETE"
        url = url_base + ":" + port + "/messages/" + group_me_id;
        send_message = false;
    }
    else if (choice == "Check Stats") {
        console.log('entered check stats');

        request = "GET";
        url = url_base + ":" + port + "/messages/" + group_me_id;
        send_message = false;
    }
    else if (choice == "Find User") {
        console.log('entered find user');

        request = "GET";
        url = url_base + ":" + port + "/messages/user/" + group_me_id;
        send_message = false;
    }
    else if (choice == "Search Phrase") {
        console.log('entered search phrase');

        request = "GET";
        url = url_base + ":" + port + "/messages/search/" + group_me_id;
        send_message = false;
    }
    else if (choice == "Count Phrase") {
        console.log('entered find user');

        request = "GET";
        url = url_base + ":" + port + "/messages/count/" + group_me_id;
        send_message = false;
    }
    else {
        console.log('huh?');
    }

    makeNetworkCallToServer(url, request, send_message, message, choice);
}


function makeNetworkCallToServer(url, request, send_body, message, choice){
    console.log('entered network call');

    var label = document.getElementById("answer-label");

    var xhr = new XMLHttpRequest();

    xhr.open(request, url, true);

    xhr.onload = function(e) {
        console.log(xhr.responseText);
        displayResponse(xhr.responseText, choice);
    }

    xhr.onerror = function(e) {
        console.error(xhr.statusText);
        label.innerHTML = "The request sent was invalid. Please make sure that the format is correct.";
    }

    if (send_body) {
        console.log(message);
        xhr.send(message);
    }
    else {
        xhr.send(null);
    }

    console.log('finished network');
}

function displayResponse(responseText, choice)
{
    console.log("display response to user");

    var label = document.getElementById("answer-label-1");
    var label2 = document.getElementById("answer-label-2");
    var label3 = document.getElementById("answer-label-3");
    var label4 = document.getElementById("answer-label-4");

    responseJson = JSON.parse(responseText);

    if (choice == "Add GroupMe") {
        console.log('entered response for add GroupMe');
        if (responseJson['result'] == "success") {
            label.innerHTML = "The group data was successfully added to the database!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
        else {
            label.innerHTML = "There was a problem adding the group to the database, check to make sure you followed the correct format in the 'How To' page!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
    }
    else if (choice == "Modify GroupMe") {
        console.log('entered response for modify GroupMe');
        if (responseJson['result'] == "success") {
            label.innerHTML = "The group data was successfully modified!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
        else {
            label.innerHTML = "There was a problem modifiying the given group, check to make sure you followed the correct format in the 'How To' page!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
    }
    else if (choice == "Delete GroupMe") {
        console.log('entered response for delete GroupMe');
        if (responseJson['result'] == "success") {
            label.innerHTML = "The group data was successfully deleted!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
        else {
            label.innerHTML = "There was a problem deleting the given group, check to make sure you followed the correct format in the 'How To' page!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
    }
    else if (choice == "Check Stats") {
        console.log('entered response for check stats');
        if (responseJson['result'] == "success") {
            label.innerHTML = "The stats for the selected group are displayed below!";
            label2.innerHTML = "Total Number of Messages: " + responseJson['num_messages'];

            var i;
            var user_info = "";

            for(i = 0; i < responseJson['user_data'].length; i++) {
                console.log(responseJson['user_data'][i]['name']);
                user_info = user_info + "Name: " + responseJson['user_data'][i]['name'] + 
                ", Like Ratio: " + responseJson['user_data'][i]['ratio'] + ", Likes: " + 
                responseJson['user_data'][i]['likes'] + ", Messages: " + 
                responseJson['user_data'][i]['messages'] + " | "; 
            }

            label3.innerHTML = "User Information: ";
            label4.innerHTML = user_info;
        }
        else {
            label.innerHTML = "There was a problem retrieving the group's stats, check to make sure you followed the correct format in the 'How To' page!"; 
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
    }
    else if (choice == "Find User") {
        console.log('entered response for find user');
        if (responseJson['result'] == "success" && responseJson['user']) {
            label.innerHTML = "The user was found in the group!";
            label2.innerHTML = "Name: " + responseJson['user']['name'];
            label3.innerHTML = "Like Ratio: " + responseJson['user']['ratio'];
            label4.innerHTML = "Number of Messages: " + responseJson['user']['messages'];
        }
        else if (responseJson['result'] == "success") {
            label.innerHTML = "The user was not found in the group!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
        else {
            label.innerHTML = "There was a problem trying to find the user in the group members, check to make sure you followed the correct format in the 'How To' page!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = ""; 
        }
    }
    else if (choice == "Search Phrase") {
        console.log('entered response for search phrase');
        if (responseJson['result'] == "success" && responseJson['found'] != "No phrases found") {
            label.innerHTML = "The phrase was found in the group!";
            label2.innerHTML = "Messages with the Phrase: " + responseJson['found'];
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
        else if (responseJson['result'] == "success" && responseJson['found'] == "No phrases found") {
            label.innerHTML = "The phrase had no occurences in the group!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
        else {
            label.innerHTML = "There was a problem trying to find the phrase in the group messages, check to make sure you followed the correct format in the 'How To' page!"; 
        }
    }
    else if (choice == "Count Phrase") {
        console.log('entered response for find user');
        if (responseJson['result'] == "success") {
            label.innerHTML = "The phrase was found " + responseJson['count'] + " times in the group!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
        else {
            label.innerHTML = "There was a problem counting the given phrase in the group, check to make sure you followed the correct format in the 'How To' page!";
            label2.innerHTML = "";
            label3.innerHTML = "";
            label4.innerHTML = "";
        }
    }
    else {
        console.log('huh?')
    }
}
