#include <stdbool.h>
#pragma GCC optimize("Ofast")

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    char _;
    int sum;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    int _; // 1 if cached 0 otherwize.
    // int num;
} pixel_sum;


/* Compute min and max of two integers, respectively */
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
#define sum_pixels_by_weight(sum, p, weight) {\
    register int w = weight;\
    sum.red += ((int) p.red) * w;\
    sum.green += ((int) p.green) * w;\
    sum.blue += ((int) p.blue) * w;\
}

#define getsum(sum, cache, backupOrg, index) {\
    sum = cache[index];\
    if (!sum._) {\
        pixel a = backupOrg[index - m];\
        pixel b = backupOrg[index];\
        pixel c = backupOrg[index + m];\
        sum.red = (int) a.red + (int) b.red + (int) c.red;\
        sum.green = (int) a.green + (int) b.green + (int) c.green;\
        sum.blue = (int) a.blue + (int) b.blue + (int) c.blue;\
        ++sum._;\
        cache[index] = sum;\
    }\
}

#define FILTER { \
    register int min_intensity = 766; \
    register int max_intensity = -1; \
    register pixel min_pixel, max_pixel, current; \
    register int i = id, t; \
    t = i; \
    current = backupOrg[t]; \
     \
    min_intensity = max_intensity = current.sum; \
    min_pixel = max_pixel = current; \
    ++ \
    t; \
    current = backupOrg[t]; \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    ++ \
    t; \
    current = backupOrg[t]; \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    i += \
    m; \
    t = i; \
    current = backupOrg[t]; \
     \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    ++ \
    t; \
    current = backupOrg[t]; \
     \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    ++ \
    t; \
    current = backupOrg[t]; \
     \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    i += \
    m; \
    t = i; \
    current = backupOrg[t]; \
     \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    ++ \
    t; \
    current = backupOrg[t]; \
     \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    ++ \
    t; \
    current = backupOrg[t]; \
     \
     \
    if (current.sum <= min_intensity) { \
    min_intensity = current.sum; \
    min_pixel = current; \
    } else if (current.sum > max_intensity) { \
    max_intensity = current.sum; \
    max_pixel = current; \
    } \
    sum_pixels_by_weight(sum, min_pixel, -1); \
    sum_pixels_by_weight(sum, max_pixel, -1);                 \
}

