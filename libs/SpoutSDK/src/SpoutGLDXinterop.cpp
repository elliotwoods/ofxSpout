/**

	spoutGLDXinterop.cpp

	See also - spoutDirectX, spoutSenderNames

	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

		Copyright (c) 2014-2016, Lynn Jarvis. All rights reserved.

		Redistribution and use in source and binary forms, with or without modification, 
		are permitted provided that the following conditions are met:

		1. Redistributions of source code must retain the above copyright notice, 
		   this list of conditions and the following disclaimer.

		2. Redistributions in binary form must reproduce the above copyright notice, 
		   this list of conditions and the following disclaimer in the documentation 
		   and/or other materials provided with the distribution.

		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"	AND ANY 
		EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
		OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE	ARE DISCLAIMED. 
		IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
		INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
		PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
		INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
		LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
		========================

		15-07-14	- ReadTexturePixels - allowed for variable OpenGL format instead of RGB only.
					- Needs testing. 
					- TODO - variable gl format for WriteTexturePixels
		21.07.14	- removed local fbo and replaced with temporary fbo within
					  texture functions due to problems with Max / Jitter
		22-07-14	- added option for DX9 or DX11
		23-07-14	- cleanup of DX9 / DX11 functions
		29-07-14	- pass format 0 for DX9 sender
		31-07-14	- Corrected DrawTexture aspect argument
		13-08-14	- OpenGL texture retained on cleanup
		14-08-14	- Corrected texture delete without context
		16-08-14	- created DrawToSharedTexture
		18-08-14	- debugging with WriteTexture method
		 -- names class revision additions --
		19-08-14	- activated event locks
		01.09.14	- removed temp fbo for texture transfers and returned to use of a common fbo
					- delete texture and fbo on cleanup
					- set texture and fbo to zero on cleanup, otherwise errors in Jitter
					- changed to vertex array draw for DrawToSharedTexture
					- Removed PAINT message from OpenDirectX9 due to crash of sender in Magic
		03.09.14	- Replaced with UpdateWindow and limited to Resolume only.
					- Cleanup
		15.09.14	- corrected access lock for DrawToSharedTexture and ReadTexturePixels
		21.09.14	- mutex texture access locks
		23.09.14	- moved general mutex texture access lock to the SpoutDirectX class
		23.09.14	- test for DirectX 11 support in UseDX9, IsDX9 and OpenDirectX
		24.09.14	- save and restore fbo for read/write/drawto texture
		28.09.14	- Added GL format argument for WriteTexturePixels
					- Added bAlignment  (4 byte alignment) flag for WriteTexturePixels
					- Changed GLformat argument from int to GLenum in ReadTexturePixels
					- Changed default GLformat from GL_RGB to GL_RGBA in ReadTexturePixels
					- Added Host FBO argument for ReadTexture, DrawToSharedTexture, WriteTexture
		12.10.14	- cleaned up CreateInterop for sender updates
		15.10.14	- added safety release of texture in CreateDX9interop in case of previous application crash
		17.10.14	- Directx 11 release context before device
		21.10.14	- removed keyed mutex lock due to reported driver problems
		21.10.14	- Allow for compatible texture formats
					  DirectX 11 format 87, DirectX9 D3DFMT_X8R8G8B8, and the default D3DFMT_A8R8G8B8
		21.10.14	- Allow DirectX texture formats to be registered in CreateInterop
		24.10.14	- Fall back to DirectX 9 if DirectX11 init fails
		24.11.14	- removed context print statement in CleanupInterop
		23.12.14	- added host fbo arg to ReadTexturePixels
					  Changed readback method to glReadPixels
		04.02.15	- Changed header default to DirectX 9 instead of DirectX 11
		09.02.15	- added invert flag to DrawSharedTexture (default true with no args)
		12.02.15	- Changed OpenDirectX to check for Intel graphics and open DirectX 9 if present
		13.02.15	- OpenDirectX9 included SendMessageTimeout before attempting to get the fg window text
		14.02.15	- Used PathStripPath function requiring shlwapi.h - (see SpoutSDK.h)
		--
		21.05.15	- OpenDirectX - Intel auto detection removed and replaced by an installer with option for DX11
					- Programmer must SetDX9(true) for compilation using DirectX 11 functions
		29.05.15	- Included SetAdapter for multiple adapters - Franz Hildgen.
		01.06.15	- Read/Write DX9 mode from registry
		02.06.15	- Added GetAdapter, GetNumAdapters, GetAdapterName
		08.06.15	- removed dx9 flag from setadapter
		20.06.15	- removed Intel / Optimus graphics detecion from GetAdapterInfo
		08.07.15	- Only reads registry for DX9 mode but does not write it
		25.08.15	- moved release texture before release of device - to be checked for a receiver
		26.08.15	- set the executable path to the sender's shared info
					- Added GetHostPath to retrieve the path of the host that produced the sender
		28.08.15	- Introduced RedrawWindow again instead of WM_PAINT due to crash with Windows 10
					  Window invalidate and redraw works OK - Win 7 32bit.
		01.09.15	- added MessageBox error warnings in CreateInterop and SpoutGLextensions::LoadGLextensions
		12.09.15	- Finalised revised SpoutMemoryShare class and functions
		14.09.15	- ReadTexture / WriteTexture change default invert to true in line with Spout class default
		15.09.15	- GetMemoryShare - do not return memoryshare true if the 2.005 installer has not set the 
					  "MemoryShare" key to avoid problems with 2.004 apps.
		21.09.15	- Change SetMemoryShareMode to apply only if there is a 2.005 installation or later
		22.09.15	- fixed source pointer start offset for memoryshare flip vertically between pBuf and pixels
		24.09.15	- Removed Enable/Disable texture target from texture bind. When a texture is first bound,
					  it assumes the specified target and we pass the required target already.
					  Problem noted with Cinder in memorshare mode.
		25.09.15	- Changed SetMemoryShare to allow for true or false.
		09.10.15	- TODO : check invert defaults for WriteTexture, WriteMemory, DrawSharedTexture, DrawToSharedTexture
					- Removed FlipVertical function - now unused with 2.005
		10.10.15	- Created DrawSharedMemory and DrawToSharedMemory - to be tested
					- made m_dxShareHandle public
		11.10.15	- Protect aganst NULL texture in ReadMemory
		12.10.15	- Add glCheckFramebufferStatusEXT to ReadTexture
		15.10.15	- Add PrintFBOstatus function
					- Add glCheckFramebufferStatusEXT and error report to all functions using glFramebufferTexture2DEXT
		22.10.15	- Moved DX11available from the directx class and changed to a test of DX11 device open
					  rather than operating system detection
		14.11.15	- changed functions to "const char *" where required
		20.11.15	- Registry read/write moved to SpoutDirectX class
		25.02.16	- Introduced read of MaxSenders from the registry for 2.005

 */

#include "spoutGLDXinterop.h"

spoutGLDXinterop::spoutGLDXinterop() {

	m_hWnd				= NULL;
	m_glTexture			= 0;
	m_fbo               = 0;
	m_TexID             = 0;
	m_hInteropObject	= NULL;
	m_hSharedMemory		= NULL;
	m_hInteropDevice	= NULL;
	
	m_hAccessMutex      = NULL;
	// LJ DEBUG
	m_hReceiverAccessMutex = NULL;
	iLastFrame = 0;

	// DX9
	bUseDX9				= false; // Use DX11 (default false) or DX9 (true)
	bUseMemory			= false; // Memoryshare
	m_pD3D				= NULL;
	m_pDevice			= NULL;
	m_dxTexture			= NULL;
	DX9format           = D3DFMT_A8R8G8B8; // default format for DX9 (21)
	
	// DX11
	g_pd3dDevice		= NULL;
	g_pSharedTexture	= NULL;
	g_pImmediateContext = NULL;
	g_driverType		= D3D_DRIVER_TYPE_NULL;
	g_featureLevel		= D3D_FEATURE_LEVEL_11_0;
	DX11format			= DXGI_FORMAT_B8G8R8A8_UNORM; // Default compatible with DX9

	m_bInitialized		= false;
	bExtensionsLoaded	= false;
	bFBOavailable		= false;
	bBLITavailable		= false;
	bPBOavailable		= false;
	bSWAPavailable		= false;

	// 08.06.15 - get DX9 flags from the registry
	// User can also request the modes to overwrite this
	DWORD dwDX9 = 0;
	spoutdx.ReadDwordFromRegistry(&dwDX9, "Software\\Leading Edge\\Spout", "DX9");
	bUseDX9 = (dwDX9 == 1); // Set the global bUseDX9 flag in this class

	// Memoryshare depends on the 2.005 SpoutDirectX utility "MemoryShare" checkbox and
	// the registry key set by the 2.005 installer which won't be present for 2.004 apps
	DWORD dwMemory = 0;
	if(spoutdx.ReadDwordFromRegistry(&dwMemory, "Software\\Leading Edge\\Spout", "MemoryShare")) {
		// Set the global bUseMemory flag in this class to the user setting for 2.005 and greater
		bUseMemory = (dwMemory == 1);
	}
	else {
		// 2.004 apps will not have the registry setting so will always be texture share
		// If the hardware is not compatible they will simply fail to work
		bUseMemory = false; 
	}

	// 24.02.16 - max senders
	// Retrieve max senders from the registry
	// Depends on the 2.005 SpoutDirectX utility "MemoryShare" checkbox and
	// the registry key set by the 2.005 installer which won't be present for 2.004 apps
	DWORD dwSenders = 10;
	if(spoutdx.ReadDwordFromRegistry(&dwSenders, "Software\\Leading Edge\\Spout", "MaxSenders")) {
		printf("spoutGLDXinterop - 2.005 - max senders = %d\n", dwSenders);
		senders.SetMaxSenders((int)dwSenders);
	}
	else {
		printf("spoutGLDXinterop - 2.004\n");
	}

}

spoutGLDXinterop::~spoutGLDXinterop() {
	// Because cleanup is not here it has to be specifically called
	// This is because it can crash on exit - see cleanup for details
	m_bInitialized = false;

}


// For external access so that the local global variables are used
// Look for Intel graphics and open DirectX 9 if present
// 21.04.15 - Auto detection removed and replaced by an installer with option for DX11
bool spoutGLDXinterop::OpenDirectX(HWND hWnd, bool bDX9)
{
	// char renderadapter[256];
	// char renderdescription[256];
	// char renderversion[256];
	// char displaydescription[256];
	// char displayversion[256];

	// If user set DX9 then just use it.
	// Also check for DirectX 11 availability.
	// DX11available tests whether DX11 intitalization succeeds
	// and this check will then switch to DirectX 9
	if(bDX9 || !DX11available()) {
		bUseDX9 = true;
		return (OpenDirectX9(hWnd));
	}

	// Open DX11	
	if(OpenDirectX11()) {
		bUseDX9 = false; // Set to indicate intialized as DX11
		// Return here if OK - Intel auto detection removed 21.04.15
		return true;
	}

	return false;
}


bool spoutGLDXinterop::OpenDeviceKey(const char* key, int maxsize, char *description, char *version)
{
	// Extract the subkey from the DeviceKey string
	HKEY hRegKey;
	DWORD dwSize, dwKey;  
	char output[256];
	strcpy_s(output, 256, key);
	string SubKey = strstr(output, "System");

	// Convert all slash to double slash using a C++ string function
	// to get subkey string required to extract registry information
	for (unsigned int i=0; i<SubKey.length(); i++) {
		if (SubKey[i] == '\\') {
			SubKey.insert(i, 1, '\\');
			++i; // Skip inserted char
		}
	}

	// Open the key to find the adapter details
	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, SubKey.c_str(), NULL, KEY_READ, &hRegKey) == 0) { 
		dwSize = MAX_PATH;
		// Adapter name
		if(RegQueryValueExA(hRegKey, "DriverDesc", NULL, &dwKey, (BYTE*)output, &dwSize) == 0) {
			strcpy_s(description, maxsize, output);
		}
		if(RegQueryValueExA(hRegKey, "DriverVersion", NULL, &dwKey, (BYTE*)output, &dwSize) == 0) {
			// Find the last 6 characters of the version string then
			// convert to a float and multiply to get decimal in the right place
			sprintf_s(output, 256, "%5.2f", atof(output + strlen(output)-6)*100.0);
			strcpy_s(version, maxsize, output);
		} // endif DriverVersion
		RegCloseKey(hRegKey);
	} // endif RegOpenKey

	return true;
}

void spoutGLDXinterop::trim(char * s) {
    char * p = s;
    int l = (int)strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}


// this function initializes and prepares Direct3D
bool spoutGLDXinterop::OpenDirectX9(HWND hWnd)
{
	HWND fgWnd = NULL;
	char fgwndName[MAX_PATH];

	// Already initialized ?
	if(m_pD3D != NULL) {
		return true;
	}

	// Create a IDirect3D9Ex object if not already created
	if(!m_pD3D) {
		m_pD3D = spoutdx.CreateDX9object(); 
	}

	if(m_pD3D == NULL) {
		return false;
	}

	// Create a DX9 device
	if(!m_pDevice) {
		m_pDevice = spoutdx.CreateDX9device(m_pD3D, hWnd); 
	}

	if(m_pDevice == NULL) {
		return false;
	}

	// Problem for FFGL plugins - might be a problem for other FFGL hosts or applications.
	// DirectX 9 device initialization creates black areas and the host window has to be redrawn.
	// But this causes a crash for a sender in Magic when the render window size is changed.
	// Not a problem for DirectX 11.
	// Not needed in Isadora.
	// Needed for Resolume.
	// For now, limit this to Resolume only.

	fgWnd = GetForegroundWindow();
	if(fgWnd) {
		// SMTO_ABORTIFHUNG : The function returns without waiting for the time-out
		// period to elapse if the receiving thread appears to not respond or "hangs."
		if(SendMessageTimeoutA(fgWnd, WM_GETTEXT, MAX_PATH, (LPARAM)fgwndName, SMTO_ABORTIFHUNG, 128, NULL) != 0) {
			// Returns the full path - get just the window name
			PathStripPathA(fgwndName);
			if(fgwndName[0]) {
				if(strstr(fgwndName, "Resolume") != NULL // Is resolume in the window title ?
				&& strstr(fgwndName, "magic") == NULL) { // Make sure it is not a user named magic project.
					// DirectX device initialization needs the window to be redrawn (creates black areas)
					// 03.05.15 - user observation that UpDateWindow does not work and Resolume GUI is still corrupted
					// 28.08.15 - user observation of a crash with Windows 10 
					// try RedrawWindow again (with InvalidateRect as well) - confirmed working with Win 7 32bit
					// https://msdn.microsoft.com/en-us/library/windows/desktop/dd145213%28v=vs.85%29.aspx
					// The WM_PAINT message is generated by the system and should not be sent by an application.					
					// SendMessage(fgWnd, WM_PAINT, NULL, NULL ); // causes problems
					InvalidateRect(fgWnd, NULL, FALSE); // make sure
			        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASENOW | RDW_INTERNALPAINT);
				}
			}
		}
	}

	return true;
}

