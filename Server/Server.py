from Tally import Tally
from Vote import Voter
import atexit
import socket
import threading
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

BUFFER_SIZE = 4096

class Server:
    def __init__(self,profile):
        #atexit.register(Server.cleanUp,self)
        self.tally = Tally(profile)
        self.serverSock = None
        self.stages = {}

    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as self.serverSock:
            with open("server.configure") as fd: host, port = socket.INADDR_ANY, int(fd.read())
            self.serverSock.bind(('127.0.0.1',port))
            self.serverSock.listen(10)
            i = 0
            try:
                while True:
                    clientSock, clientAddress = self.serverSock.accept()
                    clientSock.settimeout(1)
                    clientThread = threading.Thread(target=self.handleConnection,args=(clientSock, clientAddress))
                    clientThread.start()
            except: pass
        self.serverSock = None

    def handleConnection(self, clientSock, clientAddress):
        try:
            data = []
            try: 
                while len(part:=clientSock.recv(BUFFER_SIZE)) >= 1: data.append(part)
            except: data = b''.join(data)
            with open('cliPrivateKey.pem') as kfd: key = RSA.importKey(kfd.read())
            plain = PKCS1_OAEP.new(key).decrypt(data)

            print(plain)

            response = 'Hello' #self.castVote(data)
            
            #with open('cliPublicKey.pem') as kfd: key = RSA.importKey(kfd.read())
            #crypt = PKCS1_OAEP.new(key).encrypt(response)
            clientSock.send(bytes(response,'utf-8'))
        finally: clientSock.close()

    def suspend(self):
        if self.serverSock: self.serverSock.close()

    def castVote(self,data):
        with open('cliPrivateKey.pem') as kfd: key = RSA.importKey(kfd.read())
        plain = PKCS1_OAEP.new(key).decrypt(data)
        voter = Voter(plain.split('\n'))
        self.tally.update(voter)
        return 'Success'

    def save(self):
        self.tally.save()

    def cleanUp(self):
        self.tally.save()

def main(server): server.run()

if __name__ == '__main__': main(Server('test.profile'))
