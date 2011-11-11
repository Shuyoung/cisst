/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id$
  
  Author(s):  Balazs Vagvolgyi
  Created on: 2010

  (C) Copyright 2006-2010 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/

#include "svlImageProcessingHelper.h"
#include "cisstCommon/cmnPortability.h"
#include <fstream>


/*****************************************/
/*** svlImageProcessingInternals class ***/
/*****************************************/

svlImageProcessingInternals::svlImageProcessingInternals()
{
    // Called only by derived class
}

svlImageProcessingInternals::svlImageProcessingInternals(const svlImageProcessingInternals& CMN_UNUSED(internals))
{
    // Never called
}

svlImageProcessingInternals::~svlImageProcessingInternals()
{
    // NOP
}


/******************************************/
/*** svlImageProcessingHelper namespace ***/
/******************************************/

void svlImageProcessingHelper::ConvolutionRGB(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal)
{
    if (!input || !output || kernel.size() < 1) return;

    const int kernel_size = static_cast<int>(kernel.size());
    const int kernel_rad = kernel_size / 2;
    const int rowstride = width * 3;
    int sum1, sum2, sum3;
    unsigned char *input2;
    int *kernelptr;
    int i, j, k_val;

    if (horizontal) {

        int k, k_from, k_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum1 = sum2 = sum3 = 0;

                kernelptr = kernel.Pointer();

                k = kernel_size;
                k_from = i - kernel_rad;
                k_to   = k_from + kernel_size;
                if (k_from < 0) {
                    k += k_from;
                    kernelptr -= k_from;
                    k_from = 0;
                }
                if (k_to > width) {
                    k -= k_to - width;
                    k_to = width;
                }

                input2 = input + j * rowstride + k_from * 3;

                while (k) {

                    k_val = *kernelptr; kernelptr ++;
                    sum1 += (*input2) * k_val; input2 ++; 
                    sum2 += (*input2) * k_val; input2 ++;
                    sum3 += (*input2) * k_val; input2 ++;

                    k --;
                }

                sum1 >>= 10; sum2 >>= 10; sum3 >>= 10;

                if (sum1 < 0) sum1 = 0; else if (sum1 > 255) sum1 = 255;
                *output = static_cast<unsigned char>(sum1); output ++;
                if (sum2 < 0) sum2 = 0; else if (sum2 > 255) sum2 = 255;
                *output = static_cast<unsigned char>(sum2); output ++;
                if (sum3 < 0) sum3 = 0; else if (sum3 > 255) sum3 = 255;
                *output = static_cast<unsigned char>(sum3); output ++;
            }
        }

    }
    else {

        const int rowstride_short = rowstride - 3;
        int l, l_from, l_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum1 = sum2 = sum3 = 0;

                kernelptr = kernel.Pointer();

                l = kernel_size;
                l_from = j - kernel_rad;
                l_to   = l_from + kernel_size;
                if (l_from < 0) {
                    l += l_from;
                    kernelptr -= l_from;
                    l_from = 0;
                }
                if (l_to > height) {
                    l -= l_to - height;
                    l_to = height;
                }

                input2 = input + l_from * rowstride + i * 3;

                while (l) {

                    k_val = *kernelptr; kernelptr ++;
                    sum1 += (*input2) * k_val; input2 ++; 
                    sum2 += (*input2) * k_val; input2 ++;
                    sum3 += (*input2) * k_val; input2 ++;

                    input2 += rowstride_short;

                    l --;
                }

                sum1 >>= 10; sum2 >>= 10; sum3 >>= 10;

                if (sum1 < 0) sum1 = 0; else if (sum1 > 255) sum1 = 255;
                *output = static_cast<unsigned char>(sum1); output ++;
                if (sum2 < 0) sum2 = 0; else if (sum2 > 255) sum2 = 255;
                *output = static_cast<unsigned char>(sum2); output ++;
                if (sum3 < 0) sum3 = 0; else if (sum3 > 255) sum3 = 255;
                *output = static_cast<unsigned char>(sum3); output ++;
            }
        }

    }
}

void svlImageProcessingHelper::ConvolutionRGBA(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal)
{
    if (!input || !output || kernel.size() < 1) return;

    const int kernel_size = static_cast<int>(kernel.size());
    const int kernel_rad = kernel_size / 2;
    const int rowstride = width * 4;
    int sum1, sum2, sum3, sum4;
    unsigned char *input2;
    int *kernelptr;
    int i, j, k_val;

    if (horizontal) {

        int k, k_from, k_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum1 = sum2 = sum3 = sum4 = 0;

                kernelptr = kernel.Pointer();

                k = kernel_size;
                k_from = i - kernel_rad;
                k_to   = k_from + kernel_size;
                if (k_from < 0) {
                    k += k_from;
                    kernelptr -= k_from;
                    k_from = 0;
                }
                if (k_to > width) {
                    k -= k_to - width;
                    k_to = width;
                }

                input2 = input + j * rowstride + k_from * 4;

                while (k) {

                    k_val = *kernelptr; kernelptr ++;
                    sum1 += (*input2) * k_val; input2 ++; 
                    sum2 += (*input2) * k_val; input2 ++;
                    sum3 += (*input2) * k_val; input2 ++;
                    sum4 += (*input2) * k_val; input2 ++;

                    k --;
                }

                sum1 >>= 10; sum2 >>= 10; sum3 >>= 10; sum4 >>= 10;

                if (sum1 < 0) sum1 = 0; else if (sum1 > 255) sum1 = 255;
                *output = static_cast<unsigned char>(sum1); output ++;
                if (sum2 < 0) sum2 = 0; else if (sum2 > 255) sum2 = 255;
                *output = static_cast<unsigned char>(sum2); output ++;
                if (sum3 < 0) sum3 = 0; else if (sum3 > 255) sum3 = 255;
                *output = static_cast<unsigned char>(sum3); output ++;
                if (sum4 < 0) sum4 = 0; else if (sum4 > 255) sum4 = 255;
                *output = static_cast<unsigned char>(sum4); output ++;
            }
        }

    }
    else {

        const int rowstride_short = rowstride - 4;
        int l, l_from, l_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum1 = sum2 = sum3 = sum4 = 0;

                kernelptr = kernel.Pointer();

                l = kernel_size;
                l_from = j - kernel_rad;
                l_to   = l_from + kernel_size;
                if (l_from < 0) {
                    l += l_from;
                    kernelptr -= l_from;
                    l_from = 0;
                }
                if (l_to > height) {
                    l -= l_to - height;
                    l_to = height;
                }

                input2 = input + l_from * rowstride + i * 4;

                while (l) {

                    k_val = *kernelptr; kernelptr ++;
                    sum1 += (*input2) * k_val; input2 ++; 
                    sum2 += (*input2) * k_val; input2 ++;
                    sum3 += (*input2) * k_val; input2 ++;
                    sum4 += (*input2) * k_val; input2 ++;

                    input2 += rowstride_short;

                    l --;
                }

                sum1 >>= 10; sum2 >>= 10; sum3 >>= 10; sum4 >>= 10;

                if (sum1 < 0) sum1 = 0; else if (sum1 > 255) sum1 = 255;
                *output = static_cast<unsigned char>(sum1); output ++;
                if (sum2 < 0) sum2 = 0; else if (sum2 > 255) sum2 = 255;
                *output = static_cast<unsigned char>(sum2); output ++;
                if (sum3 < 0) sum3 = 0; else if (sum3 > 255) sum3 = 255;
                *output = static_cast<unsigned char>(sum3); output ++;
                if (sum4 < 0) sum4 = 0; else if (sum4 > 255) sum4 = 255;
                *output = static_cast<unsigned char>(sum4); output ++;
            }
        }

    }
}

void svlImageProcessingHelper::ConvolutionMono8(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal)
{
    if (!input || !output || kernel.size() < 1) return;

    const int kernel_size = static_cast<int>(kernel.size());
    const int kernel_rad = kernel_size / 2;
    unsigned char *input2;
    int *kernelptr;
    int i, j, k_val, sum;

    if (horizontal) {

        int k, k_from, k_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum = 0;

                kernelptr = kernel.Pointer();

                k = kernel_size;
                k_from = i - kernel_rad;
                k_to   = k_from + kernel_size;
                if (k_from < 0) {
                    k += k_from;
                    kernelptr -= k_from;
                    k_from = 0;
                }
                if (k_to > width) {
                    k -= k_to - width;
                    k_to = width;
                }

                input2 = input + j * width + k_from;

                while (k) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++; 

                    k --;
                }

                sum >>= 10;

                if (sum < 0) sum = 0; else if (sum > 255) sum = 255;
                *output = static_cast<unsigned char>(sum); output ++;
            }
        }

    }
    else {

        const int rowstride_short = width - 1;
        int l, l_from, l_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum = 0;
                
                kernelptr = kernel.Pointer();
                
                l = kernel_size;
                l_from = j - kernel_rad;
                l_to   = l_from + kernel_size;
                if (l_from < 0) {
                    l += l_from;
                    kernelptr -= l_from;
                    l_from = 0;
                }
                if (l_to > height) {
                    l -= l_to - height;
                    l_to = height;
                }
                
                input2 = input + l_from * width + i;
                
                while (l) {
                    
                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++; 
                    
                    input2 += rowstride_short;
                    
                    l --;
                }
                
                sum >>= 10;
                
                if (sum < 0) sum = 0; else if (sum > 255) sum = 255;
                *output = static_cast<unsigned char>(sum); output ++;
            }
        }
        
    }
}

void svlImageProcessingHelper::ConvolutionMono16(unsigned short* input, unsigned short* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal)
{
    if (!input || !output || kernel.size() < 1) return;

    const int kernel_size = static_cast<int>(kernel.size());
    const int kernel_rad = kernel_size / 2;
    unsigned short *input2;
    int *kernelptr;
    int i, j, k_val, sum;

    if (horizontal) {

        int k, k_from, k_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum = 0;

                kernelptr = kernel.Pointer();

                k = kernel_size;
                k_from = i - kernel_rad;
                k_to   = k_from + kernel_size;
                if (k_from < 0) {
                    k += k_from;
                    kernelptr -= k_from;
                    k_from = 0;
                }
                if (k_to > width) {
                    k -= k_to - width;
                    k_to = width;
                }

                input2 = input + j * width + k_from;

                while (k) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++; 

                    k --;
                }

                sum >>= 10;

                if (sum < 0) sum = 0; else if (sum > 65535) sum = 65535;
                *output = static_cast<unsigned short>(sum); output ++;
            }
        }

    }
    else {

        const int rowstride_short = width - 1;
        int l, l_from, l_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum = 0;

                kernelptr = kernel.Pointer();

                l = kernel_size;
                l_from = j - kernel_rad;
                l_to   = l_from + kernel_size;
                if (l_from < 0) {
                    l += l_from;
                    kernelptr -= l_from;
                    l_from = 0;
                }
                if (l_to > height) {
                    l -= l_to - height;
                    l_to = height;
                }

                input2 = input + l_from * width + i;

                while (l) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++; 

                    input2 += rowstride_short;

                    l --;
                }

                sum >>= 10;

                if (sum < 0) sum = 0; else if (sum > 65535) sum = 65535;
                *output = static_cast<unsigned short>(sum); output ++;
            }
        }

    }
}

