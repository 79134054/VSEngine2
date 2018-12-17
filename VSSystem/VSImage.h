#ifndef VSIMAGE_H
#define VSIMAGE_H
#include "VSSystem.h"
#include "VSMemManager.h"
#include "VSFile.h"
namespace VSEngine2
{
	class VSSYSTEM_API VSImage
	{
	public:
		enum // Image Format
		{
			IF_BMP,
			IF_TGA,
			IF_MAX
		};
		static TCHAR ms_ImageFormat[IF_MAX][10];
		VSImage();
		virtual ~VSImage() = 0;
		virtual bool Load(const TCHAR *pFileName) = 0;
		virtual bool LoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize) = 0;
		virtual const unsigned char *GetPixel(unsigned int x, unsigned int y)const = 0;
		FORCEINLINE unsigned int GetBPP()const
		{
			return m_uiBPP;
		}
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{
			return m_uiHeight;
		}
		FORCEINLINE const unsigned char *GetImg()const
		{
			return m_pImageDate;
		}
		FORCEINLINE unsigned int GetImgDateSize()const
		{
			return m_uiDataSize;
		}
		FORCEINLINE unsigned char* GetPalette()const   // Return a pointer to VGA palette
		{
			return m_pPalette;
		}
	protected:
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiBPP;
		unsigned char *m_pImageDate;
		unsigned int m_uiDataSize;
		unsigned char *m_pData;
		unsigned char *m_pPalette;
	};
	class VSSYSTEM_API VSBMPImage : public VSImage
	{
	public:
		VSBMPImage();
		~VSBMPImage();
		virtual bool Load(const TCHAR* pFilename);
		virtual bool LoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize);
		virtual const unsigned char *GetPixel(unsigned int x, unsigned int y)const;
		

	private:
		
		unsigned int m_uiEnc;
		
		unsigned int m_uiPlanes;
		unsigned int uiImgOffset;
		
		

		// Internal workers
		bool GetFile(const TCHAR* pFilename);
		bool ReadBmpHeader();
		bool LoadBmpRaw();
		bool LoadBmpRLE8();
		bool LoadBmpPalette();
		void FlipImg(); // Inverts image data, BMP is stored in reverse scanline order
		
	};
	class VSSYSTEM_API VSTGAImage : public VSImage
	{
	public:
		VSTGAImage();
		~VSTGAImage();
		virtual bool Load(const TCHAR* pFilename);
		virtual bool LoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize);
		virtual const unsigned char *GetPixel(unsigned int x, unsigned int y)const;
	private:

		unsigned char m_cEnc;
		

		// Internal workers
		bool ReadHeader();
		bool LoadRawData();
		bool LoadTgaRLEData();
		bool LoadTgaPalette();
		void BGRtoRGB();
		void FlipImg();
	};
	
	class VSSYSTEM_API VSDDSImage : public VSImage
	{
	public:
		VSDDSImage()
		{

		}
		~VSDDSImage()
		{

		}
	};
}
#endif