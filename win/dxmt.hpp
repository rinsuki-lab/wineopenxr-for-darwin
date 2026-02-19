#include <dxgi1_3.h>

// somewhat mingw __uuidof didn't work
static const GUID IID_IMTLDXGIDevice1 = 
    { 0x7d2f767d, 0x2b47, 0x4f04, { 0x86, 0x7c, 0x60, 0xdf, 0xc9, 0xae, 0x28, 0xc3 } };

MIDL_INTERFACE("7d2f767d-2b47-4f04-867c-60dfc9ae28c3")
IMTLDXGIDevice1: public IDXGIDevice3 {
    virtual void* STDMETHODCALLTYPE GetMTLDevice() = 0;
    virtual void* STDMETHODCALLTYPE GetLocalD3DKMT() = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateSwapChain(
        IDXGIFactory1 * pFactory, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1 *pDesc,
        const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
        IDXGISwapChain1 **ppSwapChain) = 0;
    virtual void* STDMETHODCALLTYPE GetMTLCommandQueue() = 0;
};
