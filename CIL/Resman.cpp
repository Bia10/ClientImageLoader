#include "CIL.h"

enum RESMAN_PARAM {
	RC_AUTO_SERIALIZE = 0x1,
	RC_AUTO_SERIALIZE_NO_CACHE = 0x2,
	RC_NO_AUTO_SERIALIZE = 0x4,
	RC_DEFAULT_AUTO_SERIALIZE = 0x0,
	RC_AUTO_SERIALIZE_MASK = 0x7,
	RC_AUTO_REPARSE = 0x10,
	RC_NO_AUTO_REPARSE = 0x20,
	RC_DEFAULT_AUTO_REPARSE = 0x0,
	RC_AUTO_REPARSE_MASK = 0x30,
};

struct CWvsApp
{
	/*Nothing for now*/
};

struct Ztl_bstr_t
{
	struct DATA
	{
		wchar_t *m_wstr;
		char *m_str;
		unsigned int m_RefCount;
	};

	Ztl_bstr_t::DATA* m_Data;
};

// Thanks @Darter
typedef void(__fastcall* CWvsApp__InitializeResMan_t)(CWvsApp* ecx, void* edx);
typedef void*(__cdecl* get_rm_t)();
typedef void(__cdecl* PcCreateObject_IWzResMan_t)(void *, void *);
typedef HRESULT(__fastcall* IWzResMan__SetResManParam_t)(void *, void *, int a, int b, int c);
typedef void*(__cdecl* _com_IWzResMan_arrow_t)(void *, void *);
typedef void*(__cdecl* get_root_t)();
typedef void(__cdecl* PcCreateObject_IWzNameSpace_t)(void *, void *);
typedef HRESULT(__cdecl* PcSetRootNameSpace_t)(void *);
typedef HRESULT(__fastcall* IWZNameSpace__Mount_t)(void *, void *, Ztl_bstr_t, void *, int a);
typedef void*(__cdecl* _com_IWzNameSpace_deref_t)(void *, void *);
typedef void*(__cdecl* _com_IWzNameSpace_arrow_t)(void *, void *);
typedef void(__cdecl* PcCreateObject_IWzFileSystem_t)(void *, void *);
typedef void(__cdecl* CWvsApp__Dir_BackSlashToSlash_t)(char *);
typedef void(__cdecl* CWvsApp__Dir_upDir_t)(char *);
typedef void(__cdecl* ztl_bstr_constructor_t)(void *, void *, const char *);
typedef HRESULT(__fastcall* IWzFileSystem__Init_t)(void *, void *, Ztl_bstr_t);
typedef void*(__cdecl* _com_IWzFileSystem_arrow_t)(int *, void *);
typedef void*(__fastcall* _com_IWzFileSystem_ptr_t)(int *, void *);
typedef void*(__cdecl* _com_IWzPackage_deref_t)(void *, void *);