// this function initializes and prepares Directx 11
bool spoutGLDXinterop::OpenDirectX11()
{
	// Quit if already initialized
	if(g_pd3dDevice != NULL) return true;

	// Create a DirectX 11 device
	if(!g_pd3dDevice) g_pd3dDevice = spoutdx.CreateDX11device();
	if(g_pd3dDevice == NULL) {
		return false;
	}

	return true;
}


// this function tests for DX11 capability
bool spoutGLDXinterop::DX11available()
{
	// Quit if DX11 is already initialized
	if(g_pd3dDevice != NULL) return true;

	// Try to create a DirectX 11 device
	ID3D11Device* pd3dDevice;
	pd3dDevice = spoutdx.CreateDX11device();
	if(pd3dDevice == NULL)
		return false;

	// Close it because not initialized yet and is just a test
	pd3dDevice->Release();

	return true;
}

// Must be called after DirectX initialization
//
// https://code.google.com/p/chromium/issues/detail?id=106438
//
// NOTES : On a “normal” system EnumDisplayDevices and IDXGIAdapter::GetDesc always concur
// i.e. the device that owns the head will be the device that performs the rendering. 
// On an Optimus system IDXGIAdapter::GetDesc will return whichever device has been selected for rendering.
// So on an Optimus system it is possible that IDXGIAdapter::GetDesc will return the dGPU whereas 
// EnumDisplayDevices will return the iGPU.
//
// This function compares the adapter descriptions of the two
// The string "Intel" reveals that it is an Intel device but 
// the Vendor ID could also be used
//
//	0x10DE	NVIDIA
//	0x163C	intel
//	0x8086  Intel
//	0x8087  Intel
//
bool spoutGLDXinterop::GetAdapterInfo(char *renderadapter, 
									  char *renderdescription, char *renderversion,
									  char *displaydescription, char *displayversion,
									  int maxsize, bool &bDX9)
{
	renderadapter[0] = 0; // DirectX adapter
	renderdescription[0] = 0;
	renderversion[0] = 0;
	displaydescription[0] = 0;
	displayversion[0] = 0;

	if(bDX9) {
		if(m_pDevice == NULL) { printf("No DX9 device\n"); return false; }

		D3DADAPTER_IDENTIFIER9 adapterinfo;
		// char            Driver[MAX_DEVICE_IDENTIFIER_STRING];
		// char            Description[MAX_DEVICE_IDENTIFIER_STRING];
		// char            DeviceName[32];         // Device name for GDI (ex. \\.\DISPLAY1)
		// LARGE_INTEGER   DriverVersion;          // Defined for 32 bit components
		// DWORD           VendorId;
		// DWORD           DeviceId;
		// DWORD           SubSysId;
		// DWORD           Revision;
		// GUID            DeviceIdentifier;
		// DWORD           WHQLLevel;
		m_pD3D->GetAdapterIdentifier (D3DADAPTER_DEFAULT, 0, &adapterinfo);
		// printf("Driver = [%s]\n", adapterinfo.Driver);
		// printf("    Description = [%s]\n", adapterinfo.Description);
		// printf("DeviceName = [%s]\n", adapterinfo.DeviceName);
		// printf("DriverVersion = [%d] [%x]\n", adapterinfo.DriverVersion, adapterinfo.DriverVersion);
		// printf("VendorId = [%d] [%x]\n", adapterinfo.VendorId, adapterinfo.VendorId);
		// printf("DeviceId = [%d] [%x]\n", adapterinfo.DeviceId, adapterinfo.DeviceId);
		// printf("SubSysId = [%d] [%x]\n", adapterinfo.SubSysId, adapterinfo.SubSysId);
		// printf("Revision = [%d] [%x]\n", adapterinfo.Revision, adapterinfo.Revision);
		strcpy_s(renderadapter, maxsize, adapterinfo.Description);

	}
	else {
		if(g_pd3dDevice == NULL) { printf("No DX11 device\n"); return false; }

		IDXGIDevice * pDXGIDevice;
		g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
		IDXGIAdapter * pDXGIAdapter;
		pDXGIDevice->GetAdapter(&pDXGIAdapter);
		DXGI_ADAPTER_DESC adapterinfo;
		pDXGIAdapter->GetDesc(&adapterinfo);
		// WCHAR Description[ 128 ];
		// UINT VendorId;
		// UINT DeviceId;
		// UINT SubSysId;
		// UINT Revision;
		// SIZE_T DedicatedVideoMemory;
		// SIZE_T DedicatedSystemMemory;
		// SIZE_T SharedSystemMemory;
		// LUID AdapterLuid;

		char output[256];
		size_t charsConverted = 0;
		wcstombs_s(&charsConverted, output, 129, adapterinfo.Description, 128);
		// printf("    Description = [%s]\n", output);
		// printf("    VendorId = [%d] [%x]\n", adapterinfo.VendorId, adapterinfo.VendorId);
		// printf("SubSysId = [%d] [%x]\n", adapterinfo.SubSysId, adapterinfo.SubSysId);
		// printf("DeviceId = [%d] [%x]\n", adapterinfo.DeviceId, adapterinfo.DeviceId);
		// printf("Revision = [%d] [%x]\n", adapterinfo.Revision, adapterinfo.Revision);
		strcpy_s(renderadapter, maxsize, output);
	}

	// DEBUG - default render adapter is the DirectX one ???
	if(renderadapter) {
		strcpy_s(renderdescription, maxsize, renderadapter);
	}

	// Use Windows functions to look for Intel graphics to see  if it is
	// the same render adapter that was detected with DirectX
	char driverdescription[256];
	char driverversion[256];
	char regkey[256];
	size_t charsConverted = 0;
	
	// Additional info
	DISPLAY_DEVICE DisplayDevice;
	DisplayDevice.cb = sizeof(DISPLAY_DEVICE);

	// 31.10.14 detect the adapter attached to the desktop
	// To query all display devices in the current session, 
	// call this function in a loop, starting with iDevNum set to 0, 
	// and incrementing iDevNum until the function fails. 
	// To select all display devices in the desktop, use only the display devices
	// that have the DISPLAY_DEVICE_ATTACHED_TO_DESKTOP flag in the DISPLAY_DEVICE structure.

	int nDevices = 0;
	for(int i=0; i<10; i++) { // should be much less than 10 adapters
		if(EnumDisplayDevices(NULL, i, &DisplayDevice, 0)) {
			// This will list all the devices
			nDevices++;
			// Get the registry key
			wcstombs_s( &charsConverted, regkey, 129, (const wchar_t *)DisplayDevice.DeviceKey, 128);
			// This is the registry key with all the information about the adapter
			OpenDeviceKey(regkey, 256, driverdescription, driverversion);
			// Is it a render adapter ?
			if(renderadapter && strcmp(driverdescription, renderadapter) == 0) {
				// printf("Windows render adapter matches : [%s] Vers [%s]\n", driverdescription, driverversion);
				strcpy_s(renderdescription, maxsize, driverdescription);
				strcpy_s(renderversion, maxsize, driverversion);
			}
			// Is it a display adapter
			if(DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
				// printf("Display adapter : [%s] Vers: %s ", driverdescription, driverversion);
				strcpy_s(displaydescription, 256, driverdescription);
				strcpy_s(displayversion, 256, driverversion);
				// printf("(Attached to desktop)\n");
			} // endif attached to desktop

		} // endif EnumDisplayDevices
	} // end search loop

	// The render adapter
	if(renderdescription) trim(renderdescription);

	return true;
}


