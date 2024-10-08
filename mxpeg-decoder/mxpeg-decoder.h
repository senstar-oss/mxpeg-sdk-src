#pragma once
extern "C"
{
    struct DecodedFrameInfo {
        int height = 0;
        int width = 0;
    };

    __declspec(dllexport) void *mxpeg_decoder_init();
    __declspec(dllexport) DecodedFrameInfo mxpeg_decoder_decode(void *pObject, unsigned char *pIn, int len, unsigned char *pOut, int *outlen);
    __declspec(dllexport) void mxpeg_decoder_deinit(void *pObject);
}
