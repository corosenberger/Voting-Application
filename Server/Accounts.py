
class Accounts:
    class Account:
        def __init__(self, voterid, email, password):
            self.voterid = voterid
            self.email = email
            self.password = password

        def __hash__(self): return hash(self.voterid)
        def __str__(self): return str(self.voterid) + '\n' + str(self.email) + '\n' + str(self.password) + '\n'

    def __init__(self): self.accounts = dict()
    def add(self, voterid, email, password): self.accounts[email] = self.Account(voterid,email,password)
    def match(self, email, password): return email in self.accounts and self.accounts[email].password == password
    def __getitem__(self,email): return self.accounts[email].voterid
    def __contains__(self, email): return email in self.accounts
    def __str__(self): return ''.join(str(a) for a in self.accounts.values())