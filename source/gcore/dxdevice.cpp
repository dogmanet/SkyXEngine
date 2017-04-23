
void ScreenQuadOnResetDevice()
{
	struct  VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	const float offset_pixel_x = 1.0f / float(D3DAPP.BackBufferWidth);
	const float offset_pixel_y = 1.0f / float(D3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE AddVertices[] =
	{
		{ -1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 0.0f, 1.0f, 0 },
		{ -1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 0.0f, 0.0f, 1 },
		{ 1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 1.0f, 0.0f, 2 },
		{ 1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 1.0f, 1.0f, 3 },
	};

	void* Vertices;
	if (!FAILED(ScreenTexture->LockVertexBuffer(0, (void**)&Vertices)))
	{
		memcpy(Vertices, AddVertices, sizeof(AddVertices));
		ScreenTexture->UnlockVertexBuffer();
	}
}

int InitD3D(HWND hwnd, bool windowed, int width, int heigth, DWORD create_device_flags)
{
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		return SXGC_ERR_NON_DETECTED_D3D;
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	memset(&D3DAPP, 0, sizeof(D3DAPP));
	D3DAPP.BackBufferWidth = width;
	D3DAPP.BackBufferHeight = heigth;
	D3DAPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	D3DAPP.BackBufferCount = 1;
	D3DAPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3DAPP.MultiSampleQuality = 0;
	D3DAPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DAPP.hDeviceWindow = hwnd;
	D3DAPP.Windowed = windowed;
	D3DAPP.EnableAutoDepthStencil = true;
	D3DAPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	D3DAPP.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL | create_device_flags | D3DCREATE_MULTITHREADED;
	D3DAPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	D3DAPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | create_device_flags | D3DCREATE_MULTITHREADED, &D3DAPP, &DXDevice)))
	{
		return SXGC_ERR_FAILED_INIT_D3D;
	}

	D3DXFONT_DESC LF;
	ZeroMemory(&LF, sizeof(D3DXFONT_DESC));
	LF.Height = 10;    // в логических единицах
	LF.Width = 6;    // в логических единицах
	LF.Weight = 6;   // насыщенность, 
	// диапазон 0(тонкий) - 1000(жирный)
	LF.Italic = 0;
	LF.CharSet = DEFAULT_CHARSET;
	LF.FaceName[0] = 0;

	D3DXCreateFontIndirect(DXDevice, &LF, &FPSText);

	D3DVERTEXELEMENT9 layoutquad[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	//IDirect3DVertexDeclaration9* VertexDeclarationQuad;
	//GData::DXDevice->CreateVertexDeclaration(layoutquad, &VertexDeclarationQuad);

	D3DXCreateMesh(2, 4, D3DXMESH_MANAGED, layoutquad, DXDevice, &ScreenTexture);

	ScreenQuadOnResetDevice();

	WORD* i = 0;
	ScreenTexture->LockIndexBuffer(0, (void**)&i);
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	ScreenTexture->UnlockIndexBuffer();

	//SkyXEngine::Core::Data::Device->CreateQuery(D3DQUERYTYPE_EVENT , &SkyXEngine::Core::Data::D3DQueryEvent);
	return(0);
}

