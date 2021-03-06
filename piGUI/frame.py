import subprocess
from Tkinter import *
import Tkinter as tk
#import tkFileDialog
from tkFileDialog import askopenfilename
import shutil
from PIL import Image, ImageTk
import os
import coordParsing
#import serialFudgerTester as coordParsing

LARGE_FONT= ("Verdana", 15)
CONFIRM_FONT= ("Verdana", 14)
thick = 0
height = 0
width = 0
letterHeight = 0
frames = {}

GRIDMODE = True

class GUI(tk.Tk):

   def __init__(self, *args, **kwargs):

      global frames
      Tk.__init__(self, *args, **kwargs)
      container = Frame(self)

      #container.pack(side="top", fill="both", expand = True)
      container.pack(fill="both", expand = True)
      #container.grid(row=0, column=0)

      container.grid_rowconfigure(0, weight=1)
      container.grid_columnconfigure(0, weight=1)


      for F in (Start, ImportFile, badFileSyntax, VerifyDesign, Redesign, VerifyParts, Reverify, Machine, Routing, Finish):
         frame = F(container, self)
         frames[F] = frame
         frame.grid(row=0, column=0, sticky="nsew")

      if (len(sys.argv) > 1):
         self.show_frame(ImportFile)
      else:
         self.show_frame(Start)

   def show_frame(self, cont):

      frame = frames[cont]
      frame.tkraise()


class Start(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self,parent)
      self.controller = controller
      #label = tk.Label(self, text="Start Page", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)
      self.rowconfigure(0, weight=1)
      self.columnconfigure(0, weight=1)

      start = Button(self, bg="#80FF80", activebackground="#00FF00", text="Start...", font=LARGE_FONT,
         command=lambda: controller.show_frame(ImportFile))
      start.grid(sticky=N+S+E+W)
      #start.pack(fill=BOTH)

      #button2 = tk.Button(self, text="Cancel",
      #   command=lambda: controller.show_frame(Start))
      #button2.pack()

class ImportFile(tk.Frame):
      
   def __init__(self, parent, controller):
      self.cont = controller
      tk.Frame.__init__(self,parent)
      self.rowconfigure(0, weight=1)
      self.rowconfigure(1, weight=1)
      self.columnconfigure(0, weight=1)
      importDialog = tk.Button(self, text="Import File", height = 5,font=LARGE_FONT, bg="#80FF80", activebackground="#00FF00",
         command=self.openFileWithFileDialog)
         #command=lambda: controller.show_frame(VerifyDesign))
      importDialog.grid(row=0, sticky=N+S+E+W)

      cancel = tk.Button(self, text="Cancel", height = 5,font=LARGE_FONT, bg="#FF8080", activebackground="#FF0000",
         #command=restartFrame)
         command=lambda: controller.show_frame(Start))
      cancel.grid(row=1, sticky=N+S+E+W)

   def openFileWithFileDialog(self):
      filename = askopenfilename()
      self.openFile(filename)

   def openFile(self, filename):
      print("importing file")
      #filename = tkinter.filedialog.askopenfilename(parenimaget=self.root)
      #filename = askopenfilename().overridedirect(1)
      app.withdraw()
      print(filename)
      app.deiconify()
      shutil.copyfile(filename, "file.txt") 
      #frames[VerifyParts].readFile()
      if filename: 
         print("New Frame: Board Design")
         # call java fuction
         self.javaExec()
         self.resize()
         self.cont.show_frame(VerifyDesign)

   def chooseFile(self):
      filename = askopenfilename(filetypes=[('cpff files', '.cpff')])
      return filename

   def resize(self):
      img = Image.open("original.jpg")

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
      frames[VerifyDesign].updateImage()
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
      
   def getErrorCodes(self):
      errorCodes = {}
      fp = open("../pathConversion/ERROR_CODES.txt", "r")
      for l in fp:
         pieces = l.split(",")
         errorCodes[int(pieces[0])] = pieces[1]
      return errorCodes

   def javaExec(self):
      # Up to redoing depending on path
      errorCodes = self.getErrorCodes()
      retCode = subprocess.call(["java", "-cp", "../pathConversion/", "FileParser", "../piGUI/file.txt"])
      #retCode = subprocess.call("java -cp ../pathConversion/rxtx-2.1-7-bins-r2/*:../pathConversion/ FileParser ../piGUI/file.txt", shell=True)
      if retCode != 0:
         errorMessage = errorCodes[retCode]
         print(errorMessage)
         sys.exit(-1)
      print("DONE!")