//
bool spoutGLDXinterop::CreateInterop(HWND hWnd, const char* sendername, unsigned int width, unsigned int height, DWORD dwFormat, bool bReceive)
{
	bool bRet = true;
	DWORD format;

	// Needs an openGL context to work
	if(!wglGetCurrentContext()) {
		MessageBoxA(NULL, "CreateInterop - no GL context", "ERROR", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	// printf("CreateInterop - %dx%d - dwFormat (%d) \n", width, height, dwFormat);

	// Texture format tests
	// Compatible formats
	// DXGI_FORMAT_R8G8B8A8_UNORM; // default DX11 format - not compatible with DX9 (28)
	// DXGI_FORMAT_B8G8R8A8_UNORM; // compatible DX11 format - works with DX9 (87)
	// DXGI_FORMAT_B8G8R8X8_UNORM; // compatible DX11 format - works with DX9 (88)
	// Allow for compatible DirectX 11 senders (format 87)
	// And compatible DirectX9 senders D3DFMT_X8R8G8B8 - 22
	// and the default D3DFMT_A8R8G8B8 - 21
	if(bUseDX9) {
		// printf("CreateInterop - DX9 mode\n"); 
		// DirectX 9
		if(dwFormat > 0) {
			if(dwFormat == 87) {
				// printf("CreateInterop - DX9 mode - compatible DX11 user format (%d), creating DX9 D3DFMT_A8R8G8B8 texture\n", dwFormat);
				format = (DWORD)D3DFMT_A8R8G8B8; // (21)
			}
			else if(dwFormat == D3DFMT_X8R8G8B8 || dwFormat == D3DFMT_A8R8G8B8) {
				// printf("CreateInterop - DX9 mode - compatible DX9 user format (%d) \n", dwFormat); 
				format = (DWORD)dwFormat; // (22)
			}
			else {
				// printf("CreateInterop - DX9 mode - incompatible user format (%d) \n", dwFormat);
				return false;
			}
		}
		else { // format is passed as zero so we assume a DX9 sender D3DFMT_A8R8G8B8
			format = (DWORD)DX9format;
			// printf("CreateInterop - DX9 mode - DX9format (%d) \n", format);
		}
	}
	else {
		// printf("CreateInterop - DX11 mode\n"); 
		// DirectX 11
		// Is this a DX11 or a DX9 sender texture?
		// A directX 11 receiver accepts DX9 formats
		if(!bReceive && dwFormat > 0) {
			// printf("CreateInterop - DX11 sender - user format %d \n", dwFormat);
			format = (DXGI_FORMAT)dwFormat;
		}
		else {
			// printf("CreateInterop - default DX11 format BGRA - format passed = %d \n", dwFormat);
			format = (DWORD)DX11format; // DXGI_FORMAT_B8G8R8A8_UNORM (87) default compatible with DX9
		}
	}

	// Quit now if the receiver can't access the shared memory info of the sender
	// Otherwise m_dxShareHandle is set by getSharedTextureInfo and is the
	// shared texture handle of the Sender texture
	if (bReceive && !getSharedTextureInfo(sendername)) {
		MessageBoxA(NULL,"Cannot retrieve sender information.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		printf("CreateInterop error 1\n");
		return false;
	}

	// printf("Sender texture format = %d, sharehanlde = %x \n", m_TextureInfo.format, m_TextureInfo.shareHandle);

	// Check the sender format for a DX9 receiver
	// It can only be from a DX9 sender (format 0, 22, 21)
	// or from a compatible DX11 sender (format 87)
	if(bReceive && bUseDX9) {
		if(!(m_TextureInfo.format == 0 
			|| m_TextureInfo.format == 22
			|| m_TextureInfo.format == 21
			|| m_TextureInfo.format == 87)) {
			// printf("Incompatible sender texture format %d \n", m_TextureInfo.format);
			MessageBoxA(NULL,"Incompatible sender texture format.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return false;
		}
	}

	// printf("CreateInterop 1\n");

	// Make sure DirectX has been initialized
	// Creates a global pointer to the DirectX device (DX11 g_pd3dDevice or DX9 m_pDevice)
	if(!OpenDirectX(hWnd, bUseDX9)) {
		MessageBoxA(NULL,"Cannot open DirectX.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		printf("CreateInterop error 2\n");
		return false;
	}

	// Allow for sender updates
	// When a sender size changes, the new texture has to be re-registered
	if(m_hInteropDevice != NULL &&  m_hInteropObject != NULL) {
		// printf("CreateInterop - wglDXUnregisterObjectNV\n");
		wglDXUnregisterObjectNV(m_hInteropDevice, m_hInteropObject);
		m_hInteropObject = NULL;
	}

	// Create an fbo for copying textures
	if(m_fbo) {
		// Delete the fbo before the texture so that any texture attachment is released
		glDeleteFramebuffersEXT(1, &m_fbo);
		m_fbo = 0;
	}
	if(m_fbo == 0)
		glGenFramebuffersEXT(1, &m_fbo); 

	// Create a local opengl texture that will be linked to a shared DirectX texture
	if(m_glTexture) {
		glDeleteTextures(1, &m_glTexture);
		m_glTexture = 0;
	}
	if(m_glTexture == 0)
		glGenTextures(1, &m_glTexture);

	// printf("CreateInterop 5 (format = %d)\n", format);

	// Create textures and GLDX interop objects
	if(bUseDX9)	bRet = CreateDX9interop(width, height, format, bReceive);
	else bRet = CreateDX11interop(width, height, format, bReceive);

	if(!bRet) {
		printf("CreateInterop error 3\n");
		MessageBoxA(NULL,"Cannot create DirectX/OpenGL interop","ERROR",MB_OK|MB_ICONEXCLAMATION);
		CleanupInterop(); // 20.11.15 - release everything
		return false;
	}

	// Now the global shared texture handle - m_dxShareHandle - has been set so a sender can be created
	// this creates the sender shared memory map and registers the sender
	if (!bReceive) {
		// We are done with the format
		// So for DirectX 9, set to zero to identify the sender as DirectX 9
		// DEBUG - allow format to be registered becasue it is now tested
		// by revised SpoutPanel 2 and by the texture formats above
		// if(bUseDX9) format = 0; 
		if(!senders.CreateSender(sendername, width, height, m_dxShareHandle, format)) {
			printf("CreateInterop error 4\n");
			MessageBoxA(NULL,"Cannot create Spout sender.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			// TODO - release
			return false;
		}
	}

	// printf("CreateInterop 6\n");

	// Set up local values for this instance
	// Needed for texture read and write size checks
	m_TextureInfo.width			= (unsigned __int32)width;
	m_TextureInfo.height		= (unsigned __int32)height;
	m_TextureInfo.shareHandle	= (unsigned __int32)m_dxShareHandle;
	m_TextureInfo.format		= format;

	// Additional unused fields available
	// DWORD usage; // texture usage
	// wchar_t description[128]; // Wyhon compatible description
	// unsigned __int32 partnerId; // Wyphon id of partner that shared it with us (not unused)

	// 26.08.15 - set the executable path to the sender's shared info
	if(!bReceive) {
		SharedTextureInfo info;
		// Access the info directly from the memory map to include the description string
		if(senders.getSharedInfo(sendername, &info)) {
			char exepath[256];
			GetModuleFileNameA(NULL, exepath, sizeof(exepath));
			// Description is defined as wide chars, but the path is stored as byte chars
			strcpy_s((char *)info.description, 256, exepath);
			senders.setSharedInfo(sendername, &info);
		}
	}

	// Initialize general texture transfer sync mutex - either sender or receiver can do this
	spoutdx.CreateAccessMutex(sendername, m_hAccessMutex);
	// LJ DEBUG
	spoutdx.CreateReceiverAccessMutex(sendername, m_hReceiverAccessMutex);

	// Now it has initialized OK
	m_bInitialized = true;

	//
	// Now we have globals for this instance
	//
	// m_TextureInfo.width			- width of the texture
	// m_TextureInfo.height			- height of the texture
	// m_TextureInfo.shareHandle	- handle of the shared texture
	// m_TextureInfo.format			- format of the texture
	// m_TextureInfo.description    - path of the executable that created the sender

	// m_glTexture					- a linked opengl texture
	// m_dxTexture					- a linked, shared DirectX texture created here
	// m_hInteropDevice				- handle to interop device created by wglDXOpenDeviceNV by init
	// m_hInteropObject				- handle to the connected texture created by wglDXRegisterObjectNV
	// m_hAccessMutex				- mutex for texture access lock
	// m_bInitialized				- whether it initialized OK

	// true means the init was OK, not the connection
	return true; 

}


//
// =================== DX9 ===============================
//
//		CreateDX9interop()
//	
//		bReceive		when receiving a texture from a DX application this must be set to true (default)
//						when sending a texture from GL to the DX application, set to false
//
bool spoutGLDXinterop::CreateDX9interop(unsigned int width, unsigned int height, DWORD dwFormat, bool bReceive) 
{

	// printf("CreateDX9interop(%dx%d, [Format = %d], %d (m_pDevice = %x)\n", width, height, dwFormat, bReceive, m_pDevice);

	// The shared texture handle of the Sender texture "m_dxShareHandle" 
	// is already set by getSharedTextureInfo, but should be NULL for a sender
	if (!bReceive) {
		// printf("    sender - setting m_dxShareHandle to NULL\n");
		// Create a new shared DirectX resource m_dxTexture 
		// with new local handle m_dxShareHandle for a sender
		m_dxShareHandle = NULL; // A sender creates a new texture
	}
	
	// Safety in case an application has crashed
	if (m_dxTexture != NULL) {
		// printf("   releasing texture\n");
		m_dxTexture->Release();
	}
	m_dxTexture = NULL;

	// Create a shared DirectX9 texture - m_dxTexture
	// by giving it a sharehandle variable - m_dxShareHandle
	// For a SENDER : the sharehandle is NULL and a new texture is created
	// For a RECEIVER : the sharehandle is valid and becomes a handle to the existing shared texture
	// USAGE is D3DUSAGE_RENDERTARGET
	if(!spoutdx.CreateSharedDX9Texture(m_pDevice,
									   width,
									   height,
									   (D3DFORMAT)dwFormat,  // default is D3DFMT_A8R8G8B8
									   m_dxTexture,
									   m_dxShareHandle)) {
		printf("    CreateSharedDX9Texture failed\n");								   
		return false;
	}

	// printf("CreateDX9interop : m_dxTexture = [%x]\n");



	// Link the shared DirectX texture to the OpenGL texture
	// This registers for interop and associates the opengl texture with the dx texture
	// by calling wglDXRegisterObjectNV which returns a handle to the interop object
	// (the shared texture) (m_hInteropObject)
	m_hInteropObject = LinkGLDXtextures(m_pDevice, m_dxTexture, m_dxShareHandle, m_glTexture); 
	if(!m_hInteropObject) {
		printf("    DX9 LinkGLDXtextures failed\n");	
		return false;
	}


	return true;
}



//
// =================== DX11 ==============================
//
bool spoutGLDXinterop::CreateDX11interop(unsigned int width, unsigned int height, DWORD dwFormat, bool bReceive ) 
{
	
	// printf("CreateDX11interop(%dx%d, [Format = %d], %d\n", width, height, dwFormat, bReceive);

	// Safety in case of application crash
	if(g_pd3dDevice != NULL) g_pd3dDevice->Release();
	g_pSharedTexture = NULL; // Important because mutex locks check for NULL

	// Create or use a shared DirectX texture that will be linked to the OpenGL texture
	// and get it's share handle for sharing textures
	if (bReceive) {
		// Retrieve the shared texture pointer via the sharehandle
		// printf("CreateDX11interop %x, %x\n", g_pd3dDevice, m_dxShareHandle);
		if(!spoutdx.OpenDX11shareHandle(g_pd3dDevice, &g_pSharedTexture, m_dxShareHandle)) {
			printf("    CreateDX11interop - error 1\n");
			return false;
		}
	} else {
		// printf("CreateDX11interop - creating texture %dx%d (g_pSharedTexture = %x)\n", width, height, g_pSharedTexture);
		// otherwise create a new shared DirectX resource g_pSharedTexture 
		// with local handle m_dxShareHandle for a sender
		m_dxShareHandle = NULL; // A sender creates a new texture with a new share handle
		if(!spoutdx.CreateSharedDX11Texture(g_pd3dDevice,
											width, height, 
											(DXGI_FORMAT)dwFormat, // default is DXGI_FORMAT_B8G8R8A8_UNORM
											&g_pSharedTexture, m_dxShareHandle)) {
			printf("    CreateDX11interop - error 2\n");
			return false;
		}
	}

	// Link the shared DirectX texture to the OpenGL texture
	// This registers for interop and associates the opengl texture with the dx texture
	// by calling wglDXRegisterObjectNV which returns a handle to the interop object
	// (the shared texture) (m_hInteropObject)
	// printf("CreateDX11interop 2\n");
	m_hInteropObject = LinkGLDXtextures(g_pd3dDevice, g_pSharedTexture, m_dxShareHandle, m_glTexture); 
	if(!m_hInteropObject) {
		printf("    DX11 LinkGLDXtextures failed\n");	
		return false;
	}

	return true;

}

//	Link a shared DirectX texture to an OpenGL texture
//	and create a GLDX interop object handle
//
//	IN	pSharedTexture  Pointer to shared the DirectX texture
//	IN	dxShareHandle   Handle of the DirectX texture to be shared
//	IN	glTextureID     ID of the OpenGL texture that is to be linked to the shared DirectX texture
//	Returns             Handle to the GL/DirectX interop object (the shared texture)
//
HANDLE spoutGLDXinterop::LinkGLDXtextures (	void* pDXdevice,
											void* pSharedTexture,
											HANDLE dxShareHandle,
											GLuint glTexture) 
{

	HANDLE hInteropObject;

	// printf("LinkGLDXtextures (%x, %x, %x, %x)\n", pDXdevice, pSharedTexture, dxShareHandle, glTexture);
	// printf("    m_hInteropDevice = %x\n", m_hInteropDevice);

	// Prepare the DirectX device for interoperability with OpenGL
	// The return value is a handle to a GL/DirectX interop device.
	if(!m_hInteropDevice) {
		// printf("    LinkGLDXtextures creating interop device from %x\n", pDXdevice);
		m_hInteropDevice = wglDXOpenDeviceNV(pDXdevice);
	}

	if (m_hInteropDevice == NULL) {
		printf("    LinkGLDXtextures error 1 : could not create interop device from %x\n", pDXdevice);
		return NULL;
	}

	// prepare shared resource
	// wglDXSetResourceShareHandle does not need to be called for DirectX
	// version 10 and 11 resources. Calling this function for DirectX 10
	// and 11 resources is not an error but has no effect.
	if (!wglDXSetResourceShareHandleNV(pSharedTexture, dxShareHandle)) {
		printf("    LinkGLDXtextures error 2 : wglDXSetResourceShareHandleNV failed\n");
		// TODO - release object and device
		return NULL;
	}

	// Prepare the DirectX texture for use by OpenGL
	// register for interop and associate the opengl texture with the dx texture
	hInteropObject = wglDXRegisterObjectNV( m_hInteropDevice,
											pSharedTexture,	// DX texture
											glTexture,		// OpenGL texture
											GL_TEXTURE_2D,	// Must be TEXTURE_2D - multisampling not supported
											WGL_ACCESS_READ_WRITE_NV); // We will write and the receiver will read

	if(!hInteropObject) {
		printf("    LinkGLDXtextures error 3 : wglDXRegisterObjectNV failed\n");
		// printf("LinkGLDXtextures (%x, %x, %x, %x)\n", pDXdevice, pSharedTexture, dxShareHandle, glTexture);
		// printf("    m_hInteropDevice = %x\n", m_hInteropDevice);	
	}
	else {
		// printf("    wglDXRegisterObjectNV OK\n");
	}

	return hInteropObject;

}


void spoutGLDXinterop::CleanupDirectX()
{
	if(bUseDX9)
		CleanupDX9();
	else
		CleanupDX11();
}


void spoutGLDXinterop::CleanupDX9()
{
	
	// 01.09.14 - texture release was missing for a receiver - caused a VRAM leak
	// If an existing texture exists, CreateTexture can fail with and "unknown error"
	// so delete any existing texture object
	// 25.08.15 - moved before release of device
	if (m_dxTexture != NULL) {
		// DEBUG
		// Should not be needed because we are not rendering with the texture
		// https://msdn.microsoft.com/en-us/library/windows/desktop/bb206253%28v=vs.85%29.aspx
		// m_pDevice->SetTexture(0, NULL);
		m_dxTexture->Release();
	}
	m_dxTexture = NULL;

	// 25.08.15 - release device before the object !
	if(m_pDevice != NULL) {
		// printf("Releasing dx device\n");
		m_pDevice->Release();
	}

	if(m_pD3D != NULL) {
		// printf("Releasing dx object\n");
		m_pD3D->Release();
	}

	m_pDevice = NULL;
	m_pD3D = NULL;

}

void spoutGLDXinterop::CleanupDX11()
{
	if(g_pSharedTexture != NULL) g_pSharedTexture->Release();
	// 21.10.14 - removed due to problems
	// spoutdx.CloseDX11(); // release immediate context before releasing the device
	if(g_pd3dDevice != NULL) g_pd3dDevice->Release();
	g_pSharedTexture = NULL; // Important because mutex locks check for NULL
	g_pd3dDevice = NULL;

}


// this is the function that cleans up Direct3D and the gldx interop
// The exit flag is a fix - trouble is with wglDXUnregisterObjectNV
// which crashes on exit to the program but not if called
// while the program is running. Likely due to no GL context on exit
void spoutGLDXinterop::CleanupInterop(bool bExit)
{
	HGLRC ctx = wglGetCurrentContext();
	
	// printf("CleanupInterop\n");

	// DEBUG
	// printf("CleanupInterop(%d) - ctx = %x\n", bExit, ctx);
	// int nCurAvailMemoryInKB = 0;
	// glGetIntegerv(0x9049, &nCurAvailMemoryInKB);
	// printf("GPU memory 1 : [%i]\n", nCurAvailMemoryInKB);
	

	// Some of these things need an opengl context so check
	if(ctx != NULL) {
		// Problem here on exit, but not on change of resolution while the program is running !?
		//
		// Problem noted with DirectX 9 if the same shared texture is re-initialized
		// Not a problem with DirectX 11 - this is a workaround just to avoid the issue
		// As yet unidentified
		if(!bUseDX9) {
			if(!bExit && m_hInteropDevice != NULL && m_hInteropObject != NULL) {
				// printf("    wglDXUnregisterObjectNV\n");
				wglDXUnregisterObjectNV(m_hInteropDevice, m_hInteropObject);
				m_hInteropObject = NULL;
			}
		}
		m_hInteropObject = NULL;

		if (m_hInteropDevice != NULL) {
			// printf("    wglDXCloseDeviceNV\n");
			wglDXCloseDeviceNV(m_hInteropDevice);
			m_hInteropDevice = NULL;
		}
		m_hInteropDevice = NULL;

		if(m_fbo) {
			// Delete the fbo before the texture so that any texture attachment 
			// is released even though it should have been
			// printf("    glDeleteFramebuffersEXT\n");
			glDeleteFramebuffersEXT(1, &m_fbo);
			m_fbo = 0;
		}
		if(m_glTexture)	{
			// printf("    glDeleteTextures\n");
			glDeleteTextures(1, &m_glTexture);
			m_glTexture = 0;
		}
		if(m_TexID)	{
			// printf("    glDeleteTextures\n");
			glDeleteTextures(1, &m_TexID);
			m_TexID = 0;
			m_TexWidth = 0;
			m_TexHeight = 0;
		}
	} // endif there is an opengl context
	/*
	else {
		printf("CleanupInterop(%d) - no context\n", bExit);
		printf("    m_hInteropObject = (%d)\n", m_hInteropObject);
		printf("    m_hInteropDevice = (%d)\n", m_hInteropDevice);
		printf("    m_fbo = (%d)\n", m_fbo);
		printf("    m_glTexture = (%d)\n", m_glTexture);
	}
	*/

	/*
	glGetIntegerv(0x9049, &nCurAvailMemoryInKB);
	// printf("GPU memory 2 : [%i]\n", nCurAvailMemoryInKB);
	CleanupDirectX();
	glGetIntegerv(0x9049, &nCurAvailMemoryInKB);
	// printf("GPU memory 3 : [%i]\n", nCurAvailMemoryInKB);
	*/

	// Close general texture access mutex
	spoutdx.CloseAccessMutex(m_hAccessMutex);
	m_hAccessMutex = NULL; // Double check that the global handle is NULL

	// LJ DEBUG
	// Close receiver texture access mutex
	spoutdx.CloseReceiverAccessMutex(m_hAccessMutex);
	m_hReceiverAccessMutex = NULL;

	m_bInitialized = false;

}

//
//	Load the Nvidia gl/dx extensions
//
bool spoutGLDXinterop::LoadGLextensions() 
{
	unsigned int caps = 0;

	caps = loadGLextensions();

	char buffer [33];
	_itoa_s(caps, buffer, 2);

	if(caps == 0) {
		return false;
	}

	if(caps & GLEXT_SUPPORT_FBO) bFBOavailable = true;
	if(caps & GLEXT_SUPPORT_FBO_BLIT) bBLITavailable = true;
	if(caps & GLEXT_SUPPORT_PBO) bPBOavailable = true;
	if(caps & GLEXT_SUPPORT_SWAP) bSWAPavailable = true;

	return true;
}

// 03.09.14 - MB mods for names map class
bool spoutGLDXinterop::getSharedTextureInfo(const char* sharedMemoryName) {

	unsigned int w, h;
	HANDLE handle;
	DWORD format;
	char name[256];
	strcpy_s(name, 256, sharedMemoryName);

	if (!senders.FindSender(name, w, h, handle, format)) {
		return false;
	}

	m_dxShareHandle = (HANDLE)handle;
	m_TextureInfo.width = w;
	m_TextureInfo.height = h;
	m_TextureInfo.shareHandle = (__int32)handle;
	m_TextureInfo.format = format;

	return true;

}



// Set texture info to shared memory for the sender init
// width and height must have been set first
// 03.09.14 - MB mods for names map class
bool spoutGLDXinterop::setSharedTextureInfo(const char* sharedMemoryName) {

	return senders.UpdateSender(sharedMemoryName, 
							m_TextureInfo.width,
							m_TextureInfo.height,
							m_dxShareHandle,
							m_TextureInfo.format);


}

// Return current sharing handle, width and height of a Sender
// Note - use the map directly - we must not use getSharedTextureInfo
// which resets the local info structure from shared memory !!!
// A receiver checks this all the time so it has to be compact
// 03.09.14 - MB mods for names map class
bool spoutGLDXinterop::getSharedInfo(char* sharedMemoryName, SharedTextureInfo* info) 
{
	return senders.getSharedInfo(sharedMemoryName, info);
} // end getSharedInfo



// Sets the given info structure to shared memory with the given name
// IMPORTANT: this modifies the local structure
// Used to change the texture dimensions before init
bool spoutGLDXinterop::setSharedInfo(char* sharedMemoryName, SharedTextureInfo* info)
{
	m_TextureInfo.width			= info->width;
	m_TextureInfo.height		= info->height;
	m_dxShareHandle				= (HANDLE)info->shareHandle; 
	// the local info structure handle "m_TextureInfo.shareHandle" gets converted 
	// into (unsigned __int32) from "m_dxShareHandle" by setSharedTextureInfo
	if(setSharedTextureInfo(sharedMemoryName)) {
		return true;
	}
	else {
		return false;
	}
}

// Utilities
//
// GLDXcompatible
//
bool spoutGLDXinterop::GLDXcompatible()
{
	//
	// ======= Hardware compatibility test =======
	//
	// Call LoadGLextensions for an initial driver compatibilty check
	// for the Nvidia OpenGL/Directx interop extensions.
	// This will fail straight away for non Nvidia hardware but it needs
	// an additional check. 
	// It is possible that the extensions load OK but that initialization will still fail.
	// This occurs when wglDXOpenDeviceNV fails. 
	// This has been noted on dual graphics machines with the NVIDIA Optimus driver.
	// "GLDXcompatible" tests for this by initializing directx and calling wglDXOpenDeviceNV
	// If OK and the debug flag has not been set, all the parameters are available
	// Otherwise it is limited to memory share
	//
	HDC hdc = wglGetCurrentDC(); // OpenGl device context is needed
	if(!hdc) {
		MessageBoxA(NULL, "Spout compatibility test\nCannot get GL device context", "GLDXcompatible", MB_OK);
		return false;
	}
	HWND hWnd = WindowFromDC(hdc); // can be null though
	if(!bExtensionsLoaded) bExtensionsLoaded = LoadGLextensions();
	if(bExtensionsLoaded) {
		/*
		// DEBUG
		// Check that everything is supported
		if(!bFBOavailable) // printf("FBO error\n");
		if(!bBLITavailable) // printf("BLIT error\n");
		// if(!bPBOavailable) // printf("FBO errror\n");
		if(!bSWAPavailable) // printf("SWAP error\n");
		*/
		// all OK so try to set up directx and open the GL/DX interop
		if(OpenDirectX(hWnd, bUseDX9)) {
			// if it passes here all is well
			return true;
		}
		else {
			printf("GLDXcompatible() : OpenDirectX failed\n");
		}
	} // end compatibility test
	else {
		printf("GLDXcompatible() : Failed to load OpenGL extensions\n");
	}

	// Failed to find extensions or Open DirectX, return false to use memoryshare mode


	return false;

} // end GLDXcompatible



// ----------------------------------------------------------
//		Access to texture using DX/GL interop functions
// ----------------------------------------------------------

// DRAW A TEXTURE INTO THE THE SHARED TEXTURE VIA AN FBO
bool spoutGLDXinterop::DrawToSharedTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, float max_x, float max_y, float aspect, bool bInvert, GLuint HostFBO)
{
	GLenum status;

	if(m_hInteropDevice == NULL || m_hInteropObject == NULL) return false;
	if(width != (unsigned  int)m_TextureInfo.width || height != (unsigned  int)m_TextureInfo.height) return false;

	// printf("Draw To Shared Texture - invert = %d\n", bInvert);

	// Wait for access to the texture
	if(spoutdx.CheckAccess(m_hAccessMutex)) {

		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {

			// Draw the input texture into the shared texture via an fbo

			// Bind our fbo
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
			glFramebufferTexture2DEXT(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_glTexture, 0);
			
			status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {

				glBindTexture(GL_TEXTURE_2D, m_glTexture);

				glColor4f(1.f, 1.f, 1.f, 1.f);
				glEnable(TextureTarget);
				glBindTexture(TextureTarget, TextureID);

				GLfloat tc[4][2] = {0};

				// Invert texture coord to user requirements
				if(bInvert) {
					tc[0][0] = 0.0;   tc[0][1] = max_y;
					tc[1][0] = 0.0;   tc[1][1] = 0.0;
					tc[2][0] = max_x; tc[2][1] = 0.0;
					tc[3][0] = max_x; tc[3][1] = max_y;
				}
				else {
					tc[0][0] = 0.0;   tc[0][1] = 0.0;
					tc[1][0] = 0.0;   tc[1][1] = max_y;
					tc[2][0] = max_x; tc[2][1] = max_y;
					tc[3][0] = max_x; tc[3][1] = 0.0;
				}

				GLfloat verts[] =  {
								-aspect, -1.0,   // bottom left
								-aspect,  1.0,   // top left
								 aspect,  1.0,   // top right
								 aspect, -1.0 }; // bottom right

				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glTexCoordPointer(2, GL_FLOAT, 0, tc );
				glEnableClientState(GL_VERTEX_ARRAY);		
				glVertexPointer(2, GL_FLOAT, 0, verts );
				glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				glBindTexture(TextureTarget, 0);
				glDisable(TextureTarget);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else {
				PrintFBOstatus(status);
			}

			// restore the previous fbo - default is 0
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);

			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		}
	}
	spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture

	return true;
}

 
// DRAW THE SHARED TEXTURE
bool spoutGLDXinterop::DrawSharedTexture(float max_x, float max_y, float aspect, bool bInvert)
{

	if(m_hInteropDevice == NULL || m_hInteropObject == NULL)
		return false;

	// printf("spoutGLDXinterop::DrawSharedTexture - bInvert = %d\n", bInvert);

	// Wait for access to the texture
	if(spoutdx.CheckAccess(m_hAccessMutex)) {

		// go ahead and access the shared texture to draw it
		// lock dx object    
		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_glTexture); // bind shared texture
			glColor4f(1.f, 1.f, 1.f, 1.f);

				/*
				GLfloat tc[4][2] = {0};

				// Invert texture coord to user requirements
				if(bInvert) {
					tc[0][0] = 0.0;   tc[0][1] = max_y;
					tc[1][0] = 0.0;   tc[1][1] = 0.0;
					tc[2][0] = max_x; tc[2][1] = 0.0;
					tc[3][0] = max_x; tc[3][1] = max_y;
				}
				else {
					tc[0][0] = 0.0;   tc[0][1] = 0.0;
					tc[1][0] = 0.0;   tc[1][1] = max_y;
					tc[2][0] = max_x; tc[2][1] = max_y;
					tc[3][0] = max_x; tc[3][1] = 0.0;
				}

				GLfloat verts[] =  {
								-aspect, -1.0,   // bottom left
								-aspect,  1.0,   // top left
								 aspect,  1.0,   // top right
								 aspect, -1.0 }; // bottom right

				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
				glTexCoordPointer(2, GL_FLOAT, 0, tc );
				glEnableClientState(GL_VERTEX_ARRAY);		
				glVertexPointer(2, GL_FLOAT, 0, verts );
				glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				*/

			// TODO - convert to vertex array - Processing crash
			glBegin(GL_QUADS);
			if(bInvert) {
				glTexCoord2f(0.0,	max_y);	glVertex2f(-aspect,-1.0); // lower left
				glTexCoord2f(0.0,	0.0);	glVertex2f(-aspect, 1.0); // upper left
				glTexCoord2f(max_x, 0.0);	glVertex2f( aspect, 1.0); // upper right
				glTexCoord2f(max_x, max_y);	glVertex2f( aspect,-1.0); // lower right
			}
			else {
				glTexCoord2f(0.0,   0.0);	glVertex2f(-aspect,-1.0); // lower left
				glTexCoord2f(0.0,   max_y);	glVertex2f(-aspect, 1.0); // upper left
				glTexCoord2f(max_x, max_y);	glVertex2f( aspect, 1.0); // upper right
				glTexCoord2f(max_x, 0.0);	glVertex2f( aspect,-1.0); // lower right
			}
			glEnd();


			glBindTexture(GL_TEXTURE_2D, 0); // unbind shared texture
			glDisable(GL_TEXTURE_2D);

			// unlock dx object
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
			
			// drop through to manage events and return true;
		} // if lock failed just keep going
	}
	spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture

	return true;

} // end DrawSharedTexture



#ifdef USE_PBO_EXTENSIONS
// TODO - this is not used - to be texted
// Load a texture into pixels via PBO - NOTE RGB only
bool spoutGLDXinterop::LoadTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, unsigned char *data)
{
	GLuint pixelBufferID;

	// PBO method - load into PBO - then texture
	// Note that PBO functions have been loaded explicitly
	// Presumably lets the GPU continue after the data has been transferred to the PBO
	// but it has an additional memcpy. 

	// Really needs two PBOs
	
	// "CopyMemory" seems a little faster than memcpy although no detailed timing tests done
	// Seems to have a small advantage, perhaps 3 or 4 fps at 1920x1080. 

	// The most time consuming part is loading from shared memory. 
	// There is a small speed advantage by putting the memoryshare read
	// inside here instead of via a local buffer (perhaps 1-2 fps) but then the changes in 
	// Sender image size are not made.
	if(bPBOavailable) {

		void *pboMemory;
		glGenBuffers(1, &pixelBufferID);

		// Bind buffer
		glBindBuffer (GL_PIXEL_UNPACK_BUFFER, pixelBufferID);

		// Null existing data
		glBufferData(GL_PIXEL_UNPACK_BUFFER, width*height*3, NULL, GL_STREAM_DRAW);

		// Map buffer - returns pointer to buffer
		pboMemory = glMapBuffer (GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);

		// Write data into pboMemory - presumably this is the time consuming bit
		// might be a very slight speed increase by using CopyMemory instead of memcpy
		CopyMemory(pboMemory, (void *)data, width*height*3);

		// Unmap buffer, indicating we are done writing data to it
		glUnmapBuffer (GL_PIXEL_UNPACK_BUFFER);

		// while PBO is still bound, transfer the data from the PBO to the texture
		// glEnable(TextureTarget);
		glBindTexture (TextureTarget, TextureID);
		glTexSubImage2D(TextureTarget, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (void *)0);
		// glDisable(TextureTarget);

		// Unbind buffer
		glBindBuffer (GL_PIXEL_UNPACK_BUFFER, 0);

		if(pixelBufferID) glDeleteBuffers(1, &TextureID);

	}
	else {

		//
		// Texture method using glTexSubImage2D may be subject to "GPU pipeline stalling"
		//
		// http://blogs.msdn.com/b/shawnhar/archive/2008/04/14/stalling-the-pipeline.aspx
		//
		// glEnable(TextureTarget);
		glBindTexture (TextureTarget, TextureID);
		glTexSubImage2D(TextureTarget, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (void *)data);
		glBindTexture (TextureTarget, 0);
		// glDisable(TextureTarget);

	}

	return true;

}
#endif



// COPY IMAGE PIXELS TO THE SHARED TEXTURE  - RGBA default format
bool spoutGLDXinterop::WriteTexturePixels(unsigned char *pixels, unsigned int width, unsigned int height, GLenum glFormat, bool bAlignment)
{

	if(m_hInteropDevice == NULL || m_hInteropObject == NULL)
		return false;

	if(width != (unsigned int)m_TextureInfo.width || height != (unsigned int)m_TextureInfo.height)
		return false;

	// Wait for access to the texture
	if(spoutdx.CheckAccess(m_hAccessMutex)) {

		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			// printf("pixels[%x], %dx%d, format[%x], %d\n", pixels, width, height, glFormat, bAlignment);
			//
			// Check for alignment different from the default
			// We assume that alignment must be 1 byte rather than the default of 4
			// GL_UNPACK_ROW_ALIGNMENT affects how pixel data is read from client memory
			//
			if(!bAlignment) glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Force 1-byte alignment

			glBindTexture(GL_TEXTURE_2D, m_glTexture); // The  shared GL texture
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, glFormat, GL_UNSIGNED_BYTE, pixels);
			glBindTexture(GL_TEXTURE_2D, 0);

			if(!bAlignment) glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Restore 4-byte alignment

			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		} // if lock failed just keep going
	}

	spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture

	return true;

} // end WriteTexturePixels


