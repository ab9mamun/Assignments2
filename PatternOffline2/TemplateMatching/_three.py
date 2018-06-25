import numpy as np
import cv2
import time
from common import show_img, cost, get_img_arrays, get_img_arrays_int
from _one import find_template_exhaustive

def heirarchical_recursive(test, ref, level):
    if level == 0:
        coord, _, _ = find_template_exhaustive(test/255.0, ref/255.0)
        return coord

    test2 = cv2.resize(test, None, fx=0.5, fy=0.5)
    ref2 = cv2.resize(ref, None, fx=0.5, fy=0.5)
    coord2 = heirarchical_recursive(test2, ref2, level - 1)
    coord = center = (coord2[0]*2, coord2[1]*2)
    #print(coord)
    search_area = (0, 0, test.shape[1] - ref.shape[1] - 1, test.shape[0] - ref.shape[0] - 1)  # top left and bottom right

    d = float('inf')
    for dx in [0, 1, -1]:
        for dy in [0, 1, -1]:
            j, i = center[0] + dx , center[1] + dy
            if j < search_area[0] or j > search_area[2] or i < search_area[1] or i > search_area[3]:
                continue
            # print(j, i, dwidth, dheight, d)
            d_cur = cost(test/255.0, ref/255.0, i, j)
            if d_cur < d:
                d = d_cur
                coord = (j, i)

    return coord

def find_template_heirarchical(test, ref, level):
    st = time.time()
    coord = heirarchical_recursive(test, ref, level)
    return coord, time.time() - st

def run(test_image, ref_image):
    test, ref = get_img_arrays_int(test_image, ref_image)

    coord , time_diff = find_template_heirarchical(test, ref, level = 4)
    print('Ans:', coord, "Time taken:{} seconds".format(time_diff))
    show_img(test_image, ref_image, coord)

def main():
    run('baby.jpg', 'babyr.jpg')

if __name__ == '__main__':
    main()