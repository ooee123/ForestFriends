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

   def importFile(self):
      print("importFile")
      # import file from remote directory
      # show an "Open" dialog box and return the path to the selected file
      
      #try:
      #   filename = askopenfilename()
      #except Exception as e:
      #   print ("I/O error")
      filename = askopenfilename()
      print(filename)
      shutil.copyfile(filename, "file.txt") 
      if filename: 
         print("New Frame: Board Design")
     

root = Tk()
root.title("Raspberry Pi GUI")
root.geometry("320x240")
app = App(root)
root.mainloop()
