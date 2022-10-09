#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <signal.h>

#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>

int main(int argc, char** argv) {

    if (argc < 2) {
        perror("incorrect useage");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
