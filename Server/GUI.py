import Server
import XMLOut
from PyQt5 import QtCore, QtGui, QtWidgets

class ServerThread(QtCore.QThread):
    def __init__(self, server=None):
        super(ServerThread, self).__init__()
        self.server = server

    def setSever(self, server):
        self.server = server

    @QtCore.pyqtSlot()
    def run(self):
        if self.server: Server.main(self.server)

    def stop(self):
        #self.server.save()
        self.server.suspend()

class ServerGUI(XMLOut.Ui_MainWindow):
    def __init__(self):
        super(ServerGUI, self).__init__()
        self.serverThread = ServerThread()
        self.server = None

    def setupUi(self, Main):
        super(ServerGUI, self).setupUi(Main)
        self.newButton.clicked.connect(self.newServer)
        self.loadButton.clicked.connect(self.loadServer)
        self.saveButton.clicked.connect(self.saveServer)
        self.restartButton.clicked.connect(self.startServer)
        self.stopButton.clicked.connect(self.stopServer)

    def startServer(self):
        self.newButton.setEnabled(False)
        self.loadButton.setEnabled(False)
        self.saveButton.setEnabled(True)
        self.restartButton.setEnabled(False)
        self.stopButton.setEnabled(True)
        self.serverThread.setSever(self.server)
        self.serverThread.start()

    def newServer(self):
        profile = QtWidgets.QFileDialog.getSaveFileName(filter='*.profile')[0]
        if profile:
            self.server = Server.Server(profile)
            self.startServer()

    def loadServer(self):
        profile = QtWidgets.QFileDialog.getOpenFileName(filter='*.profile')[0]
        if profile:
            self.server = Server.Server(profile)
            self.startServer()

    def saveServer(self):
        self.server.save()

    def stopServer(self):
        self.newButton.setEnabled(True)
        self.loadButton.setEnabled(True)
        self.saveButton.setEnabled(True)
        self.restartButton.setEnabled(True)
        self.stopButton.setEnabled(False)
        self.serverThread.stop()
        self.serverThread.wait()

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Main = QtWidgets.QMainWindow()
    ui = ServerGUI()
    ui.setupUi(Main)
    Main.show()
    sys.exit(app.exec_())