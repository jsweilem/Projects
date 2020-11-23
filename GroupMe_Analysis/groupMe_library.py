import json

''' Contains methods for interacting with a groupme database json '''

class _groupMe_database:

    def __init__(self):
        self.group_chats = dict()

    def load_groupChats(self, group_chat_file):

        with open(group_chat_file) as f:
            self.group_chats = json.load(f)

        f.close()

    def computeUserPopularity(self, members, data):
        users = []
        for member in members:
            likes = 0
            messages = 0
            for message in data:
                if message["user_id"] == member["user_id"]:
                    messages = messages + 1
                    likes = likes + len(message["favorited_by"])
            if messages > 0:
                user = {"name": member["nickname"], "ratio": float(likes) / float(messages)}
                user['likes'] = likes
                user['messages'] = messages
            else:
                user = {"name": member["nickname"], "ratio": 0.0, 'messages':0, 'likes':0}
            users.append(user)
        return users

    def search_phrase(self, searchPhrase, data, ignoreCase=True):
        ''' returns list with the message events '''
        messages = []
        for event in data:
            if event['text']:
                if ignoreCase and searchPhrase.lower() in event['text'].lower():
                    messages.append(event)
                elif searchPhrase in event['text']:
                    messages.append(event)
        return messages

    def count_phrase(self, searchPhrase, data, ignoreCase=False):
        ''' returns count '''
        count = 0
        for event in data:
            if event['text']:
                for word in event['text'].split():
                    if searchPhrase in word:
                        count += 1
        return count

if __name__ == "__main__":
    gdb = _groupMe_database()
