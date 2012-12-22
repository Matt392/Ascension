// Structures >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// Vertex shader in
struct SVSIn
{
	float4 m_vPosition : POSITION;
	float4 m_vColour : COLOR0;
	float4 m_vColour2 : TANGENT;
	float2 m_vTexCoord : TEXCOORD0;
	float3 m_vTransformations : NORMAL;
	float3 m_vScale : BINORMAL;
	//int m_fTextureIndex : BLENDINDICES;
};

// Pixel shader in
struct SPSIn
{
	float2 m_vTexCoord : TEXCOORD0;
	float4 m_vColour : COLOR0;
	float4 m_vColour2 : TANGENT;
	int m_iTextureIndex : BINORMAL;
	//int m_iTextureIndex : BLENDINDICES;
};

// Vertex shader out
struct SVSOut
{
	float4 m_vPosition : POSITION;
	SPSIn m_sPSIn;
};

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Structures
// Globals Variables >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

sampler g_sFont0 : register( s0 ) = 
sampler_state
{
	MipFilter = None;
	MinFilter = None;
	MagFilter = None;
};
sampler g_sTextureAsIs : register( s0 ) = 
sampler_state
{
	MipFilter = None;
	MinFilter = None;
	MagFilter = None;
	MaxAnisotropy = 2;
	AddressU = CLAMP;
	AddressV = CLAMP;
};
sampler g_sTextureArray[8] : register( s0 ) = 
{
	sampler_state //Texture 0
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 1
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 2
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 3
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 4
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 5
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 6
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 7
	{
		MipFilter = ANISOTROPIC;
		MinFilter = ANISOTROPIC;
		MagFilter = ANISOTROPIC;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
};
sampler g_sTextureArrayNoBlend[8] : register( s0 ) = 
{
	sampler_state //Texture 0
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 1
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 2
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 3
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 4
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 5
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 6
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
	sampler_state //Texture 7
	{
		MipFilter = None;
		MinFilter = None;
		MagFilter = None;
		MaxAnisotropy = 2;
		AddressU = CLAMP;
		AddressV = CLAMP;
	},
};
sampler g_sTexture0NoFilter : register( s2 ) = 
sampler_state
{
	MipFilter = None;
	MinFilter = None;
	MagFilter = None;
};


float4x4	g_mProjection; 
float4x4	g_mView; 

float4 g_mCameraPos = { 0.0f, 128.0f, 0.0f ,0.0f };

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Globals Variables
// Global Funcs >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

float2 CorrectTexCoordForRes( float2 vTex )
{
	float2 vBackBufferRes;
	vBackBufferRes.x = 960.0f;
	vBackBufferRes.y = 640.0f;
	vTex *= vBackBufferRes.xy;
	vTex += float2(0.25f,0.5f);
	vTex /= vBackBufferRes.xy;	
	return vTex;
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Global Funcs
// Vertex Shaders >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
SVSOut  BasicSpriteVS( SVSIn sVSData )
{
	SVSOut vsOut;
	float4x4 m_viewProj = mul(g_mView, g_mProjection);
	//Scale
	sVSData.m_vPosition.x *= sVSData.m_vScale.x;
	sVSData.m_vPosition.y *= sVSData.m_vScale.y;

	
	float fOrigX = sVSData.m_vPosition.x;
	float fOrigY = sVSData.m_vPosition.y;

	sVSData.m_vPosition.x = (fOrigX * cos(sVSData.m_vTransformations.z)) - (fOrigY * sin(sVSData.m_vTransformations.z));
	sVSData.m_vPosition.y = (fOrigY * cos(sVSData.m_vTransformations.z)) + (fOrigX * sin(sVSData.m_vTransformations.z));
	
	//Transfromation
	sVSData.m_vPosition.x += sVSData.m_vTransformations.x;
	sVSData.m_vPosition.y += sVSData.m_vTransformations.y;

	sVSData.m_vPosition = mul( m_viewProj, sVSData.m_vPosition );
	
	vsOut.m_vPosition = sVSData.m_vPosition;
	vsOut.m_sPSIn.m_vTexCoord = sVSData.m_vTexCoord;
	vsOut.m_sPSIn.m_vColour = sVSData.m_vColour;
	vsOut.m_sPSIn.m_vColour2 =  sVSData.m_vColour2;
	vsOut.m_sPSIn.m_iTextureIndex = (int)(sVSData.m_vScale.z);
	return vsOut;
}
SVSOut  GUIVS( SVSIn sVSData )
{
	SVSOut vsOut;
	
	sVSData.m_vPosition.x *= sVSData.m_vScale.x;
	sVSData.m_vPosition.y *= sVSData.m_vScale.y;
	//float4x4 m_viewProj = mul(g_mView, g_mProjection);
	float fOrigX = sVSData.m_vPosition.x;
	float fOrigY = sVSData.m_vPosition.y;

	sVSData.m_vPosition.x = (fOrigX * cos(sVSData.m_vTransformations.z)) - (fOrigY * sin(sVSData.m_vTransformations.z));
	sVSData.m_vPosition.y = (fOrigY * cos(sVSData.m_vTransformations.z)) + (fOrigX * sin(sVSData.m_vTransformations.z));
	
	sVSData.m_vPosition.x += sVSData.m_vTransformations.x;
	sVSData.m_vPosition.y += sVSData.m_vTransformations.y;
	//sVSData.m_vPosition = mul( m_viewProj, sVSData.m_vPosition );
	
	sVSData.m_vPosition.x = (sVSData.m_vPosition.x - 0.5f) * 2.0f;
	sVSData.m_vPosition.y = (-(sVSData.m_vPosition.y - 0.5f)) * 2.0f;
	
	vsOut.m_vPosition = sVSData.m_vPosition;
	vsOut.m_sPSIn.m_vTexCoord = sVSData.m_vTexCoord;
	vsOut.m_sPSIn.m_vColour = sVSData.m_vColour;
	vsOut.m_sPSIn.m_vColour2 =  sVSData.m_vColour2;
	vsOut.m_sPSIn.m_iTextureIndex = (int)(sVSData.m_vScale.z);
	return vsOut;
} 
SVSOut  AsIsVS( SVSIn sVSData )
{
	SVSOut vsOut;
	
	vsOut.m_vPosition = sVSData.m_vPosition;
	vsOut.m_sPSIn.m_vTexCoord = sVSData.m_vTexCoord;
	vsOut.m_sPSIn.m_vColour = sVSData.m_vColour;
	vsOut.m_sPSIn.m_vColour2 =  sVSData.m_vColour2;
	vsOut.m_sPSIn.m_iTextureIndex = 0;
	return vsOut;
} 
// Vertex Shaders <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// Pixel Shaders >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

float4 FontPS( SPSIn sPSIn ) : COLOR0
{
	float4 fBase = tex2D( g_sFont0, sPSIn.m_vTexCoord );
	if(fBase.r == 1.0f && fBase.g == 1.0f && fBase.b == 1.0f)
	{
		fBase.r = sPSIn.m_vColour.r;
		fBase.g = sPSIn.m_vColour.g;
		fBase.b = sPSIn.m_vColour.b;
		fBase.a *= sPSIn.m_vColour.a;
	}
	else
	{
		float fGradient = fBase.g;
		fBase.r = sPSIn.m_vColour2.r * fGradient;
		fBase.g = sPSIn.m_vColour2.g * fGradient;
		fBase.b = sPSIn.m_vColour2.b * fGradient;
		fBase.a *= sPSIn.m_vColour2.a * fGradient;
	}
	return fBase;
}

float4 BasicSpritePS( SPSIn sPSData ) : COLOR0
{
	float4 fBase;
	if(sPSData.m_iTextureIndex == 0)
	{
		fBase = tex2D( g_sTextureArray[0], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 1)
	{
		fBase = tex2D( g_sTextureArray[1], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 2)
	{
		fBase = tex2D( g_sTextureArray[2], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 3)
	{
		fBase = tex2D( g_sTextureArray[3], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 4)
	{
		fBase = tex2D( g_sTextureArray[4], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 5)
	{
		fBase = tex2D( g_sTextureArray[5], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 6)
	{
		fBase = tex2D( g_sTextureArray[6], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 7)
	{
		fBase = tex2D( g_sTextureArray[7], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	fBase.a *= sPSData.m_vColour.a;
	return fBase;
}
float4 BasicSpritePSNoBlend( SPSIn sPSData ) : COLOR0
{
	float4 fBase;
	if(sPSData.m_iTextureIndex == 0)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[0], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 1)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[1], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 2)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[2], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 3)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[3], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 4)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[4], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 5)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[5], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 6)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[6], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	else if(sPSData.m_iTextureIndex == 7)
	{
		fBase = tex2D( g_sTextureArrayNoBlend[7], CorrectTexCoordForRes(sPSData.m_vTexCoord) );
	}
	fBase.a *= sPSData.m_vColour.a;
	return fBase;
}
float4 AsIsPS( SPSIn sPSData ) : COLOR0
{
	return tex2D( g_sTextureAsIs, CorrectTexCoordForRes(sPSData.m_vTexCoord) );
}
float4 BasicSpriteNoFilterPS( float2 vTexCoord : TEXCOORD0 ) : COLOR0
{
	return tex2D( g_sTexture0NoFilter, vTexCoord );
}

// Pixel Shaders <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// Techniques >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

technique BasicSprite
{
	pass P0
	{
		VertexShader = compile vs_3_0 BasicSpriteVS();
		PixelShader = compile ps_3_0 BasicSpritePS();
		
		CULLMODE = NONE;
	}
};

technique GUI
{
	pass P0
	{
		VertexShader = compile vs_3_0 GUIVS();
		PixelShader = compile ps_3_0 BasicSpritePS();
		
		CULLMODE = NONE;
	}
};

technique GUI_No_Blend
{
	pass P0
	{
		VertexShader = compile vs_3_0 GUIVS();
		PixelShader = compile ps_3_0 BasicSpritePSNoBlend();
		
		CULLMODE = NONE;
	}
};
technique BasicSpriteNoFilter
{
	pass P0
	{
		VertexShader = compile vs_3_0 BasicSpriteVS();
		PixelShader = compile ps_3_0 BasicSpriteNoFilterPS();
		
		CULLMODE = NONE;
	}
};
technique Font
{
	pass P0
	{
		VertexShader = compile vs_3_0 GUIVS();
		PixelShader = compile ps_3_0 FontPS();
		
		CULLMODE = NONE;
	}
};
technique AsIs
{
	pass P0
	{
		VertexShader = compile vs_3_0 AsIsVS();
		PixelShader = compile ps_3_0 AsIsPS();
		
		CULLMODE = NONE;
	}
};
// Techniques <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