void svlImageProcessingHelper::ConvolutionMono32(unsigned int* input, unsigned int* output, const int width, const int height, vctDynamicVector<int> & kernel, bool horizontal)
{
    if (!input || !output || kernel.size() < 1) return;

    const int kernel_size = static_cast<int>(kernel.size());
    const int kernel_rad = kernel_size / 2;
    unsigned int *input2;
    int *kernelptr;
    int i, j, k_val, sum;

    if (horizontal) {

        int k, k_from, k_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum = 0;

                kernelptr = kernel.Pointer();

                k = kernel_size;
                k_from = i - kernel_rad;
                k_to   = k_from + kernel_size;
                if (k_from < 0) {
                    k += k_from;
                    kernelptr -= k_from;
                    k_from = 0;
                }
                if (k_to > width) {
                    k -= k_to - width;
                    k_to = width;
                }

                input2 = input + j * width + k_from;

                while (k) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++; 

                    k --;
                }

                sum >>= 10;

                if (sum < 0) sum = 0;
                *output = static_cast<unsigned int>(sum); output ++;
            }
        }

    }
    else {

        const int rowstride_short = width - 1;
        int l, l_from, l_to;

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                sum = 0;

                kernelptr = kernel.Pointer();

                l = kernel_size;
                l_from = j - kernel_rad;
                l_to   = l_from + kernel_size;
                if (l_from < 0) {
                    l += l_from;
                    kernelptr -= l_from;
                    l_from = 0;
                }
                if (l_to > height) {
                    l -= l_to - height;
                    l_to = height;
                }

                input2 = input + l_from * width + i;

                while (l) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++; 

                    input2 += rowstride_short;

                    l --;
                }

                sum >>= 10;

                if (sum < 0) sum = 0;
                *output = static_cast<unsigned int>(sum); output ++;
            }
        }

    }
}

void svlImageProcessingHelper::ConvolutionRGB(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicMatrix<int> & kernel)
{
    if (!input || !output || kernel.rows() < 1 || kernel.rows() != kernel.cols()) return;

    const int kernel_size = static_cast<int>(kernel.cols());
    const int kernel_rad = kernel_size / 2;
    const int rowstride = width * 3;
    const int rowstride_short = rowstride - kernel_size * 3;
    int i, j, k, k_, l, l_from, l_to, k_from, k_to, k_val, kernelstride;
    int sum1, sum2, sum3;
    unsigned char *input2;
    int *kernelptr;

    for (j = 0; j < height; j ++) {
        for (i = 0; i < width; i ++) {

            sum1 = sum2 = sum3 = 0;

            kernelstride = 0;
            kernelptr = kernel.Pointer();

            l = kernel_size;
            l_from = j - kernel_rad;
            l_to   = l_from + kernel_size;
            if (l_from < 0) {
                l += l_from;
                kernelptr -= l_from * kernel_size;

                l_from = 0;
            }
            if (l_to > height) {
                l -= l_to - height;
                l_to = height;
            }

            k = kernel_size;
            k_from = i - kernel_rad;
            k_to   = k_from + kernel_size;
            if (k_from < 0) {
                k += k_from;
                kernelptr -= k_from;
                kernelstride -= k_from;

                k_from = 0;
            }
            if (k_to > width) {
                k_ = k_to - width;
                k -= k_;
                kernelstride += k_;

                k_to = width;
            }

            input2 = input + l_from * rowstride + k_from * 3;

            while (l) {

                k_ = k;
                while (k_) {

                    k_val = *kernelptr; kernelptr ++;
                    sum1 += (*input2) * k_val; input2 ++; 
                    sum2 += (*input2) * k_val; input2 ++;
                    sum3 += (*input2) * k_val; input2 ++;
                    
                    k_ --;
                }

                input2 += rowstride_short + kernelstride * 3;
                kernelptr += kernelstride;

                l --;
            }

            sum1 >>= 10; sum2 >>= 10; sum3 >>= 10;

            if (sum1 < 0) sum1 = 0; else if (sum1 > 255) sum1 = 255;
            *output = static_cast<unsigned char>(sum1); output ++;
            if (sum2 < 0) sum2 = 0; else if (sum2 > 255) sum2 = 255;
            *output = static_cast<unsigned char>(sum2); output ++;
            if (sum3 < 0) sum3 = 0; else if (sum3 > 255) sum3 = 255;
            *output = static_cast<unsigned char>(sum3); output ++;
        }
    }
}

void svlImageProcessingHelper::ConvolutionRGBA(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicMatrix<int> & kernel)
{
    if (!input || !output || kernel.rows() < 1 || kernel.rows() != kernel.cols()) return;

    const int kernel_size = static_cast<int>(kernel.cols());
    const int kernel_rad = kernel_size / 2;
    const int rowstride = width * 4;
    const int rowstride_short = rowstride - kernel_size * 4;
    int i, j, k, k_, l, l_from, l_to, k_from, k_to, k_val, kernelstride;
    int sum1, sum2, sum3, sum4;
    unsigned char *input2;
    int *kernelptr;

    for (j = 0; j < height; j ++) {
        for (i = 0; i < width; i ++) {

            sum1 = sum2 = sum3 = sum4 = 0;

            kernelstride = 0;
            kernelptr = kernel.Pointer();

            l = kernel_size;
            l_from = j - kernel_rad;
            l_to   = l_from + kernel_size;
            if (l_from < 0) {
                l += l_from;
                kernelptr -= l_from * kernel_size;

                l_from = 0;
            }
            if (l_to > height) {
                l -= l_to - height;
                l_to = height;
            }

            k = kernel_size;
            k_from = i - kernel_rad;
            k_to   = k_from + kernel_size;
            if (k_from < 0) {
                k += k_from;
                kernelptr -= k_from;
                kernelstride -= k_from;

                k_from = 0;
            }
            if (k_to > width) {
                k_ = k_to - width;
                k -= k_;
                kernelstride += k_;

                k_to = width;
            }

            input2 = input + l_from * rowstride + k_from * 4;

            while (l) {

                k_ = k;
                while (k_) {

                    k_val = *kernelptr; kernelptr ++;
                    sum1 += (*input2) * k_val; input2 ++; 
                    sum2 += (*input2) * k_val; input2 ++;
                    sum3 += (*input2) * k_val; input2 ++;
                    sum4 += (*input2) * k_val; input2 ++;

                    k_ --;
                }

                input2 += rowstride_short + kernelstride * 4;
                kernelptr += kernelstride;

                l --;
            }

            sum1 >>= 10; sum2 >>= 10; sum3 >>= 10; sum4 >>= 10;

            if (sum1 < 0) sum1 = 0; else if (sum1 > 255) sum1 = 255;
            *output = static_cast<unsigned char>(sum1); output ++;
            if (sum2 < 0) sum2 = 0; else if (sum2 > 255) sum2 = 255;
            *output = static_cast<unsigned char>(sum2); output ++;
            if (sum3 < 0) sum3 = 0; else if (sum3 > 255) sum3 = 255;
            *output = static_cast<unsigned char>(sum3); output ++;
            if (sum4 < 0) sum4 = 0; else if (sum4 > 255) sum4 = 255;
            *output = static_cast<unsigned char>(sum4); output ++;
        }
    }
}

void svlImageProcessingHelper::ConvolutionMono8(unsigned char* input, unsigned char* output, const int width, const int height, vctDynamicMatrix<int> & kernel)
{
    if (!input || !output || kernel.rows() < 1 || kernel.rows() != kernel.cols()) return;

    const int kernel_size = static_cast<int>(kernel.cols());
    const int kernel_rad = kernel_size / 2;
    const int rowstride_short = width - kernel_size;
    int i, j, k, k_, l, l_from, l_to, k_from, k_to, k_val, kernelstride, sum;
    unsigned char *input2;
    int *kernelptr;

    for (j = 0; j < height; j ++) {
        for (i = 0; i < width; i ++) {

            sum = 0;

            kernelstride = 0;
            kernelptr = kernel.Pointer();

            l = kernel_size;
            l_from = j - kernel_rad;
            l_to   = l_from + kernel_size;
            if (l_from < 0) {
                l += l_from;
                kernelptr -= l_from * kernel_size;

                l_from = 0;
            }
            if (l_to > height) {
                l -= l_to - height;
                l_to = height;
            }

            k = kernel_size;
            k_from = i - kernel_rad;
            k_to   = k_from + kernel_size;
            if (k_from < 0) {
                k += k_from;
                kernelptr -= k_from;
                kernelstride -= k_from;

                k_from = 0;
            }
            if (k_to > width) {
                k_ = k_to - width;
                k -= k_;
                kernelstride += k_;

                k_to = width;
            }

            input2 = input + l_from * width + k_from;

            while (l) {

                k_ = k;
                while (k_) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++;

                    k_ --;
                }

                input2 += rowstride_short + kernelstride;
                kernelptr += kernelstride;

                l --;
            }

            sum >>= 10;

            if (sum < 0) sum = 0; else if (sum > 255) sum = 255;
            *output = static_cast<unsigned char>(sum); output ++;
        }
    }
}

void svlImageProcessingHelper::ConvolutionMono16(unsigned short* input, unsigned short* output, const int width, const int height, vctDynamicMatrix<int> & kernel)
{
    if (!input || !output || kernel.rows() < 1 || kernel.rows() != kernel.cols()) return;

    const int kernel_size = static_cast<int>(kernel.cols());
    const int kernel_rad = kernel_size / 2;
    const int rowstride_short = width - kernel_size;
    int i, j, k, k_, l, l_from, l_to, k_from, k_to, k_val, kernelstride, sum;
    unsigned short *input2;
    int *kernelptr;

    for (j = 0; j < height; j ++) {
        for (i = 0; i < width; i ++) {

            sum = 0;

            kernelstride = 0;
            kernelptr = kernel.Pointer();

            l = kernel_size;
            l_from = j - kernel_rad;
            l_to   = l_from + kernel_size;
            if (l_from < 0) {
                l += l_from;
                kernelptr -= l_from * kernel_size;

                l_from = 0;
            }
            if (l_to > height) {
                l -= l_to - height;
                l_to = height;
            }

            k = kernel_size;
            k_from = i - kernel_rad;
            k_to   = k_from + kernel_size;
            if (k_from < 0) {
                k += k_from;
                kernelptr -= k_from;
                kernelstride -= k_from;

                k_from = 0;
            }
            if (k_to > width) {
                k_ = k_to - width;
                k -= k_;
                kernelstride += k_;

                k_to = width;
            }

            input2 = input + l_from * width + k_from;

            while (l) {

                k_ = k;
                while (k_) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++;

                    k_ --;
                }

                input2 += rowstride_short + kernelstride;
                kernelptr += kernelstride;

                l --;
            }

            sum >>= 10;

            if (sum < 0) sum = 0; else if (sum > 65535) sum = 65535;
            *output = static_cast<unsigned short>(sum); output ++;
        }
    }
}

