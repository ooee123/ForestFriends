import tkFileDialog
from Tkinter import *
from PIL import Image
import os

root= Tk()

def resizeIt():
   filename = tkFileDialog.askopenfilename()
   basewidth = 300
   img = Image.open(filename)
   wpercent = (basewidth / float(img.size[0]))
   hsize = int((float(img.size[1]) * float(wpercent)))
   img = img.resize((basewidth, hsize), Image.ANTIALIAS)
   img.save('resize.jpg')
   os.remove(filename) # deletes the original image after you have got the resized image


Button(text='add image', command=resizeIt).pack()

root.mainloop()
