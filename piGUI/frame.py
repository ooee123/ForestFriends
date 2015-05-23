import subprocess
from Tkinter import *
import Tkinter as tk
#import tkFileDialog
from tkFileDialog import askopenfilename
import shutil
from PIL import Image, ImageTk
import os

LARGE_FONT= ("Verdana", 15)

frames = {}

class GUI(tk.Tk):

   def __init__(self, *args, **kwargs):

      tk.Tk.__init__(self, *args, **kwargs)
      container = tk.Frame(self)

      container.pack(side="top", fill="both", expand = True)

      container.grid_rowconfigure(0, weight=1)
      container.grid_columnconfigure(0, weight=1)


      for F in (Start, ImportFile, badFileSyntax, VerifyDesign, Redesign, VerifyParts, Reverify, Machine, Machining, Cancel, Finish):
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

      button = tk.Button(self, bg="red", text="Start...", font=LARGE_FONT,height = 10, 
         command=lambda: controller.show_frame(ImportFile))
      button.pack(fill=BOTH)

      #button2 = tk.Button(self, text="End",
      #   command=lambda: controller.show_frame(Start))
      #button2.pack()


def restartFrame():
   os.execv("/usr/bin/python", ["python", "frame.py"])

class ImportFile(tk.Frame):
      
   def __init__(self, parent, controller):
      self.cont = controller
      tk.Frame.__init__(self,parent)
      #label = tk.Label(self, text="Import File", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)

      button = tk.Button(self, text="Import File", height = 5,font=LARGE_FONT,
         command=self.openFile)
         #command=lambda: controller.show_frame(VerifyDesign))
      button.pack(fill=X)

      button2 = tk.Button(self, text="End", height = 5,font=LARGE_FONT,
         command=restartFrame)
         #command=lambda: controller.show_frame(Start))
      button2.pack(fill=X)


   def openFile(self):
      print("importing file")
      #filename = tkinter.filedialog.askopenfilename(parenimaget=self.root)
      #filename = askopenfilename().overridedirect(1)
      app.withdraw()
      filename = askopenfilename(filetypes=[('cpff files', '.cpff')])
      print(filename)
      app.deiconify()
      shutil.copyfile(filename, "file.txt") 
      frames[VerifyParts].readFile()
      if filename: 
         print("New Frame: Board Design")
         # call java fuction
         self.javaExec()
         self.resize()
         self.cont.show_frame(VerifyDesign)

   def resize(self):
      img = Image.open("original.jpg")

      #print("image is opened")
      #print(str(img.size[0]) + " " + str( img.size[1]))
      ratio = img.size[0] / img.size[1]; 
      #print(str(ratio))
      if ratio >= 2:
         # width + golden ratio
         basewidth = 300
         wpercent = (basewidth / float (img.size[0]))
         hsize = int ((float (img.size[1]) * float (wpercent)))
         img = img.resize((basewidth, hsize), Image.ANTIALIAS)
      else:# if ratio < 2:
         # height
         basewidth = 150
         hpercent = (basewidth / float (img.size[1]))
         wsize = int ((float (img.size[0]) * float (hpercent)))
         img = img.resize((wsize, basewidth), Image.ANTIALIAS)

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
      #retCode = subprocess.call(["java", "-cp", "../pathConversion/rxtx-2.1-7-bins-r2/*:../pathConversion/.", "FileParser", "../piGUI/file.txt"])
      retCode = subprocess.call("java -cp ../pathConversion/rxtx-2.1-7-bins-r2/*:../pathConversion/ FileParser ../piGUI/file.txt", shell=True)
      print("DONE!")

class badFileSyntax(tk.Frame):   
   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Input file design is incorrect,", font=LARGE_FONT)
      label.pack(pady=1,padx=10)
      label = tk.Label(self, text="please try again.", font=LARGE_FONT)
      label.pack(pady=1,padx=10)

      button2 = tk.Button(self, text="End", height = 3,font=LARGE_FONT,
         command=restartFrame)
         #command=lambda: controller.show_frame(Start))
      button2.pack(fill=X, anchor=S)

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

      #label = tk.Label(self, text="Design is..", font=LARGE_FONT)
      #label.pack(side=LEFT)
      button1 = tk.Button(self, text="Correct Design", width = 12, font=LARGE_FONT,
         command=lambda: controller.show_frame(VerifyParts))
      button1.pack(side=LEFT, anchor=S)

      button2 = tk.Button(self, text="Incorrect Design", width = 14, font=LARGE_FONT,
         command=lambda: controller.show_frame(Redesign))
      button2.pack(side=LEFT, anchor=S)