void svlImageProcessingHelper::ConvolutionMono32(unsigned int* input, unsigned int* output, const int width, const int height, vctDynamicMatrix<int> & kernel)
{
    if (!input || !output || kernel.rows() < 1 || kernel.rows() != kernel.cols()) return;

    const int kernel_size = static_cast<int>(kernel.cols());
    const int kernel_rad = kernel_size / 2;
    const int rowstride_short = width - kernel_size;
    int i, j, k, k_, l, l_from, l_to, k_from, k_to, k_val, kernelstride, sum;
    unsigned int *input2;
    int *kernelptr;

    for (j = 0; j < height; j ++) {
        for (i = 0; i < width; i ++) {

            sum = 0;

            kernelstride = 0;
            kernelptr = kernel.Pointer();

            l = kernel_size;
            l_from = j - kernel_rad;
            l_to   = l_from + kernel_size;
            if (l_from < 0) {
                l += l_from;
                kernelptr -= l_from * kernel_size;
                
                l_from = 0;
            }
            if (l_to > height) {
                l -= l_to - height;
                l_to = height;
            }

            k = kernel_size;
            k_from = i - kernel_rad;
            k_to   = k_from + kernel_size;
            if (k_from < 0) {
                k += k_from;
                kernelptr -= k_from;
                kernelstride -= k_from;
                
                k_from = 0;
            }
            if (k_to > width) {
                k_ = k_to - width;
                k -= k_;
                kernelstride += k_;
                
                k_to = width;
            }

            input2 = input + l_from * width + k_from;

            while (l) {

                k_ = k;
                while (k_) {

                    k_val = *kernelptr; kernelptr ++;
                    sum += (*input2) * k_val; input2 ++;

                    k_ --;
                }

                input2 += rowstride_short + kernelstride;
                kernelptr += kernelstride;

                l --;
            }

            sum >>= 10;

            if (sum < 0) sum = 0;
            *output = static_cast<unsigned int>(sum); output ++;
        }
    }
}

void svlImageProcessingHelper::ResampleMono8(unsigned char* src, const unsigned int srcwidth, const unsigned int srcheight,
                                             unsigned char* dst, const unsigned int dstwidth, const unsigned int dstheight)
{
    unsigned int i, j;
    unsigned int x1, y1, x2, y2;
    unsigned char *psrc, *plsrc, *pdst;
    
    // vertical sampling loop
    plsrc = src;
    pdst = dst;
    y1 = 0;
    y2 = dstheight;
    for (j = 0; j < dstheight; j ++) {
        
        // horizontal sampling loop
        psrc = plsrc;
        x1 = 0;
        x2 = dstwidth;
        for (i = 0; i < dstwidth; i ++) {
            *pdst = *psrc;
            pdst ++;
            
            x1 += srcwidth;
            while (x1 >= x2) {
                x2 += dstwidth;
                psrc ++;
            }
        }
        
        y1 += srcheight;
        while (y1 >= y2) {
            y2 += dstheight;
            plsrc += srcwidth;
        }
    }
}

void svlImageProcessingHelper::ResampleAndInterpolateHMono8(unsigned char* src, const unsigned int srcwidth,
                                                            unsigned char* dst, const unsigned int dstwidth,
                                                            const unsigned int height)
{
    unsigned int i, j;
    unsigned int x1, x2;
    int wx1, wx2;
    unsigned char *psrc, *plsrc, *pdst;
    unsigned char prev_col, this_col;

    // eliminating division by using integral powers of 2
    const unsigned int fast_dstwidth = 256;   // 2^8
    const unsigned int fast_srcwidth = fast_dstwidth * srcwidth / dstwidth;
    
    plsrc = src;
    pdst = dst;
    for (j = 0; j < height; j ++) {
        
        // horizontal sampling loop
        psrc = plsrc;
        x1 = 0;
        x2 = 128;
        prev_col = this_col = *psrc;
        wx1 = 0;
        wx2 = fast_dstwidth;
        
        for (i = 0; i < dstwidth; i ++) {
            *pdst = (wx1 * prev_col + wx2 * this_col) >> 8;
            pdst ++;
            
            x1 += fast_srcwidth;
            while (x1 > x2) {
                x2 += fast_dstwidth;
                prev_col = this_col; this_col = *psrc; psrc ++;
            }
            
            wx1 = x2 - x1;
            wx2 = fast_dstwidth - wx1;
        }
        plsrc += srcwidth;
    }
}

void svlImageProcessingHelper::ResampleAndInterpolateVMono8(unsigned char* src, const unsigned int srcheight,
                                                            unsigned char* dst, const unsigned int dstheight,
                                                            const unsigned int width)
{
    unsigned int i, j;
    unsigned int y1, y2;
    int wy1, wy2;
    unsigned char *psrc, *pcsrc, *pdst, *pcdst;
    unsigned char prev_col, this_col;
    
    // eliminating division by using integral powers of 2
    const unsigned int fast_dstheight = 256;   // 2^8
    const unsigned int fast_srcheight = fast_dstheight * srcheight / dstheight;
    
    pcsrc = src;
    pcdst = dst;
    for (j = 0; j < width; j ++) {
        
        // vertical sampling loop
        psrc = pcsrc;
        pdst = pcdst;
        y1 = 0;
        y2 = 128;
        prev_col = this_col = *psrc;
        wy1 = 0;
        wy2 = fast_dstheight;
        
        for (i = 0; i < dstheight; i ++) {
            *pdst = (wy1 * prev_col + wy2 * this_col) >> 8;
            pdst += width;
            
            y1 += fast_srcheight;
            while (y1 > y2) {
                y2 += fast_dstheight;
                prev_col = this_col; this_col = *psrc; psrc += width;
            }
            
            wy1 = y2 - y1;
            wy2 = fast_dstheight - wy1;
        }
        pcsrc ++;
        pcdst ++;
    }
}

void svlImageProcessingHelper::ResampleRGB24(unsigned char* src, const unsigned int srcwidth, const unsigned int srcheight,
                                             unsigned char* dst, const unsigned int dstwidth, const unsigned int dstheight)
{
    unsigned int i, j;
    unsigned int x1, y1, x2, y2;
    unsigned char *psrc, *plsrc, *pdst;
    const unsigned int srcstride = srcwidth * 3;
    
    // vertical sampling loop
    plsrc = src;
    pdst = dst;
    y1 = 0;
    y2 = dstheight;
    for (j = 0; j < dstheight; j ++) {
        
        // horizontal sampling loop
        psrc = plsrc;
        x1 = 0;
        x2 = dstwidth;
        for (i = 0; i < dstwidth; i ++) {
            *pdst = psrc[0]; pdst ++;
            *pdst = psrc[1]; pdst ++;
            *pdst = psrc[2]; pdst ++;
            
            x1 += srcwidth;
            while (x1 >= x2) {
                x2 += dstwidth;
                psrc += 3;
            }
        }
        
        y1 += srcheight;
        while (y1 >= y2) {
            y2 += dstheight;
            plsrc += srcstride;
        }
    }
}

void svlImageProcessingHelper::ResampleAndInterpolateHRGB24(unsigned char* src, const unsigned int srcwidth,
                                                            unsigned char* dst, const unsigned int dstwidth,
                                                            const unsigned int height)
{
    unsigned int i, j;
    unsigned int x1, x2;
    int wx1, wx2;
    unsigned char *psrc, *plsrc, *pdst;
    unsigned char prev_r, prev_g, prev_b, this_r, this_g, this_b;
    const unsigned int srcstride = srcwidth * 3;
    
    // eliminating division by using integral powers of 2
    const unsigned int fast_dstwidth = 256;   // 2^8
    const unsigned int fast_srcwidth = fast_dstwidth * srcwidth / dstwidth;
    
    plsrc = src;
    pdst = dst;
    for (j = 0; j < height; j ++) {
        
        // horizontal sampling loop
        psrc = plsrc;
        x1 = 0;
        x2 = 128;
        prev_r = this_r = psrc[0];
        prev_g = this_g = psrc[1];
        prev_b = this_b = psrc[2];
        wx1 = 0;
        wx2 = fast_dstwidth;
        
        for (i = 0; i < dstwidth; i ++) {
            *pdst = (wx1 * prev_r + wx2 * this_r) >> 8;
            pdst ++;
            *pdst = (wx1 * prev_g + wx2 * this_g) >> 8;
            pdst ++;
            *pdst = (wx1 * prev_b + wx2 * this_b) >> 8;
            pdst ++;
            
            x1 += fast_srcwidth;
            while (x1 > x2) {
                x2 += fast_dstwidth;
                prev_r = this_r; this_r = *psrc; psrc ++;
                prev_g = this_g; this_g = *psrc; psrc ++;
                prev_b = this_b; this_b = *psrc; psrc ++;
            }
            
            wx1 = x2 - x1;
            wx2 = fast_dstwidth - wx1;
        }
        plsrc += srcstride;
    }
}

void svlImageProcessingHelper::ResampleAndInterpolateVRGB24(unsigned char* src, const unsigned int srcheight,
                                                            unsigned char* dst, const unsigned int dstheight,
                                                            const unsigned int width)
{
    unsigned int i, j;
    unsigned int y1, y2;
    int wy1, wy2;
    unsigned char *psrc, *pcsrc, *pdst, *pcdst;
    unsigned char prev_r, prev_g, prev_b, this_r, this_g, this_b;
    const unsigned int stride = width * 3 - 2;
    
    // eliminating division by using integral powers of 2
    const unsigned int fast_dstheight = 256;   // 2^8
    const unsigned int fast_srcheight = fast_dstheight * srcheight / dstheight;
    
    pcsrc = src;
    pcdst = dst;
    for (j = 0; j < width; j ++) {
        
        // vertical sampling loop
        psrc = pcsrc;
        pdst = pcdst;
        y1 = 0;
        y2 = 128;
        prev_r = this_r = psrc[0];
        prev_g = this_g = psrc[1];
        prev_b = this_b = psrc[2];
        wy1 = 0;
        wy2 = fast_dstheight;
        
        for (i = 0; i < dstheight; i ++) {
            *pdst = (wy1 * prev_r + wy2 * this_r) >> 8;
            pdst ++;
            *pdst = (wy1 * prev_g + wy2 * this_g) >> 8;
            pdst ++;
            *pdst = (wy1 * prev_b + wy2 * this_b) >> 8;
            pdst += stride;
            
            y1 += fast_srcheight;
            while (y1 > y2) {
                y2 += fast_dstheight;
                prev_r = this_r; this_r = *psrc; psrc ++;
                prev_g = this_g; this_g = *psrc; psrc ++;
                prev_b = this_b; this_b = *psrc; psrc += stride;
            }
            
            wy1 = y2 - y1;
            wy2 = fast_dstheight - wy1;
        }
        pcsrc += 3;
        pcdst += 3;
    }
}

