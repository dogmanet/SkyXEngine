#include "IVideoRenderer.h"



namespace GUI
{

	IVideoRenderer::IVideoRenderer():
		m_bLoaded(false), 
		m_pAviFile(NULL), 
		m_pAviStream(NULL), 
		m_pImageData(NULL), 
		m_bLooped(false)
	{
		static UINT id = 0;
		m_iInstanceId = id;
		id++;

		IVideoUpdateManager::RegisterPlayer(this);
	}

	IVideoRenderer::~IVideoRenderer()
	{
		IVideoUpdateManager::UnregisterPlayer(this);
		Release();
	}

	void IVideoRenderer::SetLooped(bool l)
	{
		m_bLooped = l;
	}

	bool IVideoRenderer::IsLoaded()
	{
		return(m_bLoaded);
	}

	UINT IVideoRenderer::GetInstanceId()
	{
		return(m_iInstanceId);
	}

	void IVideoRenderer::OpenFile(const WCHAR * szFile)
	{
		Release();

		m_fLastDrawTime = -1.0f;

		int res = AVIFileOpenW(&m_pAviFile, szFile, OF_READ, NULL);
		if(res != AVIERR_OK)
		{
			if(m_pAviFile)
			{
				AVIFileRelease(m_pAviFile);
				m_pAviFile = NULL;
			}
			return;
		}

		AVIFILEINFOW pAviInfo;
		res = AVIFileInfoW(m_pAviFile, &pAviInfo, sizeof(AVIFILEINFOW));


		m_iWidth = pAviInfo.dwWidth;
		m_iHeight = pAviInfo.dwHeight;

		m_pImageData = new UINT[m_iWidth * m_iHeight];

		for(UINT i = 0; i < m_iWidth * m_iHeight; i++)
		{
			m_pImageData[i] = 0xFF000000;
		}


		m_fClipLength = ((float)pAviInfo.dwLength * (float)pAviInfo.dwScale) / ((float)pAviInfo.dwRate);
		m_fFrameLength = (float)pAviInfo.dwScale / (float)pAviInfo.dwRate;

		res = AVIFileGetStream(m_pAviFile, &m_pAviStream, streamtypeVIDEO, 0);
		m_iFirstFrame = AVIStreamStart(m_pAviStream);
		m_iCurrentFrame = m_iFirstFrame;
		m_iNumFrames = AVIStreamLength(m_pAviStream);

		if(res != AVIERR_OK || m_iFirstFrame == -1 || m_iNumFrames == -1)
		{
			if(m_pAviStream)
			{
				AVIStreamRelease(m_pAviStream);
				m_pAviStream = NULL;
			}

			AVIFileRelease(m_pAviFile);
			m_pAviFile = NULL;
			return;
		}

		m_pAviFrame = AVIStreamGetFrameOpen(m_pAviStream, PBITMAPINFOHEADER(AVIGETFRAMEF_BESTDISPLAYFMT));

		if(FAILED(GetGUI()->GetDevice()->CreateTexture(m_iWidth, m_iHeight, 0, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture.m_pTexture, NULL)))
		{
			return;
		}
		m_texture.m_iHeight = m_iHeight;
		m_texture.m_iWidth = m_iWidth;
		m_texture.m_szName = L"";
		m_texture.LoadFromMem((byte*)m_pImageData);

		m_bLoaded = true;
	}

	void IVideoRenderer::Play()
	{
		if(!IsPlaying())
		{
			m_state = STATE_PLAY;
			m_fLastDrawTime = (float)GetTickCount() / 1000.0f;
		}
	}
	void IVideoRenderer::Pause()
	{
		if(IsPlaying())
		{
			m_state = STATE_PAUSE;
		}
	}
	void IVideoRenderer::Stop()
	{
		m_iCurrentFrame = m_iFirstFrame;
		m_state = STATE_STOP;
	}

	int IVideoRenderer::GetLength()
	{
		return(m_iNumFrames);
	}

	void IVideoRenderer::SetPos(int pos)
	{
		m_iCurrentFrame = pos;
	}

	int IVideoRenderer::GetPos()
	{
		return(m_iCurrentFrame);
	}

	CPITexture IVideoRenderer::GetTexture()
	{
		return(&m_texture);
	}

