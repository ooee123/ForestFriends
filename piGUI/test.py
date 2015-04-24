from Tkinter import *
import Tkinter as tk
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
      filename = askopenfilename()
      print(filename)
      shutil.copyfile(filename, "file.txt") 
      if filename: 
         print("New Frame: Board Design")

class GUI:
   def __init__(self, *args, **kwargs):
      tk.Tk.__init__(self, *args, **kwargs)
      container = tk.Frame(self)
      container.pack(side = "top", fill="both", expant = True)
      container.grid_rowconfigure(0, weight = 1)
      container.grid_columnconfigure(0, weight = 1)

      self.frames = {}

      for F in (ImportFile, VerifyDesign, VerifyParts, Start):
         frame = F(container, self)
         self.frames[F] = frame
         frame.grid(row=0, column=0, sticky="nsew")

      self.show_frame(StartPage)

      def show_frame(self, cont):
         frame = self.frames[cont]
         frame.tkraise()


class ImportFile(tk.Frame):
   def __init__(self, parent, controller):
      label = tk.Label(self, text="Page One!!!", font=LARGE_FONT)
      label.pack(pady=10,padx=10)

   

class VerifyDesign(tk.Frame):
   def __init__(self, parent, controller):
      label = tk.Label(self, text="Page One!!!", font=LARGE_FONT)
      label.pack(pady=10,padx=10)


class VerifyParts(tk.Frame):
   def __init__(self, parent, controller):
      label = tk.Label(self, text="Page One!!!", font=LARGE_FONT)
      label.pack(pady=10,padx=10)


class Start(tk.Frame):
   def __init__(self, parent, controller):
      label = tk.Label(self, text="Page One!!!", font=LARGE_FONT)
      label.pack(pady=10,padx=10)


#root = Tk()
#root.title("Raspberry Pi GUI")
#root.geometry("320x240")
#app = App(root)
app = GUI()
root.mainloop()
