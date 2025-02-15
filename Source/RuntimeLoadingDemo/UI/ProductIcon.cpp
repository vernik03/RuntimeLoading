// Fill out your copyright notice in the Description page of Project Settings.


#include "ProductIcon.h"
#include "Engine/Texture2D.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Components/Image.h"
#include "Misc/FileHelper.h"
#include "Modules/ModuleManager.h"

bool UProductIcon::LoadImageFromPath(const FString& ImagePath, UImage* TargetImage)
{
    // 1. Загружаем файл в массив байтов
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *ImagePath))
    {
        return false;
    }

    // 2. Получаем модуль ImageWrapper и определяем формат
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
    const EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(FileData.GetData(), FileData.Num());
    if (ImageFormat == EImageFormat::Invalid)
    {
        return false;
    }

    // 3. Создаем ImageWrapper
    const TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
    if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
    {
        return false;
    }

    // 4. Получаем RAW-данные
    TArray64<uint8> RawData;
    if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData) || RawData.Num() == 0)
    {
        return false;
    }

    // 5. Создаем текстуру
    UTexture2D* Texture = UTexture2D::CreateTransient(
        ImageWrapper->GetWidth(),
        ImageWrapper->GetHeight(),
        PF_B8G8R8A8
    );
    if (!Texture)
    {
        return false;
    }

    // 6. Копируем данные в текстуру
    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
    void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
    Mip.BulkData.Unlock();
    Texture->UpdateResource();

    // 7. Присваиваем текстуру виджету
    TargetImage->SetBrushFromTexture(Texture, true); // Второй аргумент: bMatchSize
    return true;
}