	void IVideoRenderer::Update()
	{
		if(m_state != STATE_PLAY)
			return;

		if(m_fLastDrawTime == -1)
		{
			m_fLastDrawTime = (float)GetTickCount() / 1000.0f;
		}

		int targetFrame = m_iCurrentFrame + (int)floor((((float)GetTickCount() / 1000.0f) - m_fLastDrawTime) / m_fFrameLength);
		if(targetFrame != m_iCurrentFrame)
		{
			m_fLastDrawTime += (float)(targetFrame - m_iCurrentFrame) * m_fFrameLength;
			m_iCurrentFrame = targetFrame;
			// Movie is at end
			if(m_iCurrentFrame >= m_iNumFrames + m_iFirstFrame)
			{
				if(!IsLooped())
				{
					Stop();
					return;
				}
				else
				{
					m_iCurrentFrame = m_iFirstFrame;
				}
			}



			LPBITMAPINFOHEADER lpbi;
			lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_pAviFrame, m_iCurrentFrame);
			if(!lpbi)
			{
				return;
			}
			// Create pointer to Device Indepenedent Bitmap (DIB) Pixel Data (written in reverse - see Google or MSDN)
			BYTE *pData = (BYTE*)(lpbi + 1);
			UINT * pixels = m_pImageData;

			// Write 24 bpp Bitmap data (B8:G8:R8) to texture pixels
			if(lpbi->biBitCount == 24)
			{
				long tWidth = m_iWidth;
				long vWidth = m_iWidth;
				long vHeight = m_iHeight;
				long texPos;
				long bitPos;
				DWORD val;
				for(long i = 0; i < vHeight; i++)
				{
					for(long j = 0; j < vWidth; j++)
					{
						texPos = tWidth*i + j;
						bitPos = (vWidth*(vHeight - i - 1) + (j)) * 3;
						val = 0xFF000000 | pData[bitPos + 2] << 16 | pData[bitPos + 1] << 8 | pData[bitPos];
						pixels[texPos] = val;
					}
				}
			}
			// Write 16 bpp Bitmap data (X1:B5:G5:R5) to texture pixels
			else if(lpbi->biBitCount == 16)
			{
				WORD *pDataWord = (WORD*)pData;
				long tWidth = m_iWidth;
				long vWidth = m_iWidth;
				long vHeight = m_iHeight;
				long texPos, bitPos;
				WORD colorDat, tmp;
				DWORD val;
				BYTE r, g, b;
				for(long i = 0; i < vHeight; i++)
				{
					for(long j = 0; j < vWidth; j++)
					{
						texPos = tWidth*i + j;
						bitPos = vWidth*(vHeight - i - 1) + (j);
						colorDat = pDataWord[bitPos];
						tmp = colorDat;
						b = (tmp & 0x1F) << 3;
						tmp = tmp >> 5;
						g = (tmp & 0x1F) << 3;
						tmp = tmp >> 5;
						r = (tmp & 0x1F) << 3;
						val = 0xFF000000 | (r) << 16 | (g) << 8 | (b);
						pixels[texPos] = val;
					}
				}
			}
			else if(lpbi->biBitCount == 32)
			{
				DWORD *pDataInt = (DWORD*)pData;
				long tWidth = m_iWidth;
				long vWidth = m_iWidth;
				long vHeight = m_iHeight;
				long texPos, bitPos;

				for(long i = 0; i < vHeight; i++)
				{
					for(long j = 0; j < vWidth; j++)
					{
						texPos = tWidth*i + j;
						bitPos = vWidth*(vHeight - i - 1) + (j);
						pixels[texPos] = 0xFF000000 | pDataInt[bitPos];
					}
				}
			}
		}

		m_texture.LoadFromMem((byte*)m_pImageData);
	}

	bool IVideoRenderer::IsPlaying()
	{
		return(m_state == STATE_PLAY);
	}
	bool IVideoRenderer::IsStopped()
	{
		return(m_state == STATE_STOP);
	}
	bool IVideoRenderer::IsPaused()
	{
		return(m_state == STATE_PAUSE);
	}
	bool IVideoRenderer::IsLooped()
	{
		return(m_bLooped);
	}

	void IVideoRenderer::Release()
	{
		if(m_pAviFile)
		{
			AVIFileRelease(m_pAviFile);
			m_pAviFile = NULL;
		}
		if(m_pAviStream)
		{
			AVIStreamRelease(m_pAviStream);
			m_pAviStream = NULL;
		}

		if(m_texture.GetAPItexture())
		{
			m_texture.GetAPItexture()->Release();
		}

		SX_SAFE_DELETE_A(m_pImageData);
	}




	Array<IVideoRenderer*> IVideoUpdateManager::m_pUpdateList;

	void IVideoUpdateManager::Update()
	{
		for(UINT i = 0; i < m_pUpdateList.size(); i++)
		{
			m_pUpdateList[i]->Update();
		}
	}

	void IVideoUpdateManager::RegisterPlayer(IVideoRenderer * player)
	{
		m_pUpdateList.push_back(player);
	}

	void IVideoUpdateManager::UnregisterPlayer(IVideoRenderer * player)
	{
		for(UINT i = 0; i < m_pUpdateList.size(); i++)
		{
			if(m_pUpdateList[i] == player)
			{
				m_pUpdateList.erase(i);
				return;
			}
		}
	}
};