#pragma once

#include "d3d9_interfaces.h"
#include "d3d9_multithread.h"
#include "dxvk_morrowind_interop.h"

#include "../dxvk/dxvk_extension_provider.h"

namespace dxvk {

  class D3D9InterfaceEx;
  class D3D9CommonTexture;
  class D3D9DeviceEx;
  struct D3D9_COMMON_TEXTURE_DESC;

  class D3D9VkInteropInterface final : public ID3D9VkInteropInterface1 {

  public:

    D3D9VkInteropInterface(
            D3D9InterfaceEx*      pInterface);

    ~D3D9VkInteropInterface();

    ULONG STDMETHODCALLTYPE AddRef();
    
    ULONG STDMETHODCALLTYPE Release();
    
    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                riid,
            void**                ppvObject);

    void STDMETHODCALLTYPE GetInstanceHandle(
            VkInstance*           pInstance);

    void STDMETHODCALLTYPE GetPhysicalDeviceHandle(
            UINT                  Adapter,
            VkPhysicalDevice*     pPhysicalDevice);

    HRESULT STDMETHODCALLTYPE GetInstanceExtensions(
            UINT*                 pExtensionCount,
      const char**                ppExtensions);

  private:

    D3D9InterfaceEx* m_interface;
    DxvkExtensionList m_extensions = { };

  };

  class D3D9VkInteropTexture final : public ID3D9VkInteropTexture {

  public:

    D3D9VkInteropTexture(
            IUnknown*             pInterface,
            D3D9CommonTexture*    pTexture);

    ~D3D9VkInteropTexture();

    ULONG STDMETHODCALLTYPE AddRef();
    
    ULONG STDMETHODCALLTYPE Release();
    
    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                riid,
            void**                ppvObject);

    HRESULT STDMETHODCALLTYPE GetVulkanImageInfo(
            VkImage*              pHandle,
            VkImageLayout*        pLayout,
            VkImageCreateInfo*    pInfo);

    D3D9CommonTexture* GetCommonTexture() { return m_texture; }

  private:

    IUnknown*          m_interface;
    D3D9CommonTexture* m_texture;

  };

  class D3D9VkInteropDevice final : public ID3D9VkInteropDevice {

  public:

    D3D9VkInteropDevice(
            D3D9DeviceEx*         pInterface);

    ~D3D9VkInteropDevice();

    ULONG STDMETHODCALLTYPE AddRef();
    
    ULONG STDMETHODCALLTYPE Release();
    
    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                riid,
            void**                ppvObject);

    void STDMETHODCALLTYPE GetVulkanHandles(
            VkInstance*           pInstance,
            VkPhysicalDevice*     pPhysDev,
            VkDevice*             pDevice);

    void STDMETHODCALLTYPE GetSubmissionQueue(
            VkQueue*              pQueue,
            uint32_t*             pQueueIndex,
            uint32_t*             pQueueFamilyIndex);

    void STDMETHODCALLTYPE TransitionTextureLayout(
            ID3D9VkInteropTexture*    pTexture,
      const VkImageSubresourceRange*  pSubresources,
            VkImageLayout             OldLayout,
            VkImageLayout             NewLayout);

    void STDMETHODCALLTYPE FlushRenderingCommands();

    void STDMETHODCALLTYPE LockSubmissionQueue();

    void STDMETHODCALLTYPE ReleaseSubmissionQueue();

    void STDMETHODCALLTYPE LockDevice();
    
    void STDMETHODCALLTYPE UnlockDevice();

    bool STDMETHODCALLTYPE WaitForResource(
            IDirect3DResource9*  pResource,
            DWORD                MapFlags);

    HRESULT STDMETHODCALLTYPE CreateImage(
            const D3D9VkExtImageDesc* desc,
            IDirect3DResource9**      ppResult);

  private:

    template <typename ResourceType>
    HRESULT CreateTextureResource(
            const D3D9_COMMON_TEXTURE_DESC& desc,
            IDirect3DResource9**            ppResult);

    D3D9DeviceEx*  m_device;
    D3D9DeviceLock m_lock;

  };

  class DxvkMorrowindInterop final : public IDxvkMorrowindInterop {

  public:

    DxvkMorrowindInterop(
            D3D9DeviceEx*         pInterface);

    ~DxvkMorrowindInterop();

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                riid,
            void**                ppvObject);

    uint32_t STDMETHODCALLTYPE GetInterfaceVersion();

    uint64_t STDMETHODCALLTYPE GetCapabilities();

    HRESULT STDMETHODCALLTYPE ResolveDepthMinV1(
            IDirect3DSurface9*    sourceMsaaDepth,
            IDirect3DSurface9*    destinationIntz);

  private:

    uint64_t GetCapabilitiesLocked() const;

    D3D9DeviceEx* m_device;

  };

  class DxvkMorrowindPplInterop final : public IDxvkMorrowindPplInterop1 {

  public:

    DxvkMorrowindPplInterop(
            D3D9DeviceEx*         pInterface);

    ~DxvkMorrowindPplInterop();

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                riid,
            void**                ppvObject);

    uint64_t STDMETHODCALLTYPE GetCapabilities();

    HRESULT STDMETHODCALLTYPE DrawPplV1(
      const DxvkMorrowindPplDrawV1* draw);

  private:

    D3D9DeviceEx* m_device;

  };

  class DxvkMorrowindMemoryInterop final : public IDxvkMorrowindMemoryInterop1 {

  public:

    DxvkMorrowindMemoryInterop(
            D3D9DeviceEx*         pInterface);

    ~DxvkMorrowindMemoryInterop();

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(
            REFIID                riid,
            void**                ppvObject);

    HRESULT STDMETHODCALLTYPE GetDeviceLocalMemoryBudgetV1(
            uint64_t*             memoryBudget,
            uint64_t*             memoryUsed);

  private:

    D3D9DeviceEx* m_device;

  };

  class D3D9VkExtInterface final : public ID3D9VkExtInterface {

  public:

    D3D9VkExtInterface(D3D9InterfaceEx *pInterface);

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);

    void STDMETHODCALLTYPE UnlockAdditionalFormats();

  private:

    D3D9InterfaceEx *m_interface;

  };

}
