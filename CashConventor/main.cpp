#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#include"Header.h"
#include"DataBase.h"

#pragma comment (lib, "Winhttp.lib")

const int MAX_LENGHT = 15000;

HFONT hFont = CreateFont(20, 0, 0, 2, FW_REGULAR, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Roboto");//fonts
HWND hEditMoney, hComboBox; 
DataBase DB("d:\\DBFOLDER\\money.db"); // create DB object


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
            HMENU hMenuHist = CreateMenu();

            AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)clientElement::aboutApp, L"About program");
            AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)clientElement::rateHist, L"Rate History");

            SetMenu(hWnd, hMenuAbout);
            SetMenu(hWnd, hMenuHist);
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
                L"������� ����������",
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
                L"��������������",
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
                    MessageBox(hWnd, L"������� ���������� ������!", L"������", MB_ICONERROR);
                    break;
                }
                else
                {
       
                    string strMoney(wStrMoney.begin(), wStrMoney.end());
                    
                    //---------------------check data---------------------------
                    if (atoi(strMoney.c_str()) < 1 || !regex_match(strMoney, regNum))
                    {
                        MessageBox(hWnd, L"�������� ������:\n������ ������ ��������� ������ ������������� �����.", nullptr, MB_ICONERROR);
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

                        if (!dirExists("D:\\DBFOLDER"))// check folder 
                        {
                            system("mkdir D:\\DBFOLDER"); // if no folder, create
                        }
                        
                        DB.createBD(); //create DB
                        DB.createTable("CREATE TABLE IF NOT EXISTS RATE_HIST_UA("
		                               "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		                               "DATE_RATE DATE,"
		                               "CURRENCY TEXT,"
                                       "UNITS INTEGER,"
                                       "RATE NUMERIC);"
                        );

                        if (cashList["USD"] != 0) { // if api dont give data
                            DB.insertData(
                                "INSERT INTO RATE_HIST_UA(DATE_RATE, CURRENCY,UNITS,RATE)"
                                "VALUES(date('now'), 'USD', '1'," + to_string(cashList["USD"]) + "),"
                                "(date('now'), 'EUR', '1'," + to_string(cashList["EUR"]) + "),"
                                "(date('now'), 'PLN', '1'," + to_string(cashList["PLN"]) + "),"
                                "(date('now'), 'RUB', '10'," + to_string(cashList["RUB"]) + ");"
                            );
                        }
                        

                        if (strType == "UAH")
                        {
                            double UAHtoUSD = intMoney / cashList["USD"];
                            double UAHtoEUR = intMoney / cashList["EUR"];
                            double UAHtoPLN = intMoney / cashList["PLN"];
                            double UAHtoRUB = intMoney * 10 / cashList["RUB"];
                            string finalText = strType + "\n" + to_string(intMoney) + " UAH = " + to_string(UAHtoUSD) + " USD\n" + to_string(intMoney) + " UAH = " + to_string(UAHtoEUR) + " EUR\n" + to_string(intMoney) + " UAH = " + to_string(UAHtoPLN) + " PLN \n" + to_string(intMoney) + " UAH = " + to_string(UAHtoRUB) + " RUB";
                            wstring textOut(finalText.begin(), finalText.end());
                            LPCWSTR textNew = (LPCWSTR)textOut.c_str();
                            MessageBox(hWnd, textNew, L"���� ���", MB_ICONINFORMATION);
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
                            MessageBox(hWnd, textNew, L"���� ���", MB_ICONINFORMATION);


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
                            MessageBox(hWnd, textNew, L"���� ���", MB_ICONINFORMATION);
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
                            MessageBox(hWnd, textNew, L"���� ���", MB_ICONINFORMATION);

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
                            MessageBox(hWnd, textNew, L"���� ���", MB_ICONINFORMATION);
                        }
  

                    }

                }

            }
            break;
            //----------------------header action-------------------------
            case clientElement::aboutApp: {
                MessageBox(hWnd, L"CASH CONVERTER v 1.0 \n\nby Maksym Homenko", L"About program", MB_OK);
            }
            break;
            case clientElement::rateHist: {
               // DB.selectData("SELECT * FROM RATE_HIST_UA;");
                MessageBox(hWnd, L"rateHist", L"Rate history", MB_OK);
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