__attribute__((always_inline)) inline static void doConvolution(Image *image, int kernelScale, bool filter) {
    pixel_sum *sums = calloc(m * n, sizeof(pixel_sum)); // sums[i][j] = [i - 1][j] + [i][j] + [i + 1][j]
    pixel *backupOrg = malloc(m * n * sizeof(pixel));

    Image img = *image;
    register bool flag = (kernelScale == 7) || (kernelScale == 9);

    {
        if (filter) {
            register int index, temp, index3;
            for (index = 0, temp = m * n, index3 = 0; index < temp; ++index) {
                backupOrg[index] = *((pixel *) &img.data[index3]);
                pixel p = backupOrg[index];
                p.sum = (int) p.red + (int) p.green + (int) p.blue;
                backupOrg[index] = p;
                index3 += 3;
            }
            if (flag) {
                register int temp = m * (n - 1) - 1, index, id = 0;
                for (index = m + 1; index < temp; ++index, ++id) {
                    pixel_sum sum = {0, 0, 0};
                    register int index_1 = index - 1;

                    pixel_sum tempsum;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, 1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, 1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, 1);

                    FILTER

                    // assign kernel's result to pixel at [i,j]
                    pixel current_pixel;
                    int red = sum.red / kernelScale, green = sum.green / kernelScale, blue = sum.blue / kernelScale;
                    red = max(red, 0), green = max(green, 0), blue = max(blue, 0);
                    current_pixel.red = (unsigned char) min(red, 255);
                    current_pixel.green = (unsigned char) min(green, 255);
                    current_pixel.blue = (unsigned char) min(blue, 255);
                    memcpy(&img.data[(index << 1) + index], &current_pixel, 3);
                }
            } else {
                register int temp = m * (n - 1) - 1, index, id = 0;
                for (index = m + 1; index < temp; ++index, ++id) {
                    pixel_sum sum = {0, 0, 0};
                    register int index_1 = index - 1;
                    pixel_sum tempsum;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, -1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, -1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, -1);
                    pixel p = backupOrg[index];
                    sum_pixels_by_weight(sum, p, 10);

                    FILTER

                    // assign kernel's result to pixel at [i,j]
                    pixel current_pixel;
                    sum.red = max(sum.red, 0);
                    sum.green = max(sum.green, 0);
                    sum.blue = max(sum.blue, 0);
                    current_pixel.red = (unsigned char) min(sum.red, 255);
                    current_pixel.green = (unsigned char) min(sum.green, 255);
                    current_pixel.blue = (unsigned char) min(sum.blue, 255);
                    memcpy(&img.data[(index << 1) + index], &current_pixel, 3);
                }
            }

        } else {
            register int index, temp, index3;
            for (index = 0, temp = m * n, index3 = 0; index < temp; ++index) {
                backupOrg[index] = *((pixel *) &img.data[index3]);
                index3 += 3;
            }
            if (flag) {
                register int temp = m * (n - 1) - 1, index;
                for (index = m + 1; index < temp; ++index) {
                    pixel_sum sum = {0, 0, 0};
                    register int index_1 = index - 1;

                    pixel_sum tempsum;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, 1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, 1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, 1);

                    // assign kernel's result to pixel at [i,j]
                    pixel current_pixel;
                    int red = sum.red / kernelScale, green = sum.green / kernelScale, blue = sum.blue / kernelScale;
                    red = max(red, 0), green = max(green, 0), blue = max(blue, 0);
                    current_pixel.red = (unsigned char) min(red, 255);
                    current_pixel.green = (unsigned char) min(green, 255);
                    current_pixel.blue = (unsigned char) min(blue, 255);
                    memcpy(&img.data[(index << 1) + index], &current_pixel, 3);
                }
            } else {
                register int temp = m * (n - 1) - 1, index;
                for (index = m + 1; index < temp; ++index) {
                    pixel_sum sum = {0, 0, 0};
                    register int index_1 = index - 1;

                    pixel_sum tempsum;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, -1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, -1);
                    ++index_1;
                    getsum(tempsum, sums, backupOrg, index_1);
                    sum_pixels_by_weight(sum, tempsum, -1);
                    pixel p = backupOrg[index];
                    sum_pixels_by_weight(sum, p, 10);

                    // assign kernel's result to pixel at [i,j]
                    pixel current_pixel;
                    sum.red = max(sum.red, 0);
                    sum.green = max(sum.green, 0);
                    sum.blue = max(sum.blue, 0);
                    current_pixel.red = (unsigned char) min(sum.red, 255);
                    current_pixel.green = (unsigned char) min(sum.green, 255);
                    current_pixel.blue = (unsigned char) min(sum.blue, 255);
                    memcpy(&img.data[(index << 1) + index], &current_pixel, 3);
                }
            }
        }

    }

    *image = img;

    free(backupOrg);
    free(sums);
}

void myfunction(Image *image, char *srcImgpName, char *blurRsltImgName, char *sharpRsltImgName,
                char *filteredBlurRsltImgName, char *filteredSharpRsltImgName, char flag) {
    Image img = *image;
    Image *p_img = &img;


    if (flag == '1') {
        // blur image
        doConvolution(p_img, 9, false);

        // write result image to file
        writeBMP(p_img, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        doConvolution(p_img, 1, false);

        // write result image to file
        writeBMP(p_img, srcImgpName, sharpRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        doConvolution(p_img, 7, true);

        // write result image to file
        writeBMP(p_img, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        doConvolution(p_img, 1, false);

        // write result image to file
        writeBMP(p_img, srcImgpName, filteredSharpRsltImgName);
    }
}

