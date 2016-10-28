#pragma once

class Detour
{
public:
	Detour(int func, int hook_func)
		: func_((byte*)func),
		hook_func_(hook_func),
		original_bytes_(new byte[6]),
		new_bytes_(new byte[6]),
		is_applied_(false)
	{
		LoadFunctionMemory();
		Apply();
	}

	~Detour() 
	{
		if (is_applied_)
		{
			Restore();
			delete original_bytes_;
			delete new_bytes_;
		}
	}

	bool Apply()
	{
		if (is_applied_)
			return false;
		
		memcpy(func_, new_bytes_, 6);
		
		is_applied_ = true;
		return true;
	}

	bool Restore()
	{
		if (!is_applied_)
			return false;

		memcpy(func_, original_bytes_, 6);
		
		is_applied_ = false;
		return true;
	}

	void* GetOrig() 
	{
		return func_;
	}

private:
	void LoadFunctionMemory()
	{
		DWORD old_protection_;
		VirtualProtect(func_, 6, PAGE_EXECUTE_READWRITE, &old_protection_);

		*new_bytes_ = 0x68;  // push (address provided through hookFunc)
		*(int*)(new_bytes_ + 1) = hook_func_;
		new_bytes_[5] = 0xc3;  // return
		
		memcpy(original_bytes_, func_, 6);
	}
	
	int hook_func_;
	byte* func_;
	byte* original_bytes_;
	byte* new_bytes_;
	bool is_applied_;
};