void svlImageProcessingHelper::DeinterlaceBlending(unsigned char* buffer, const unsigned int width, const unsigned int height)
{
    unsigned int i, j;
    int ar, ag, ab;
    unsigned char *r0, *g0, *b0;
    unsigned char *r1, *g1, *b1;
    const int colstride = width * 3;
    
    r0 = buffer;
    g0 = r0 + 1;
    b0 = g0 + 1;
    
    r1 = r0 + colstride;
    g1 = r1 + 1;
    b1 = g1 + 1;
    
    for (j = 0; j < height; j += 2) {
        for (i = 0; i < width; i ++) {
            ar = (*r0 + *r1) >> 1; ag = (*g0 + *g1) >> 1; ab = (*b0 + *b1) >> 1;
            *r0 = ar; *g0 = ag; *b0 = ab;
            *r1 = ar; *g1 = ag; *b1 = ab;
            
            r0 += 3; g0 += 3; b0 += 3;
            r1 += 3; g1 += 3; b1 += 3;
        }
        r0 += colstride; g0 += colstride; b0 += colstride;
        r1 += colstride; g1 += colstride; b1 += colstride;
    }
}

void svlImageProcessingHelper::DeinterlaceDiscarding(unsigned char* buffer, const unsigned int width, const unsigned int height)
{
    unsigned int i, j;
    unsigned char *r0, *g0, *b0;
    unsigned char *r1, *g1, *b1;
    const int colstride = width * 3;
    
    r0 = buffer;
    g0 = r0 + 1;
    b0 = g0 + 1;
    
    r1 = r0 + colstride;
    g1 = r1 + 1;
    b1 = g1 + 1;
    
    for (j = 0; j < height; j += 2) {
        for (i = 0; i < width; i ++) {
            *r1 = *r0; *g1 = *g0; *b1 = *b0;
            
            r0 += 3; g0 += 3; b0 += 3;
            r1 += 3; g1 += 3; b1 += 3;
        }
        r0 += colstride; g0 += colstride; b0 += colstride;
        r1 += colstride; g1 += colstride; b1 += colstride;
    }
}

void svlImageProcessingHelper::DeinterlaceAdaptiveBlending(unsigned char* buffer, const unsigned int width, const unsigned int height)
{
    unsigned int i, j;
    int ar, ag, ab;
    unsigned int diff, diffinv;
    unsigned char *r0, *g0, *b0;
    unsigned char *r1, *g1, *b1;
    unsigned char *r2, *g2, *b2;
    const int colstride = width * 3;
    
    r0 = buffer;
    g0 = r0 + 1;
    b0 = g0 + 1;
    
    r1 = r0 + colstride;
    g1 = r1 + 1;
    b1 = g1 + 1;
    
    r2 = r1 + colstride;
    g2 = r2 + 1;
    b2 = g2 + 1;
    
    for (j = 0; j < height; j += 2) {
        for (i = 0; i < width; i ++) {
            ar = (*r0 + *r2) >> 1; ag = (*g0 + *g2) >> 1; ab = (*b0 + *b2) >> 1;
            
            ar -= *r1; if (ar < 0) ar = -ar;
            ag -= *g1; if (ag < 0) ag = -ag;
            ab -= *b1; if (ab < 0) ab = -ab;
            diff = (ar + ag + ab) << 2;
            if (diff > 765) diff = 765;
            diffinv = 765 - diff;
            *r1 = (diff * ar + diffinv * (*r1)) / 765;
            *g1 = (diff * ag + diffinv * (*g1)) / 765;
            *b1 = (diff * ab + diffinv * (*b1)) / 765;
            
            r0 += 3; g0 += 3; b0 += 3;
            r1 += 3; g1 += 3; b1 += 3;
            r2 += 3; g2 += 3; b2 += 3;
        }
        r0 += colstride; g0 += colstride; b0 += colstride;
        r1 += colstride; g1 += colstride; b1 += colstride;
        r2 += colstride; g2 += colstride; b2 += colstride;
    }
}

void svlImageProcessingHelper::DeinterlaceAdaptiveDiscarding(unsigned char* buffer, const unsigned int width, const unsigned int height)
{
    unsigned int i, j;
    int ar, ag, ab;
    unsigned int diff, diffinv;
    unsigned char *r0, *g0, *b0;
    unsigned char *r1, *g1, *b1;
    unsigned char *r2, *g2, *b2;
    const int colstride = width * 3;
    
    r0 = buffer;
    g0 = r0 + 1;
    b0 = g0 + 1;
    
    r1 = r0 + colstride;
    g1 = r1 + 1;
    b1 = g1 + 1;
    
    r2 = r1 + colstride;
    g2 = r2 + 1;
    b2 = g2 + 1;
    
    for (j = 0; j < height; j += 2) {
        for (i = 0; i < width; i ++) {
            ar = (*r0 + *r2) >> 1; ag = (*g0 + *g2) >> 1; ab = (*b0 + *b2) >> 1;
            
            ar -= *r1; if (ar < 0) ar = -ar;
            ag -= *g1; if (ag < 0) ag = -ag;
            ab -= *b1; if (ab < 0) ab = -ab;
            diff = (ar + ag + ab) << 1;
            if (diff > 765) diff = 765;
            diffinv = 765 - diff;
            *r1 = (diff * ar + diffinv * (*r1)) / 765;
            *g1 = (diff * ag + diffinv * (*g1)) / 765;
            *b1 = (diff * ab + diffinv * (*b1)) / 765;

            r0 += 3; g0 += 3; b0 += 3;
            r1 += 3; g1 += 3; b1 += 3;
            r2 += 3; g2 += 3; b2 += 3;
        }
        r0 += colstride; g0 += colstride; b0 += colstride;
        r1 += colstride; g1 += colstride; b1 += colstride;
        r2 += colstride; g2 += colstride; b2 += colstride;
    }
}


/**************************************************************/
/*** svlImageProcessingHelper::RectificationInternals class ***/
/**************************************************************/

svlImageProcessingHelper::RectificationInternals::RectificationInternals() :
    svlImageProcessingInternals(),
    Width(0),
    Height(0),
    idxDest(0),
    idxDestSize(0),
    idxSrc1(0),
    idxSrc1Size(0),
    idxSrc2(0),
    idxSrc2Size(0),
    idxSrc3(0),
    idxSrc3Size(0),
    idxSrc4(0),
    idxSrc4Size(0),
    blendSrc1(0),
    blendSrc1Size(0),
    blendSrc2(0),
    blendSrc2Size(0),
    blendSrc3(0),
    blendSrc3Size(0),
    blendSrc4(0),
    blendSrc4Size(0)
{
}

svlImageProcessingHelper::RectificationInternals::~RectificationInternals()
{
    Release();
}

bool svlImageProcessingHelper::RectificationInternals::Generate(unsigned int width, unsigned int height,
                                                                const svlSampleCameraGeometry & geometry,
                                                                unsigned int cam_id)
{
    const unsigned int cam_count = geometry.GetCameraCount();
    if (cam_id >= cam_count) return false;

    const svlSampleCameraGeometry::Intrinsics & intrinsics = geometry.GetIntrinsics(cam_id);

    // `intrinsics.fc` is of type `vct2`
    // `intrinsics.cc` is of type `vct2`
    // `intrinsics.a`  is of type `double`
    // `intrinsics.kc` is of type `vct7`

    if (cam_count == 1) {
    // Single camera
        return SetFromCameraCalibration(height,width,vct3x3::Eye(),intrinsics.fc, intrinsics.cc, intrinsics.kc, intrinsics.a, 0);
        // TO DO
    }
    else if (cam_count == 2) {
    // Stereo camera rig

        // The extrinsic parameters in the Camera Calibration Toolbox (CCT) format are
        // only specified once, for the right camera. The left camera extrinsics
        // are assumed to be all zero (i.e. the left camera frame is the world frame).
        // Therefore, the variable `extrinsics_orig` will be zero and `extrinsics` will
        // contain the extrinsics from the CCT file format.
        const svlSampleCameraGeometry::Extrinsics & extrinsics_orig = geometry.GetExtrinsics(SVL_LEFT);
        const svlSampleCameraGeometry::Extrinsics & extrinsics      = geometry.GetExtrinsics(SVL_RIGHT);

        // `extrinsics.om` is of type `vctRodRot3`
        // `extrinsics.T`  is of type `vct3`

        // TO DO
    }
    else {
        // Higher number of cameras are not supported yet
        return false;
    }

    return false;
}

