#coding:utf-8
import socket
'''
class Mysock(object):
    def __init__(self,url,port):
        self.url = url
        self.port = port
        self._socket = socket.socket()
        self.response = b''

    def connect(self):
        self._socket.connect( (self.url,self.port) )

    def recv(self):
        chunk = self._socket.recv(4069)
        while chunk:
            self.response += chunk
            chunk = self._socket.recv(4069)
        return self.response

    def send(self):
        

if __name__=='__main__':
    socket = Mysock('localhost.com',5000)
    socket.connect()
    socket.send()
    response = socket.recv()
    print response
'''
if __name__=='__main__':
    sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    sock.bind( ('127.0.0.1',9999) )
    sock.listen(3)
    print 'Waiting...'
    while(True):
        s, addr = sock.accept()
        s.send("Welcome")
        data = s.recv(1024)
        print data
        if data == "Bye":
            sock.close()
            break
    '''
    request = 'GET {} HTTP/1.0\r\nHost: localhost\r\n\r\n'.format('')
    sock.send(request.encode('ascii'))
    chunk = socket.recv(4069)
    while chunk:
        print chunk
        chunk = socket.recv(4069)
    '''











    
    
