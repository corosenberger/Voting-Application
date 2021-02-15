from Tally import Tally
import atexit
import socket

class Server:
    def __init__(self,profile):
        atexit.register(Server.cleanUp,self)
        self.tally = Tally(profile)

    def run(self):
        while True:
            pass #server code here

    def cleanUp(self):
        self.tally.save()