bool svlImageProcessingHelper::RectificationInternals::Load(const std::string &filepath, int explen)
{
    Release();

    std::ifstream file(filepath.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) return false;

    double dbl;
    const unsigned int maxwidth = 1920;
    const unsigned int maxheight = 1200;
    const unsigned int size = maxwidth * maxheight;
    double* dblbuf = new double[size];
    char* chbuf    = new char[(16 * size) + 1];
    int valcnt, i;

    // lutpos:
    //          1 - width, height
    //          2 - destination index lut
    //          3 - source index lut 1
    //          4 - source index lut 2
    //          5 - source index lut 3
    //          6 - source index lut 4
    //          7 - source blending lut 1
    //          8 - source blending lut 2
    //          9 - source blending lut 3
    //         10 - source blending lut 4
    int lutpos = 0;

    while (lutpos < 10) {
        switch (lutpos) {
            case 0:
                if (LoadLine(file, &dbl, chbuf, 1, explen) < 1) goto labError;
                Height = static_cast<int>(dbl);
                if (LoadLine(file, &dbl, chbuf, 1, explen) < 1) goto labError;
                Width = static_cast<int>(dbl);
                
                if (Width > maxwidth || Height > maxheight) goto labError;
                
                lutpos ++;
                break;
                
            case 1:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                idxDestSize = valcnt;
                idxDest = new unsigned int[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    idxDest[i] = static_cast<unsigned int>(dblbuf[i] + 0.5);
                }
                
                lutpos ++;
                break;
                
            case 2:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                idxSrc1Size = valcnt;
                idxSrc1 = new unsigned int[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    idxSrc1[i] = static_cast<unsigned int>(dblbuf[i] + 0.5);
                }
                
                lutpos ++;
                break;
                
            case 3:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                idxSrc2Size = valcnt;
                idxSrc2 = new unsigned int[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    idxSrc2[i] = static_cast<unsigned int>(dblbuf[i] + 0.5);
                }
                
                lutpos ++;
                break;
                
            case 4:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                idxSrc3Size = valcnt;
                idxSrc3 = new unsigned int[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    idxSrc3[i] = static_cast<unsigned int>(dblbuf[i] + 0.5);
                }
                
                lutpos ++;
                break;
                
            case 5:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                idxSrc4Size = valcnt;
                idxSrc4 = new unsigned int[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    idxSrc4[i] = static_cast<unsigned int>(dblbuf[i] + 0.5);
                }
                
                lutpos ++;
                break;
                
            case 6:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                blendSrc1Size = valcnt;
                blendSrc1 = new unsigned char[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    blendSrc1[i] = static_cast<unsigned char>(dblbuf[i] * 256);
                }
                
                lutpos ++;
                break;
                
            case 7:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                blendSrc2Size = valcnt;
                blendSrc2 = new unsigned char[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    blendSrc2[i] = static_cast<unsigned char>(dblbuf[i] * 256);
                }
                
                lutpos ++;
                break;
                
            case 8:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                blendSrc3Size = valcnt;
                blendSrc3 = new unsigned char[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    blendSrc3[i] = static_cast<unsigned char>(dblbuf[i] * 256);
                }
                
                lutpos ++;
                break;
                
            case 9:
                valcnt = LoadLine(file, dblbuf, chbuf, size, explen);
                if (valcnt < 1) goto labError;
                blendSrc4Size = valcnt;
                blendSrc4 = new unsigned char[valcnt];
                for (i = 0; i < valcnt; i ++) {
                    blendSrc4[i] = static_cast<unsigned char>(dblbuf[i] * 256);
                }
                
                lutpos ++;
                break;
        }
    }

    file.close();

    valcnt = idxDestSize;
    if (idxSrc1Size != valcnt ||
        idxSrc2Size != valcnt ||
        idxSrc3Size != valcnt ||
        idxSrc4Size != valcnt ||
        blendSrc1Size != valcnt ||
        blendSrc2Size != valcnt ||
        blendSrc3Size != valcnt ||
        blendSrc4Size != valcnt) goto labError;

    TransposeLUTArray(idxDest, valcnt, Width, Height);
    TransposeLUTArray(idxSrc1, valcnt, Width, Height);
    TransposeLUTArray(idxSrc2, valcnt, Width, Height);
    TransposeLUTArray(idxSrc3, valcnt, Width, Height);
    TransposeLUTArray(idxSrc4, valcnt, Width, Height);

    for (i = 0; i < valcnt; i ++) {
        idxDest[i] *= 3;
        idxSrc1[i] *= 3;
        idxSrc2[i] *= 3;
        idxSrc3[i] *= 3;
        idxSrc4[i] *= 3;
    }

    if (dblbuf) delete [] dblbuf;
    if (chbuf) delete [] chbuf;

    return true;

labError:
    if (file.is_open()) file.close();

    Release();

    if (dblbuf) delete [] dblbuf;
    if (chbuf) delete [] chbuf;

    return false;
}

