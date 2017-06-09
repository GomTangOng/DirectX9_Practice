#pragma once

#define DXENGINE DXEngine::GetInstance()

class DXEngine
{
public:
	DXEngine();
	~DXEngine();

	// Window Function
	int EnterMsgLoop();

	// DX Init Functions
	bool InitDX(HINSTANCE hInstance, HWND hwnd, const int width, const int height, const bool windowed, D3DDEVTYPE deviceType);
	bool IsHardwareVertexProcessing();
	bool CreateDevice(const int width, const int height, bool windowed, D3DDEVTYPE deviceType);

	bool Setup();
	void Cleanup();
	bool Render(const float timeDelta);
private :
	HINSTANCE m_hInstance;
	HWND	  m_hwnd;

	IDirect3D9* m_pId3d9;
	IDirect3DDevice9* m_pDevice;
	D3DCAPS9    m_caps;
	int			m_iHardwareVertexProcessing;
public :
	static DXEngine* GetInstance()
	{
		static DXEngine instance;
		return &instance;
	}
};

