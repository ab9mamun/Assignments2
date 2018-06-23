import cv2
import numpy as np
import matplotlib.patches as patches
import matplotlib.pyplot as plt
import time
from PIL import Image



def show_img(test_image,ref_image, coord, h, w):
    imarray = np.array(Image.open(test_image), dtype=np.uint8)
    imarray2 = np.array(Image.open(ref_image), dtype=np.uint8)

    # Create figure and axes
    fig,ax = plt.subplots(1,2)

    # Display the image
    ax[0].imshow(imarray)
    ax[1].imshow(imarray2)
    # Create a Rectangle patch
    rect = patches.Rectangle( coord,h,w,linewidth=1,edgecolor='r',facecolor='none')

    # Add the patch to the Axes
    ax[0].add_patch(rect)
    plt.show()


def find_d(test, ref, i, j, ref_h, ref_w):
    d = 0
    for height in range(ref_h):
        for width in range(ref_w):
            for channel in range(3):
                d= d+ (test[height+i][width+j][channel] - ref[height][width][channel])**2

    return d


def main():

    test_image = 'mamun.jpg'
    ref_image = 'mamun_cropped.jpg'

    test_orig = cv2.imread(test_image)
    ref_orig = cv2.imread(ref_image)

    test = test_orig / 255.0
    ref = ref_orig / 255.0

    test_h, test_w = test.shape[:2]
    ref_h, ref_w = ref.shape[:2]
  #  print(test_h, test_w, ref_h, ref_w)
   # print(test)

    coord = (0,0)
    d = float('inf')
    d_cur = None
    st = time.time()
    for i in range (0, test_h-ref_h+1):  # from 0 to test_h - ref_h
        #if (i %5 == 0):
            #print('Scanning in row', i, '...')
        for j in range(0, test_w - ref_w+1):  # from 0 to test_w - ref_w
            print('Scanning in row', i,'col', j, '... d = ', d)
            d_cur = find_d(test, ref, i, j, ref_h, ref_w)
            if d_cur < d:
                d = d_cur
                coord = (i,j)
            if d == 0:
                break
        if d == 0:
            break

    et = time.time()

    print('Ans:', coord, "Time taken:{} seconds".format(et - st))
    show_img(test_image, ref_image, coord, ref_h, ref_w)





if __name__ == '__main__':
    main()