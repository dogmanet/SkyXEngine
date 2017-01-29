
#ifndef __render_func
#define __render_func

namespace SXRenderFunc
{
	void SetSamplerFilter(DWORD id, DWORD value);
	inline void SetSamplerAddress(DWORD id, DWORD value);
	void ComDeviceLost();
	inline void SetSamplerFilter(DWORD begin_id, DWORD end_id, DWORD value);
	inline void SetSamplerAddress(DWORD begin_id, DWORD end_id, DWORD value);

	void UpdateView(DWORD timeDelta);
	void OutputDebugInfo(DWORD timeDelta);

	void RenderInMRT(DWORD timeDelta);
	void UpdateShadow(DWORD timeDelta);
	void RenderShadow(DWORD timeDelta);
	void ComLighting(DWORD timeDelta, bool render_sky, bool blend_in_old);
	void GameRender(DWORD timeDelta);
	void LevelEditorRender(DWORD timeDelta);
};

#endif