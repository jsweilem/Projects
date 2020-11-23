#!/usr/bin/env python3

import unittest
import requests
import json

''' Tests the functionality of API methods '''


class TestGroupMeGets(unittest.TestCase):

    SITE_URL = 'http://localhost:8080'  # replace with your assigned port id
    print("Testing for server: " + SITE_URL)
    MESSAGES_URL = SITE_URL + '/messages/'

    def is_json(self, resp):
        try:
            json.loads(resp)
            return True
        except ValueError:
            return False

    def test_group_get(self):
        ''' Check our get '''
        id = "chat_exists"
        r = requests.get(self.MESSAGES_URL + id)
        self.assertTrue(self.is_json(r.content.decode()))
        resp = json.loads(r.content.decode())

        self.assertEqual(resp["chat"], "valid_chat")

        id = "Joe_GM"
        r = requests.get(self.MESSAGES_URL + id)
        self.assertTrue(self.is_json(r.content.decode()))
        resp = json.loads(r.content.decode())

        self.assertEqual(resp["chat"][0]["avatar_url"],
                         "https://i.groupme.com/1024x1024.jpeg.4abf7090f9174bf7b3377b9e70aabcbb")
        self.assertEqual(resp["num_messages"], 11)
        self.assertEqual(resp["user_data"][0]['ratio'], .5)

        id = "not here"
        r = requests.get(self.MESSAGES_URL + id)
        self.assertTrue(self.is_json(r.content.decode()))
        resp = json.loads(r.content.decode())

        self.assertEqual(resp["result"], "error")

    def test_group_get_user(self):
        r = requests.get(self.MESSAGES_URL + "/user/Joe_GM~Josh-Bottelberghe")
        self.assertTrue(self.is_json(r.content.decode()))
        resp = json.loads(r.content.decode('utf-8'))
        self.assertEqual(resp['result'], 'success')
        self.assertEqual(resp["user"]["name"], "Josh Bottelberghe")
        self.assertEqual(resp["user"]["ratio"], .5)

    def test_search_phrases(self):
        r = requests.get(self.MESSAGES_URL + "/search/Joe_GM~Data")
        self.assertTrue(self.is_json(r.content.decode()))
        resp = json.loads(r.content.decode('utf-8'))
        self.assertEqual(resp['result'], 'success')
        self.assertEqual(len(resp["found"]), 3)

    def test_count_phrases(self):
        r = requests.get(self.MESSAGES_URL + "/count/Joe_GM~data")
        self.assertTrue(self.is_json(r.content.decode()))
        resp = json.loads(r.content.decode('utf-8'))
        self.assertEqual(resp['result'], 'success')
        self.assertEqual(resp["count"], 4)

    def test_group_put(self):
        new = {"chat": ["message data", "messages"]}
        r = requests.put(self.MESSAGES_URL + "NEW_CHAT", data=json.dumps(new))
        resp = json.loads(r.content.decode('utf-8'))
        self.assertEqual(resp['result'], 'success')

    def test_group_post(self):
        new = {"chat": ["message data", "messages"]}
        r = requests.put(self.MESSAGES_URL + "NEW_CHAT", data=json.dumps(new))
        resp = json.loads(r.content.decode('utf-8'))
        self.assertEqual(resp['result'], 'success')

        new = {"chat": ["message data", "new messages"]}
        r = requests.put(self.MESSAGES_URL + "NEW_CHAT", data=json.dumps(new))
        resp = json.loads(r.content.decode('utf-8'))
        self.assertEqual(resp['result'], 'success')

    def test_group_delete(self):
        new = {"chat": ["message data", "messages"]}
        r = requests.put(self.MESSAGES_URL + "NEW_CHAT", data=json.dumps(new))
        group_id = "NEW_CHAT"
        r = requests.delete(self.MESSAGES_URL + group_id)
        resp = json.loads(r.content.decode())
        self.assertEqual(resp['result'], 'success')


if __name__ == "__main__":
    unittest.main()
