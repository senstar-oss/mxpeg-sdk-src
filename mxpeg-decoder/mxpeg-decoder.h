#pragma once
extern "C"
{
    __declspec(dllexport) void *InitMxpeg();

    __declspec(dllexport) void DecodeMxpeg(void *pObject, BYTE *pIn, int len, BYTE *pOut, int *outlen);
    __declspec(dllexport) void UnInitMxpeg(void *pObject);
}
