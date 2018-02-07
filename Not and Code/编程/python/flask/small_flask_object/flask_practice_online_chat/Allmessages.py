import time
class AllMessages(object):
    def __init__(self):
        #self._all_message = {"message":[],"latest_time":0}
        self._all_message = {"message":[{"name":"Bob",\
                                         "content":"the first message",\
                                         "timestamp":1495329455.086971,\
                                         "time":"2017-5-21"}, \
                                        {"name": "Jack", \
                                         "content": "the second message", \
                                         "timestamp": 1495330209.358412,\
                                         "time": "2017-5-21"}],\
                             "latest_time":1495330209.358412}


    def check_time(self, timestamp):
        new_message = []
        if timestamp >=self._all_message["latest_time"] or timestamp==-1:
            return None

        for msg in self._all_message["message"]:  # msg is a dict
            if timestamp < msg["timestamp"]:
                new_message.append(msg)
        return {"message":new_message, \
                "latest_time":self._all_message["latest_time"], \
                }

    def add(self,message, name):
        current_time = time.time()
        new_message = {"content":message, "name":name, \
                       "time":time.asctime( time.localtime(time.time()) ), \
                       "timestamp":current_time }

        if len(self._all_message["message"])>=1000:
            self._all_message.pop(0)
        if  current_time>self._all_message["latest_time"]:
            self._all_message["latest_time"] = current_time

        self._all_message["message"].append(new_message)

    def all_messages(self):
        return self._all_message

    def lateset_time(self):
        return self._all_message["latest_time"]
