#ifdef _MSC_VER
#pragma pack(push, 0x8)
#endif

#include "CoreUObject_parameters.hpp"
namespace UE4
{
	//---------------------------------------------------------------------------
	//Functions
	//---------------------------------------------------------------------------


	//---------------------------------------------------------------------------
	//UObject Functions
	//---------------------------------------------------------------------------

	std::string UObject::GetName() const
	{
		auto Name = *reinterpret_cast<FName*>((byte*)this + GameProfile::SelectedGameProfile.defs.UObject.Name);
		std::string name(Name.GetName());
		if (Name.Number > 0)
		{
			name += '_' + std::to_string(Name.Number);
		}

		auto pos = name.rfind('/');
		if (pos == std::string::npos)
		{
			return name;
		}

		return name.substr(pos + 1);
	}

	std::string UObject::GetFullName() const
	{
		std::string name;
		auto Class = GetClass();
		if (Class != nullptr)
		{
			std::string temp;
			for (auto p = GetOuter(); p; p = p->GetOuter())
			{
				temp = p->GetName() + "." + temp;
			}

			name = Class->GetName();
			name += " ";
			name += temp;
			name += GetName();
		}

		return name;
	}

	int32_t UObject::GetIndex() const 
	{ 
		return Read<int32_t>((byte*)this + GameProfile::SelectedGameProfile.defs.UObject.Index); 
	};

	UClass* UObject::GetClass() const 
	{ 
		return Read<class UClass*>((byte*)this + GameProfile::SelectedGameProfile.defs.UObject.Class); 
	};

	UObject* UObject::GetOuter() const 
	{ 
		return Read<UObject*>((byte*)this + GameProfile::SelectedGameProfile.defs.UObject.Outer); 
	};

	bool UObject::IsA(UClass* cmp) const
	{
		for (auto super = GetClass(); super; super = static_cast<UClass*>(super->GetSuperField()))
		{
			if (super == cmp)
			{
				return true;
			}
		}

		return false;
	}

	void UObject::ExecuteUbergraph(int EntryPoint)
	{
		static auto fn = UObject::FindObject<UFunction>("Function CoreUObject.Object.ExecuteUbergraph");

		UObject_ExecuteUbergraph_Params params;
		params.EntryPoint = EntryPoint;

		//auto flags = fn->FunctionFlags;

		UObject::ProcessEvent(fn, &params);

		//fn->FunctionFlags = flags;
	}

	bool UObject::DoesObjectContainFunction(std::string Function)
	{
		if (IsChunkedArray())
		{
			for (int i = 0; i < GObjects->GetAsChunckArray().Num(); ++i)
			{
				auto object = GObjects->GetAsChunckArray().GetByIndex(i).Object;

				if (object == nullptr)
				{
					continue;
				}

				if (object->GetName() == Function)
				{
					if (object->GetOuter() == this->GetClass())
					{
						return true;
					}
				}
			}
			return false;
		}
		else
		{
			for (int i = 0; i < GObjects->GetAsTUArray().Num(); ++i)
			{
				auto object = GObjects->GetAsTUArray().GetByIndex(i).Object;

				if (object == nullptr)
				{
					continue;
				}

				if (object->GetName() == Function)
				{
					if (object->GetOuter() == this->GetClass())
					{
						return true;
					}
				}
			}
			return false;
		}
		return false;
	}

	void UObject::ProcessEvent(class UFunction* function, void* parms)
	{
		return reinterpret_cast<void(*)(UObject*, class UFunction*, void*)>(GameProfile::SelectedGameProfile.ProcessEvent)(this, function, parms);
	}

	UObject* UObject::StaticLoadObject(class UClass* uclass, UObject* InOuter, const wchar_t* InName, const wchar_t* Filename, unsigned int LoadFlags, void* Sandbox, bool bAllowObjectReconciliation)
	{
		return reinterpret_cast<UObject * (__fastcall*)(class UClass*, UObject*, const wchar_t*, const wchar_t*, unsigned int, void*, bool)>(GameProfile::SelectedGameProfile.StaticLoadObject)(uclass, InOuter, InName, Filename, LoadFlags, Sandbox, bAllowObjectReconciliation);
	}

	bool UObject::CallFunctionByNameWithArguments(const wchar_t* Str, void* Ar, UE4::UObject* Executor, bool bForceCallWithNonExec)
	{
		return reinterpret_cast<bool (*)(UE4::UObject*, const wchar_t*, void*, UE4::UObject*, bool)>(GameProfile::SelectedGameProfile.CallFunctionByNameWithArguments)(this, Str, Ar, Executor, bForceCallWithNonExec);
	}

	//---------------------------------------------------------------------------
	//UField Functions
	//-

	UField* UField::GetNext() const 
	{ 
		return Read<UField*>((byte*)this + GameProfile::SelectedGameProfile.defs.UField.Next); 
	};

	//---------------------------------------------------------------------------
	//UClass Functions
	//---------------------------------------------------------------------------

	UClass* UClass::LoadClassFromString(const wchar_t* InName, bool bAllowObjectReconciliation)
	{
		return (UClass*)UObject::StaticLoadObject(UClass::StaticClass(), nullptr, InName, nullptr, 0, nullptr, bAllowObjectReconciliation);
	}

	//---------------------------------------------------------------------------
	//UStruct Functions
	//---------------------------------------------------------------------------

	UStruct* UStruct::GetSuperField() const 
	{ 
		return Read<UStruct*>((byte*)this + GameProfile::SelectedGameProfile.defs.UStruct.SuperStruct); 
	};

