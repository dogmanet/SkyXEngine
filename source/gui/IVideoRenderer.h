#ifndef _IVideoRenderer_H_
#define _IVideoRenderer_H_


#include "ITexture.h"

#include <Vfw.h>


namespace GUI
{

	class IVideoRenderer
	{
		enum STATE
		{
			STATE_STOP,
			STATE_PLAY,
			STATE_PAUSE
		};

	public:
		IVideoRenderer();
		~IVideoRenderer();

		void OpenFile(const WCHAR * szFile);
		void Play();
		void Pause();
		void Stop();

		int GetLength();
		int GetPos();
		void SetPos(int pos);

		CPITexture GetTexture();

		void Update();

		bool IsPlaying();
		bool IsStopped();
		bool IsPaused();
		bool IsLooped();

		void Release();

		UINT GetInstanceId();

		void SetLooped(bool l);

		bool IsLoaded();

	protected:
		ITexture m_texture;
		UINT m_iWidth;
		UINT m_iHeight;

		STATE m_state;

		bool m_bLooped;

		bool m_bLoaded;

		UINT m_iInstanceId;

		PAVIFILE m_pAviFile;
		PAVISTREAM m_pAviStream;
		PGETFRAME m_pAviFrame;

		float m_fClipLength;
		float m_fFrameLength;
		float m_fLastDrawTime;

		int m_iFirstFrame;
		int m_iCurrentFrame;
		int m_iNumFrames;

		UINT * m_pImageData;
	};

	class IVideoUpdateManager
	{
	public:
		static void Update();
		static void RegisterPlayer(IVideoRenderer * player);
		static void UnregisterPlayer(IVideoRenderer * player);

	protected:

		static Array<IVideoRenderer*> m_pUpdateList;
	};
};

#endif
