from PIL import Image
import sys

img = Image.new('RGB', (4096, 1024))
p = img.load()

with open(sys.argv[1]) as f:
    i = 0
    alert = False
    for l in f:
        n = int(l)
        i = i+1
        if n == 0:
            for j in range (1024):
                p[i,j] = (40,36,37)
        if n == 1:
            for j in range (1024):
                p[i,j] = (255,0,0)
        if n == 2:
            for j in range (1024):
                p[i,j] = (0,255,0)
        if n == 3:
            alert = True
            for j in range (1024):
                p[i,j] = (0,0,255)
        if i == 4095: break
if alert:
    img.save(sys.argv[1] + "ALERT.png", "PNG")
else:
    img.save(sys.argv[1] + ".png", "PNG")