BOOL Hook_InitializeResMan(BOOL bEnable) {

	// Init
	static auto CWvsApp__InitializeResMan = reinterpret_cast<CWvsApp__InitializeResMan_t>(0x008E7BA0);

	// ResMan
	static auto get_rm = reinterpret_cast<get_rm_t>(0x00000000);
	static auto PcCreateObject_IWzResMan = reinterpret_cast<PcCreateObject_IWzResMan_t>(0x00000000);
	static auto IWzResMan__SetResManParam = reinterpret_cast<IWzResMan__SetResManParam_t>(0x00000000);
	static auto _com_IWzResMan_arrow = reinterpret_cast<_com_IWzResMan_arrow_t>(0x00000000);

	// NameSpace
	static auto get_root = reinterpret_cast<get_root_t>(0x00000000);
	static auto PcCreateObject_IWzNameSpace = reinterpret_cast<PcCreateObject_IWzNameSpace_t>(0x00000000);
	static auto PcSetRootNameSpace = reinterpret_cast<PcSetRootNameSpace_t>(0x00000000);
	static auto IWZNameSpace__Mount = reinterpret_cast<IWZNameSpace__Mount_t>(0x00000000);
	static auto _com_IWzNameSpace_deref = reinterpret_cast<_com_IWzNameSpace_deref_t>(0x00000000);
	static auto _com_IWzNameSpace_arrow = reinterpret_cast<_com_IWzNameSpace_arrow_t>(0x00000000);

	// Filesystem
	static auto PcCreateObject_IWzFileSystem = reinterpret_cast<PcCreateObject_IWzFileSystem_t>(0x00000000);
	static auto CWvsApp__Dir_BackSlashToSlash = reinterpret_cast<CWvsApp__Dir_BackSlashToSlash_t>(0x00000000);
	static auto CWvsApp__Dir_upDir = reinterpret_cast<CWvsApp__Dir_upDir_t>(0x00000000);
	static auto ztl_bstr_constructor = reinterpret_cast<ztl_bstr_constructor_t>(0x00000000);
	static auto IWzFileSystem__Init = reinterpret_cast<IWzFileSystem__Init_t>(0x00000000);
	static auto _com_IWzFileSystem_arrow = reinterpret_cast<_com_IWzFileSystem_arrow_t>(0x00000000);
	static auto _com_IWzFileSystem_ptr = reinterpret_cast<_com_IWzFileSystem_ptr_t>(0x00000000);
	static auto _com_IWzPackage_deref = reinterpret_cast<_com_IWzPackage_deref_t>(0x00000000);

	CWvsApp__InitializeResMan_t Hook = [](CWvsApp* ecx, void* edx) -> void {

		auto g_rm = get_rm();

		// Create Resource Manager Instance.
		PcCreateObject_IWzResMan((void*)L"ResMan", g_rm);

		void* pIWzResMan_Instance = _com_IWzResMan_arrow(g_rm, nullptr);
		IWzResMan__SetResManParam(pIWzResMan_Instance, nullptr, RC_AUTO_REPARSE | RC_AUTO_SERIALIZE, -1, -1);

		// Create NameSpace Instance.
		auto g_root = get_root();
		PcCreateObject_IWzNameSpace((void*)L"NameSpace", g_root);

		void* pIWzNameSpace_Instance = _com_IWzNameSpace_deref(g_root, nullptr);
		PcSetRootNameSpace(pIWzNameSpace_Instance); // PcSetRootNameSpace(*((void**)g_root));

		// Initialize FileSystem
		int pIWzFileSystem;
		void* com_iwfsp = _com_IWzFileSystem_ptr(&pIWzFileSystem, nullptr);
		PcCreateObject_IWzFileSystem((void*)L"NameSpace#FileSystem", com_iwfsp);
		char sStartPath[MAX_PATH];
		GetModuleFileNameA(NULL, sStartPath, MAX_PATH);
		CWvsApp__Dir_BackSlashToSlash(sStartPath);
		CWvsApp__Dir_upDir(sStartPath);

		printf("Path: %s\n", sStartPath);

		Ztl_bstr_t* p_bstr_data = new Ztl_bstr_t();
		ztl_bstr_constructor(p_bstr_data, nullptr, sStartPath);

		void* pIWzFileSystem_Idek = _com_IWzFileSystem_arrow(&pIWzFileSystem, nullptr);
		auto iahefauf = IWzFileSystem__Init(pIWzFileSystem_Idek, nullptr, *p_bstr_data);

		// Mount time OwO
		// bms does some checks, but who cares about that
		void* ppRoot = _com_IWzNameSpace_arrow(g_root, nullptr); // *((void**)g_root)
		Ztl_bstr_t* p_bstr_path = new Ztl_bstr_t();
		void* p_down;

		ztl_bstr_constructor(p_bstr_path, nullptr, (const char*)"/");
		p_down = _com_IWzPackage_deref(&pIWzFileSystem, nullptr);

		auto ajefgeaf = IWZNameSpace__Mount(ppRoot, nullptr, *p_bstr_path, p_down, 0);

		// Create FileSystem Instance
		int pDataFileSystem;
		void* com_iwdsp = _com_IWzFileSystem_ptr(&pDataFileSystem, nullptr);
		PcCreateObject_IWzFileSystem((void*)L"NameSpace#FileSystem", com_iwdsp);

		Ztl_bstr_t* pDataStr = new Ztl_bstr_t();
		ztl_bstr_constructor(pDataStr, nullptr, "./Data");

		void* pDataArrow = _com_IWzFileSystem_arrow(&pDataFileSystem, nullptr);
		auto fsinitret = IWzFileSystem__Init(pDataArrow, nullptr, *pDataStr);

		p_down = _com_IWzPackage_deref(&pDataFileSystem, nullptr);
		p_bstr_path = new Ztl_bstr_t();
		ztl_bstr_constructor(p_bstr_path, nullptr, (const char*)"/");
		ppRoot = _com_IWzNameSpace_arrow(get_root(), nullptr); // *((void**)g_root)
		auto ejhafaf = IWZNameSpace__Mount(ppRoot, nullptr, *p_bstr_path, p_down, 0);

	};


	return SetHook(bEnable, reinterpret_cast<void**>(&CWvsApp__InitializeResMan), Hook);
}