class badFileSyntax(tk.Frame):   
   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      self.controller = controller
      #label = tk.Label(self, text="Input file design is incorrect,", font=LARGE_FONT)
      #label.pack(pady=1,padx=10)
      #label = tk.Label(self, text="please try again.", font=LARGE_FONT)
      #label.pack(pady=1,padx=10)

      button2 = tk.Button(self, text="Input file design is incorrect, \n please try again.", height = 10,font=LARGE_FONT,
         #command=restartFrame)
         command=lambda: controller.show_frame(Start))
      button2.pack(fill=BOTH)

class VerifyDesign(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      self.cont = controller
      #label = tk.Label(self, text="Verify Design", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)
      self.rowconfigure(0, weight=1)
      self.rowconfigure(1, weight=1)
      self.columnconfigure(0, weight=1)
      self.columnconfigure(1, weight=1)
      correctButton = tk.Button(self, text="Correct", width = 12, font=CONFIRM_FONT, bg="#80FF80", activebackground="#00FF00",
         #command=lambda: controller.show_frame(VerifyParts))
         command=self.verifyParts)
      #button1.pack(side=LEFT, anchor=S)
      #correctButton.pack(side=LEFT, anchor=S)
      correctButton.grid(row=1, column=0, sticky=N+S+E+W)

      incorrectButton = tk.Button(self, text="Incorrect", width = 14, font=CONFIRM_FONT, bg="#FF8080", activebackground="#FF0000",
         command=lambda: self.cont.show_frame(Redesign))
      #button2.pack(side=LEFT, anchor=S)
      #incorrectButton.pack(side=RIGHT,anchor=S)
      incorrectButton.grid(row=1, column=1, sticky=N+S+E+W)
      self.label = Label(self, image=None)

      #label.pack(side=TOP)
      self.label.grid(row=0, column=0, columnspan=2)

   def verifyParts(self):
      frames[VerifyParts].readFile()
      self.cont.show_frame(VerifyParts)

   def updateImage(self):
      # display image design
      photo = ImageTk.PhotoImage(Image.open("resize.jpg"))
      #self.label = Label(self, image = photo)
      self.label.config(image = photo)
      self.label.image = photo
      self.update()
      #self.label.image = photo
      #label.pack(side=TOP)
      #self.label.grid(row=0, column=0, columnspan=2)

      #label = tk.Label(self, text="Design is..", font=LARGE_FONT)
      #label.pack(side=LEFT)


class Redesign(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      #label = tk.Label(self, text="Please redesign", font=LARGE_FONT)
      #label.pack(pady=2, fill=X)
      #label = tk.Label(self, text="and start over", font=LARGE_FONT)
      #label.pack(pady=2, fill=X)

      button1 = tk.Button(self, text="Please redesign and start over.", height = 10, font=LARGE_FONT,
         #command=restartFrame)
         command=lambda: controller.show_frame(Start))
      button1.pack(fill=BOTH)


class VerifyParts(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      self.rowconfigure(0, weight=1)
      self.rowconfigure(1, weight=1)
      self.rowconfigure(2, weight=1)
      self.rowconfigure(3, weight=1)
      self.rowconfigure(4, weight=1)
      self.rowconfigure(5, weight=1)
      self.columnconfigure(0, weight=1)
      self.columnconfigure(1, weight=1)
      #label = tk.Label(self, text="Verify Parts", font=LARGE_FONT)
      #label.pack(pady=10,padx=10)
      
      #label = tk.Label(self, text="Design is..", font=LARGE_FONT)
      #label.pack(side=LEFT)
      button1 = tk.Button(self, text="Correct", width = 12, height = 1, font=CONFIRM_FONT, bg="#80FF80", activebackground="#00FF00",
         command=lambda: controller.show_frame(Machine))
      #button1.pack(anchor=S, side=LEFT)
      button1.grid(row=5, column=0, sticky=N+S+E+W)

      button2 = tk.Button(self, text="Incorrect", width = 14, height = 1, font=CONFIRM_FONT, bg="#FF8080", activebackground="#FF0000",
         command=lambda: controller.show_frame(Reverify))
      #button2.pack(anchor=S, side=RIGHT)
      button2.grid(row=5, column=1, sticky=N+S+E+W)

   def readFile(self):
      global thick
      global height
      global width
      global letterHeight
      data = open("file.txt", "r")
      line = data.readline()
      thick = int(line)
      if int(line) == 1:
         line = str("0.75")
      else:
         if int(line) == 2:
            line = str("1.5")
      print("Thickness %s" % line)
      label1 = tk.Label(self, text=("Board Thickness: " + line), font=LARGE_FONT)
      
      #label1.pack(side=TOP, anchor=W)
      label1.grid(row=0, columnspan=2, sticky=E+W+N+S)

      #print "read line %s" % (line)
      width = data.readline()
      label2 = tk.Label(self, text=("Width of Board: " + width), font=LARGE_FONT)
      #label2.pack(side=TOP, anchor=W)
      label2.grid(row=1, columnspan=2, sticky=E+W+N+S)
      #label2.pack(anchor=N)#, compound=TOP)
      #print "read line %s" % (line)
      height = data.readline()
      label3 = tk.Label(self, text=("Height of Board: " + height), font=LARGE_FONT)
      label3.grid(row=2, columnspan=2, sticky=E+W+N+S)
      #label3.pack(anchor=N, compound=side)
      #print "read line %s" % (line)
      letterHeight = data.readline()
      label4 = tk.Label(self, text=("Letter Height: " + letterHeight), font=LARGE_FONT)
      label4.grid(row=3, columnspan=2, sticky=E+W+N+S)
      #label4 = tk.Label(self, text=("Router Bit Size: " + line), font=LARGE_FONT)
      #label4.pack(anchor=N)
      #print "read line %s" % (line)
      print "data read"
      data.close()

class Reverify(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      #label = tk.Label(self, text="Please adjust parts", font=LARGE_FONT)
      #label.pack(pady=2,padx=10)
      #label = tk.Label(self, text="and start over", font=LARGE_FONT)
      #label.pack(pady=2,padx=10)

      button1 = tk.Button(self, text="Please adjust parts \nand start over.", font=LARGE_FONT, height = 10,
         #command=restartFrame)
         command=lambda: controller.show_frame(Start))
      button1.pack(fill = BOTH)

class Machine(tk.Frame):

   def startMachining(self):
      self.cont.show_frame(Routing)
      frames[Routing].start()

   def __init__(self, parent, controller):
      self.cont = controller
      tk.Frame.__init__(self, parent)
      self.rowconfigure(0, weight=1)
      self.rowconfigure(1, weight=1)
      self.columnconfigure(0, weight=1)
      #label = tk.Label(self, text="Machining will start", font=LARGE_FONT)
      #label.pack(pady=2,padx=10)
      #label = tk.Label(self, text="when you are ready!", font=LARGE_FONT)
      #label.pack(pady=2,padx=10)

      button1 = tk.Button(self, text="Start Machining", font=LARGE_FONT, height= 5, bg="#80FF80", activebackground="#00FF00",
         #command=self.loadCoordPars)
         command=self.startMachining)
      #button1.pack(anchor=S, fill=X)
      button1.grid(row=0, sticky="nswe")

      button2 = tk.Button(self, text="Cancel", font=LARGE_FONT,height = 5, bg="#FF8080", activebackground="#FF0000",
         #command=restartFrame)
         command=lambda: controller.show_frame(Start))
      #button2.pack(anchor=S, fill=X)
      button2.grid(row=1, sticky="nswe")

      #button3 = tk.Button(self, text="Quit", fg="red",  font=LARGE_FONT, command = self.quit)
      #button3.pack(fill=X)

   def loadCoordPars(self) :
      self.cont.show_frame(Finish)
      coordParsing.coordParsing(thick)
   
class Routing(tk.Frame):

   def start(self):
      self.painter = subprocess.Popen(["java", "-cp", "../pathConversion", "Painter", str(width), str(height), str(letterHeight)], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
      self.step = 1
      self.totalSteps = coordParsing.getNumberOfSteps() + 2
      coordParsing.setCallback(self.labelCallback)
      coordParsing.coordParsing(thick)
      self.cont.show_frame(Finish)

   def labelCallback(self, x, y, z, message):
      self.painter.stdin.write("{0} {1} {2}\n".format(int(x) / 64, int(y) / 64, int(z)))
      self.xTarget.config(text="X Target: %s" % str(x))
      self.yTarget.config(text="Y Target: %s" % str(y))
      self.zTarget.config(text="Z Target: %s" % str(z))
      self.steps.config(text="Step: {0} / {1}".format(self.step, self.totalSteps))
      self.message.config(text=message)
      self.painter.stdout.readline()
      photo = ImageTk.PhotoImage(Image.open("original.jpg"))
      self.preview.config(image=photo)

      self.update()
      self.step += 1

   def __init__(self, parent, controller):
      self.cont = controller
      tk.Frame.__init__(self, parent)
      self.rowconfigure(0, weight=1)
      self.rowconfigure(1, weight=1)
      self.rowconfigure(2, weight=1)
      self.rowconfigure(3, weight=1)
      self.rowconfigure(4, weight=1)
      self.rowconfigure(5, weight=1)
      self.step = 0
      self.totalSteps = 0
      self.columnconfigure(0, weight = 1)
      self.xTarget = tk.Label(self, text=("X Target: "), font=LARGE_FONT)
      self.xTarget.grid(row=0, sticky=E+W+N+S)

      self.yTarget = tk.Label(self, text=("Y Target: "), font=LARGE_FONT)
      self.yTarget.grid(row=1, sticky=E+W+N+S)

      self.zTarget = tk.Label(self, text=("Z Target: "), font=LARGE_FONT)
      self.zTarget.grid(row=2, sticky=E+W+N+S)

      self.steps = tk.Label(self, text=("Step: {0} / {1}".format(self.step, self.totalSteps)), font=LARGE_FONT)
      self.steps.grid(row=3, sticky=E+W+N+S)

      self.message = tk.Label(self, text="", font=LARGE_FONT)
      self.message.grid(row=5, sticky=E+W+N+S)

      # display image design
      importedImage = Image.open("original.jpg")
      photo = ImageTk.PhotoImage(importedImage)
      self.preview = Label(self, image=photo)
      self.preview.grid(row=4)

class Finish(tk.Frame):

   def __init__(self, parent, controller):
      tk.Frame.__init__(self, parent)
      self.columnconfigure(0, weight=1)
      self.rowconfigure(0, weight=1)
      button = tk.Button(self, text="Machining is finished. \nMachine more boards.", font=LARGE_FONT, bg="#80FF80", activebackground="#00FF00",
         command=lambda: controller.show_frame(Start))
         #command=restartFrame)
      #button.pack(fill=BOTH)
      button.grid(sticky="nswe")

app = GUI()
#app.lower()
#app.overrideredirect(True)
app.title("Forest Friends")
app.geometry("640x480")
#app.geometry("320x240")
#app.geometry("{0}x{1}+0+0".format(app.winfo_screenwidth(), app.winfo_screenheight()))
app.focus_set()
#app.bind("<Escape>", lambda e: app.quit())
if (len(sys.argv) > 1):
   frames[ImportFile].openFile(sys.argv[1])
app.mainloop()

