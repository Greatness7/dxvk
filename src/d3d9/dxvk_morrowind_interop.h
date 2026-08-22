#pragma once

// Private Morrowind/DXVK COM ABI.
//
// Prerequisites are deliberately limited to platform headers so this file can
// be kept byte-for-byte equivalent in the independent MGE-XE and DXVK trees.
#include <stdint.h>
#include <unknwn.h>
#include <d3d9.h>

static constexpr uint32_t DXVK_MORROWIND_INTEROP_VERSION = 1;

static constexpr uint64_t DXVK_MORROWIND_CAP_MSAA_DEPTH_RESOLVE = 1ull << 0;

MIDL_INTERFACE("2ff12bfc-4622-4d9d-bcbf-1501f37e8aa3")
IDxvkMorrowindInterop : public IUnknown {
    virtual uint32_t STDMETHODCALLTYPE GetInterfaceVersion() = 0;
    virtual uint64_t STDMETHODCALLTYPE GetCapabilities() = 0;

    virtual HRESULT STDMETHODCALLTYPE ResolveDepthMinV1(
        IDirect3DSurface9* sourceMsaaDepth,
        IDirect3DSurface9* destinationIntz) = 0;
};

#ifndef _MSC_VER
__CRT_UUID_DECL(IDxvkMorrowindInterop,
    0x2ff12bfc, 0x4622, 0x4d9d, 0xbc, 0xbf, 0x15, 0x01, 0xf3, 0x7e, 0x8a, 0xa3);
#endif