//
// COPY AN OPENGL TEXTURE TO THE SHARED OPENGL TEXTURE
// 
bool spoutGLDXinterop::WriteTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO)
{
	GLenum status;

	if(m_hInteropDevice == NULL || m_hInteropObject == NULL) return false;
	if(width != m_TextureInfo.width || height != m_TextureInfo.height) return false;

	/*
	// Basic code for debugging 0.85 - 0.90 msec
	wglDXLockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
	glFramebufferTexture2DEXT(READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, TextureTarget, TextureID, 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	wglDXUnlockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
	return true;
	*/

	// printf("WriteTexture - bInvert = %d\n", bInvert);
	// Wait for access to the texture
	if(spoutdx.CheckAccess(m_hAccessMutex)) {

		// lock dx object
		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			// fbo is a  local FBO and width/height are the dimensions of the texture.
			// "TextureID" is the source texture, and "m_glTexture" is destination texture
			// which should have been already created

			// bind the FBO (for both, READ_FRAMEBUFFER_EXT and DRAW_FRAMEBUFFER_EXT)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

			// Attach the Input texture to the color buffer in our frame buffer - note texturetarget 
			glFramebufferTexture2DEXT(READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, TextureTarget, TextureID, 0);
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

			// Attach target texture (the shared texture we write into) to second attachment point
			if(bInvert) {
				glFramebufferTexture2DEXT(DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, m_glTexture, 0);
				glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
			}

			status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {

				// Default invert flag is false so do the flip to get it the right way up if the user wants that
				if(bInvert) {
					// Blit method with checks - 0.75 - 0.85 msec
					// copy one texture buffer to the other while flipping upside down (OpenGL and DirectX have different texture origins)
					glBlitFramebufferEXT(0, 0,			// srcX0, srcY0, 
										 width, height, // srcX1, srcY1
										 0, height,		// dstX0, dstY0,
										 width, 0,		// dstX1, dstY1,
										 GL_COLOR_BUFFER_BIT, GL_NEAREST); // GLbitfield mask, GLenum filter
				}
				else { 
					// no invert flag means leave it alone
					glBindTexture(GL_TEXTURE_2D, m_glTexture);
					glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			else {
				PrintFBOstatus(status);
			}

			// restore the previous fbo - default is 0
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);

			// unbind the shared texture
			glBindTexture(GL_TEXTURE_2D, 0);

			// unlock dx object
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		}
		spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture
		return true;

	}
	else {
		// printf("WriteTexture - no access\n");
	}
	// There is no reader
	spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture


	/*
	// LJ DEBUG - version with both sender and receiver access mutex
	// Wait for the receiver to release access to the texture
	if(spoutdx.CheckReceiverAccess(m_hReceiverAccessMutex)) {

		// lock dx object
		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			// fbo is a  local FBO and width/height are the dimensions of the texture.
			// "TextureID" is the source texture, and "m_glTexture" is destination texture
			// which should have been already created

			// bind the FBO (for both, READ_FRAMEBUFFER_EXT and DRAW_FRAMEBUFFER_EXT)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

			// Attach the Input texture to the color buffer in our frame buffer - note texturetarget 
			glFramebufferTexture2DEXT(READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, TextureTarget, TextureID, 0);
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

			// Attach target texture (the shared texture we write into) to second attachment point
			if(bInvert) {
				glFramebufferTexture2DEXT(DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, m_glTexture, 0);
				glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
			}

			status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {
				// Default invert flag is false so do the flip to get it the right way up if the user wants that
				if(bInvert) {
					// Blit method with checks - 0.75 - 0.85 msec
					// copy one texture buffer to the other while flipping upside down (OpenGL and DirectX have different texture origins)
					glBlitFramebufferEXT(0, 0,			// srcX0, srcY0, 
										 width, height, // srcX1, srcY1
										 0, height,		// dstX0, dstY0,
										 width, 0,		// dstX1, dstY1,
										 GL_COLOR_BUFFER_BIT, GL_NEAREST); // GLbitfield mask, GLenum filter
				}	
				else { 
					// no invert flag means leave it alone
					glBindTexture(GL_TEXTURE_2D, m_glTexture);
					glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			else {
				PrintFBOstatus(status);
			}
			// restore the previous fbo - default is 0
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);
			// unbind the shared texture
			glBindTexture(GL_TEXTURE_2D, 0);
			// unlock dx object
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		}
		// Clear the sender access mutex and allow the receiver access to the texture
		// The sender will not write again until the receiver has
		// read the data and released the receiver access mutex
		spoutdx.AllowAccess(m_hAccessMutex); 

	}
	*/

	return false;

} // end WriteTexture


