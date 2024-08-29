#pragma once

#include "uevr/API.hpp"

using namespace uevr;

class CH_Hacker_AnimBP_C : public API::UObject
{
  public:
	using API::UObject::get_full_name;

	static API::UClass *static_class()
	{
		static API::UClass *result = nullptr;
		if(!result) {
			result = API::get()->find_uobject<API::UClass>(L"AnimBlueprintGeneratedClass /Game/Art/FirstPerson/CH_Hacker_AnimBP.CH_Hacker_AnimBP_C");
		}
		return result;
	}

	static CH_Hacker_AnimBP_C*get_instance()
	{
		auto klass = CH_Hacker_AnimBP_C::static_class();
		CH_Hacker_AnimBP_C* Object = nullptr;
        
		if(klass) {
            std::vector<CH_Hacker_AnimBP_C*> List = klass->get_objects_matching<CH_Hacker_AnimBP_C>();
            for(size_t i = 0; i < List.size(); i++) {
                Object = List[i];
                
                std::wstring ObjName = Object->get_full_name();
                // API::get()->log_info("CH_Hacker_AnimBP_C: Object %d of %d, Object name: %ls",i, List.size(), ObjName.c_str());

				if(ObjName.find(L"CH_Hacker_AnimBP_C") != std::wstring::npos) 
				{ 
					// API::get()->log_info("CH_Hacker_AnimBP_C found");
                }
				else
				{
					// API::get()->log_info("CH_Hacker_AnimBP_C not found");
				}
            }

			return Object;  
		}
		return nullptr;
	}
    
	void SetUseIK(bool value)
	{
		static const auto func = CH_Hacker_AnimBP_C::static_class()->find_function(L"SetUseIK");
		if (!func) {
			// API::get()->log_info("SetUseIK not found");
			return;
		}
		else
		{
			// API::get()->log_info("SetUseIK found");
		}

		struct
		{
			bool value;
			bool res;
		} params{ .value = value, .res = false };

		process_event(func, &params);
	}
};