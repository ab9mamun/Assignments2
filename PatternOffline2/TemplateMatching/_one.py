import numpy as np
import time
from common import show_img, cost, get_img_arrays, get_img_arrays_int

def find_template_exhaustive(test, ref):
    st = time.time()
    then = st
    d_thresh = ref.shape[0] * ref.shape[1] * 0.001
    #Each pixel was an integer from 0 to 255 before we normalized them. So minimum difference between
    #two pixels is 1/255 = 0.0039.. becomes 0.000015... if squared. And we are allowing average squared difference
    #of 0.001

    print('Acceptable cost value:', d_thresh)
    coord = (0, 0)
    d = float('inf')
    max_h, max_w =  test.shape[0]- ref.shape[0], test.shape[1] - ref.shape[1]

    after_print = 0
    for i in range(0, max_h + 1):  # from 0 to test_h - ref_h
        #log---------
        if after_print > 1000:
            now = time.time()
            if now - then > 1:
                print('Scanning in row', i, '... d=', d)
                then = now
                after_print = 0
        ###logend-----

        for j in range(0, max_w + 1):  # from 0 to test_w - ref_w
            d_cur = cost(test, ref, i, j)
            if d_cur < d:
                d = d_cur
                coord = (j, i)
                if d<d_thresh:
                    return coord, False, time.time() - st
        after_print += max_w

    return coord, True, time.time() - st

def run(test_image, ref_image):
    test, ref = get_img_arrays(test_image, ref_image)

    coord , completed,  time_diff = find_template_exhaustive(test, ref)
    print('Program ended', completed * 'normally.', (not completed) * 'at accepted cost.')
    print('Ans:', coord, "Time taken: {} seconds".format(time_diff))
    show_img(test_image, ref_image, coord)

def main():
    run('baby.jpg', 'babyr.jpg')

if __name__ == '__main__':
    main()