// COPY FROM THE SHARED OPENGL TEXTURE TO AN OPENGL TEXTURE
bool spoutGLDXinterop::ReadTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO)
{
	GLenum status;
	// DWORD dwFrame = 0;

	if(m_hInteropDevice == NULL || m_hInteropObject == NULL) return false;
	if(width != (unsigned int)m_TextureInfo.width || height != (unsigned int)m_TextureInfo.height) return false;

	/*
	// Basic code for debugging
	if(spoutdx.CheckAccess(m_hAccessMutex)) {
		wglDXLockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
		// Attach the Input texture (the shared texture) to the color buffer in our frame buffer 
		if(m_glTexture > 0) {
			glFramebufferTexture2DEXT(READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_glTexture, 0);
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {
				// Copy from the fbo (shared texture attached) to the dest texture
				glBindTexture(TextureTarget, TextureID);
				glCopyTexSubImage2D(TextureTarget, 0, 0, 0, 0, 0, width, height);
				glBindTexture(TextureTarget, 0);
			}
			else {
				PrintFBOstatus(status);
			}
		}
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);

		wglDXUnlockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
	}
	spoutdx.AllowAccess(m_hAccessMutex);

	return true;
	*/

	/*
	// LJ DEBUG to clear alpha from the texture
	GLuint myTexture = 0;
	InitTexture(myTexture, GL_RGBA, width, height);

	if(spoutdx.CheckAccess(m_hAccessMutex)) {
		wglDXLockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glFramebufferTexture2DEXT(DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, myTexture, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

		// Draw the shared texture to the local texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_glTexture); // bind shared texture
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);	glVertex2f(-1,-1); // lower left
		glTexCoord2f(0.0, 0.0);	glVertex2f(-1, 1); // upper left
		glTexCoord2f(1.0, 0.0);	glVertex2f( 1, 1); // upper right
		glTexCoord2f(1.0, 1.0);	glVertex2f( 1,-1); // lower right
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0); // unbind shared texture
		glDisable(GL_TEXTURE_2D);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);
		wglDXUnlockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
	}
	spoutdx.AllowAccess(m_hAccessMutex);
	*/


	/*
	if(spoutdx.CheckAccess(m_hAccessMutex)) {
		wglDXLockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
		// Clear the shared texture alpha
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
		glFramebufferTexture2DEXT(DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_glTexture, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glColorMask(FALSE, FALSE, FALSE, TRUE);//This ensures that only alpha will be effected
		glClearColor(1, 1, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);
		wglDXUnlockObjectsNV(m_hInteropDevice, 1, &m_hInteropObject);
	}
	spoutdx.AllowAccess(m_hAccessMutex);
	*/

	// printf("ReadTexture - bInvert = %d\n", bInvert);
	if(spoutdx.CheckAccess(m_hAccessMutex)) {

		// lock interop
		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			
			// bind the FBO (for both, READ_FRAMEBUFFER_EXT and DRAW_FRAMEBUFFER_EXT)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

			// Attach the Input texture (the shared texture) to the color buffer in our frame buffer - note texturetarget 
			glFramebufferTexture2DEXT(READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_glTexture, 0);
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

			// Attach target texture (the one we read into and return) to second attachment point
			if(bInvert) {
				glFramebufferTexture2DEXT(DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, TextureTarget, TextureID, 0);
				glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
			}

			status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {

				// Flip if the user wants that
				if(bInvert) {
					// copy one texture buffer to the other while flipping upside down
					glBlitFramebufferEXT(0,     0,		// srcX0, srcY0, 
										 width, height, // srcX1, srcY1
										 0,     height,	// dstX0, dstY0,
										 width, 0,		// dstX1, dstY1,
										 GL_COLOR_BUFFER_BIT, GL_LINEAR);
				}
				else { 
					// Copy from the fbo (shared texture attached) to the dest texture
					glBindTexture(TextureTarget, TextureID);
					glCopyTexSubImage2D(TextureTarget, 0, 0, 0, 0, 0, width, height);
					glBindTexture(TextureTarget, 0);
				}
			}
			else {
				PrintFBOstatus(status);
			}

			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); // 04.01.16

			// LJ DEBUG
			// glClearColor(1, 1, 1, 1);
			// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			// restore the previous fbo - default is 0
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);

			// unlock dx object
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		}
		else {
			// printf("ReadTexture - no lock\n");
		}
		spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture
	}
	else {
		// printf("ReadTexture - no access\n");
	}
	spoutdx.AllowAccess(m_hAccessMutex);


	/*
	// LJ DEBUG - version with both sender and receiver access mutex
	// Wait for the sender to release access to the texture
	if(spoutdx.CheckAccess(m_hAccessMutex)) {

		// lock interop
		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			
			// bind the FBO (for both, READ_FRAMEBUFFER_EXT and DRAW_FRAMEBUFFER_EXT)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
			
			// Attach the Input texture (the shared texture) to the color buffer in our frame buffer - note texturetarget 
			glFramebufferTexture2DEXT(READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_glTexture, 0);
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

			// Attach target texture (the one we read into and return) to second attachment point
			if(bInvert) {
				glFramebufferTexture2DEXT(DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, TextureTarget, TextureID, 0);
				glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
			}

			status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {

				// Flip if the user wants that
				if(bInvert) {
					// copy one texture buffer to the other while flipping upside down
					glBlitFramebufferEXT(0,     0,		// srcX0, srcY0, 
										 width, height, // srcX1, srcY1
										 0,     height,	// dstX0, dstY0,
										 width, 0,		// dstX1, dstY1,
										 GL_COLOR_BUFFER_BIT, GL_LINEAR);
				}
				else { 
					// Copy from the fbo (shared texture attached) to the dest texture
					glBindTexture(TextureTarget, TextureID);
					glCopyTexSubImage2D(TextureTarget, 0, 0, 0, 0, 0, width, height);
					glBindTexture(TextureTarget, 0);
				}
			}
			else {
				PrintFBOstatus(status);
			}

			// restore the previous fbo - default is 0
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);

			// unlock dx object
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		}
		else {
			printf("ReadTexture - no lock\n");
		}
		// Clear the receiver access mutex and allow the sender access to the texture
		// The receiver will not read again until the sender has
		// written the data and released the sender access mutex
		spoutdx.AllowAccess(m_hReceiverAccessMutex); // Allow the sender access to the texture
	}
	*/

	return true;

} // end ReadTexture


//
// COPY THE SHARED TEXTURE TO IMAGE PIXELS - 15-07-14 allowed for variable format instead of RGB only
bool spoutGLDXinterop::ReadTexturePixels(unsigned char *pixels, unsigned int width, unsigned int height, GLenum glFormat, GLuint HostFBO)
{
	GLenum status;

	if(m_hInteropDevice == NULL || m_hInteropObject == NULL) return false;
	if(width != m_TextureInfo.width || height != m_TextureInfo.height) return false;

	// retrieve opengl texture data directly to image pixels rather than via an fbo and texture
	// Wait for access to the texture
	if(spoutdx.CheckAccess(m_hAccessMutex)) {
		// lock dx object
		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			/*
			//
			// glGetTexImage method
			//
			glBindTexture(GL_TEXTURE_2D, m_glTexture);
			glGetTexImage(GL_TEXTURE_2D, 0,  glFormat,  GL_UNSIGNED_BYTE, pixels);
			glBindTexture(GL_TEXTURE_2D, 0);
			*/

			//
			// fbo attachment method - current fbo now has to be passed in
			//

			// Bind our local fbo
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo); 
			// Attach the shared texture to the color buffer in our frame buffer
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_glTexture, 0);

			status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {
				// read the pixels from the framebuffer
				glReadPixels(0, 0, width, height, glFormat, GL_UNSIGNED_BYTE, pixels);
			}
			else {
				PrintFBOstatus(status);
			}

			// restore the previous fbo - default is 0
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);


			// Unlock interop object
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);

			// drop through to manage events and return true;
		} // if lock failed just keep going
	}
	spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture



	/*
	// ColourBar image for testing
	// http://stackoverflow.com/questions/6939422/how-do-i-create-a-color-bar-tv-test-pattern
	//
	typedef struct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} RGBA;
		
	const RGBA BAR_COLOUR[8] =
	{
		{ 255, 255, 255, 255 },  // 100% White
		{ 255, 255,   0, 255 },  // Yellow
		{   0, 255, 255, 255 },  // Cyan
		{   0, 255,   0, 255 },  // Green
		{ 255,   0, 255, 255 },  // Magenta
		{ 255,   0,   0, 255 },  // Red
		{   0,   0, 255, 255 },  // Blue
		{   0,   0,   0, 255 },  // Black
	};

	// Allocate frame buffer
	size_t      frameBytes = width*height*sizeof(RGBA);
	RGBA*       frame = (RGBA*)malloc(frameBytes);
	unsigned    columnWidth = width / 8;

	// Generate complete frame
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			unsigned col_idx = x / columnWidth;
			frame[y*width+x] = BAR_COLOUR[col_idx];
		}
	}

	// transfer to the caller pixels
	memcpy((void *)pixels, (void *)frame, width*height*4);

	free((void*)frame);
	*/


	return true;

} // end ReadTexturePixels 



