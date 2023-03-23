/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
	#define Debug(x, ...)		 //if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Cyan, FString::Printf(TEXT(x), __VA_ARGS__));}
	#define DebugInfo(x, ...)	 //if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Green, FString::Printf(TEXT(x), __VA_ARGS__));}
	#define DebugError(x, ...)	 //if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Red, FString::Printf(TEXT(x), __VA_ARGS__));}
	#define DebugWarning(x, ...) //if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}
	#define DebugData(x, ...)	 //if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Purple, FString::Printf(TEXT(x), __VA_ARGS__));}
	#define DebugAction(x, ...)  //if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 600.f, FColor::Blue, FString::Printf(TEXT(x), __VA_ARGS__));}
#else
	#define Debug(x, ...)
	#define DebugInfo(x, ...)
	#define DebugError(x, ...)
	#define DebugWarning(x, ...)
	#define DebugData(x, ...)
	#define DebugAction(x, ...)
#endif
