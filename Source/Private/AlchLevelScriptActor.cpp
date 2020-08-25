// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchLevelScriptActor.h"

TArray<FVector2D> AAlchLevelScriptActor::GenerateLevel(UTexture2D* NoiseTexture, int Granularity)
{
    TextureCompressionSettings OldCompressionSettings = NoiseTexture->CompressionSettings; TextureMipGenSettings OldMipGenSettings = NoiseTexture->MipGenSettings; bool OldSRGB = NoiseTexture->SRGB;

    NoiseTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    NoiseTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
    NoiseTexture->SRGB = false;
    NoiseTexture->UpdateResource();

    const FColor* FormatedImageData = static_cast<const FColor*>( NoiseTexture->PlatformData->Mips[0].BulkData.LockReadOnly());
    TArray<FVector2D> FoliagePositions;

    for(int32 X = 0; X < NoiseTexture->GetSizeX(); X += Granularity)
    {
        for (int32 Y = 0; Y < NoiseTexture->GetSizeY(); Y += Granularity)
        {
            FColor PixelColor = FormatedImageData[Y * NoiseTexture->GetSizeX() + X];

            // Black/dark indicates where to place foliage
            if(ColorVal(PixelColor) < .5f)
            {
                FVector2D Position = FVector2D(X,Y);

                if(NoiseTexture->GetSizeX() != 0)
                    Position.X /= NoiseTexture->GetSizeX();
                
                if(NoiseTexture->GetSizeY() != 0)
                    Position.Y /= NoiseTexture->GetSizeY();
                
                FoliagePositions.Add(Position);
            }
        }
    }

    NoiseTexture->PlatformData->Mips[0].BulkData.Unlock();
    NoiseTexture->CompressionSettings = OldCompressionSettings;
    NoiseTexture->MipGenSettings = OldMipGenSettings;
    NoiseTexture->SRGB = OldSRGB;
    NoiseTexture->UpdateResource();
    
    return FoliagePositions;
}
