# Raspberry Pi GUI

from Tkinter import *
from tkFileDialog import askopenfilename
import shutil

def importFile():
   print("importFile")
   #import file from remote directory
   filename = askopenfilename() # show an "Open" dialog box and return the path to the selected file
   print(filename)
   shutil.copyfile(filename, "file.txt")

def createWindow(): 
   print("createWindow")
   # Directory
   Tk().withdraw() # we don't want a full GUI, so keep the root window from appearing
   
   # Create the window
   root = Tk()

   # Modify root window
   root.title("Raspberry Pi GUI")
   root.geometry("320x240")

   #kick off the event loop
   root.mainloop()
   
def main():
   createWindow()
   importFile()

#if __name__ == '__main__': Hello().mainloop()

main()
