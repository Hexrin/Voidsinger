from PIL import Image, ImageQt
import re
import pandas as pd
import sys
import pyperclip

from PyQt5 import QtGui, QtCore, QtWidgets

class scrollableLabel(QtWidgets.QScrollArea):

    # constructor
    def __init__(self, *args, **kwargs):

        QtWidgets.QScrollArea.__init__(self, *args, **kwargs)

        #self.setSliderDown(True)
        # making widget resizable
        self.setWidgetResizable(True)

        # making qwidget object
        content = QtWidgets.QWidget(self)
        self.setWidget(content)

        # vertical box layout
        lay = QtWidgets.QVBoxLayout(content)

        # creating label
        self.label = QtWidgets.QLabel(content)

        # setting alignment to the text
        self.label.setAlignment(QtCore.Qt.AlignLeft | QtCore.Qt.AlignTop)

        # making label multi-line
        self.label.setWordWrap(True)

        # adding label to the layout
        lay.addWidget(self.label)

        self.scrollbar = self.verticalScrollBar()

    # the setText method
    def setText(self, text):

        # setting text to the label
        self.label.setText(text)

    def text(self):

        return self.label.text()

class ImageReader(QtWidgets.QWidget):

    def __init__(self):
        
        super(ImageReader, self).__init__()
        
        self.mainLayout = QtWidgets.QVBoxLayout(self)
        selectFileButton = QtWidgets.QPushButton("Select File")
        selectFileButton.clicked.connect(self.selectFile)
        self.mainLayout.addWidget(selectFileButton)
        
        self.imgLabel = QtWidgets.QLabel()
        self.mainLayout.addWidget(self.imgLabel)
        
        self.parsedLabel = scrollableLabel()
        self.mainLayout.addWidget(self.parsedLabel)
        
        copyToClipboardButton = QtWidgets.QPushButton("Copy to Clipboard")
        copyToClipboardButton.clicked.connect(self.copyToClipboard)
        self.mainLayout.addWidget(copyToClipboardButton)

    def selectFile(self):
        
        fileSelector = QtWidgets.QFileDialog(self)
        fileSelector.setWindowTitle("Choose Image to Read")
        fileSelector.setFileMode(QtWidgets.QFileDialog.ExistingFile)
        
        selected  = fileSelector.exec()
        
        if selected:
            
            self.parseImage(fileSelector.selectedFiles()[0])
        
    def parseImage(self, file):

        img = Image.open(file)
        imgWidth, imgHeight = img.size
        img = img.convert("RGBA")
        imgdata = img.getdata()

        self.imgLabel.setPixmap(QtGui.QPixmap.fromImage(ImageQt.ImageQt(img)))
        
        x_pos = 0
        y_pos = 1



        pixel_value = "("
        x = [ ]
        y = [ ]

        for item in imgdata:
            
            if (x_pos) == imgWidth:
                x_pos = 1
                y_pos += 1
            else:
                x_pos += 1

            if item[3] != 0:
           
                pixel_value +=("(X="+str(x_pos) + ", Y="+str(y_pos)+"),")
         
        pixel_value = pixel_value[:-1]         
            
        pixel_value += ")"

    
        #pixel_value, x, y = zip(*sorted(zip(pixel_value, x, y), reverse=True))

        print(pixel_value)
        self.parsedLabel.setText(pixel_value)

    def copyToClipboard(self):
        
        pyperclip.copy(self.parsedLabel.text())
        
        
if __name__ == "__main__":

    #start the app
    app = QtWidgets.QApplication(sys.argv)
    mainWindow = ImageReader()
    mainWindow.setWindowTitle("Image parser to array")
    mainWindow.show()
    sys.exit(app.exec_())
