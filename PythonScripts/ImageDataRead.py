from PIL import Image
import re
import pandas as pd


img = Image.open(r"C:\Users\schultheisz\Creative Cloud Files\ThrusterBounds.png")
imgWidth, imgHeight = img.size
img = img.convert("RGBA")
imgdata = img.getdata()

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