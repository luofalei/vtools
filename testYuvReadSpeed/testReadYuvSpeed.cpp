// testReadYuvSpeed.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

static uint8_t yuv_buf[1024 * 1024 * 16 * 8 * 3];  // 16K 10bit

int main(int argc, char * argv[])
{
    const char *input_seq = "demo.yuv";     // argv[1]
    int img_w = 3840;                       // argv[2]
    int img_h = 2160;                       // argv[3]
    int bit_depth = 10;                     // argv[4]
    int frm_size = img_w * img_h * 3;

    clock_t tm_start;
    FILE *fp = NULL;
    int frame_no = 0;

    if (argc >= 2) {
        input_seq = argv[1];
        if (argc >= 4) {
            img_w = atoi(argv[2]);
            img_h = atoi(argv[3]);
        }
        if (argc >= 5) {
            bit_depth = atoi(argv[4]);
        }
    }

    frm_size = img_w * img_h * 3;
    if (bit_depth <= 8) {
        frm_size >>= 1;
    }

    fp = fopen(input_seq, "rb");

    tm_start = clock();
    while (1) {
        double tm_cur;
        if (feof(fp)) {
            break;
        }
        frame_no++;
        if (frm_size != fread(yuv_buf, 1, frm_size, fp)) {
            break;
        }
        tm_cur = (double)(clock() - tm_start) + 1e-9;
        printf("Frame [%4d]: %12.3lf sec, %8.3lf fps\n",
               frame_no, 
               tm_cur / CLOCKS_PER_SEC,
               frame_no * CLOCKS_PER_SEC / tm_cur);
    }

    if (fp != NULL) {
        fclose(fp);
    }

    return 0;
}

