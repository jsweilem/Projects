import cherrypy
import re
import json
from groupMe_library import _groupMe_database

''' Handles HTTP requests '''


class GroupMeController(object):

    def __init__(self, gdb=None):
        if gdb is None:
            self.gdb = _groupMe_database()
        else:
            self.gdb = gdb

        self.gdb.load_groupChats('messages.json')

    def GET_STATS(self, group_id):
        ''' send back stats for a group me'''
        output = {'result': 'success'}

        try:
            # TODO: pull the groupme from the movie id, and generate stats to respond with
            # the id has a list of the message data [0] (conversation) and information about the chat [1]
            output["chat"] = self.gdb.group_chats[group_id][0]
            output["num_messages"] = self.gdb.group_chats[group_id][1]["messages"]["count"]
            user_data = self.gdb.computeUserPopularity(
                self.gdb.group_chats[group_id][1]["members"], self.gdb.group_chats[group_id][0])
            output["user_data"] = user_data

        except Exception as ex:
            output['result'] = 'error'
            output['message'] = str(ex)

        return json.dumps(output)

    def GET_USER(self, group_and_user):
        ''' the body will contain a user to display general stats for'''
        output = {'result': 'success'}
        args = group_and_user.split("~")
        group_id = args[0]

        if '-' in args[1]:
            username = args[1].replace('-', ' ')

        try:
            # TODO: pull the groupme from the movie id, use user from body and generate stats to respond with
            user_data = self.gdb.computeUserPopularity(
                self.gdb.group_chats[group_id][1]["members"], self.gdb.group_chats[group_id][0])

            for user in user_data:
                if user["name"] == username:
                    output["user"] = user
        except Exception as ex:
            output['result'] = 'error'
            output['message'] = str(ex)

        return json.dumps(output)

    def GET_SEARCH(self, GM_phrase):
        ''' the body will contain a user to display general stats for'''
        output = {'result': 'success'}

        group_id, phrase = GM_phrase.split("~")

        try:
            # TODO: pull the groupme from the movie id, use user from body and generate stats to respond with
            phrases = self.gdb.search_phrase(phrase, self.gdb.group_chats[group_id][0])

            if phrases:
                output["found"] = []
                for e in phrases:
                    output["found"].append(e["text"])
            else:
                output["found"] = "No phrases found"
        except Exception as ex:
            output['result'] = 'error'
            output['message'] = str(ex)

        return json.dumps(output)

    def GET_COUNT(self, GM_phrase):
        ''' the body will contain a user to display general stats for'''
        output = {'result': 'success'}

        group_id, phrase = GM_phrase.split("~")

        try:
            # TODO: pull the groupme from the movie id, use user from body and generate stats to respond with
            count = self.gdb.count_phrase(phrase, self.gdb.group_chats[group_id][0])

            output["count"] = count

        except Exception as ex:
            output['result'] = 'error'
            output['message'] = str(ex)

        return json.dumps(output)

    def PUT_GROUP(self, group_id):
        '''take the body and add it to the messages'''
        output = {'result': 'success'}

        data = json.loads(cherrypy.request.body.read().decode('utf-8'))

        try:
            # TODO: take body and add an entry with the group_id as a key
            self.gdb.group_chats[group_id] = data["chat"]
        except Exception as ex:
            output['result'] = 'error'
            output['message'] = str(ex)

        return json.dumps(output)

    def POST_GROUP(self, group_id):
        '''take the body and add it to the messages'''
        output = {'result': 'success'}

        data = json.loads(cherrypy.request.body.read().decode('utf-8'))

        try:
            # TODO: take body and add an entry with the group_id as a key
            self.gdb.group_chats[group_id] = data["chat"]
        except Exception as ex:
            output['result'] = 'error'
            output['message'] = str(ex)

        return json.dumps(output)

    def DELETE_GROUP(self, group_id):
        '''take the body and update messages'''
        output = {'result': 'success'}

        try:
            # TODO: take body and add an entry with the group_id as a key
            self.gdb.group_chats.pop(group_id)
        except Exception as ex:
            output['result'] = 'error'
            output['message'] = str(ex)

        return json.dumps(output)