/**************************************************************************************************
* SetFromCameraCalibration					
*	Generates the lookup table from the same parameters as the Maltab function rectindex.m	
*
* Input:
*	height	unsigned int					- pixel height of the image
*	width	unsigned int					- pixel width of the image
*	R		vct3x3							- rotation matrix
*	f		vct2							- is the focal length
*	c		vct2							- is the camera center
*	k		vctFixedSizeVector<double,5>	- distortion coefficents
*	alpha:	double							- skew
*	KK_new	vct3x3							- camera model of the target camera
*	videoch	unsigned int					- the video channal for which this table is going to be used.
*	
* Output:
*	bool									- true for success, false otherwise
*
*
* Last Change, S. Schafer, 2011/05/17, added Thin Prism Distortion
***********************************************************************************************************/
bool svlImageProcessingHelper::RectificationInternals::SetFromCameraCalibration(unsigned int height,unsigned int width,vct3x3 R,vct2 f, vct2 c, vctFixedSizeVector<double,7> k, double alpha, unsigned int videoch)
{

	//==============Setup, Variables==============//
    Release();
	bool debug = false;

    const unsigned int maxwidth = 1920;
    const unsigned int maxheight = 1200;
    const unsigned int size = maxwidth * maxheight;
    int valcnt, i;
	vct3x3 KK_new = vct3x3::Eye();

	KK_new.at(0,0) = f(0);
	KK_new.at(0,1) = 0;
	KK_new.at(0,2) = c(0);
	KK_new.at(1,0) = 0;
	KK_new.at(1,1) = f(1);
	KK_new.at(1,2) = c(1);
	KK_new.at(2,0) = 0;
	KK_new.at(2,1) = 0;
	KK_new.at(2,2) = 1;

	vctDynamicMatrixRef<double> mx,my,KK_new_inv,R_ref;
	vctDynamicVectorRef<double> rays1,rays2,rays3,rows,cols,x1,x2;
	vctVec px,py,ones_rows,ones_cols,px2,py2,px_0,py_0,alpha_x,alpha_y,alpha_x_neg,alpha_y_neg,a1,a2,a3,a4,ind_1,ind_2,ind_3,ind_4,ind_new,px2_good,py2_good,px_0_good,py_0_good,px_good,py_good;
	vctMat rays,x,xd;
	vctDynamicVector<vctVec::index_type> good_points;
	std::vector<vctVec::index_type> good_points_temp;

	if(debug){
		printf("=========svlImageProcessingHelper::RectificationInternals::SetFromCameraCalibration: parameters ==========\n");
		printf("width: %d height: %d f:(%8.8lf,%8.8lf) c:(%8.8lf,%8.8lf) alpha:%8.8lf KK_new([%8.8lf,%8.8lf,%8.8lf,%8.8lf,%8.8lf,%8.8lf,%8.8lf,%8.8lf,%8.8lf], channel:%d\n",
			width,height,f(0),f(1),c(0),c(1), alpha, KK_new.at(0,0),KK_new.at(0,1),KK_new.at(0,2),KK_new.at(1,0),KK_new.at(1,1),KK_new.at(1,2),KK_new.at(2,0),KK_new.at(2,1),KK_new.at(2,2),videoch);
	}
	px.SetSize(height*width);
	py.SetSize(height*width);

	//double check strides
	mx.SetRef(height,width,width,1,px.Pointer());
	my.SetRef(height,width,width,1,py.Pointer());

	ones_rows.SetSize(height);
	ones_rows.SetAll(1.0);

	ones_cols.SetSize(width);
	ones_cols.SetAll(1.0);

	//double check strides
	rows.SetRef(height,mx.Pointer());
	cols.SetRef(width,my.Pointer());
	
	for(unsigned int i = 0; i < height || i < width;i++){
		if(i < height)
			rows(i) = i;
		if(i < width)
			cols(i) = i;
	}


	mx.OuterProductOf(ones_rows,cols);
	my.OuterProductOf(rows,ones_cols);


	rays.SetSize(3,height*width);

	//double check strides
    rays1.SetRef(rays.Row(0).size(),rays.Row(0).Pointer(),rays.Row(0).stride());
    rays2.SetRef(rays.Row(1).size(),rays.Row(1).Pointer(),rays.Row(1).stride());
    rays3.SetRef(rays.Row(2).size(),rays.Row(2).Pointer(),rays.Row(2).stride());

	rays1.Assign(px);
	rays2.Assign(py);
	rays3.SetAll(1.0);

	vct3x3 KK_new_inverse;
	KK_new_inverse.SetAll(0.0);
    //need cisst netlib
    //KK_new_inverse.Assign(KK_new);
    //nmrInverse(KK_new_inverse);

	if(KK_new.at(0,0) == 0.0)
	{
		KK_new_inverse.at(0,0) = 0;
	}else{
		KK_new_inverse.at(0,0) = 1/KK_new.at(0,0);
	}
	if(KK_new.at(1,1) == 0.0)
	{
		KK_new_inverse.at(1,1) = 0;
	}else{
		KK_new_inverse.at(1,1) = 1/KK_new.at(1,1);
	}
    KK_new_inverse.at(2,2) = 1;
	if(KK_new.at(0,2) == 0.0)
	{
		KK_new_inverse.at(0,2) = 0;
	}else{
		KK_new_inverse.at(0,2) = (-1/KK_new.at(0,0)) * KK_new.at(0,2);
	} 
	if(KK_new.at(1,2) == 0.0)
	{
		KK_new_inverse.at(1,2) = 0;
	}else{
		KK_new_inverse.at(1,2) = (-1/KK_new.at(1,1)) * KK_new.at(1,2);;
	}

	KK_new_inv.SetRef(KK_new_inverse);

	rays.Assign(KK_new_inv*rays);

	R_ref.SetRef(R);

	rays.Assign(R_ref.TransposeRef()*rays);

	x.SetSize(2,height*width);

	//Double Check strides
    x1.SetRef(x.Row(0).size(),x.Row(0).Pointer(),x.Row(0).stride());
    x2.SetRef(x.Row(1).size(),x.Row(1).Pointer(),x.Row(1).stride());

	x1.Assign(rays1.ElementwiseDivide(rays3));
	x2.Assign(rays2.ElementwiseDivide(rays3));

	//==============Distortion calculation==============//
#pragma region DISTORTION

	vctVec r2, r4, r6, kr2, kr4, kr6, x1squared, x2squared,cdist, ones2x1, a1tangential, a1tangential2, a2tangential, a3tangential, deltaX1, deltaX2;

	vctVec du_TP, dv_TP; //for thisn prism -SS

	vctMat xdtemp;

	xdtemp.SetSize(2,height*width);

	r2.SetSize(x1.size());
	r4.SetSize(x1.size());
	r6.SetSize(x1.size());
	kr2.SetSize(x1.size());
	kr4.SetSize(x1.size());
	kr6.SetSize(x1.size());
	x1squared.SetSize(x1.size());
	x2squared.SetSize(x2.size());
	cdist.SetSize(x1.size());
	a1tangential.SetSize(x1.size());
	a1tangential2.SetSize(x1.size());
	a2tangential.SetSize(x1.size());
	a3tangential.SetSize(x1.size());
	deltaX1.SetSize(x1.size());
	deltaX2.SetSize(x1.size());

	//added -SS
	du_TP.SetSize(x1.size());
	dv_TP.SetSize(x2.size());
	//~added


	//% Add distortion
	//r2 = x(1,:).^2 + x(2,:).^2;
	x1squared.Assign(x1);
	x1squared.ElementwiseMultiply(x1);

	x2squared.Assign(x2);
	x2squared.ElementwiseMultiply(x2);

	r2.Assign(x1squared);
	r2.Add(x2squared);

	//r4 = r2.^2;
	r4.Assign(r2);
	r4.ElementwiseMultiply(r2);

	//r6 = r2.^3;
	r6.Assign(r2);
	r6.ElementwiseMultiply(r2);
	r6.ElementwiseMultiply(r2);

	//% Radial distortion:
	//cdist = 1 + k(1) * r2 + k(2) * r4 + k(5) * r6;
	cdist.SetAll(1.0);
	kr2.Assign(r2);
	kr2.Multiply(k[0]);
	kr4.Assign(r4);
	kr4.Multiply(k[1]);
	kr6.Assign(r6);
	kr6.Multiply(k[4]);

	cdist.Add(kr2 + kr4 + kr6);

	ones2x1.SetSize(2);
	ones2x1.SetAll(1.0);

	//xd1 = x .* (ones(2,1)*cdist);
	xdtemp.OuterProductOf(ones2x1,cdist);

	//MOVE TO AFTER TANGENTIAL DISTORTION DUE TO TANGENTIAL DISTORTION'S DEPENDENCE ON ORIGINAL x for x1, x2
	//xd1 = x .* (ones(2,1)*cdist);
	//x.ElementwiseMultiply(xdtemp);

	if(debug)
	{
		for(unsigned int i = 0; i < rays1.size();i++)
		{
			if(debug && (i<10 || i > rays1.size()-10))
			{
				printf("SetFromCameraCalibration after distortion at %d x1,x2 (%8.8lf,%8.8lf)\n",i,x1.at(i),x2.at(i));
			}
		}
	}

	//tangential distortion:
	//a1 = 2.*x(1,:).*x(2,:);
	a1tangential.SetAll(0.0);
	a1tangential.Assign(x2);
	a1tangential.ElementwiseMultiply(x1);
	a1tangential.Multiply(2.0);
	a1tangential2.SetAll(0.0);
	a1tangential2.Assign(x2);
	a1tangential2.ElementwiseMultiply(x1);
	a1tangential2.Multiply(2.0);

	//a2 = r2 + 2*x(1,:).^2;
	a2tangential.SetAll(0.0);
	a2tangential.Assign(x1);
	a2tangential.ElementwiseMultiply(x1);
	a2tangential.Multiply(2.0);
	a2tangential.Add(r2);

	//a3 = r2 + 2*x(2,:).^2;
	a3tangential.SetAll(0.0);
	a3tangential.Assign(x2);
	a3tangential.ElementwiseMultiply(x2);
	a3tangential.Multiply(2.0);
	a3tangential.Add(r2);

	//delta_x = [k(3)*a1 + k(4)*a2 ;
	//		   k(3) * a3 + k(4)*a1];
	deltaX1.Assign(a2tangential.Multiply(k[3]));
	deltaX1.Add(a1tangential.Multiply(k[2]));
	deltaX2.Assign(a1tangential2.Multiply(k[3]));

	deltaX2.Add(a3tangential.Multiply(k[2]));

	//before we apply all this we have to determined the thin prism distortion -SS
	//this prism distortion: du = s1*(u^2+v^2), dv = s2*(u^2+v^2)

	//ThinPrism -SS
	du_TP.SetAll(0.0);
	du_TP.Assign(r2);
	du_TP.Multiply(k[5]);

	dv_TP.SetAll(0.0);
	dv_TP.Assign(r2);
	dv_TP.Multiply(k[6]);
	//~ThinPrism


	//MOVED FROM RADIAL DISTORTION
	//xd1 = x .* (ones(2,1)*cdist);
	x.ElementwiseMultiply(xdtemp);

	//xd = xd1 + delta_x;
	x1.Add(deltaX1);
	x2.Add(deltaX2);

	//Add ThinPrism to distortion -SS
	x1.Add(du_TP);
	x2.Add(dv_TP);
	//~Add

#pragma endregion DISTORTION
	//==============Distortion calculation==============//

	px2.SetSize(x1.size());
	py2.SetSize(x2.size());

	//reconvert in pixels
	//px2.Assign(x1.Add(x2.Multiply(alpha)).Multiply(f(0)).Add(c(0)));
	px2.Assign(x2);
	px2.Multiply(alpha);
	px2.Add(x1);
	px2.Multiply(f(0));
	px2.Add(c(0));

	py2.Assign(x2);
	py2.Multiply(f(1));
	py2.Add(c(1));

	//Interpolate between the closest pixels:
	px_0.SetSize(px2.size());
	py_0.SetSize(py2.size());

	px_0.Assign(px2);
	py_0.Assign(py2);

	px_0.FloorSelf();
	py_0.FloorSelf();


	//Create good_points
	if(debug)
		printf("SetFromCameraCalibration: Checking for good points\n");
	for(unsigned int i = 0; i < px_0.size();i++)
	{
		if(debug && (i<10 || i > rays1.size()-10))
		{
			printf("SetFromCameraCalibration at %d x1,x2 (%8.8lf,%8.8lf)\n",i,x1.at(i),x2.at(i));
			printf("SetFromCameraCalibration at %d px2,py2 (%8.8lf,%8.8lf)\n",i,px2.at(i),py2.at(i));
			printf("SetFromCameraCalibration at %d px_0,py_0 (%8.8lf,%8.8lf)\n",i,px_0.at(i),py_0.at(i));
		}
		if((px_0.at(i) >= 0) && (px_0.at(i) <= (width-2)) && (py_0.at(i) >= 0) && (py_0.at(i) <= (height-2))){
				good_points_temp.push_back(i);
		}
	}

	good_points.SetSize(good_points_temp.size());

	for(unsigned int i = 0; i < good_points.size();i++)
	{
		good_points[i] = good_points_temp[i];
	}

	if(debug)
	{
		printf("SetFromCameraCalibration computed %d good points\n", good_points.size());
	}

	//Use good_points
	px2_good.SetSize(good_points.size());
	py2_good.SetSize(good_points.size());
	px_0_good.SetSize(good_points.size());
	py_0_good.SetSize(good_points.size());

	px2_good.SelectFrom(px2,good_points);
	py2_good.SelectFrom(py2,good_points);

	px_0_good.SelectFrom(px_0,good_points);
	py_0_good.SelectFrom(py_0,good_points);


	alpha_x.SetSize(px2_good.size());
	alpha_y.SetSize(py2_good.size());

	alpha_x_neg.SetSize(px2_good.size());
	alpha_y_neg.SetSize(px2_good.size());

	alpha_x.Assign(px2_good.Subtract(px_0_good));
    alpha_y.Assign(py2_good.Subtract(py_0_good));

	alpha_x_neg.Assign(alpha_x.Negation());
	alpha_x_neg.Add(1);

	alpha_y_neg.Assign(alpha_y.Negation());
	alpha_y_neg.Add(1);

	a1.SetSize(px2_good.size());
	a2.SetSize(px2_good.size());
	a3.SetSize(px2_good.size());
	a4.SetSize(px2_good.size());

	a1.Assign(alpha_y_neg);
	a1.ElementwiseMultiply(alpha_x_neg);

	a2.Assign(alpha_y_neg);
	a2.ElementwiseMultiply(alpha_x);

	a3.Assign(alpha_y);
	a3.ElementwiseMultiply(alpha_x_neg);

	a4.Assign(alpha_y);
	a4.ElementwiseMultiply(alpha_x);

	ind_1.SetSize(px2_good.size());
	ind_2.SetSize(px2_good.size());
	ind_3.SetSize(px2_good.size());
	ind_4.SetSize(px2_good.size());

	ind_1.Assign(px_0_good);
	ind_1.Multiply(height);
	ind_1.Add(py_0_good);
	
	ind_2.Assign(px_0_good);
	ind_2.Add(1);
	ind_2.Multiply(height);
	ind_2.Add(py_0_good);

	ind_3.Assign(px_0_good);
	ind_3.Multiply(height);
	ind_3.Add(py_0_good);
	ind_3.Add(1);

	ind_4.Assign(px_0_good);
	ind_4.Add(1);
	ind_4.Multiply(height);
	ind_4.Add(py_0_good);
	ind_4.Add(1);

	px_good.SetSize(good_points.size());
	py_good.SetSize(good_points.size());

	px_good.SelectFrom(px,good_points);
	py_good.SelectFrom(py,good_points);

	ind_new.SetSize(good_points.size());

	ind_new.Assign(px_good);
	ind_new.Multiply(height);
	ind_new.Add(py_good);

	//debugging
	if(debug)
	{

		for(unsigned int i = 0; i < ind_new.size();i++)
		{
			if(debug && (i<10 || i > ind_new.size()-10))
			{
				printf("SetFromCameraCalibration %d ind_new: %8.8lf\n",i,ind_new.at(i));
			}
		}
		for(unsigned int i = 0; i < ind_1.size();i++)
		{
			if(debug && (i<10 || i > ind_1.size()-10))
			{
				printf("SetFromCameraCalibration %d ind_1: %8.8lf\n",i,ind_1.at(i));
			}
		}
		for(unsigned int i = 0; i < ind_2.size();i++)
		{
			if(debug && (i<10 || i > ind_2.size()-10))
			{
				printf("SetFromCameraCalibration %d ind_2: %8.8lf\n",i,ind_2.at(i));
			}
		}
		for(unsigned int i = 0; i < ind_3.size();i++)
		{
			if(debug && (i<10 || i > ind_3.size()-10))
			{
				printf("SetFromCameraCalibration %d ind_3: %8.8lf\n",i,ind_3.at(i));
			}
		}
		for(unsigned int i = 0; i < ind_4.size();i++)
		{
			if(debug && (i<10 || i > ind_4.size()-10))
			{
				printf("SetFromCameraCalibration %d ind_4: %8.8lf\n",i,ind_4.at(i));
			}
		}
		for(unsigned int i = 0; i < a1.size();i++)
		{
			if(debug && (i<10 || i > a1.size()-10))
			{
				printf("SetFromCameraCalibration %d a1: %8.8lf\n",i,a1.at(i));
			}
		}
		for(unsigned int i = 0; i < a2.size();i++)
		{
			if(debug && (i<10 || i > a2.size()-10))
			{
				printf("SetFromCameraCalibration %d a2: %8.8lf\n",i,a2.at(i));
			}
		}
		for(unsigned int i = 0; i < a3.size();i++)
		{
			if(debug && (i<10 || i > a3.size()-10))
			{
				printf("SetFromCameraCalibration %d a3: %8.8lf\n",i,a3.at(i));
			}
		}

		for(unsigned int i = 0; i < a4.size();i++)
		{
			if(debug && (i<10 || i > a4.size()-10))
			{
				printf("SetFromCameraCalibration %d a4: %8.8lf\n",i,a4.at(i));
			}
		}
	}

	//========SetTable(height,width,ind_new,ind_1,ind_2,ind_3,ind_4,a1,a2,a3,a4,videoch);========//
	Height = height;
	Width = width;
	if (Width > maxwidth || Height > maxheight) goto labError;
               
	//this is not efficient 
	valcnt = static_cast<int>(ind_new.size());

	idxDestSize = static_cast<int>(ind_new.size());
    idxDest = new unsigned int[ind_new.size()];

	idxSrc1Size = static_cast<int>(ind_1.size());
    idxSrc1 = new unsigned int[ind_1.size()];

	idxSrc2Size = static_cast<int>(ind_2.size());
    idxSrc2 = new unsigned int[ind_2.size()];

	idxSrc3Size = static_cast<int>(ind_3.size());
    idxSrc3 = new unsigned int[ind_3.size()];

	idxSrc4Size = static_cast<int>(ind_4.size());
    idxSrc4 = new unsigned int[ind_4.size()];

	blendSrc1Size = static_cast<int>(a1.size());
    blendSrc1 = new unsigned char[a1.size()];

	blendSrc2Size = static_cast<int>(a2.size());
    blendSrc2 = new unsigned char[a2.size()];

	blendSrc3Size = static_cast<int>(a3.size());
    blendSrc3 = new unsigned char[a3.size()];

	blendSrc4Size = static_cast<int>(a4.size());
    blendSrc4 = new unsigned char[a4.size()];

    for (i = 0; i < valcnt; i ++) {
        idxDest[i] = static_cast<unsigned int>(ind_new[i] + 0.5);

        idxSrc1[i] = static_cast<unsigned int>(ind_1[i] + 0.5);
		idxSrc2[i] = static_cast<unsigned int>(ind_2[i] + 0.5);
		idxSrc3[i] = static_cast<unsigned int>(ind_3[i] + 0.5);
		idxSrc4[i] = static_cast<unsigned int>(ind_4[i] + 0.5);

		blendSrc1[i] = static_cast<unsigned char>(a1[i] * 256);
		blendSrc2[i] = static_cast<unsigned char>(a2[i] * 256);
		blendSrc3[i] = static_cast<unsigned char>(a3[i] * 256);
		blendSrc4[i] = static_cast<unsigned char>(a4[i] * 256);
    }

	TransposeLUTArray2(idxDest, valcnt, Width, Height);
    TransposeLUTArray2(idxSrc1, valcnt, Width, Height);
    TransposeLUTArray2(idxSrc2, valcnt, Width, Height);
    TransposeLUTArray2(idxSrc3, valcnt, Width, Height);
    TransposeLUTArray2(idxSrc4, valcnt, Width, Height);

    for (i = 0; i < valcnt; i ++) {
        idxDest[i] *= 3;
        idxSrc1[i] *= 3;
        idxSrc2[i] *= 3;
        idxSrc3[i] *= 3;
        idxSrc4[i] *= 3;
    }

    return true;

labError:
    Release();

    return false;

}

