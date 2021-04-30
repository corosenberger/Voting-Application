from Tally import Tally
from Vote import Voter
from Accounts import Accounts
import Perceptron
import os
import re
import atexit
import socket
import threading
import pickle
from datetime import date, datetime
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

BUFFER_SIZE = 4096
PUBLIC_KEY_FILENAME = 'key_send.pem'
PRIVATE_KEY_FILENAME = 'key_recv.pem'
NET_SAVE_FILE = 'Voter Net.sav'
NET_ACCEPT_INDEX, NET_REJECT_INDEX = 0, 1

class Server:
    def __init__(self,profile):
        atexit.register(Server.save,self)
        self.profile = profile
        self.voterid = self.accounts = self.tally = None
        if os.path.exists(profile): 
            with open(profile,'rb+') as pfd: self.voterids, self.accounts, self.tally = pickle.load(pfd)
        else:
            self.voterids, self.accounts, self.tally = set(), Accounts(), Tally()
        self.serverSock = None
        self.logins = dict()
        self.insignup = dict()
        self.net = Perceptron.initFromFile(NET_SAVE_FILE)

    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as self.serverSock:
            with open("server.configure") as fd: host, port = socket.INADDR_ANY, int(fd.read())
            self.serverSock.bind(('',port))
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
            clientAddress = clientAddress[0]
            data = []
            try: 
                while len(part:=clientSock.recv(BUFFER_SIZE)) >= 1: data.append(part)
            except: data = b''.join(data)
            with open(PRIVATE_KEY_FILENAME) as kfd: key = RSA.importKey(kfd.read())

            try:
                padding = True
                plainList = PKCS1_OAEP.new(key).decrypt(data).decode("utf-8").split('\n')
            except:
                padding = False
                plainList = data.decode("utf-8").split('\n')

            print(plainList)
            print(clientAddress)

            type = plainList.pop()
            if type == 'vote':
                response = self.castVote(self.logins[clientAddress],clientAddress,plainList)
            elif type == 'create':
                response = self.createAccount(plainList[0],plainList[1],self.insignup[clientAddress],clientAddress) \
                    if clientAddress in self.insignup else 'Major Failure'
            elif type == 'signup':
                response = self.signup(plainList[0],clientAddress)
            elif type == 'login':
                response = self.login(plainList[0],plainList[1],clientAddress)
            elif type == 'logout':
                response = self.logout(clientAddress)
            else:
                response = 'Failure'

            print(response)
            
            with open(PUBLIC_KEY_FILENAME) as kfd: key = RSA.importKey(kfd.read())

            if padding:
                crypt = PKCS1_OAEP.new(key).encrypt(bytes(response,'utf-8'))
            else:
                crypt = bytes(response,'utf-8')

            clientSock.send(crypt)
        finally: clientSock.close()

    def suspend(self):
        if self.serverSock: self.serverSock.close()

    def castVote(self, voterid, ip, candidates):
        try:
            l1,l2,l3 = [int(s) for s in ip.split('.')]
            tm = int(datetime.datetime.utcnow().timestamp())
            netOut = self.net.computeOutput([voterid,l1,l2,l3,tm])
            if netOut[NET_ACCEPT_INDEX] < netOut[NET_REJECT_INDEX]:
                #currently net is untrained - no data on computerized voting rip
                #so just setting this as a 'pass' statement
                #in the future this would be wired to return failure
                #i.e. comment out this next line:
                #return 'Failure'
                pass
        except: pass
        
        voter = Voter(voterid,ip,datetime.now(),candidates)
        self.tally.update(voter)
        return 'Success'

    def signup(self, voterid, voteraddr):
        if voterid not in self.voterids: return 'Failure'
        self.insignup[voteraddr] = voterid
        return 'Success'

    def createAccount(self, email, password, voterid, voteraddr):
        if voterid not in self.voterids or voterid in self.tally.rawVotes: 
            del self.insignup[voteraddr]
            return 'Failure'
        emailcheck = re.match(r"[^@]+@[^@]+\.[^@]+", email) and email not in self.accounts
        passcheck =  re.match(r"^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*#?&])[A-Za-z\d@$!#%*?&]{6,20}$",password)
        print(emailcheck)
        print(passcheck)
        if emailcheck and passcheck:
            self.accounts.add(voterid,email,password)
            self.logins[voteraddr] = voterid
            del self.insignup[voteraddr]
            self.voterids.remove(voterid)
            return 'Success'
        return 'Failure'

    def login(self, email, password, voteaddr):
        if not self.accounts.match(email,password): return 'Failure'
        self.logins[voteaddr] = self.accounts[email]
        return 'Success'

    def logout(self,voteraddr):
        if voteraddr in self.logins: del self.logins[voteraddr]
        return 'Success'

    def save(self):
        with open(self.profile,'wb') as pfd: pickle.dump([self.voterids,self.accounts,self.tally],pfd)

def main(server): server.run()

if __name__ == '__main__': 
    server = Server('test.profile')
    for v in server.tally.rawVotes.items(): print(v[1])
    print(server.accounts)
    print(server.tally)
