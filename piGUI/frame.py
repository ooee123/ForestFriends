import subprocess
from Tkinter import *
import Tkinter as tk
from tkFileDialog import askopenfilename
import shutil
from PIL import Image, ImageTk
import os

LARGE_FONT= ("Verdana", 12)

frames = {}

class GUI(tk.Tk):

   def __init__(self, *args, **kwargs):

      tk.Tk.__init__(self, *args, **kwargs)
      container = tk.Frame(self)

      container.pack(side="top", fill="both", expand = True)

      container.grid_rowconfigure(0, weight=1)
      container.grid_columnconfigure(0, weight=1)


      for F in (Start, ImportFile, badFileSyntax, VerifyDesign, Redesign, VerifyParts, Reverify, Machine):
         frame = F(container, self)
         frames[F] = frame
         frame.grid(row=0, column=0, sticky="nsew")

      self.show_frame(Start)
      


   def show_frame(self, cont):

      frame = frames[cont]
      frame.tkraise()


class Start(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self,parent)
      #label = tk.Label(self, text="Start Page", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)

      button = tk.Button(self, height=21, width=21, bg="red", text="Start...", font=("Verdana", 20),
         command=lambda: controller.show_frame(ImportFile))
      #button.grid(row = 2, column = 2, sticky= W)
      button.pack()

      #button2 = tk.Button(self, text="End",
      #   command=lambda: controller.show_frame(Start))
      #button2.pack()



class ImportFile(tk.Frame):
      
   def __init__(self, parent, controller):
      self.cont = controller
      tk.Frame.__init__(self,parent)
      #label = tk.Label(self, text="Import File", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)

      button = tk.Button(self, text="Import File",width = 18, height = 4,font=("Verdana", 20),
         command=self.openFile)
         #command=lambda: controller.show_frame(VerifyDesign))
      button.pack()

      button2 = tk.Button(self, text="End",width = 18, height = 3,font=("Verdana", 20),
         command=lambda: controller.show_frame(Start))
      button2.pack()

   def openFile(self):
      print("importing file")
      filename = askopenfilename()
      print(filename)
      shutil.copyfile(filename, "file.txt") 
      if (checkSyntax()):
         frames[VerifyParts].readFile()
         if filename: 
            print("New Frame: Board Design")
            # call java fuction
            self.javaExec()
            self.resize()
            self.cont.show_frame(VerifyDesign)
      else:
         self.cont.show_frame(badFileSyntax)

   def resize(self):
      basewidth = 300
      img = Image.open("original.jpg")

      print("image is opened")
      wpercent = (basewidth / float (img.size[0]))
      hsize = int ((float (img.size[1]) * float (wpercent)))
      img = img.resize((basewidth, hsize), Image.ANTIALIAS)
      img.save('resize.jpg')
      frames[VerifyDesign].updateImage(self.cont)
      #os.remove("original.jpg")

   def checkSyntax(self):
      data = open("file.txt", "r")
      for i in range(3):
         if NOT (data.readline().isnumeric()):
            return false

      for line in data:
         if NOT (data.readline().isnumeric()):
            return false
         if NOT (data.readline().isnumeric()):
            return false
         data.readline()
      
      return true;
      
   def javaExec(self):
      # Up to redoing depending on path
      #os.chdir("../pathConversion/")
      subprocess.call(["java", "-cp", "../pathConversion/rxtx-2.1-7-bins-r2/*:../pathConversion/.", "FileParser", "../piGUI/file.txt"])
      print("DONE!")

class badFileSyntax(tk.Frame):
   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Input file design is incorrect, please try again.", font=LARGE_FONT)
      label.pack(pady=10,padx=10)

      button2 = tk.Button(self, text="End",width = 18, height = 3,font=("Verdana", 20),
         command=lambda: controller.show_frame(Start))
      button2.pack()

class VerifyDesign(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      #label = tk.Label(self, text="Verify Design", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)

   def updateImage(self, controller):
      # display image design
      photo = ImageTk.PhotoImage(Image.open("resize.jpg"))
      label = Label(self, image = photo)
      label.image = photo
      label.pack()

      button1 = tk.Button(self, text="Design is correct",
         command=lambda: controller.show_frame(VerifyParts))
      button1.pack()

      button2 = tk.Button(self, text="Design is not correct",
         command=lambda: controller.show_frame(Redesign))
      button2.pack()


class Redesign(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Please redesign and start over", font=LARGE_FONT)
      label.pack(pady=10,padx=10)

      button1 = tk.Button(self, text="End",
         command=lambda: controller.show_frame(Start))
      button1.pack()


class VerifyParts(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Verify Parts", font=LARGE_FONT)
      label.pack(pady=10,padx=10)
      

      button1 = tk.Button(self, text="Parts are correct",
         command=lambda: controller.show_frame(Machine))
      button1.pack()

      button2 = tk.Button(self, text="Parts are not correct",
         command=lambda: controller.show_frame(Reverify))
      button2.pack()

   def readFile(self):
      data = open("file.txt", "r")
      line = data.readline()
      label1 = tk.Label(self, text=("Width of Board: " + line), font=LARGE_FONT)
      label1.pack(pady=3,padx=10)
      print "read line %s" % (line)
      line = data.readline()
      label2 = tk.Label(self, text=("Height of Board: " + line), font=LARGE_FONT)
      label2.pack(pady=3,padx=10)
      print "read line %s" % (line)
      line = data.readline()
      label3 = tk.Label(self, text=("Letter height: " + line), font=LARGE_FONT)
      label3.pack(pady=3,padx=10)
      print "read line %s" % (line)
      data.close()

class Reverify(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Please adjust your parts and start over", font=LARGE_FONT)
      label.pack(pady=10,padx=10)

      button1 = tk.Button(self, text="End",
         command=lambda: controller.show_frame(Start))
      button1.pack()

class Machine(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Machining will start when you are ready!", font=LARGE_FONT)
      label.pack(pady=10,padx=10)

      button1 = tk.Button(self, text="Start Machining",
         command=lambda: controller.show_frame(Machine))
      button1.pack()

      button2 = tk.Button(self, text="End",
         command=lambda: controller.show_frame(Start))
      button2.pack()

      button3 = tk.Button(self, text="Quit", fg="red", command = self.quit)
      button3.pack()

app = GUI()
app.title("Forest Friends")
app.geometry("320x240")
app.mainloop()