// BIND THE SHARED TEXTURE
// for use in an application - this locks the interop object and binds the shared texture
// Locks remain in place, so afterwards a call to UnbindSharedTxeture MUST be called
bool spoutGLDXinterop::BindSharedTexture()
{
	bool bRet = false;

	if(m_hInteropDevice == NULL || m_hInteropObject == NULL)
		return false;

	// Wait for access to the texture
	if(spoutdx.CheckAccess(m_hAccessMutex)) {
		// lock dx object
		if(LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			// Bind our shared OpenGL texture
			glBindTexture(GL_TEXTURE_2D, m_glTexture);
			bRet = true;
		}
		else {
			bRet = false;
		}
	}

	// Leave locked for succcess, release interop lock and allow texture access for fail
	if(!bRet) {
		UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		spoutdx.AllowAccess(m_hAccessMutex); // Allow access to the texture
	}

	return bRet;

} // end BindSharedTexture


// UNBIND THE SHARED TEXTURE
// for use in an application - this unbinds the shared texture and unlocks the interop object
bool spoutGLDXinterop::UnBindSharedTexture()
{
	if(m_hInteropDevice == NULL || m_hInteropObject == NULL)
		return false;
	
	// Unbind our shared OpenGL texture
	glBindTexture(GL_TEXTURE_2D,0);
	// unlock dx object
	UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
	// Allow access to the texture
	spoutdx.AllowAccess(m_hAccessMutex);
	
	return true;

} // end BindSharedTexture



/*

	GL/DX Interop lock

	A return value of TRUE indicates that all objects were
    successfully locked.  A return value of FALSE indicates an
    error. If the function returns FALSE, none of the objects will be locked.

	Attempting to access an interop object via GL when the object is
    not locked, or attempting to access the DirectX resource through
    the DirectX API when it is locked by GL, will result in undefined
    behavior and may result in data corruption or program
    termination. Likewise, passing invalid interop device or object
    handles to this function has undefined results, including program
    termination.

	Note that only one GL context may hold the lock on the
    resource at any given time --- concurrent access from multiple GL
    contexts is not currently supported.

	http://halogenica.net/sharing-resources-between-directx-and-opengl/

	This lock triggers the GPU to perform the necessary flushing and stalling
	to guarantee that the surface has finished being written to before reading from it. 

	DISCUSSION: The Lock/Unlock calls serve as synchronization points
    between OpenGL and DirectX. They ensure that any rendering
    operations that affect the resource on one driver are complete
    before the other driver takes ownership of it.

	This function assumes only one object to 

	Must return S_OK (0) - otherwise the error can be checked.

*/
HRESULT spoutGLDXinterop::LockInteropObject(HANDLE hDevice, HANDLE *hObject)
{
	DWORD dwError;
	HRESULT hr;

	if(hDevice == NULL || hObject == NULL || *hObject == NULL) {
		return E_HANDLE;
	}

	// lock dx object
	if(wglDXLockObjectsNV(hDevice, 1, hObject) == TRUE) {
		return S_OK;
	}
	else {
		dwError = GetLastError();
		switch (dwError) {
			case ERROR_BUSY :			// One or more of the objects in <hObjects> was already locked.
				hr = E_ACCESSDENIED;	// General access denied error
				// printf("	spoutGLDXinterop::LockInteropObject ERROR_BUSY\n");
				break;
			case ERROR_INVALID_DATA :	// One or more of the objects in <hObjects>
										// does not belong to the interop device
										// specified by <hDevice>.
				hr = E_ABORT;			// Operation aborted
				// printf("	spoutGLDXinterop::LockInteropObject ERROR_INVALID_DATA\n");
				break;
			case ERROR_LOCK_FAILED :	// One or more of the objects in <hObjects> failed to 
				hr = E_ABORT;			// Operation aborted
				// printf("	spoutGLDXinterop::LockInteropObject ERROR_LOCK_FAILED\n");
				break;
			default:
				hr = E_FAIL;			// unspecified error
				// printf("	spoutGLDXinterop::LockInteropObject UNKNOWN_ERROR\n");
				break;
		} // end switch
	} // end false

	return hr;

} // LockInteropObject


//
// Must return S_OK (0) - otherwise the error can be checked.
//
HRESULT spoutGLDXinterop::UnlockInteropObject(HANDLE hDevice, HANDLE *hObject)
{
	DWORD dwError;
	HRESULT hr;

	if(hDevice == NULL || hObject == NULL || *hObject == NULL) {
		return E_HANDLE;
	}

	if (wglDXUnlockObjectsNV(hDevice, 1, hObject) == TRUE) {
		return S_OK;
	}
	else {
		dwError = GetLastError();
		switch (dwError) {
			case ERROR_NOT_LOCKED :
				hr = E_ACCESSDENIED;
				// printf("	spoutGLDXinterop::UnLockInteropObject ERROR_NOT_LOCKED\n");
				break;
			case ERROR_INVALID_DATA :
				// printf("	spoutGLDXinterop::UnLockInteropObject ERROR_INVALID_DATA\n");
				hr = E_ABORT;
				break;
			case ERROR_LOCK_FAILED :
				hr = E_ABORT;
				// printf("	spoutGLDXinterop::UnLockInteropObject ERROR_LOCK_FAILED\n");
				break;
			default:
				hr = E_FAIL;
				// printf("	spoutGLDXinterop::UnLockInteropObject UNKNOWN_ERROR\n");
				break;
		} // end switch
	} // end fail

	return hr;

} // end UnlockInteropObject


// Needs an OpenGL context due to DX11 compatibility check
bool spoutGLDXinterop::UseDX9(bool bDX9)
{
	if(bDX9 == true) {
		// Set to DirectX 9
		// DirectX 11 is the default but is checked by OpenDirectX.
		bUseDX9 = bDX9;
		return true;
	}

	// Check for DirectX 11 availability if the user requested it
	if(DX11available()) {
		bUseDX9 = false;
		return true;
	}

	// Set to use DirectX 9 if DirectX 11 is not available
	bUseDX9 = true;
	return false;

}

bool spoutGLDXinterop::isDX9()
{
	// Check support for DirectX 11
	// It is checked with OpenDirectX but this might not have been called yet.
	// The user can call this after the Spout SetDX9 call to check if it succeeded.
	if(!DX11available()) {
		bUseDX9 = true;
	}

	// Otherwise return what has been set
	// This can be checked after directX initialization
	// to find out if DirectX 11 initialization failed
	return bUseDX9;
}

// Set flag - does not require OpenGL context - UseDX9 does
void spoutGLDXinterop::SetDX9(bool bDX9)
{
	bUseDX9 = bDX9;
}

// Return existing flag - does not require OpenGL context - isDX9 does
bool spoutGLDXinterop::GetDX9()
{
	return bUseDX9;
}


bool spoutGLDXinterop::GetMemoryShareMode()
{
	// Already set true, so just return it
	// It is set by the user via the "SpoutDirectX" utility for 2.005 and later
	// and it is also picked up when the SpoutGLDXinterop class initializes
	// including a compatibility check and set to true for incompatible hardware.
	if(bUseMemory) {
		return bUseMemory;
	}
	else {
		//
		// MemoryShare is not set
		//
		// Try to get the MemoryShare key from the registry
		// The key is set by the 2.005 installer and won't be present for 2.004 apps. 
		//
		// Setting MemoryShare for 2.004 has to be avoided because the sender sharehandle will be
		// null and this will crash existing 2.004 receivers. 
		//
		// The result is that memoryshare mode will remain false for 2.004 or below
		// but can be set by the user for 2.005 and above.
		//
		DWORD dwMemory = 0;
		if(spoutdx.ReadDwordFromRegistry(&dwMemory, "Software\\Leading Edge\\Spout", "MemoryShare")) {
			// printf("spoutGLDXinterop::GetMemoryShareMode = dwMemory = %d\n", dwMemory);
			// The registry key is there so this is 2.005 or above
			if(!GLDXcompatible()) {
				// printf("    !GLDXcompatible\n");
				// Force memoryshare if not compatible
				bUseMemory = true;
				// Write to the registry now - this function is called by the SpoutDirectX utility when it starts
				spoutdx.WriteDwordToRegistry((DWORD)bUseMemory, "Software\\Leading Edge\\Spout", "MemoryShare");
			}
			else {
				// Return what is in the registry
				bUseMemory = (dwMemory == 1);
				// printf("    bUseMemory = %d\n", bUseMemory);
			}
			return bUseMemory;
		}
	}

	// If not >2.004 the only option is texture share
	return false;
}


bool spoutGLDXinterop::SetMemoryShareMode(bool bMem)
{
	// DWORD dwMemory = 0;

	bUseMemory = bMem; // 19.10.15 - user setting for this app

	return true;

	/*
	if(bMem) {// Already set true, so just return true
		if(bUseMemory) {
			return true;
		}
		else {
			//
			// Otherwise try to get the MemoryShare key from the registry
			// The key is set by the 2.005 installer and won't be present for 2.004 installations. 
			// Setting MemoryShare for 2.004 has to be avoided because the sender sharehandle will be
			// null and this will crash existing 2.004 receivers. 
			//
			if(spoutdx.ReadDwordFromRegistry(&dwMemory, "Software\\Leading Edge\\Spout", "MemoryShare")) {
				bUseMemory = true;
				spoutdx.WriteDwordToRegistry((DWORD)bUseMemory, "Software\\Leading Edge\\Spout", "MemoryShare");
				return true;
			}
		}
	}
	else {
		bUseMemory = false;
		// Set registry for 2.005 and above
		if(spoutdx.ReadDwordFromRegistry(&dwMemory, "Software\\Leading Edge\\Spout", "MemoryShare")) {
			spoutdx.WriteDwordToRegistry((DWORD)bUseMemory, "Software\\Leading Edge\\Spout", "MemoryShare");
			return true;
		}
	}

	// If not >2.004 the only option is texture share
	return false;
	*/

}

GLuint spoutGLDXinterop::GetGLtextureID()
{
	return m_glTexture;
}


void spoutGLDXinterop::SetDX11format(DXGI_FORMAT textureformat)
{
	DX11format = textureformat;
}

void spoutGLDXinterop::SetDX9format(D3DFORMAT textureformat)
{
	DX9format = textureformat;
}

// Set graphics adapter for Spout output
bool spoutGLDXinterop::SetAdapter(int index) 
{
	if(spoutdx.SetAdapter(index)) {
		// printf("spoutGLDXinterop::SetAdapter(%d) OK\n", index);
		return true;
	}

	spoutdx.SetAdapter(-1); // make sure globals are reset to default

	return false;
}

// Get current adapter index
int spoutGLDXinterop::GetAdapter() 
{
	return spoutdx.GetAdapter();
}


// Get the path of the host that produced the sender
// from the description string in the sender info memory map
// Description is defined as wide chars, but the path is stored as byte chars
bool spoutGLDXinterop::GetHostPath(const char *sendername, char *hostpath, int maxchars)
{
	SharedTextureInfo info;
	int n;

	if(!senders.getSharedInfo(sendername, &info))
		return false;

	n = maxchars;
	if(n > 256) n = 256; // maximum field width in shared memory

	strcpy_s(hostpath, n, (char *)info.description);

	return true;
}


// Get the number of graphics adapters in the system
int spoutGLDXinterop::GetNumAdapters()
{
	return spoutdx.GetNumAdapters();
}

// Get an adapter name
bool spoutGLDXinterop::GetAdapterName(int index, char *adaptername, int maxchars)
{
	return spoutdx.GetAdapterName(index, adaptername, maxchars);
}


// Needs OpenGL context
int spoutGLDXinterop::GetVerticalSync()
{
	if(!bExtensionsLoaded) bExtensionsLoaded = LoadGLextensions();

	// needed for both sender and receiver
	if(bSWAPavailable) {
		return(wglGetSwapIntervalEXT());
	}
	return 0;
}


bool spoutGLDXinterop::SetVerticalSync(bool bSync)
{
	if(!bExtensionsLoaded) bExtensionsLoaded = LoadGLextensions();

	if(bSWAPavailable) {
		if(bSync)
			wglSwapIntervalEXT(1); // lock to monitor vsync
		else
			wglSwapIntervalEXT(0); // unlock from monitor vsync
		return true;
	}
	return false;
}

// For debugging - (gluErrorString needs glu32.lib)
void spoutGLDXinterop::GLerror() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("GL error = %d (0x%x)\n", err, err);
		// printf("GL error = %d (0x%x) %s\n", err, err, gluErrorString(err));
	}
}	


void spoutGLDXinterop::PrintFBOstatus(GLenum status)
{
	// printf("FBO error status = %d (0x%x)\n", status, status);
	if (status == GL_FRAMEBUFFER_UNSUPPORTED_EXT)
		printf("GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT)
		printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT\n");
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT)
		printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT\n");
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)
		printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT - width-height problems?\n");
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)
		printf("GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n");
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)
		printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT\n");
	else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT)
		printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT\n");
	// else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT)
	// 	printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT\n");
	else 
		printf("\n Unknown Code: glCheckFramebufferStatusEXT returned %i (%x)\n",status, status);
	GLerror();
}
	

