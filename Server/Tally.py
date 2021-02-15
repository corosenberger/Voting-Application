from Vote import Voter
from collections import Counter
import pickle
import os

class Tally:
    def __init__(self,profile):
        self.profile = profile
        self.rawVotes = dict()
        if os.path.exists(profile):
            with open(profile,'rb+') as votes: self.rawVotes = pickle.load(votes)
        self.counts = Counter((e,c) for voter in self.rawVotes.values() for e,c in voter) if self.rawVotes else Counter()

    def update(self,voter):
        self.remove(voter)
        if type(voter) is str: voter = Voter(voter.split(','))
        self.rawVotes[voter.voterid] = voter
        for e,c in voter: self.counts[e,c] += 1

    def remove(self,voter):
        if type(voter) is str: voter = Voter(voter.split(','))
        if voter in self:
            for e,c in self.rawVotes[voter.voterid]: self.counts[e,c] -= 1
            del self.rawVotes[voter.voterid]

    def save(self):
        with open(self.profile,'wb+') as votes:
            pickle.dump(self.rawVotes,votes)

    def __len__(self): return len(self.rawVotes)
    def __contains__(self,voter): return voter in self.rawVotes.values()
    def __getitem__(self,key): return self.counts[key]
    def __iter__(self): return ((a,b,c) for (a,b),c in self.counts.items())
    def __str__(self): return ''.join('Election: ' + e + ', Candidate: ' + c + ', Votes: ' \
            + str(count) + '\n' for e,c,count in self)[:-1]
    def __call__(self): return ''.join(str(voter) + '\n' for voter in self.rawVotes)[:-1]

if __name__ == '__main__':
    testVoter1 = "1,1,1,12:00,NJ Presidential:John Smith,NJ Gubernatorial:John Smith"
    testVoter2 = "2,1,1,12:00,NJ Presidential:Josh Smith,NJ Gubernatorial:John Smith"
    testVoter3 = "3,1,1,12:00,NJ Presidential:John Smith,NJ Gubernatorial:Josh Smith"
    testVoter4 = "4,1,1,12:00,NJ Presidential:John Smith,NJ Gubernatorial:Josh Smith"
    tally = Tally('testFile')
    tally.update(testVoter1)
    tally.update(testVoter2)
    tally.update(testVoter3)
    tally.update(testVoter4)
    print(tally)