void svlImageProcessingHelper::RectificationInternals::TransposeLUTArray2(unsigned int* index, unsigned int size, unsigned int width, unsigned int height)
{
    unsigned int i, x, y, val;

    for (i = 0; i < size; i ++) {
		val = index[i];
        x = val / height;
        y = val % height;
        index[i] = y * width + x;
    }
}

int svlImageProcessingHelper::RectificationInternals::LoadLine(std::ifstream &file, double* dblbuf, char* chbuf, unsigned int size, int explen)
{
    unsigned int bufsize = (16 * size) + 1; // max text line length
    unsigned int datalen; // actual data size
    unsigned int bufpos = 0;
    unsigned int filepos;
    char* tbuf;
    char ch;
    int ival;
    int counter = 0;
    int linebreak = 0;
    double dbl;
    double negexp[] = {1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0.00000001, 0.000000001};
	double negexpTens[] = {1.0, 0.0000000001, 0.00000000001, 0.000000000001, 0.000000000001, 0.000000000001, 0.000000000001, 0.000000000001, 0.000000000001, 0.0000000000001, 0.0000000000001};
    double posexp[] = {1.0, 10.0, 100.0, 1000.0, 10000.0, 100000.0, 1000000.0, 10000000.0, 100000000.0, 1000000000.0};
    
    filepos = static_cast<unsigned int>(file.tellg());
    file.read(chbuf, bufsize);
    datalen = static_cast<unsigned int>(file.gcount());
    file.clear();
    
    if (explen == 3) {
        while (datalen > 16 ) {
            bufpos += 2;
            
            tbuf = chbuf + bufpos;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // ?.xxxxxxxes00x
            dbl = ch;
            
            tbuf += 2;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.?xxxxxxes00x
            dbl += 0.1 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.x?xxxxxes00x
            dbl += 0.01 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xx?xxxxes00x
            dbl += 0.001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxx?xxxes00x
            dbl += 0.0001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxxx?xxes00x
            dbl += 0.00001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxxxx?xes00x
            dbl += 0.000001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxxxxx?es00x
            dbl += 0.0000001 * ch;
            
            tbuf += 2;
            
            // x.xxxxxxxe?00x
            if (*tbuf == '-') {
                tbuf += 3;
                ival = *tbuf - 48; if (ival < 0 || ival > 9) break;
                
                // x.xxxxxxxe-00?
                dbl *= negexp[ival];
            }
            else {
                tbuf += 3;
                ival = *tbuf - 48; if (ival < 0 || ival > 9) break;
                
                // x.xxxxxxxe+00?
                dbl *= posexp[ival];
            }
            
            dblbuf[counter] = dbl;
            bufpos += 14;
            counter ++;
            datalen -= 16;
            
            // if end of line, seek back
            ch = chbuf[bufpos];
            if (ch == 0x0a) { // LF
                file.seekg(filepos + bufpos + 1);
                linebreak = 1;
                break;
            }
            else if (ch == 0x0d) { // CRLF
                file.seekg(filepos + bufpos + 2);
                linebreak = 1;
                break;
            }
        }
    }
    else if (explen == 2) {
        while (datalen > 16 ) {
            bufpos += 3;
            
            tbuf = chbuf + bufpos;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // ?.xxxxxxxes0x
            dbl = ch;
            
            tbuf += 2;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.?xxxxxxes0x
            dbl += 0.1 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.x?xxxxxes0x
            dbl += 0.01 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xx?xxxxes0x
            dbl += 0.001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxx?xxxes0x
            dbl += 0.0001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxxx?xxes0x
            dbl += 0.00001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxxxx?xes0x
            dbl += 0.000001 * ch;
            
            tbuf ++;
            ch = *tbuf - 48; if (ch < 0 || ch > 9) break;
            
            // x.xxxxxx?es0x
            dbl += 0.0000001 * ch;
            
            tbuf += 2;
            
            // x.xxxxxxxe?0x
            if (*tbuf == '-') {
                tbuf ++;
                ival = *tbuf - 48; if (ival < 0 || ival > 9) break;
                
                // x.xxxxxxxe-?x
                dbl *= negexpTens[ival];
                
				tbuf ++;
                ival = *tbuf - 48; if (ival < 0 || ival > 9) break;
                
                // x.xxxxxxxe-x?
                dbl *= negexp[ival];
            }
            else {
                tbuf += 2;
                ival = *tbuf - 48; if (ival < 0 || ival > 9) break;
                
                // x.xxxxxxxe+0?
                dbl *= posexp[ival];
            }
            
            dblbuf[counter] = dbl;
            bufpos += 13;
            counter ++;
            datalen -= 16;
            
            // if end of line, seek back
            ch = chbuf[bufpos];
            if (ch == 0x0a) { // LF
                file.seekg(filepos + bufpos + 1);
                linebreak = 1;
                break;
            }
            else if (ch == 0x0d) { // CRLF
                file.seekg(filepos + bufpos + 2);
                linebreak = 1;
                break;
            }
        }
    }

    if (linebreak == 1) return counter;
    return 0;
}

void svlImageProcessingHelper::RectificationInternals::TransposeLUTArray(unsigned int* index, unsigned int size, unsigned int width, unsigned int height)
{
    unsigned int i, x, y, val;

    for (i = 0; i < size; i ++) {
        val = index[i] - 1;
        x = val / height;
        y = val % height;
        index[i] = y * width + x;
    }
}

void svlImageProcessingHelper::RectificationInternals::Release()
{
    if (idxDest) delete [] idxDest;
    if (idxSrc1) delete [] idxSrc1;
    if (idxSrc2) delete [] idxSrc2;
    if (idxSrc3) delete [] idxSrc3;
    if (idxSrc4) delete [] idxSrc4;
    if (blendSrc1) delete [] blendSrc1;
    if (blendSrc2) delete [] blendSrc2;
    if (blendSrc3) delete [] blendSrc3;
    if (blendSrc4) delete [] blendSrc4;

    Width = 0;
    Height = 0;
    idxDest = 0;
    idxDestSize = 0;
    idxSrc1 = 0;
    idxSrc1Size = 0;
    idxSrc2 = 0;
    idxSrc2Size = 0;
    idxSrc3 = 0;
    idxSrc3Size = 0;
    idxSrc4 = 0;
    idxSrc4Size = 0;
    blendSrc1 = 0;
    blendSrc1Size = 0;
    blendSrc2 = 0;
    blendSrc2Size = 0;
    blendSrc3 = 0;
    blendSrc3Size = 0;
    blendSrc4 = 0;
    blendSrc4Size = 0;
}


/*********************************************************/
/*** svlImageProcessingHelper::ExposureInternals class ***/
/*********************************************************/

svlImageProcessingHelper::ExposureInternals::ExposureInternals() :
    svlImageProcessingInternals(),
    Brightness(0.0),
    Contrast(0.0),
    Gamma(0.0),
    Modified(true)
{
    Curve.SetAll(0);
}

void svlImageProcessingHelper::ExposureInternals::SetBrightness(double brightness)
{
    if (brightness > 100.0) brightness = 100.0;
    else  if (brightness < -100.0) brightness = -100.0;

    if (Brightness != brightness) {
        Brightness = brightness;
        Modified = true;
    }
}

double svlImageProcessingHelper::ExposureInternals::GetBrightness()
{
    return Brightness;
}

void svlImageProcessingHelper::ExposureInternals::SetContrast(double contrast)
{
    if (contrast > 100.0) contrast = 100.0;
    else  if (contrast < -100.0) contrast = -100.0;

    if (Contrast != contrast) {
        Contrast = contrast;
        Modified = true;
    }
}

double svlImageProcessingHelper::ExposureInternals::GetContrast()
{
    return Contrast;
}

void svlImageProcessingHelper::ExposureInternals::SetGamma(double gamma)
{
    if (gamma > 100.0) gamma = 100.0;
    else  if (gamma < -100.0) gamma = -100.0;

    if (Gamma != gamma) {
        Gamma = gamma;
        Modified = true;
    }
}

double svlImageProcessingHelper::ExposureInternals::GetGamma()
{
    return Gamma;
}

