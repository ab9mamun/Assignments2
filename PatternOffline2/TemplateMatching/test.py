import cv2
import numpy as np
import scipy
from common import show_img, cost, get_img_arrays, get_img_arrays_int

d = float('inf')

test, ref = get_img_arrays('mamun2.jpg', 'mamun2r.jpg')
test2, ref2 = get_img_arrays_int('mamun2.jpg', 'mamun2r.jpg')

#ref = scipy.misc.imresize(test, 0.5)
ref = cv2.resize(test,None, fx=0.5, fy=0.5)
ref2 = cv2.resize(test2, None, fx=0.5, fy=0.5)/255.0

print(ref-ref2)

coord = (1,2)
print(coord*2)
#print(ref2)
#print(cv2.__version__)
#print('Hello world')