/*
bool spoutGLDXinterop::ReadDwordFromRegistry(DWORD *pValue, const char *subkey, const char *valuename)
{
	HKEY  hRegKey;
	LONG  regres;
	DWORD  dwSize, dwKey;  

	dwSize = MAX_PATH;

	// Does the key exist
	regres = RegOpenKeyExA(HKEY_CURRENT_USER, subkey, NULL, KEY_READ, &hRegKey);
	if(regres == ERROR_SUCCESS) {
		// Read the key DWORD value
		regres = RegQueryValueExA(hRegKey, valuename, NULL, &dwKey, (BYTE*)pValue, &dwSize);
		RegCloseKey(hRegKey);
		if(regres == ERROR_SUCCESS)
			return true;
	}

	// Just quit if the key does not exist
	return false;

}

bool spoutGLDXinterop::WriteDwordToRegistry(DWORD dwValue, const char *subkey, const char *valuename)
{
	HKEY  hRegKey;
	LONG  regres;
	char  mySubKey[512];

	// The required key
	strcpy_s(mySubKey, 512, subkey);

	// Does the key already exist ?
	regres = RegOpenKeyExA(HKEY_CURRENT_USER, mySubKey, NULL, KEY_ALL_ACCESS, &hRegKey);
	if(regres != ERROR_SUCCESS) { 
		// Create a new key
		regres = RegCreateKeyExA(HKEY_CURRENT_USER, mySubKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL, &hRegKey, NULL);
	}

	if(regres == ERROR_SUCCESS && hRegKey != NULL) {
		// Write the DWORD value
		regres = RegSetValueExA(hRegKey, valuename, 0, REG_DWORD, (BYTE*)&dwValue, 4);
		// For immediate read after write - necessary here because the app might set the values 
		// and read the registry straight away and it might not be available yet
		// The key must have been opened with the KEY_QUERY_VALUE access right (included in KEY_ALL_ACCESS)
		RegFlushKey(hRegKey); // needs an open key
		RegCloseKey(hRegKey); // Done with the key
    }

	if(regres == ERROR_SUCCESS)
		return true;
	else
		return false;

}
*/

// =======================================================
//               2.005 Memoryshare functions
// =======================================================

//
// Write texture pixel data to shared memory
//
bool spoutGLDXinterop::WriteMemory(GLuint TexID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert)
{

	unsigned char *pBuffer = memoryshare.LockSenderMemory();
	if(!pBuffer) {
		return false;
	}

	// printf("spoutGLDXinterop::WriteMemory, %d, %d, [%x], [%x] (bInvert = %d)\n", width, height, TexID, TextureTarget, bInvert);

	// Invert image data if the user wants it - default is false
	if(bInvert) {
		unsigned char *pixels = NULL;
		unsigned int imagesize = width*height*4; // RGBA
		pixels = (unsigned char *)malloc(imagesize*sizeof(unsigned char));
		if(!pixels) return false;
		// Read the opengl texture into the temporary pixel buffer
		// When a texture is first bound, it assumes the specified target
		glBindTexture(TextureTarget, TexID);
		glGetTexImage(TextureTarget, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
		glBindTexture(TextureTarget, 0);
		// Flip vertically between pixels and pBuffer
		unsigned char *src = pixels+(width*height*4)-width*4;
		unsigned char *dst = pBuffer;
		for(unsigned int i = 0; i<height; i++) {
			CopyMemory((void *)dst, (void *)src, width*4 );
			src -= width*4;
			dst += width*4;
		}
		free((void *)pixels);
	}
	else {
		// Read the opengl texture into the memory map buffer
		glBindTexture(TextureTarget, TexID);
		glGetTexImage(TextureTarget, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pBuffer);
		glBindTexture(TextureTarget, 0);
	}

	memoryshare.UnlockSenderMemory();

	return true;
}

//
// Read shared memory to texture pixel data
//
bool spoutGLDXinterop::ReadMemory(GLuint TexID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert)
{
	unsigned char *pBuffer = memoryshare.LockSenderMemory();
	if(!pBuffer) return false;

	// printf("spoutGLDXinterop::ReadMemory, %d, %d, [%x], [%x] (bInvert = %d)\n", width, height, TexID, TextureTarget, bInvert);

	// printf("ReadMemory - bInvert = %d\n", bInvert);
	// Invert image data if the user wants it - default is false
	if(bInvert) {
		unsigned char *pixels = NULL;
		unsigned int imagesize = width*height*4; // RGBA
		pixels = (unsigned char *)malloc(imagesize*sizeof(unsigned char));
		if(!pixels) return false;
		// Flip vertically between pBuf and pixels
		unsigned char *src = pBuffer+(width*height*4)-width*4;
		unsigned char *dst = pixels;
		for(unsigned int i = 0; i<height; i++) {
			CopyMemory((void *)dst, (void *)src, width*4 );
			src -= width*4;
			dst += width*4;
		}
		// Copy the inverted pixel buffer to the opengl texture
		if(TexID > 0 && TextureTarget > 0) {
			glBindTexture(TextureTarget, TexID);
			glTexSubImage2D(TextureTarget, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
			glBindTexture(TextureTarget, 0);
		}
		
		free((void *)pixels);
	}
	else {
		// Copy the memory map pixels directly to the opengl texture
		if(TexID > 0 && TextureTarget > 0) {
			glBindTexture(TextureTarget, TexID);
			glTexSubImage2D(TextureTarget, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pBuffer);
			glBindTexture(TextureTarget, 0);
		}
	}

	memoryshare.UnlockSenderMemory();

	return true;
}

//
// Write image pixels to shared memory
//
bool spoutGLDXinterop::WriteMemoryPixels(unsigned char *pixels, unsigned int width, unsigned int height)
{
	unsigned char *pBuffer = memoryshare.LockSenderMemory();
	if(!pBuffer) return false;

	// printf("WriteMemoryPixels(%d, %d)\n", width, height);

	// Set pixels directly to to shared memory
	CopyMemory((void *)pBuffer, (void *)pixels, width*height*4 );

	memoryshare.UnlockSenderMemory();

	return true;

}

//
// Read shared memory to image pixels
//
bool spoutGLDXinterop::ReadMemoryPixels(unsigned char *pixels, unsigned int width, unsigned int height)
{
	unsigned char *pBuffer = memoryshare.LockSenderMemory();
	if(!pBuffer) {
		printf("ReadMemoryPixels lock failed\n");
		return false;
	}

	// Receive pixels directly from shared memory
	CopyMemory((void *)pixels, (void *)pBuffer, width*height*4 );

	memoryshare.UnlockSenderMemory();

	return true;

}




// DRAW A TEXTURE INTO SHARED MEMORY - equivalent to DrawToSharedTexture
bool spoutGLDXinterop::DrawToSharedMemory(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, float max_x, float max_y, float aspect, bool bInvert, GLuint HostFBO)
{
	unsigned int memWidth, memHeight;
	GLenum status;

	// Get the memoryshare size
	if(!memoryshare.GetSenderMemorySize(memWidth, memHeight))
		return false;

	// Sender size check - quit if not equal
	if(width != memWidth || height != memHeight) 
		return false;

	// Find the shared memory buffer pointer
	unsigned char *pBuffer = memoryshare.LockSenderMemory();
	if(!pBuffer) return false;

	// Create local buffer texture if not yet created or re-create if the memoryshare size has changed 
	if(m_TexID == 0 || width != m_TexWidth || height != m_TexHeight) 
		InitTexture(m_TexID, GL_RGBA, width, height);

	// Create an fbo if not already
	if(m_fbo == 0) glGenFramebuffersEXT(1, &m_fbo); 
	
	// Draw the input texture into the local texture via an fbo
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	// Destination is the fbo with local texture attached
	glFramebufferTexture2DEXT(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_TexID, 0);

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status == GL_FRAMEBUFFER_COMPLETE_EXT) {
	
		glBindTexture(GL_TEXTURE_2D, m_TexID);
	
		// Draw the input texture
		glColor4f(1.f, 1.f, 1.f, 1.f);
		glEnable(TextureTarget);
		glBindTexture(TextureTarget, TextureID);

		GLfloat tc[4][2] = {0};

		// Invert texture coord to user requirements
		if(bInvert) {
			tc[0][0] = 0.0;   tc[0][1] = max_y;
			tc[1][0] = 0.0;   tc[1][1] = 0.0;
			tc[2][0] = max_x; tc[2][1] = 0.0;
			tc[3][0] = max_x; tc[3][1] = max_y;
		}
		else {
			tc[0][0] = 0.0;   tc[0][1] = 0.0;
			tc[1][0] = 0.0;   tc[1][1] = max_y;
			tc[2][0] = max_x; tc[2][1] = max_y;
			tc[3][0] = max_x; tc[3][1] = 0.0;
		}

		GLfloat verts[] =  {
						-aspect, -1.0,   // bottom left
						-aspect,  1.0,   // top left
						 aspect,  1.0,   // top right
						 aspect, -1.0 }; // bottom right
	
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, 0, tc );
		glEnableClientState(GL_VERTEX_ARRAY);		
		glVertexPointer(2, GL_FLOAT, 0, verts );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindTexture(TextureTarget, 0);
		glDisable(TextureTarget);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		PrintFBOstatus(status);
	}


	// restore the previous fbo - default is 0
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);

	// Now read the local opengl texture into the memory map buffer
	glBindTexture(GL_TEXTURE_2D, m_TexID);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	memoryshare.UnlockSenderMemory();

	return true;
}

//
// Draw shared memory via texture - equivalent to DrawSharedTexture
//
bool spoutGLDXinterop::DrawSharedMemory(float max_x, float max_y, float aspect, bool bInvert)
{
	unsigned int width, height;

	// printf("spoutGLDXinterop::DrawMemory, %d, %d, [%x], [%x] (bInvert = %d)\n", width, height, TexID, TextureTarget, bInvert);

	// Get the memoryshare size
	if(!memoryshare.GetSenderMemorySize(width, height))
		return false;

	// Find the shared memory buffer pointer
	unsigned char *pBuffer = memoryshare.LockSenderMemory();
	if(!pBuffer) return false;

	// Create a local buffer texture if not yet created or re-create if the memoryshare size has changed 
	if(m_TexID == 0 || width != m_TexWidth || height != m_TexHeight) 
		InitTexture(m_TexID, GL_RGBA, width, height);

	// Copy the memory map pixels directly to the opengl texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_TexID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pBuffer);

	// Draw the texture
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glBegin(GL_QUADS);
	if(bInvert) {
		glTexCoord2f(0.0,	max_y);	glVertex2f(-aspect,-1.0); // lower left
		glTexCoord2f(0.0,	0.0);	glVertex2f(-aspect, 1.0); // upper left
		glTexCoord2f(max_x, 0.0);	glVertex2f( aspect, 1.0); // upper right
		glTexCoord2f(max_x, max_y);	glVertex2f( aspect,-1.0); // lower right
	}
	else {
		glTexCoord2f(0.0,   0.0);	glVertex2f(-aspect,-1.0); // lower left
		glTexCoord2f(0.0,   max_y);	glVertex2f(-aspect, 1.0); // upper left
		glTexCoord2f(max_x, max_y);	glVertex2f( aspect, 1.0); // upper right
		glTexCoord2f(max_x, 0.0);	glVertex2f( aspect,-1.0); // lower right
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	memoryshare.UnlockSenderMemory();

	return true;
}


// Initialize local texture
bool spoutGLDXinterop::InitTexture(GLuint &texID, GLenum GLformat, unsigned int width, unsigned int height)
{
	if(texID != 0) glDeleteTextures(1, &texID);	
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GLformat, width, height, 0, GLformat, GL_UNSIGNED_BYTE, NULL); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}


bool spoutGLDXinterop::CheckSpout2004()
{
	// 2.004 - 2.005 transition
	// Defaults :
	//		2.004	bInvert = true
	//		2.005	bInvert = false
	// This is a 2.005 app and the default is false 
	// Check for 2.005 and set to true if present
	// Look for the memorymode registry key
	// Memoryshare depends on the 2.005 SpoutDirectX utility "MemoryShare" checkbox and
	// the registry key set by the 2.005 installer which won't be present for 2.004 apps
	DWORD dwMemory = 0;
	if(!spoutdx.ReadDwordFromRegistry(&dwMemory, "Software\\Leading Edge\\Spout", "MemoryShare")) {
		// 2.004 apps will not have the registry setting so set bInvert to the 2.004 app default true
		return true; 
	}

	// Otherwise return 2.005 default false
	return false;

}


