#include <stdio.h>
#include <assert.h>

#include <octave/oct.h>
#include <octave/parse.h>
#include <octave/dMatrix.h>

#include <omp.h>

#define EIGHT_BITS 256
#define NUM_THREADS 12

DEFUN_DLD (hist_omp, args,, "Compute histogram of given (image) matrix")
{
    octave_value_list result;
    octave_idx_type im_width, im_height;
    dim_vector image_dims;
    dim_vector result_dims(NUM_THREADS, EIGHT_BITS);
    Matrix image;
    Matrix result_mat(result_dims);
    unsigned int total_px;
    unsigned int part;
    // int hist_arr[NUM_THREADS][EIGHT_BITS];

    omp_set_num_threads(NUM_THREADS);

    image = args(0).matrix_value();
    
    assert(image.ndims() != 1 && "Input matrix's dims must equals 1!");

    image_dims = image.dims();
    im_width = image_dims.elem(0);
    im_height = image_dims.elem(1);
    total_px = im_width * im_height;
    part = total_px / NUM_THREADS;

    int ii;
    #pragma omp parallel for schedule(static, NUM_THREADS)
    for (ii = 0; ii < NUM_THREADS; ii++)
    {
        // device code
        // calc working space within this threads
        int start, end;
        int px_idx;
        int px_x, px_y;
        int px_value;
        start = part * ii;
        end = start + part;

        // the last thread has to do more works than others
        if (ii == NUM_THREADS - 1)
            end = total_px;

        for (px_idx = 0; px_idx < end; px_idx++)
        {
            px_x = px_idx / im_width;
            px_y = px_idx - ((px_x * im_width) + 1);
            px_value = image(px_x, px_y);
            // hist_arr[thread_idx][px_value] += 1;
            result_mat(ii, px_value) = result_mat(ii, px_value) + 1;
        }

    }

    result(0) = result_mat;

    return(result);
}
