from Tkinter import *
from tkFileDialog import askopenfilename
import shutil

class App:
   def __init__(self, master):
      frame = Frame(master)
      frame.pack()
      self.button = Button(frame,text="Import File", command=self.importFile)
      self.button.pack(side=LEFT)
      self.button = Button(frame,text="QUIT", fg="red",command=frame.quit)
      self.button.pack(side=LEFT)
      #self.slogan = Button(frame,text="Hello", command=self.write_slogan)
      #self.slogan.pack(side=LEFT)

   def write_slogan(self):
      print "Tkinter is easy to use!"

   def importFile(self):
      print("importFile")
      #import file from remote directory
      filename = askopenfilename() # show an "Open" dialog box and return the path to the selected file
      print(filename)
      shutil.copyfile(filename, "file.txt")

root = Tk()
root.title("Raspberry Pi GUI")
root.geometry("320x240")
app = App(root)
root.mainloop()
