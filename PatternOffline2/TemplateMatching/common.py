import matplotlib.patches as patches
import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
import cv2

def get_img_arrays(test_image, ref_image):
    test = cv2.imread(test_image, cv2.IMREAD_GRAYSCALE) / 255.0
    ref = cv2.imread(ref_image, cv2.IMREAD_GRAYSCALE)   /255.0
    return test, ref

def get_img_arrays_int(test_image, ref_image):
    test = cv2.imread(test_image, cv2.IMREAD_GRAYSCALE)
    ref = cv2.imread(ref_image, cv2.IMREAD_GRAYSCALE)
    return test, ref


def cost(test, ref, i, j):
    temp = test[i:i+ref.shape[0], j:j+ref.shape[1]]
    #print(j, i)
    return np.sum(np.square(temp - ref))


def show_img(test_image,ref_image, coord): # coord = (x, y)
    test = np.array(Image.open(test_image), dtype=np.uint8)
    ref = np.array(Image.open(ref_image), dtype=np.uint8)
    # Create figure and axes
    fig,ax = plt.subplots(1,3, gridspec_kw = {'width_ratios':[ref.shape[1], test.shape[1],test.shape[1]]})
    ax[0].set_title('Ref. image')
    ax[1].set_title('Test image')
    ax[2].set_title('Coord. in Test image')
    # Display the image
    ax[0].imshow(ref) #, shape=imarray.shape[:2])
    ax[1].imshow(test) #, shape=imarray2.shape[:2])
    ax[2].imshow(test)
    # Create a Rectangle patch
    rect = patches.Rectangle( coord,ref.shape[1],ref.shape[0],linewidth=2,edgecolor='r',facecolor='none')
    # Add the patch to the Axes
    ax[2].add_patch(rect)
    plt.show()