	UField* UStruct::GetChildren() const 
	{ 
		return Read<UField*>((byte*)this + GameProfile::SelectedGameProfile.defs.UStruct.Children); 
	};

	int32_t UStruct::GetPropertySize() const 
	{ 
		return Read<int32_t>((byte*)this + GameProfile::SelectedGameProfile.defs.UStruct.PropertiesSize); 
	};

	//---------------------------------------------------------------------------
	//UFunction Functions
	//---------------------------------------------------------------------------

	int32_t UFunction::GetFunctionFlags() const { return Read<int32_t>((byte*)this + GameProfile::SelectedGameProfile.defs.UFunction.FunctionFlags); };
	void* UFunction::GetFunction() const { return Read<void*>((byte*)this + GameProfile::SelectedGameProfile.defs.UFunction.Func); };

	//---------------------------------------------------------------------------
	//UEProperty Functions
	//---------------------------------------------------------------------------

	int32_t UEProperty::GetArrayDim() const
	{
		return Read<int32_t>((byte*)this + GameProfile::SelectedGameProfile.defs.Property.ArrayDim);
	};
	/*
	int32_t UEProperty::GetElementSize() const
	{
		return Read<int32_t>((byte*)this + GameProfile::SelectedGameProfile.defs.Property.ElementSize);
	};
	*/

	int32_t UEProperty::GetOffset() const
	{
		return Read<int32_t>((byte*)this + GameProfile::SelectedGameProfile.defs.Property.Offset);
	};

	//---------------------------------------------------------------------------
	//FField Functions
	//---------------------------------------------------------------------------

	/*FName* FField::GetClass() const
	{
		return Read<class FName*>((byte*)this + GameProfile::SelectedGameProfile.defs.FField.Class);
	};*/

	FField* FField::GetNext() const
	{
		return Read<class FField*>((byte*)this + GameProfile::SelectedGameProfile.defs.FField.Next);
	};

	std::string FField::GetName() const
	{
		auto Name = *reinterpret_cast<FName*>((byte*)this + GameProfile::SelectedGameProfile.defs.FField.Name);
		return Name.GetName();
	};

	//---------------------------------------------------------------------------
	//UWorld Functions
	//---------------------------------------------------------------------------

	ULevel* UWorld::GetPersistentLevel() const
	{ 
		return Read<ULevel*>((byte*)this + GameProfile::SelectedGameProfile.defs.UWorld.PersistentLevel); 
	};

	AActor* UWorld::SpawnActor(UClass* uclass, const  FTransform* transform, const FActorSpawnParameters* params)
	{
		return reinterpret_cast<AActor * (*)(UWorld*, UClass*, const FTransform*, const FActorSpawnParameters*)>(GameProfile::SelectedGameProfile.SpawnActorFTrans)(this, uclass, transform, params);
	}

	//---------------------------------------------------------------------------
	//ULevel Functions
	//---------------------------------------------------------------------------

	TArray<class AActor*> ULevel::GetWorldActors() const
	{ 
		return Read<TArray<class AActor*>>((byte*)this + GameProfile::SelectedGameProfile.defs.ULevel.WorldArray); 
	};

	//---------------------------------------------------------------------------
	//Actor Functions
	//---------------------------------------------------------------------------

	FTransform AActor::GetTransform()
	{
		static auto fn = UObject::FindObject<UFunction>("Function Engine.Actor.GetTransform");
		struct
		{
			FTransform ReturnValue;
		}params;
		UObject::ProcessEvent(fn, &params);
		return params.ReturnValue;
	}

	FRotator AActor::GetActorRotation()
	{
		static auto fn = UObject::FindObject<UFunction>("Function Engine.Actor.K2_GetActorRotation");
		struct
		{
			FRotator ReturnValue;
		}params;
		UObject::ProcessEvent(fn, &params);
		return params.ReturnValue;
	}

	//---------------------------------------------------------------------------
	//GameplayStatics Functions
	//---------------------------------------------------------------------------

	class AActor* UGameplayStatics::BeginDeferredActorSpawnFromClass(class UClass* ActorClass, const struct FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, class AActor* Owner)
	{
		static auto fn = UObject::FindObject<UFunction>("Function Engine.GameplayStatics.BeginDeferredActorSpawnFromClass");
		struct
		{
			class UObject* WorldContextObject;
			class UClass* ActorClass;
			struct FTransform SpawnTransform;
			ESpawnActorCollisionHandlingMethod CollisionHandlingOverride;
			class AActor* Owner;
			class AActor* ReturnValue;
		}params;
		params.WorldContextObject = UWorld::GetWorld();
		params.ActorClass = ActorClass;
		params.SpawnTransform = SpawnTransform;
		params.CollisionHandlingOverride = CollisionHandlingOverride;
		params.Owner = Owner;
		UObject::ProcessEvent(fn, &params);
		return params.ReturnValue;
	};

	class FString UGameplayStatics::GetCurrentLevelName(bool bRemovePrefixString)
	{
		static auto fn = UObject::FindObject<UFunction>("Function Engine.GameplayStatics.GetCurrentLevelName");
		struct
		{
			class UObject* WorldContextObject;
			bool bRemovePrefixString;
			class FString ReturnValue;
		}params;
		params.WorldContextObject = UWorld::GetWorld();
		params.bRemovePrefixString = bRemovePrefixString;
		UObject::ProcessEvent(fn, &params);
		return params.ReturnValue;
	}

}

#ifdef _MSC_VER
#pragma pack(pop)
#endif
