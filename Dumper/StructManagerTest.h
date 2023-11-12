#pragma once
#include "HashStringTable.h"
#include "StructManager.h"
#include <cassert>

#define CHECK_RESULT(Result, Expected) \
if (Result != Expected) \
{ \
	std::cout << __FUNCTION__ << ":\nResult: \n" << Result << "\nExpected: \n" << Expected << std::endl; \
} \
else \
{ \
	std::cout << __FUNCTION__ << ": Everything is fine!" << std::endl; \
}

class StructManagerTest
{
public:
	static inline void TestAll()
	{
		TestInit();
		TestInfo();
		TestIsFinal();
		std::cout << std::endl;
	}

	static inline void TestInit()
	{
		StructManager StructInfos;
		StructInfos.Init();
		StructInfos.Init();
		StructInfos.Init();

		std::cout << __FUNCTION__ << " --> NumStructInfos: 0x" << std::hex << StructInfos.StructInfoOverrides.size() << "\n" << std::endl;
	}

	static inline void TestInfo()
	{
		StructManager StructInfos;
		StructInfos.Init(); 

		UEStruct AActor = ObjectArray::FindClassFast("Actor");
		UEStruct UObject = ObjectArray::FindClassFast("Object");
		UEStruct UEngine = ObjectArray::FindClassFast("Engine");
		UEStruct FVector = ObjectArray::FindObjectFast<UEStruct>("Vector");
		UEStruct FTransform = ObjectArray::FindObjectFast<UEStruct>("Transform");
		UEStruct UKismetSystemLibrary = ObjectArray::FindClassFast("KismetSystemLibrary");
		UEStruct ULevelStreaming = ObjectArray::FindClassFast("LevelStreaming");
		//UEStruct FPrimitiveComponentInstanceData = ObjectArray::FindObjectFast<UEStruct>("PrimitiveComponentInstanceData");
		//UEStruct FStaticMeshComponentInstanceData = ObjectArray::FindObjectFast<UEStruct>("StaticMeshComponentInstanceData");
		UEStruct UFortAIHotSpotSlot = ObjectArray::FindClassFast("FortAIHotSpotSlot");
		UEStruct UAIHotSpotSlot = ObjectArray::FindClassFast("AIHotSpotSlot");

		StructInfoHandle ActorInfo = StructInfos.GetInfo(AActor);
		StructInfoHandle ObjectInfo = StructInfos.GetInfo(UObject);
		StructInfoHandle EngineInfo = StructInfos.GetInfo(UEngine);
		StructInfoHandle VectorInfo = StructInfos.GetInfo(FVector);
		StructInfoHandle TransformInfo = StructInfos.GetInfo(FTransform);
		StructInfoHandle KismetSystemLibraryInfo = StructInfos.GetInfo(UKismetSystemLibrary);
		StructInfoHandle LevelStreamingInfo = StructInfos.GetInfo(ULevelStreaming);
		//StructInfoHandle PrimitiveComponentInstanceData = StructInfos.GetInfo(FPrimitiveComponentInstanceData);
		//StructInfoHandle StaticMeshComponentInstanceData = StructInfos.GetInfo(FStaticMeshComponentInstanceData);
		//StructInfoHandle FortAIHotSpotSlot = StructInfos.GetInfo(UFortAIHotSpotSlot);
		//StructInfoHandle AIHotSpotSlot = StructInfos.GetInfo(UAIHotSpotSlot);

#define StructInfoHandleToDebugInfo(InfoHandle) \
	InfoHandle.GetName().GetUniqueName(), InfoHandle.GetName().IsUnique(), InfoHandle.GetSize(), InfoHandle.GetAlignment(), InfoHandle.ShouldUseExplicitAlignment(), InfoHandle.IsFinal()

		std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(ActorInfo));
		std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(ObjectInfo));
		std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(EngineInfo));
		std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(VectorInfo));
		std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(TransformInfo));
		std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(KismetSystemLibraryInfo));
		std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(LevelStreamingInfo));
		//std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(PrimitiveComponentInstanceData));
		//std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(StaticMeshComponentInstanceData));
		//std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(FortAIHotSpotSlot));
		//std::cout << std::format("{}[{}]: {{ Size=0x{:X}, Alignment=0x{:X}, bUseExplicitAlignment={}, bIsFinal={} }}\n", StructInfoHandleToDebugInfo(AIHotSpotSlot));
		std::cout << std::endl;
	}

	static inline void TestIsFinal()
	{
		StructManager StructInfos;
		StructInfos.Init();

		bool bIsEverythingFine = true;

		for (auto& [StructIdx, Info] : StructInfos.StructInfoOverrides)
		{
			if (!Info.bIsFinal)
				continue;

			UEStruct Struct = ObjectArray::GetByIndex<UEStruct>(StructIdx);

			for (auto Obj : ObjectArray())
			{
				if (!Obj.IsA(EClassCastFlags::Struct) || Obj.IsA(EClassCastFlags::Function))
					continue;

				if (Obj.Cast<UEStruct>().GetSuper() == Struct)
				{
					std::cout << std::format("Struct '{}' was marked as 'final' even tho the child class '{}' is inheriting from it.", Struct.GetCppName(), Obj.GetCppName()) << std::endl;
					break;
				}
			}
		}

		std::cout << __FUNCTION__ << ": " << (bIsEverythingFine ? "Everything is fine!" : "Nothing is fine!") << std::endl;
	}
};