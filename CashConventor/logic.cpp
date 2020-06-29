#include"Header.h"

bool dirExists(const string& forderDir)
{
	DWORD ftyp = GetFileAttributesA(forderDir.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

map<string, double> GetValueFromHTTP() {

    map<string, double> cashList = { {"USD",0},{"EUR", 0},{"RUB", 0},{"PLN", 0} };
    
    HINTERNET hSession = WinHttpOpen(L"CashConventor", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession)
    {
        return cashList;
    }

    HINTERNET hConnect = WinHttpConnect(hSession, L"bank.gov.ua", INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect)
    {
        return cashList;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/NBU_Exchange/exchange", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest)
    {
        return cashList;
    }

    WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

    WinHttpReceiveResponse(hRequest, NULL);

    DWORD dwSize{}, dwDowloaded{};
    char* pszOutBuffer{};
    string buffer{};
    string value{};
    do
    {
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
        {
            cout << "Error" << GetLastError();
        }

        pszOutBuffer = new char[dwSize + 1];
        if (!pszOutBuffer)
        {
            dwSize = 0;
        }
        else {
            ZeroMemory(pszOutBuffer, dwSize + 1);
            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDowloaded))
            {
                cout << "Error-2" << GetLastError();

            }
            else {
                printf("%s", pszOutBuffer);
            }
            buffer += pszOutBuffer;
            delete[]pszOutBuffer;
        }

    } while (dwSize > 0);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);


    try
    {
        
        cashList["RUB"] = stof(buffer.substr(buffer.find("RUB") + 56, 5));//for 10 rub
        cashList["EUR"] = stof(buffer.substr(buffer.find("EUR") + 55, 5));//for 1 eur
        cashList["USD"] = stof(buffer.substr(buffer.find("USD") + 55, 5));//for 1 usd
        cashList["PLN"] = stof(buffer.substr(buffer.find("PLN") + 55, 5));//for 1 pln 

    }
    catch (...)
    {
        return cashList;
    }
    return cashList;
}