// DEBUGGING
//
// COPY A DWORD TO THE FIRST 4 PIXELS OF AN OPENGL TEXTURE
//
bool spoutGLDXinterop::WriteDword(GLuint TextureID, GLuint TextureTarget, DWORD dwFrameNumber)
{
		// DWORD = 32bits, 8 bytes
		// 0 through 4294967295 decimal
		// 4294967295/60 = 71582788/60 = 1193046 mins = 19884 hrs = 828 days

		// printf("dwFrame %d\n", dwFrameNumber);
		unsigned __int32 iFrameNumber = (unsigned __int32)dwFrameNumber;

		unsigned __int8 pixels[64];
		memset((void *)pixels, 0, 64*sizeof(unsigned __int8));
		unsigned __int32 n = 0;

		/*
		// Base 8 modulus method
		// Units, 8, 64, 512, 4096, 32768, 262144, 2097152,
		// 16777216, 134217728, 1073741824, 8589934592, 68719476736, 549755813888, 4398046511104, 35184372088832
		// units
		unsigned __int8 ii = 1;
		n = (iFrameNumber % 8);
		pixels[0]  = (unsigned __int8)n << 1;
		pixels[1] = 0;
		pixels[2] = 0;
		pixels[3] = 0;
		// 8
		n = (((iFrameNumber)/8) % 8);
		pixels[ 4]  = (unsigned __int8)n << 1;
		pixels[ 5] = 0;
		pixels[ 6] = 0;
		pixels[ 7] = 0;
		// 64
		n = (((iFrameNumber)/64) % 8);
		pixels[ 8]  = (unsigned __int8)n << 1;
		pixels[ 9] = 0;
		pixels[10] = 0;
		pixels[11] = 0;
		// 512
		n = (((iFrameNumber)/512) % 8);
		pixels[12]  = (unsigned __int8)n << 1;
		pixels[13] = 0;
		pixels[14] = 0;
		pixels[15] = 0;
		// 4096
		n = (((iFrameNumber)/4096) % 8);
		pixels[16]  = (unsigned __int8)n << 1;
		pixels[17] = 0;
		pixels[18] = 0;
		pixels[19] = 0;
		// 32768
		n = (((iFrameNumber)/32768) % 8);
		pixels[20]  = (unsigned __int8)n << 1;
		pixels[21] = 0;
		pixels[22] = 0;
		pixels[23] = 0;
		// 262144
		n = (((iFrameNumber)/262144) % 8);
		pixels[24]  = (unsigned __int8)n << 1;
		pixels[25] = 0;
		pixels[26] = 0;
		pixels[27] = 0;
		// 2097152
		n = (((iFrameNumber)/2097152) % 8);
		pixels[28]  = (unsigned __int8)n << 1;
		pixels[29] = 0;
		pixels[30] = 0;
		pixels[31] = 0;
		*/

		//
		// Reduction method base 8
		//
		// http://embeddedgurus.com/stack-overflow/2011/02/efficient-c-tip-13-use-the-modulus-operator-with-caution/
		//
		pixels[31] = 0;
		pixels[30] = 0;
		pixels[29] = 0;
		// 2097152
		n = iFrameNumber/2097152U;
		iFrameNumber -= n*2097152U;
		pixels[28]  = (unsigned __int8)n << 1;
		pixels[27] = 0;
		pixels[26] = 0;
		pixels[25] = 0;
		// 262144
		n = iFrameNumber/262144U;
		iFrameNumber -= n*262144U;
		pixels[24]  = (unsigned __int8)n << 1;
		pixels[23] = 0;
		pixels[22] = 0;
		pixels[21] = 0;
		// 32768
		n = iFrameNumber/32768U;
		iFrameNumber -= n*32768U;
		pixels[20]  = (unsigned __int8)n << 1;
		pixels[19] = 0;
		pixels[18] = 0;
		pixels[17] = 0;
		// 4096
		n = iFrameNumber/4096U;
		iFrameNumber -= n*4096U;
		pixels[16]  = (unsigned __int8)n << 1;
		pixels[15] = 0;
		pixels[14] = 0;
		pixels[13] = 0;
		// 512
		n = iFrameNumber/512U;
		iFrameNumber -= n*512U;
		pixels[12]  = (unsigned __int8)n << 1;
		pixels[11] = 0;
		pixels[10] = 0;
		pixels[9] = 0;
		// 64
		n = iFrameNumber/64U;
		iFrameNumber -= n*64U;
		pixels[8]  = (unsigned __int8)n << 1;
		pixels[7] = 0;
		pixels[6] = 0;
		pixels[5] = 0;
		// 8
		n = iFrameNumber/8U;
		iFrameNumber -= n*8U;
		pixels[4]  = (unsigned __int8)n << 1;
		pixels[3] = 0;
		pixels[2] = 0;
		pixels[1] = 0;
		// 1s
		n = iFrameNumber;
		// if(n == 0) pixels[0] = 2;
		// else
			pixels[0]  = (unsigned __int8)n << 1;

		/*
		// ASCII conversion method - works OK
		char temp[64];
		temp[0] = temp[1] = temp[2] = temp[3] = temp[4] = temp[5] = 0;
		sprintf_s(temp, 64, "%6.6d", iFrameNumber);
		temp[6] = 0;
		printf("%c%c%c%c%c%c\n", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]);

		pixels[11] = 1;
		pixels[10] = 1;
		// 100000s
		pixels[9]  = (unsigned __int8)(temp[0]-48) + 1;
		// 10000s
		pixels[8]  = (unsigned __int8)(temp[1]-48) + 1;
		pixels[7] = 1;
		pixels[6] = 1;
		// 1000s
		pixels[5]  = (unsigned __int8)(temp[2]-48) + 1;
		// 100s
		pixels[4]  = (unsigned __int8)(temp[3]-48) + 1;
		pixels[3] = 1;
		pixels[2] = 1;
		// 10s
		pixels[1]  = (unsigned __int8)(temp[4]-48) + 1;
		// 1s
		pixels[0]  = (unsigned __int8)(temp[5]-48) + 1;
		*/


		/*
		// Modulus 10 method
		// 01xx 45xx 89xx
		// units
		// n = (int)iFrameNumber % 10;
		n = (iFrameNumber % 10) + 1;
		pixels[0]  = (unsigned __int8)n;

		// tens
		n = (((iFrameNumber)/10) % 10) + 1;
		pixels[1]  = (unsigned __int8)n;
		pixels[2] = 1;
		pixels[3] = 1;
		// 100s
		n = (((iFrameNumber)/100) % 10) + 1;
		pixels[4]  = (unsigned __int8)n;
		// 1000s
		n = (((iFrameNumber)/1000) % 10) + 1;
		pixels[5]  = (unsigned __int8)n;
		pixels[6] = 1;
		pixels[7] = 1;
		// 10000s
		n = (((iFrameNumber)/10000) % 10) + 1;
		pixels[8]  = (unsigned __int8)n;
		// 100000s
		n = (((iFrameNumber)/100000) % 10) + 1;
		pixels[9]  = (unsigned __int8)n;
		pixels[10] = 1;
		pixels[11] = 1;
		*/


		/*
		//
		// Reduction method base 10
		//
		// http://embeddedgurus.com/stack-overflow/2011/02/efficient-c-tip-13-use-the-modulus-operator-with-caution/
		//
		pixels[11] = 0;
		pixels[10] = 0;
		// 100000s
		n = iFrameNumber/100000U;
		iFrameNumber -= n*100000U;
		pixels[9]  = (unsigned __int8)n;
		// 10000s
		n = iFrameNumber/10000U;
		iFrameNumber -= n*10000U;
		pixels[8]  = (unsigned __int8)n;
		pixels[7] = 0;
		pixels[6] = 0;
		// 1000s
		n = iFrameNumber/1000U;
		iFrameNumber -= n*1000U;
		pixels[5]  = (unsigned __int8)n;
		// 100s
		n = iFrameNumber/100U;
		iFrameNumber -= n*100U;
		pixels[4]  = (unsigned __int8)n;
		pixels[3] = 0;
		pixels[2] = 0;
		// 10s
		n = iFrameNumber/10U;
		iFrameNumber -= n*10U;
		pixels[1]  = (unsigned __int8)n;
		// 1s
		n = iFrameNumber;
		pixels[0]  = (unsigned __int8)n;
		*/


		/*
		printf("Write      : %2d %2d %2d %2d %2d %2d %2d %2d\n",
						pixels[ 0], pixels[4], pixels[ 8], pixels[12],
						pixels[16],	pixels[20], pixels[24],	pixels[28]);
		*/
		// Write 32 bytes (8 rgba pixels) to the texture
		glBindTexture(TextureTarget, TextureID);
		glTexSubImage2D(TextureTarget, 0, 0, 0, 16, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
		glBindTexture(TextureTarget, 0);

		/*
		printf("Write      : %2d %2d %2d %2d %2d %2d %2d %2d\n",
						pixels[ 0] >> 1, pixels[4] >> 1, pixels[ 8] >> 1, pixels[12] >> 1,
						pixels[16] >> 1,	pixels[20] >> 1, pixels[24] >> 1,	pixels[28] >> 1);
		*/

		return true;

} // end WriteDword



// READ A DWORD FROM AN OPENGL TEXTURE
bool spoutGLDXinterop::ReadDword(GLuint TextureID, GLuint TextureTarget, DWORD &dwValue, unsigned int width, unsigned int height, bool bInvert)
{
	UNREFERENCED_PARAMETER(width);

	unsigned __int32 iFrame = 0;
	
	unsigned __int8 pixels[64];

	if(TextureID == 0) {
		printf("ReadDword : NULL texture\n");
		return false;
	}

	memset((void *)pixels, 0, 64*sizeof(unsigned __int8));

	// Copy the first 8 rgba pixels (32 bytes) from the texture
	glBindTexture(TextureTarget, TextureID);
	if(bInvert)	glReadPixels(0, height-1, 8, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
	else glReadPixels(0, 0, 8, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
	glBindTexture(TextureTarget, 0);

	/*
	// Base 10
	iFrame = ((unsigned __int32)pixels[0])       + ((unsigned __int32)pixels[1])*10 +
			 ((unsigned __int32)pixels[4])*100   + ((unsigned __int32)pixels[5])*1000 +
			 ((unsigned __int32)pixels[8])*10000 + ((unsigned __int32)pixels[9])*100000;
	*/

	// Base 8
	// Units, 8, 64, 512, 4096, 32768, 262144, 2097152,
	// 16777216, 134217728, 1073741824, 8589934592, 68719476736, 549755813888, 4398046511104, 35184372088832
	// 0xxx 4xxx 8xxx 12xxx 16xxx 20xxx 24xxx 
	iFrame = ((unsigned __int32)pixels[ 0] >> 1)        + ((unsigned __int32)pixels[ 4] >> 1)*8 +
			 ((unsigned __int32)pixels[ 8] >> 1)*64     + ((unsigned __int32)pixels[12] >> 1)*512 +
			 ((unsigned __int32)pixels[16] >> 1)*4096   + ((unsigned __int32)pixels[20] >> 1)*32768 +
			 ((unsigned __int32)pixels[24] >> 1)*262144 + ((unsigned __int32)pixels[28] >> 1)*2097152;


	/*
	// Base 16
	// // Units, 16, 256, 4096, 65536, 524288, 4194304, 33554432
	// 00 01xx 04 05xx 08 09xx 12 13xx 
	iFrame = ((unsigned __int32)pixels[ 0]-1)         + ((unsigned __int32)pixels[ 1]-1)*16 +
			 ((unsigned __int32)pixels[ 4]-1)*256     + ((unsigned __int32)pixels[ 5]-1)*4096 +
			 ((unsigned __int32)pixels[ 8]-1)*65536   + ((unsigned __int32)pixels[ 9]-1)*524288 +
			 ((unsigned __int32)pixels[12]-1)*4194304 + ((unsigned __int32)pixels[13]-1)*33554432;
	*/

	if(iFrame != iLastFrame) {
		iLastFrame = iFrame;
		dwValue = (DWORD)iFrame;
		/*
		printf("Read Dword  : %d\n", iFrame);
		printf("Read       : %2d %2d %2d %2d %2d %2d %2d %2d\n",
						pixels[ 0] >> 1, pixels[ 4] >> 1, pixels[ 8] >> 1, pixels[12] >> 1,
						pixels[16] >> 1,	pixels[20] >> 1, pixels[24] >> 1,	pixels[28] >> 1);
		*/
		return true;
	}

	return false;

} // end ReadDword




// OpenGL context for situations where there is none
bool spoutGLDXinterop::InitOpenGL()
{
		HDC hdc = NULL;
		// HWND hwnd = NULL;
		HWND hwndButton = NULL;
		HGLRC hRc = NULL;

		HGLRC glContext = wglGetCurrentContext();

		if(glContext == NULL) {

			// We only need an OpenGL context with no render window because we don't draw to it
			// so create an invisible dummy button window. This is then independent from the host
			// program window (GetForegroundWindow). If SetPixelFormat has been called on the
			// host window it cannot be called again. This caused a problem in Mapio.
			// https://msdn.microsoft.com/en-us/library/windows/desktop/dd369049%28v=vs.85%29.aspx
			//
			if(!hwndButton || !IsWindow(hwndButton)) {
				hwndButton = CreateWindowA("BUTTON",
					            "SpoutOpenGL",
								WS_OVERLAPPEDWINDOW,
								0, 0, 32, 32,
								NULL, NULL, NULL, NULL);
			}

			if(!hwndButton) { 
				// printf("InitOpenGL error 1\n");
				MessageBoxA(NULL, "Error 1\n", "InitOpenGL", MB_OK);
				return false; 
			}

			hdc = GetDC(hwndButton);
			if(!hdc) { 
				// printf("InitOpenGL error 2\n"); 
				MessageBoxA(NULL, "Error 2\n", "InitOpenGL", MB_OK); 
				return false; 
			}
			
			PIXELFORMATDESCRIPTOR pfd;
			ZeroMemory( &pfd, sizeof( pfd ) );
			pfd.nSize = sizeof( pfd );
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;
			int iFormat = ChoosePixelFormat(hdc, &pfd);
			if(!iFormat) { 
				// printf("InitOpenGL error 3\n"); 
				MessageBoxA(NULL, "Error 3\n", "InitOpenGL", MB_OK);
				return false; 
			}

			if(!SetPixelFormat(hdc, iFormat, &pfd)) {
				DWORD dwError = GetLastError();
				// printf("InitOpenGL error 4 (Error %d (%x))\n", dwError, dwError); 
				// 2000 (0x7D0) The pixel format is invalid.
				// Caused by repeated call of  the SetPixelFormat function
				char temp[128];
				sprintf_s(temp, "InitOpenGL Error 4\nSetPixelFormat\nError %d (%x)", dwError, dwError);
				MessageBoxA(NULL, temp, "InitOpenGL", MB_OK); 
				return false; 
			}

			hRc = wglCreateContext(hdc);
			if(!hRc) { 
				// printf("InitOpenGL error 5\n"); 
				MessageBoxA(NULL, "Error 5\n", "InitOpenGL", MB_OK); 
				return false; 
			}

			wglMakeCurrent(hdc, hRc);
			if(wglGetCurrentContext() == NULL) {
				// printf("InitOpenGL error 6\n");
				MessageBoxA(NULL, "Error 6\n", "InitOpenGL", MB_OK);
				return false; 
			}
		}

		return true;
}


bool spoutGLDXinterop::CloseOpenGL()
{		
		
		HGLRC ctx = wglGetCurrentContext();
		if(ctx != NULL) {
			// printf("CloseOpenGL OK\n");
			// MessageBoxA(NULL, "Context OK\n", "CloseOpenGL", MB_OK);
			wglDeleteContext(ctx);
			return true;
		}

		// printf("CloseOpenGL no cotext\n");
		// MessageBoxA(NULL, "No context\n", "CloseOpenGL", MB_OK);

		return false;
}

