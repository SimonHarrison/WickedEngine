#define DIRECTIONALLIGHT_SOFT
#define DISABLE_ALPHATEST
#include "objectHF.hlsli"

[earlydepthstencil]
GBUFFEROutputType_Thin main(PixelInputType input)
{
	OBJECT_PS_MAKE

	OBJECT_PS_SAMPLETEXTURES

	OBJECT_PS_SPECULARANTIALIASING

	OBJECT_PS_DEGAMMA

	OBJECT_PS_LIGHT_BEGIN

	OBJECT_PS_LIGHT_TILED

	OBJECT_PS_PLANARREFLECTIONS

	OBJECT_PS_VOXELRADIANCE

	OBJECT_PS_LIGHT_END

	OBJECT_PS_EMISSIVE

	OBJECT_PS_FOG

	OBJECT_PS_OUT_FORWARD
}