class Redesign(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Please redesign", font=LARGE_FONT)
      label.pack(pady=2, fill=X)
      label = tk.Label(self, text="and start over", font=LARGE_FONT)
      label.pack(pady=2, fill=X)

      button1 = tk.Button(self, text="End", height = 2, font=LARGE_FONT,
         command=restartFrame)
         #command=lambda: controller.show_frame(Start))
      button1.pack(fill=X, anchor=S)


class VerifyParts(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      #label = tk.Label(self, text="Verify Parts", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)
      
      #label = tk.Label(self, text="Design is..", font=LARGE_FONT)
      #label.pack(side=LEFT)
      button1 = tk.Button(self, text="Correct Parts", width = 12, height = 1, font=LARGE_FONT,
         command=lambda: controller.show_frame(Machine))
      button1.pack(anchor=S, fill=X)

      button2 = tk.Button(self, text="Incorrect Parts", width = 14, height = 1, font=LARGE_FONT,
         command=lambda: controller.show_frame(Reverify))
      button2.pack(anchor=S, fill=X)

   def readFile(self):
      data = open("file.txt", "r")
      line = data.readline()
      label1 = tk.Label(self, text=("Width of Board: " + line), font=LARGE_FONT)
      label1.pack(anchor=S)
      #print "read line %s" % (line)
      line = data.readline()
      label2 = tk.Label(self, text=("Height of Board: " + line), font=LARGE_FONT)
      label2.pack(anchor=S)
      #print "read line %s" % (line)
      #line = data.readline()
      #label3 = tk.Label(self, text=("Board Thickness: " + line), font=LARGE_FONT)
      #label3.pack(anchor=S)
      #print "read line %s" % (line)
      line = data.readline()
      label4 = tk.Label(self, text=("Letter Height: " + line), font=LARGE_FONT)
      #label4 = tk.Label(self, text=("Router Bit Size: " + line), font=LARGE_FONT)
      label4.pack(anchor=S)
      #print "read line %s" % (line)
      print "data read"
      data.close()

class Reverify(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Please adjust parts", font=LARGE_FONT)
      label.pack(pady=2,padx=10)
      label = tk.Label(self, text="and start over", font=LARGE_FONT)
      label.pack(pady=2,padx=10)

      button1 = tk.Button(self, text="End", font=LARGE_FONT,
         command=restartFrame)
         #command=lambda: controller.show_frame(Start))
      button1.pack(anchor=S, fill=X)

class Machine(tk.Frame):

   def __init__(self, parent, controller):
      self.cont = controller
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Machining will start", font=LARGE_FONT)
      label.pack(pady=2,padx=10)
      label = tk.Label(self, text="when you are ready!", font=LARGE_FONT)
      label.pack(pady=2,padx=10)

      button1 = tk.Button(self, text="Start Machining", font=LARGE_FONT,
         command=self.loadCoordPars)
      button1.pack(anchor=S, fill=X)

      button2 = tk.Button(self, text="End", font=LARGE_FONT,
         command=restartFrame)
         #`command=lambda: controller.show_frame(Start))
      button2.pack(anchor=S, fill=X)

      #button3 = tk.Button(self, text="Quit", fg="red",  font=LARGE_FONT, command = self.quit)
      #button3.pack(fill=X)

   def loadCoordPars(self) :
      subprocess.call("python coordParsing.py", shell=True)
      self.cont.show_frame(Machining)

class Machining(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)

      label = tk.Label(self, text="In progress...", font=LARGE_FONT)
      label.pack(pady=10,padx=10)

      #button1 = tk.Button(self, text="Cancel Machining", font=LARGE_FONT,
         #command=lambda: controller.show_frame(Cancel))
         #command=cancelFlag)
      #button1.pack(fill=X)

         
      button2 = tk.Button(self, text="Pseudo Finish Button", font=LARGE_FONT,
         command=lambda: controller.show_frame(Finish))
      button2.pack(fill=X)

   

class Cancel(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Cancled", font=LARGE_FONT)
      label.pack(pady=10,padx=10)

      button1 = tk.Button(self, text="Return to Start", font=LARGE_FONT,
         command=restartFrame)
         #command=lambda: controller.show_frame(Start))
      button1.pack(fill=X)

   

class Finish(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      label = tk.Label(self, text="Machining is finished.", font=LARGE_FONT)
      label.pack(anchor=S)

      button1 = tk.Button(self, text="Machine more boards", font=LARGE_FONT,
         #command=lambda: controller.show_frame(Start))
         command=restartFrame)
      button1.pack(anchor=S)

app = GUI()
#app.lower()
app.overrideredirect(True)
app.title("Forest Friends")
app.geometry("320x240")
#app.geometry("{0}x{1}+0+0".format(app.winfo_screenwidth(), app.winfo_screenheight()))
app.focus_set()
app.bind("<Escape>", lambda e: app.quit())
app.mainloop()
