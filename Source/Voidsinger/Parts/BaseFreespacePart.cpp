//// Fill out your copyright notice in the Description page of Project Settings.
//
//#include "BaseFreespacePart.h"
//#include "PartGridComponent.h"
//
//
////Comment -Mabel Suggestion
//void UBaseFreespacePart::MergeParts(UBaseFreespacePart* Other)
//{
//	FIntPoint RelativeLoc = Other->GetPartGridLocation() - GetPartGridLocation();
//
//	//This would take a long time if a part was very large. I wish you could do append or += somehow here. Or maybe merge the smaller one into the bigger one every time?
//	//I don't know. It's annoying that ActualShape is relative in this case. -Mabel Suggestion
//	for (FIntPoint PixelLoc : Other->GetShape())
//	{
//		ActualShape.Emplace(PixelLoc + RelativeLoc);
//	}
//	Other->DestroyPart();
//}
