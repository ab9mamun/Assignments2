import cv2
import numpy as np
import matplotlib.patches as patches
import matplotlib.pyplot as plt
import time
from PIL import Image



def show_img(test_image,ref_image, coord, h, w): # coord = (x, y)
    imarray = np.array(Image.open(test_image), dtype=np.uint8)
    imarray2 = np.array(Image.open(ref_image), dtype=np.uint8)

    size1 = imarray.shape
    size2 = imarray2.shape
    # Create figure and axes
    fig,ax = plt.subplots(1,3, gridspec_kw = {'width_ratios':[size2[1], size1[1],size1[1]]})


    # Display the image
    ax[0].imshow(imarray2) #, shape=imarray.shape[:2])
    ax[1].imshow(imarray) #, shape=imarray2.shape[:2])
    ax[2].imshow(imarray)

    # Create a Rectangle patch
    rect = patches.Rectangle( coord,w,h,linewidth=2,edgecolor='r',facecolor='none')
   # rect2 = patches.Rectangle((0,0), size1[0], size1[1],linewidth=0.1,edgecolor='w',facecolor='none' )

    # Add the patch to the Axes
    ax[2].add_patch(rect)
    #ax[1].add_patch(rect2)

    plt.show()


def find_d(test, ref, i, j, ref_h, ref_w):
    temp = test[i:i+ref_h, j:j+ref_w]
   # print(temp.shape, ref.shape)
    d = np.sum(np.square(temp - ref))
    return d


def run(test_image, ref_image):
    test_orig = cv2.imread(test_image, cv2.IMREAD_GRAYSCALE)
    ref_orig = cv2.imread(ref_image, cv2.IMREAD_GRAYSCALE)

    test = test_orig / 255.0
    ref = ref_orig / 255.0

    test_h, test_w = test.shape[:2]
    ref_h, ref_w = ref.shape[:2]
    #  print(test_h, test_w, ref_h, ref_w)
    # print(test)

    coord = (0, 0)
    d = float('inf')
    d_cur = None
    st = time.time()
    then = st
    max_h = test_h - ref_h
    max_w = test_w - ref_w

    for i in range(0, max_h + 1):  # from 0 to test_h - ref_h

        for j in range(0, max_w + 1):  # from 0 to test_w - ref_w

            if (i * max_w + j) % 1000 == 0:
                now = time.time()
                if now - then > 1:
                    print('Scanning in row', i, 'col', j, '... d=', d)
                    then = now


            # print('Scanning in row', i,'col', j, '... d = ', d)
            d_cur = find_d(test, ref, i, j, ref_h, ref_w)
            if d_cur < d:
                d = d_cur
                coord = (j, i)
            if d == 0:
                break
        if d == 0:
            break

    et = time.time()

    print('Ans:', coord, "Time taken:{} seconds".format(et - st))
    show_img(test_image, ref_image, coord, ref_h, ref_w)







def main():
    run('baby.jpg', 'babyr.jpg')

if __name__ == '__main__':
    main()