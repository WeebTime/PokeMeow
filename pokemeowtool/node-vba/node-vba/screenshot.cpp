
#include <atlimage.h>
#include "screenshot.h"

size_t get_png_size(CImage *image)
{
	HGLOBAL	hMem = ::GlobalAlloc(GMEM_MOVEABLE, 0);
	if (!hMem) {
		return 0;
	}

	IStream* pstm = NULL;
	HRESULT hr = ::CreateStreamOnHGlobal(hMem, TRUE, &pstm);
	if (FAILED(hr)) {
		GlobalFree(hMem);
		return 0;
	}
	hr = image->Save(pstm, Gdiplus::ImageFormatPNG);
	if (FAILED(hr)) {
		pstm->Release();
		GlobalFree(hMem);
		return 0;
	}

	ULARGE_INTEGER liSize;
	IStream_Size(pstm, &liSize);
	if (liSize.HighPart > 0) {
		return 0;
	}

	pstm->Release();
	GlobalFree(hMem);
	return liSize.LowPart;
}

int window_screenshot(HWND hWnd, unsigned char **buf, size_t *buf_len)
{
	RECT rc = {0}, rcClient;
	// ��ȡ�豸�����Ϣ�ĳߴ��С
	GetWindowRect(hWnd, &rc);
	GetClientRect(hWnd, &rcClient);

	HDC dc = GetDC(NULL); // Ҫ��ͼ�Ĵ��ھ����Ϊ����ȫ��
	if (dc == NULL) {
		return -1;
	}

	int width = rcClient.right - rcClient.left, height = rcClient.bottom - rcClient.top;
	int sidebar = ((rc.right - rc.left - width) / 2);
	int xpos = rc.left + sidebar,
		ypos = rc.top + (rc.bottom - rc.top - height - sidebar);

	//printf("%d %d %d %d\n", xpos, ypos, rc.left, rc.top);

	int nBitPerPixel = GetDeviceCaps(dc, BITSPIXEL);
	CImage *image = new CImage();
	// ����ͼ�����ÿ�ߣ�����
	image->Create(width, height, nBitPerPixel);
	// ��ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת��
	BitBlt(
		image->GetDC(),  // ���浽��Ŀ�� ͼƬ���� ������
		0, 0,           // ��ʼ x, y ����
		width, height,  // ��ͼ���
		dc,             // ��ȡ����� �����ľ��
		xpos, ypos,     // ָ��Դ�����������Ͻǵ� X, y �߼�����
		SRCCOPY);

	// �ͷ� DC���
	ReleaseDC(hWnd, dc);
	// �ͷ�ͼƬ������
	image->ReleaseDC();
	// ����ͼƬ

	//size_t img_size = get_png_size(image);
	HGLOBAL	hMem = ::GlobalAlloc(GMEM_MOVEABLE, 0);
	if (!hMem) {
		delete image;
		return -2;
	}

	IStream* pstm = NULL;
	HRESULT hr = ::CreateStreamOnHGlobal(hMem, TRUE, &pstm);
	if (FAILED(hr)) {
		delete image;
		GlobalFree(hMem);
		return hr;
	}
	hr = image->Save(pstm, Gdiplus::ImageFormatPNG);
	if (FAILED(hr)) {
		pstm->Release();
		delete image;
		GlobalFree(hMem);
		return hr;
	}

	ULARGE_INTEGER liSize;
	IStream_Size(pstm, &liSize);
	if (liSize.HighPart > 0) {
		// too large
		pstm->Release();
		delete image;
		GlobalFree(hMem);
		return 1;
	}

	IStream_Reset(pstm);

	unsigned char *nbuf = (unsigned char *)malloc(liSize.LowPart);
	IStream_Read(pstm, nbuf, liSize.LowPart);
	/*unsigned char *mem = (unsigned char *)GlobalLock(hMem);
	memcpy(nbuf, mem, liSize.LowPart);
	GlobalUnlock(hMem);*/

	*buf = nbuf;
	*buf_len = liSize.LowPart;
	//image->Save(L"D:\\ScreenShot32.png", Gdiplus::ImageFormatPNG);

	//*pimg = image;
	pstm->Release();
	delete image;
	GlobalFree(hMem);
	return 0;
}