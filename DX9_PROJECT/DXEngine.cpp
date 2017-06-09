#include "stdafx.h"
#include "Resource.h"
#include "DXEngine.h"

DXEngine::DXEngine()
{
}


DXEngine::~DXEngine()
{
	Memory::Release(m_pDevice);
	Memory::Release(m_pId3d9);
}

int DXEngine::EnterMsgLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_DX9_PROJECT));

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime) * 0.001f;

			Render(timeDelta);

			lastTime = currTime;
		}
	}

	return msg.wParam;
}


bool DXEngine::InitDX(HINSTANCE hInstance, HWND hwnd, const int width, const int height, const bool windowed, D3DDEVTYPE deviceType)
{
	m_hInstance = hInstance;
	m_hwnd = hwnd;

	m_pId3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!IsHardwareVertexProcessing())
	{
		::MessageBox(0, L"No support hardware vertex processing", 0, 0);
	}
	if (!CreateDevice(width, height, windowed, deviceType)) return false;


	return true;
}

bool DXEngine::IsHardwareVertexProcessing()
{
	if (!SUCCEEDED(m_pId3d9->GetDeviceCaps(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											&m_caps)))
	{
		::MessageBox(0, L"GetDeviceCaps() Failed", 0, 0);
		return false;
	}

	if (m_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		m_iHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		return true;
	}
		
	m_iHardwareVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	return false;
}

bool DXEngine::CreateDevice(const int width, const int height, const bool windowed, D3DDEVTYPE deviceType)
{
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (!SUCCEEDED(m_pId3d9->CreateDevice(D3DADAPTER_DEFAULT,
										deviceType,
										m_hwnd,
										m_iHardwareVertexProcessing,
										&d3dpp,
										&m_pDevice)))
	{
		::MessageBox(0, L"CreateDevice() Failed", 0, 0);
		return false;
	}

	return true;
}

bool DXEngine::Setup()
{
	return true;
}

void DXEngine::Cleanup()
{
}

bool DXEngine::Render(const float timeDelta)
{
	if (m_pDevice)
	{
		m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		m_pDevice->Present(0, 0, 0, 0);
	}

	return true;
}
