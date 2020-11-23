#!/usr/bin/env python3
import cherrypy
from groupMeController import GroupMeController
from groupMe_library import _groupMe_database

''' Server for GroupMe Database API '''


class optionsController:
    def OPTIONS(self, *args, **kwargs):
        return ""


def CORS():
    cherrypy.response.headers["Access-Control-Allow-Origin"] = "*"
    cherrypy.response.headers["Access-Control-Allow-Methods"] = "GET, PUT, POST, DELETE, OPTIONS"
    cherrypy.response.headers["Access-Control-Allow-Credentials"] = "true"


def start_service():
    dispatcher = cherrypy.dispatch.RoutesDispatcher()
    gdb = _groupMe_database()

    groupMeController = GroupMeController(gdb=gdb)

    # get the stats for a groupme
    dispatcher.connect('computeUserPopularity', '/messages/:group_id', controller=groupMeController,
                       action='GET_STATS', conditions=dict(method=['GET']))
    # search a GM for a phrase (group~phrase)
    dispatcher.connect(None, '/messages/search/:GM_phrase', controller=groupMeController,
                       action='GET_SEARCH', conditions=dict(method=['GET']))
    # count the times a message has occured in a GM (group~phrase)
    dispatcher.connect(None, '/messages/count/:GM_phrase', controller=groupMeController,
                       action='GET_COUNT', conditions=dict(method=['GET']))
    # get the like commnet ratio for a memeber (group~userID)
    dispatcher.connect(None, '/messages/user/:group_and_user',
                       controller=groupMeController, action='GET_USER', conditions=dict(method=['GET']))
    # put a group chat to the server
    dispatcher.connect(None, '/messages/:group_id', controller=groupMeController,
                       action='PUT_GROUP', conditions=dict(method=['PUT']))
    # update a group message
    dispatcher.connect(None, '/messages/:group_id', controller=groupMeController,
                       action='POST_GROUP', conditions=dict(method=['POST']))
    # add a group message to the data
    dispatcher.connect(None, '/messages/:group_id', controller=groupMeController,
                       action='DELETE_GROUP', conditions=dict(method=['DELETE']))

    # CORS
    dispatcher.connect('gm_stats', '/messages/:group_id', controller=optionsController,
                       action='OPTIONS', conditions=dict(method=['OPTIONS']))
    dispatcher.connect('gm_stats', '/messages/search/:GM_phrase', controller=optionsController,
                       action='OPTIONS', conditions=dict(method=['OPTIONS']))
    dispatcher.connect('gm_stats', '/messages/count/:GM_phrase', controller=optionsController,
                       action='OPTIONS', conditions=dict(method=['OPTIONS']))
    dispatcher.connect('gm_stats', '/messages/user/:group_and_user', controller=optionsController,
                       action='OPTIONS', conditions=dict(method=['OPTIONS']))

    conf = {
        'global': {
            'server.thread_pool': 5,  # optional argument
            'server.socket_host': 'localhost',
            'server.socket_port': 8080,  # change port number to your assigned 8080 is arbitrary on my machine
        },
        '/': {
            'request.dispatch': dispatcher,
            'tools.CORS.on': True,
        }
    }

    cherrypy.config.update(conf)
    app = cherrypy.tree.mount(None, config=conf)
    cherrypy.quickstart(app)

# end of start_service


if __name__ == '__main__':
    cherrypy.tools.CORS = cherrypy.Tool('before_finalize', CORS)
    start_service()
