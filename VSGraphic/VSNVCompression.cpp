#include "VSNVCompression.h"
using namespace VSEngine2;
void VSNVCompression::Setup( void* SourceData, unsigned int PixelFormat, unsigned int SizeX, unsigned int SizeY, bool SRGB, bool bIsNormalMap)
{
	nvtt::Format TextureFormat = nvtt::Format_RGBA;
	if (PixelFormat == NV_DXT3)
	{
		TextureFormat = nvtt::Format_DXT3;
	}
	else if (PixelFormat == NV_DXT5)
	{
		TextureFormat = nvtt::Format_DXT5;
	}

	InputOptions.setTextureLayout(nvtt::TextureType_2D, SizeX, SizeY);

	// Not generating mips with NVTT, we will pass each mip in and compress it individually
	InputOptions.setMipmapGeneration(false, -1);
	InputOptions.setMipmapData(SourceData, SizeX, SizeY);

	if (SRGB)
	{
		InputOptions.setGamma(2.2f, 2.2f);
	}
	else
	{
		InputOptions.setGamma(1.0f, 1.0f);
	}

	// Only used for mip and normal map generation
	InputOptions.setWrapMode(nvtt::WrapMode_Mirror);
	InputOptions.setFormat(nvtt::InputFormat_BGRA_8UB);

	CompressionOptions.setFormat(TextureFormat);

	// Highest quality is 2x slower with only a small visual difference
	// Might be worthwhile for normal maps though
	CompressionOptions.setQuality(nvtt::Quality_Production);

	if (bIsNormalMap)
	{
		// Use the weights originally used by nvDXT for normal maps
		//@todo - re-evaluate these
		CompressionOptions.setColorWeights(0.4f, 0.4f, 0.2f);
		InputOptions.setNormalMap(true);
	}
	else
	{
		CompressionOptions.setColorWeights(1, 1, 1);
	}

	Compressor.enableCudaAcceleration(false);
	unsigned int uiSize = Compressor.estimateSize(InputOptions, CompressionOptions);
	OutputHandler.ReserveMemory(uiSize);

	// We're not outputting a dds file so disable the header
	OutputOptions.setOutputHeader( false);
	OutputOptions.setOutputHandler( &OutputHandler );
	OutputOptions.setErrorHandler( &ErrorHandler );
}

/** Performs the DXT compression. */
bool VSNVCompression::Process()
{
	const bool bSuccess = Compressor.process(InputOptions, CompressionOptions, OutputOptions);
	return bSuccess && ErrorHandler.bSuccess;
}