void svlImageProcessingHelper::ExposureInternals::CalculateCurve()
{
    if (!Modified) return;

    const double cntrst = pow(10.0, Contrast / 100.0);
    const double gmma = pow(10.0, -Gamma / 100.0);
    const double scale100 = 2.55;
    double dbval;
    int result;

    for (unsigned int i = 0; i < 256; i ++) {
        dbval = (Brightness * scale100 + cntrst * i) / 255.0;
        result = static_cast<int>(pow(dbval, gmma) * 255.0 + 0.5);

        if (result < 0) result = 0;
        else if (result > 255) result = 255;
        Curve[i] = result;
    }

    Modified = false;
}

/*************************************************************/
/*** svlImageProcessingHelper::BlobDetectorInternals class ***/
/*************************************************************/

svlImageProcessingHelper::BlobDetectorInternals::BlobDetectorInternals() :
    svlImageProcessingInternals(),
    BlobCount(0)
{
}

unsigned int svlImageProcessingHelper::BlobDetectorInternals::CalculateLabels(const svlSampleImageMono8* image,
                                                                              svlSampleImageMono32* labels)
{
    return CalculateLabelsInternal(const_cast<svlSampleImageMono8*>(image),
                                   labels,
                                   SVL_LEFT);
}

unsigned int svlImageProcessingHelper::BlobDetectorInternals::CalculateLabels(const svlSampleImageMono8Stereo* image,
                                                                              svlSampleImageMono32Stereo* labels,
                                                                              const unsigned int videoch)
{
    return CalculateLabelsInternal(const_cast<svlSampleImageMono8Stereo*>(image),
                                   labels,
                                   videoch);
}

bool svlImageProcessingHelper::BlobDetectorInternals::GetBlobs(const svlSampleImageMono8* image,
                                                               const svlSampleImageMono32* labels,
                                                               svlSampleBlobs* blobs,
                                                               unsigned int min_area,
                                                               unsigned int max_area,
                                                               double min_compactness,
                                                               double max_compactness)
{
    return GetBlobsInternal(const_cast<svlSampleImageMono8*>(image),
                            const_cast<svlSampleImageMono32*>(labels),
                            blobs,
                            SVL_LEFT,
                            min_area,
                            max_area,
                            min_compactness,
                            max_compactness);
}

bool svlImageProcessingHelper::BlobDetectorInternals::GetBlobs(const svlSampleImageMono8Stereo* image,
                                                               const svlSampleImageMono32Stereo* labels,
                                                               svlSampleBlobs* blobs,
                                                               const unsigned int videoch,
                                                               unsigned int min_area,
                                                               unsigned int max_area,
                                                               double min_compactness,
                                                               double max_compactness)
{
    return GetBlobsInternal(const_cast<svlSampleImageMono8Stereo*>(image),
                            const_cast<svlSampleImageMono32Stereo*>(labels),
                            blobs,
                            videoch,
                            min_area,
                            max_area,
                            min_compactness,
                            max_compactness);
}

unsigned int svlImageProcessingHelper::BlobDetectorInternals::CalculateLabelsInternal(svlSampleImage* image,
                                                                                      svlSampleImage* labels,
                                                                                      const unsigned int videoch)
{
    if (!image || !labels || videoch >= image->GetVideoChannels()) return 0;

    const int width  = static_cast<int>(image->GetWidth(videoch));
    const int height = static_cast<int>(image->GetHeight(videoch));

    unsigned int cbufsize = width * height / 2;
    if (ContourBuffer.cols() < cbufsize) ContourBuffer.SetSize(4, cbufsize);

    int *fx  = ContourBuffer.Pointer();
    int *fy  = fx + cbufsize;
    int *fnx = fy + cbufsize;
    int *fny = fnx + cbufsize;
    unsigned int fu, fnu;

    unsigned int *blobids = reinterpret_cast<unsigned int*>(labels->GetUCharPointer(videoch));
    memset(blobids, 0, labels->GetDataSize(videoch));

    unsigned char *imgbuf = const_cast<unsigned char*>(image->GetUCharPointer(videoch));
    const int width_m1  = width - 1;
    const int height_m1 = height - 1;

    const unsigned int FBSm1  = cbufsize - 1;
    unsigned int k, off, off2, imgval, c = 0, maxblobcount = 1;
    int i, j, x, y;
    int *tptr;


    for (j = 0; j < height; j ++) {
        for (i = 0; i < width; i ++) {
            imgval = imgbuf[c];

            if (imgval == 0 || blobids[c] != 0) {
                c ++;
                continue;
            }

            // Fill current pixel
            *fnx = i; *fny = j; fnu = 1;
            blobids[c] = maxblobcount;

            while (fnu) {
                // Swap fill buffers
                tptr = fx; fx = fnx; fnx = tptr;
                tptr = fy; fy = fny; fny = tptr;
                fu = fnu; fnu = 0;

                for (k = 0; k < fu; k ++) {
                    x = fx[k]; y = fy[k];
                    off = y * width + x;

                    // Check left neighbor
                    if (x > 0) {
                        off2 = off - 1;
                        if (blobids[off2] == 0 && imgbuf[off2] == imgval && fnu < FBSm1) {
                            // Fill pixel
                            fnx[fnu] = x - 1; fny[fnu] = y; fnu ++;
                            blobids[off2] = maxblobcount;
                        }
                    }

                    // Check right neighbor
                    if (x < width_m1) {
                        off2 = off + 1;
                        if (blobids[off2] == 0 && imgbuf[off2] == imgval && fnu < FBSm1) {
                            // Fill pixel
                            fnx[fnu] = x + 1; fny[fnu] = y; fnu ++;
                            blobids[off2] = maxblobcount;
                        }
                    }

                    // Check top neighbor
                    if (y > 0) {
                        off2 = off - width;
                        if (blobids[off2] == 0 && imgbuf[off2] == imgval && fnu < FBSm1) {
                            // Fill pixel
                            fnx[fnu] = x; fny[fnu] = y - 1; fnu ++;
                            blobids[off2] = maxblobcount;
                        }
                    }

                    // Check bottom neighbor
                    if (y < height_m1) {
                        off2 = off + width;
                        if (blobids[off2] == 0 && imgbuf[off2] == imgval && fnu < FBSm1) {
                            // Fill pixel
                            fnx[fnu] = x; fny[fnu] = y + 1; fnu ++;
                            blobids[off2] = maxblobcount;
                        }
                    }
                }
            }

            maxblobcount ++;
            c ++;
        }
    }

    BlobCount = maxblobcount - 1;
    return BlobCount;
}

bool svlImageProcessingHelper::BlobDetectorInternals::GetBlobsInternal(svlSampleImage* image,
                                                                       svlSampleImage* labels,
                                                                       svlSampleBlobs* blobs,
                                                                       const unsigned int videoch,
                                                                       const unsigned int min_area,
                                                                       const unsigned int max_area,
                                                                       const double min_compactness,
                                                                       const double max_compactness)
{
    if (!image || !labels || !blobs ||
        videoch >= image->GetVideoChannels() ||
        videoch >= blobs->GetChannelCount()) return false;

    const unsigned int blobsbuffsize = blobs->GetBufferSize();
    const unsigned int maxblobcount = std::min(BlobCount, blobsbuffsize);
    unsigned int *blobids = reinterpret_cast<unsigned int*>(labels->GetUCharPointer(videoch));
    unsigned char *imgbuf = image->GetUCharPointer(videoch);
    svlBlob *blbbuf = blobs->GetBlobsPointer(videoch);
    const int width  = static_cast<int>(image->GetWidth(videoch));
    const int height = static_cast<int>(image->GetHeight(videoch));
    const int width_m1  = width - 1;
    const int height_m1 = height - 1;

    bool do_filtering = false;
    double compactness, db_area, db_circumference;
    unsigned int k;
    svlBlob *blob;
    int i, j;


    blob = blbbuf;
    for (k = 0; k < maxblobcount; k ++) {
        blob->ID            = k + 1;
        blob->used          = true;
        blob->left          = 100000;
        blob->right         = -1;
        blob->top           = 100000;
        blob->bottom        = -1;
        blob->center_x      = 0;
        blob->center_y      = 0;
        blob->area          = 0;
        blob->circumference = 0;
        blob->label         = 0;
        blob ++;
    }

    for (j = 0; j < height; j ++) {
        for (i = 0; i < width; i ++) {

            k = *blobids;
            if (k > 0 && k <= maxblobcount) {

                blob = blbbuf + k - 1;

                // Bounding rectangle
                if (i < blob->left) blob->left = i;
                if (i > blob->right) blob->right = i;
                if (j < blob->top) blob->top = j;
                if (j > blob->bottom) blob->bottom = j;

                // Center of weight
                blob->center_x += i;
                blob->center_y += j;

                // Area
                blob->area ++;

                // Circumference
                if ((i > 0         && blobids[    -1] != k) ||
                    (i < width_m1  && blobids[     1] != k) ||
                    (j > 0         && blobids[-width] != k) ||
                    (j < height_m1 && blobids[ width] != k)) {
                    blob->circumference ++;
                }

                // Label
                if (blob->label == 0) blob->label = *imgbuf;
            }

            imgbuf ++;
            blobids ++;
        }
    }

    blob = blbbuf;
    for (k = 0; k < maxblobcount; k ++) {
        blob->center_x /= blob->area;
        blob->center_y /= blob->area;
        blob ++;
    }

    if (min_area > 0 || max_area > 0) {
        do_filtering = true;

        blob = blbbuf;
        for (k = 0; k < maxblobcount; k ++) {
            if ((max_area && blob->area > max_area) || blob->area < min_area) {
                blob->used = false;
            }
            blob ++;
        }
    }

    if (min_compactness > 0.0 || max_compactness > 0.0) {
        do_filtering = true;

        blob = blbbuf;
        for (k = 0; k < maxblobcount; k ++) {

            // r=sqrt(area/pi)
            // compactness=(area/circumference)/r
            // compactness=1.0 (if prefect circular disk)
            // compactness<1.0 (if any other shape)

            db_area          = static_cast<double>(blob->area);
            db_circumference = static_cast<double>(blob->circumference);
            compactness = std::min(1.0, db_area / (db_circumference * std::sqrt(db_area * 0.318309886183791)));

            if ((min_compactness > 0.0 && compactness < min_compactness) ||
                (max_compactness > 0.0 && compactness > max_compactness)) {
                blob->used = false;
            }
            blob ++;
        }
    }

    if (do_filtering) {
        blobids = reinterpret_cast<unsigned int*>(labels->GetUCharPointer(videoch));

        for (j = 0; j < height; j ++) {
            for (i = 0; i < width; i ++) {

                k = *blobids;
                if (k > 0 && k <= maxblobcount) {
                    if (blbbuf[k - 1].used == false) *blobids = 0;
                }

                blobids ++;
            }
        }
    }

    blobs->SetBufferUsed(maxblobcount, videoch);

    return true;
}

