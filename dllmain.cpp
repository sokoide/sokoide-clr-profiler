// Copyright (c) .NET Foundation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "ClassFactory.h"

const IID IID_IUnknown = {0x00000000,
                          0x0000,
                          0x0000,
                          {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

const IID IID_IClassFactory = {
    0x00000001,
    0x0000,
    0x0000,
    {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};

BOOL STDMETHODCALLTYPE DllMain(HMODULE hModule, DWORD ul_reason_for_call,
                               LPVOID lpReserved) {
    return TRUE;
}

extern "C" HRESULT STDMETHODCALLTYPE DllGetClassObject(REFCLSID rclsid,
                                                       REFIID riid,
                                                       LPVOID* ppv) {
    // {DB03B7FB-2425-4CBF-A375-D9A67A66C370}
    const GUID CLSID_CorProfiler = {
        0xDB03B7FB,
        0x2425,
        0x4CBF,
        {0xA3, 0x75, 0xD9, 0xA6, 0x7A, 0x66, 0xC3, 0x70}};

    if (ppv == nullptr || rclsid != CLSID_CorProfiler) {
        return E_FAIL;
    }

    auto factory = new ClassFactory;
    if (factory == nullptr) {
        return E_FAIL;
    }

    return factory->QueryInterface(riid, ppv);
}

extern "C" HRESULT STDMETHODCALLTYPE DllCanUnloadNow() { return S_OK; }
