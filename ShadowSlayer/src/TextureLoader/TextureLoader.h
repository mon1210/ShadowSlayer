#pragma once

struct IWICImagingFactory;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

typedef enum _FillMode{
	None = 0,
	Linear = 1
} FillMode;

class TextureLoader
{
private:
	TextureLoader(void);
	virtual ~TextureLoader(void);

public:
	static void Initialize();
	static TextureLoader *GetInstance();
	static void Destroy();
	static HRESULT CreateTextureFromFile(ID3D11DeviceContext *pContext, const TCHAR *pFilename, ID3D11Texture2D **ppTexture, DWORD *pSrcWidth, DWORD *pSrcHeight, FillMode dwFillmode = FillMode::None);
	static HRESULT CreateD2D1BitmapFromFile(ID2D1RenderTarget *pRenderTarget,const TCHAR *pFilename, ID2D1Bitmap **ppBitmap);
protected:
	IWICImagingFactory *m_pFactory;
private:
	static TextureLoader *m_pInstance;
};

