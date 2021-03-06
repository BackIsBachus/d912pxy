/*
MIT License

Copyright(c) 2018-2019 megai2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include "stdafx.h"

IP7_Client        *logger = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

	if (!d3d9_proxy_dll_main(hModule, ul_reason_for_call, lpReserved))
		return FALSE;


    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		P7_Set_Crash_Handler();

		//create P7 client object
#ifdef _DEBUG
		logger = P7_Create_Client(TM("/P7.Pool=32768"));
#else
		logger = P7_Create_Client(TM("/P7.Pool=32768 /P7.Sink=FileBin"));
#endif

		if (logger == NULL)
		{
			MessageBox(0, L"P7 init error", L"frame_analyzer", MB_ICONERROR);
			return 0;
		}


		logger->Share(TM("logger"));

		D3D9ProxyCb_set_OnDevCreate(&app_cb_D3D9Dev_create);
		D3D9ProxyCb_set_OnDevDestroy(&app_cb_D3D9Dev_destroy);

		break;
	}
    case DLL_THREAD_ATTACH:
		break;
    case DLL_THREAD_DETACH:
		break;
    case DLL_PROCESS_DETACH:
		if (logger)
		{
			P7_Exceptional_Flush();
			logger->Release();
		}
        break;
    }
    return TRUE;
}

