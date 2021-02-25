# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'GUI.ui'
#
# Created by: PyQt5 UI code generator 5.9.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(511, 562)
        MainWindow.setMinimumSize(QtCore.QSize(511, 452))
        MainWindow.setMaximumSize(QtCore.QSize(511, 600))
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.newButton = QtWidgets.QPushButton(self.centralwidget)
        self.newButton.setGeometry(QtCore.QRect(10, 10, 491, 101))
        self.newButton.setObjectName("newButton")
        self.saveButton = QtWidgets.QPushButton(self.centralwidget)
        self.saveButton.setEnabled(False)
        self.saveButton.setGeometry(QtCore.QRect(10, 230, 491, 101))
        self.saveButton.setObjectName("saveButton")
        self.stopButton = QtWidgets.QPushButton(self.centralwidget)
        self.stopButton.setEnabled(False)
        self.stopButton.setGeometry(QtCore.QRect(10, 450, 491, 101))
        self.stopButton.setObjectName("stopButton")
        self.loadButton = QtWidgets.QPushButton(self.centralwidget)
        self.loadButton.setGeometry(QtCore.QRect(10, 120, 491, 101))
        self.loadButton.setObjectName("loadButton")
        self.restartButton = QtWidgets.QPushButton(self.centralwidget)
        self.restartButton.setEnabled(False)
        self.restartButton.setGeometry(QtCore.QRect(10, 340, 491, 101))
        self.restartButton.setObjectName("restartButton")
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Server GUI"))
        self.newButton.setText(_translate("MainWindow", "New Profile"))
        self.saveButton.setText(_translate("MainWindow", "Save Tally"))
        self.stopButton.setText(_translate("MainWindow", "Suspend Server"))
        self.loadButton.setText(_translate("MainWindow", "Load Profile"))
        self.restartButton.setText(_translate("MainWindow", "Restart Server"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())

