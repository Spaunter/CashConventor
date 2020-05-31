#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#include <iostream>
#include <windows.h>
#include <string>
#include <regex>
#include <winhttp.h>
#include <map>
#include <math.h>
#include <commctrl.h>//for droplist

#pragma comment (lib, "Winhttp.lib")

using namespace std;

const int MAX_LENGHT = 15000;

enum clientElement // user elements
{
	button,
	startMoney,
    droplist,
	aboutApp
};

const regex regNum("^[0-9]*$"); 
const regex regLetter("^[a-z]$");

HFONT hFont = CreateFont(20, 0, 0, 2, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Roboto");//fonts
HWND hEditMoney, hComboBox; 
static float USD_CURRENCY{};

map<string, double> GetValueFromHTTP() {

    map<string, double> cashList = { {"USD",0},{"EUR", 0},{"RUB", 0},{"PLN", 0}};
    HINTERNET hSession = WinHttpOpen(L"CashConventor", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) 
    {
        return cashList;
    }

    HINTERNET hConnect = WinHttpConnect(hSession,  L"bank.gov.ua", INTERNET_DEFAULT_HTTPS_PORT, 0);
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

    } while (dwSize>0);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

   
    try
    {
        cashList["RUB"]=stof(buffer.substr(buffer.find("RUB") + 56, 5));//for 10 rub
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






int CALLBACK  wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
   

    MSG msg{};                          
    HWND hwnd{};                         
    WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; 
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    //wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        switch (uMsg)
        {
        case WM_CREATE:
        {
            //-----------------------header menu-----------------------------------
            HMENU hMenuBar = CreateMenu();
            HMENU hMenuAbout = CreateMenu();
           
            AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)clientElement::aboutApp, L"О программе");

            SetMenu(hWnd, hMenuAbout);
            SetMenu(hWnd, hMenuBar);
            //-------------------------------------------------------------------------
            //--------------drop list-------------------------------
           hComboBox = CreateWindow(
                WC_COMBOBOX,
                L"",
                CBS_DROPDOWN | CBS_HASSTRINGS | WS_VISIBLE | WS_CHILD | WS_BORDER,
                250, 20, 70, 17,
                hWnd, reinterpret_cast<HMENU>(clientElement::droplist), nullptr, nullptr);

            SendMessage(
                hComboBox,
                (UINT)CB_ADDSTRING,
                (WPARAM)0, (LPARAM)TEXT("UAH"));
            SendMessage(
                hComboBox,
                (UINT)CB_ADDSTRING,
                (WPARAM)0, (LPARAM)TEXT("RUB"));
            SendMessage(
                hComboBox,
                (UINT)CB_ADDSTRING,
                (WPARAM)0, (LPARAM)TEXT("USD"));
            SendMessage(
                hComboBox,
                (UINT)CB_ADDSTRING,
                (WPARAM)0, (LPARAM)TEXT("EUR"));
            SendMessage(
                hComboBox,
                (UINT)CB_ADDSTRING,
                (WPARAM)0, (LPARAM)TEXT("PLN"));


            SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
            //------------------------count money fild----------------------------------
            HWND hTextCount = CreateWindow(
                L"STATIC",
                L"Введите количество",
                WS_CHILD | WS_VISIBLE,
                50, 20, 200, 30, hWnd, nullptr, nullptr, nullptr
            );
            SendMessage(hTextCount, WM_SETFONT, (WPARAM)hFont, TRUE);
            hEditMoney = CreateWindowEx(
                WS_EX_CLIENTEDGE,
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
                150, 55, 80, 30, hWnd, reinterpret_cast<HMENU>(clientElement::startMoney), nullptr, nullptr
            );
                       
            //------------------------------button-----------------------------------
            HWND hButton = CreateWindow(
                L"BUTTON",
                L"Конвертировать",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
                120, 100, 150, 40, hWnd, reinterpret_cast<HMENU>(clientElement::button), nullptr, nullptr
            );
            SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

        }
        return 0;


        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {//--------------------------------------action on push-----------------------------
            case clientElement::button:
            {
                //-----------------convert in wstring-------------------------
                wstring wStrMoney{};
                wStrMoney.resize(MAX_LENGHT);
                GetWindowText(hEditMoney, &wStrMoney[0], MAX_LENGHT);
                wStrMoney.erase(remove(begin(wStrMoney), end(wStrMoney), 0), end(wStrMoney));
               
                if (wStrMoney.empty()) 
                {
                    MessageBox(hWnd, L"Укажите количество валюты!", L"Ошибка", MB_ICONERROR);
                    break;
                }
                else
                {
       
                    string strMoney(wStrMoney.begin(), wStrMoney.end());
                    
                    //---------------------check data---------------------------
                    if (atoi(strMoney.c_str()) < 1 || !regex_match(strMoney, regNum))
                    {
                        MessageBox(hWnd, L"Неверный формат:\nСтрока должна содержать только положительные числа.", nullptr, MB_ICONERROR);
                        SetWindowText(hEditMoney, nullptr);
                        break;
                    }
                    else {
                        int intMoney = atoi((to_string(stoi(wStrMoney))).c_str());

                        wstring wStrTypeM{};
                        wStrTypeM.resize(MAX_LENGHT);
                        GetWindowText(hComboBox, &wStrTypeM[0], MAX_LENGHT);
                        wStrTypeM.erase(remove(begin(wStrTypeM), end(wStrTypeM), 0), end(wStrTypeM));
                        string strType(wStrTypeM.begin(), wStrTypeM.end());

                        map<string, double> cashList = GetValueFromHTTP();

                        if (strType == "UAH")
                        {
                            double UAHtoUSD = intMoney / cashList["USD"];
                            double UAHtoEUR = intMoney / cashList["EUR"];
                            double UAHtoPLN = intMoney / cashList["PLN"];
                            double UAHtoRUB = intMoney * 10 / cashList["RUB"];
                            string finalText = strType + "\n" + to_string(intMoney) + " UAH = " + to_string(UAHtoUSD) + " USD\n" + to_string(intMoney) + " UAH = " + to_string(UAHtoEUR) + " EUR\n" + to_string(intMoney) + " UAH = " + to_string(UAHtoPLN) + " PLN \n" + to_string(intMoney) + " UAH = " + to_string(UAHtoRUB) + " RUB";
                            wstring textOut(finalText.begin(), finalText.end());
                            LPCWSTR textNew = (LPCWSTR)textOut.c_str();
                            MessageBox(hWnd, textNew, L"Курс НБУ", MB_ICONINFORMATION);
                        }
                        if (strType == "RUB")
                        {
                            double RUBtoUAH = intMoney * (cashList["RUB"]/10);
                            double RUBtoUSD = intMoney * (cashList["RUB"]/10/ cashList["USD"]);
                            double RUBtoEUR = intMoney * (cashList["RUB"]/10/ cashList["EUR"]);
                            double RUBtoPLN = intMoney * (cashList["RUB"]/10/ cashList["PLN"]);
                            string finalText = strType + "\n" + to_string(intMoney) + " RUB = " + to_string(RUBtoUAH) + " UAH\n" + to_string(intMoney) + " RUB = " + to_string(RUBtoUSD) + " USD\n" + to_string(intMoney) + " RUB = " + to_string(RUBtoEUR) + " EUR \n" + to_string(intMoney) + " RUB = " + to_string(RUBtoPLN) + " PLN";
                            wstring textOut(finalText.begin(), finalText.end());
                            LPCWSTR textNew = (LPCWSTR)textOut.c_str();
                            MessageBox(hWnd, textNew, L"Курс НБУ", MB_ICONINFORMATION);


                        }
                        if (strType == "USD")
                        {
                            double USDtoUAH = intMoney * cashList["USD"];
                            double USDtoEUR = intMoney * (cashList["USD"]/cashList["EUR"]);
                            double USDtoPLN = intMoney * (cashList["USD"]/cashList["PLN"] );
                            double USDtoRUB = intMoney * (cashList["USD"]/cashList["RUB"]*10);
                            string finalText = strType + "\n" + to_string(intMoney) + " USD = " + to_string(USDtoUAH) + " UAH\n" + to_string(intMoney) + " USD = " + to_string(USDtoEUR) + " EUR\n" + to_string(intMoney) + " USD = " + to_string(USDtoPLN) + " PLN \n" + to_string(intMoney) + " USD = " + to_string(USDtoRUB) + " RUB";
                            wstring textOut(finalText.begin(), finalText.end());
                            LPCWSTR textNew = (LPCWSTR)textOut.c_str();
                            MessageBox(hWnd, textNew, L"Курс НБУ", MB_ICONINFORMATION);
                        }
                        if (strType == "EUR")
                        {
                            double EURtoUAH = intMoney * cashList["EUR"];
                            double EURtoUSD = intMoney * (cashList["EUR"] / cashList["USD"]);
                            double EURtoPLN = intMoney * (cashList["EUR"] / cashList["PLN"]);
                            double EURtoRUB = intMoney * (cashList["EUR"] / cashList["RUB"] * 10);
                            string finalText = strType + "\n" + to_string(intMoney) + " EUR = " + to_string(EURtoUAH) + " UAH\n" + to_string(intMoney) + " EUR = " + to_string(EURtoUSD) + " USD\n" + to_string(intMoney) + " EUR = " + to_string(EURtoPLN) + " PLN \n" + to_string(intMoney) + " EUR = " + to_string(EURtoRUB) + " RUB";
                            wstring textOut(finalText.begin(), finalText.end());
                            LPCWSTR textNew = (LPCWSTR)textOut.c_str();
                            MessageBox(hWnd, textNew, L"Курс НБУ", MB_ICONINFORMATION);

                        }
                        if (strType == "PLN")
                        {
                            double PLNtoUAH = intMoney * cashList["PLN"];
                            double PLNtoUSD = intMoney * (cashList["PLN"] / cashList["USD"]);
                            double PLNtoEUR = intMoney * (cashList["PLN"] / cashList["EUR"]);
                            double PLNtoRUB = intMoney * (cashList["PLN"] / cashList["RUB"] * 10);
                            string finalText = strType + "\n" + to_string(intMoney) + " PLN = " + to_string(PLNtoUAH) + " UAH\n" + to_string(intMoney) + " PLN = " + to_string(PLNtoUSD) + " USD\n" + to_string(intMoney) + " PLN = " + to_string(PLNtoEUR) + " EUR \n" + to_string(intMoney) + " PLN = " + to_string(PLNtoRUB) + " RUB";
                            wstring textOut(finalText.begin(), finalText.end());
                            LPCWSTR textNew = (LPCWSTR)textOut.c_str();
                            MessageBox(hWnd, textNew, L"Курс НБУ", MB_ICONINFORMATION);
                        }

                       
                        

                    }

                }

            }
            break;
            //----------------------header action-------------------------
            case clientElement::aboutApp: {
                MessageBox(hWnd, L"CASH CONVERTER v 1.0 \n\nby Maksym Homenko", L"O программе", MB_OK);
            }
            break;
         
            }

        }
        return 0;

        case WM_DESTROY:
        {
            PostQuitMessage(EXIT_SUCCESS);
        }
        return 0;

        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    };
    wc.lpszClassName = L"MyAppClass";
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    if (!RegisterClassEx(&wc)) { return EXIT_FAILURE; }

    hwnd = CreateWindow(wc.lpszClassName, L"CashCONVERTER", /*WS_OVERLAPPEDWINDOW*/ WS_SYSMENU/*only close*/, 700, 300, 400, 220, nullptr, nullptr, wc.hInstance, nullptr);
    if (hwnd == INVALID_HANDLE_VALUE) { return EXIT_FAILURE; }



    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);        

    while (GetMessage(&msg, nullptr, 0, 0)) 
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg);  
    }

    return static_cast<int> (msg.wParam);

};

