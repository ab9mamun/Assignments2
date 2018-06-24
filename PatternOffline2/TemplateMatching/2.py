import numpy as np
import time
from common import show_img, cost, get_img_arrays
import math

def logsearch(test, ref, search_area):
    d_thresh = ref.shape[0] * ref.shape[1] * 0.001
    # Each pixel was an integer from 0 to 255 before we normalized them. So minimum difference between
    # two pixels is 1/255 = 0.0039.. becomes 0.000015... if squared. And we are allowing average squared difference
    # of 0.001
    print('Acceptable cost value:', d_thresh)
    #print(search_area)

    # search width = p, height = q
    p = search_area[2] - search_area[0]
    q = search_area[3] - search_area[1]
    dwidth = 2**(math.floor(math.log2(p/2)) -1)
    dheight = 2 ** (math.floor(math.log2(q/2)) - 1)
    coord = center = (p//2, q//2)
    #print(dwidth, dheight)

    d = cost(test, ref, center[1], center[0])
    while dwidth >=1 and dheight >=1:
        for dx in [0,1,-1]:
            for dy in [0, 1, -1]:
                if dx ==0 and dy == 0:
                    continue

                j, i = center[0]+dx*dwidth, center[1] + dy*dheight
                d_cur = cost(test, ref, i, j)
                if d_cur<d:
                    d = d_cur
                    coord = (j, i)
                    if d < d_thresh:
                        return coord
        dwidth//=2
        dheight//=2
        center = coord

    return coord


def find_template_2dlog(test, ref):
    st = time.time()

    search_area = (0, 0, test.shape[1]- ref.shape[1]-1, test.shape[0] - ref.shape[0]-1) # top left and bottom right
    coord = logsearch(test, ref, search_area)
    return coord, time.time() - st

def run(test_image, ref_image):
    test, ref = get_img_arrays(test_image, ref_image)
    coord , time_diff = find_template_2dlog(test, ref)
    print('Ans:', coord, "Time taken:{} seconds".format(time_diff))
    show_img(test_image, ref_image, coord)

def main():
    run('mamun2.jpg', 'mamun2r.jpg')

if __name__ == '__main__':
    main()