class Voter:
    def __init__(self, voterid, ip, time, candidates):
        self.voterid = voterid
        self.ip = ip
        self.time = time
        self.candidates = dict((vote[:vote.find(':')], vote[vote.find(':')+1:]) for vote in candidates)

    def __len__(self): return len(self.candidates)
    def __eq__(self, other): return self.voterid == other.voterid 
    def __nonzero__(self): return bool(self.candidates)
    def __contains__(self,key): return key in self.candidates
    def __getitem__(self,key): return self.candidates[key]
    def __setitem__(self, key, value): self.candidates[key] = value
    def __delitem__(self, key): del self.candidates[key]
    def __iter__(self): return (x for x in self.candidates.items())
    def __hash__(self): return hash(self.voterid)
    
    def __str__(self):
        attString = 'VoterId: ' + str(self.voterid)  + ', Location: ' + str(self.ip) + ', Time: ' + str(self.time) + '\n'
        canString = ''.join(e + ': ' + c + ', ' for e,c in self.candidates.items())[:-2]
        return attString + canString