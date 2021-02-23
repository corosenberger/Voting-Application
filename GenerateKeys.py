from Crypto.PublicKey import RSA

keyType = 'PEM'
size = 4096
e = 3

def main(keyType=keyType, size=size, e=e):
    authNewKey = RSA.generate(size, e=e)
    authPrivateKey = authNewKey.exportKey(keyType)
    authPublicKey = authNewKey.publickey().exportKey(keyType)

    confNewKey = RSA.generate(size, e=e)
    confPrivateKey = confNewKey.exportKey(keyType)
    confPublicKey = confNewKey.publickey().exportKey(keyType)

    cliNewKey = RSA.generate(size, e=e)
    cliPrivateKey = cliNewKey.exportKey(keyType)
    cliPublicKey = cliNewKey.publickey().exportKey(keyType)

    with open('authPrivateKey.pem', 'wb+') as fd: fd.write(authPrivateKey)
    with open('authPublicKey.pem', 'wb+') as fd: fd.write(authPublicKey)

    with open('confPrivateKey.pem', 'wb+') as fd: fd.write(confPrivateKey)
    with open('confPublicKey.pem', 'wb+') as fd: fd.write(confPublicKey)

    with open('cliPrivateKey.pem', 'wb+') as fd: fd.write(cliPrivateKey)
    with open('cliPublicKey.pem', 'wb+') as fd: fd.write(cliPublicKey)

if __name__ == '__main__': main()
    