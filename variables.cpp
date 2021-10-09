#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <iostream>
#include <WinUser.h>
#include <Windows.h>
#include <future>

bool triggeractive = false;
uintptr_t buffer;
bool espactive = true;
bool radaractive = false;
bool gdibox = false;
bool bhop = false;
bool noflash = false;
bool norecoil = false;
int menu_code = -100;

const int screenX = GetSystemMetrics(SM_CXSCREEN);
const int screenY = GetSystemMetrics(SM_CYSCREEN);

#define EnemyPen 0x000000FF

HDC hdc = NULL;
HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);
HANDLE hProc = NULL;
DWORD pId = NULL;
HWND hWnd = NULL;

DWORD localPlayer = NULL;
uintptr_t moduleBase = NULL;
uintptr_t engineModule = NULL;


struct glowStructEnemy {
	float red = 1.f;
	float green = 0.f;
	float blue = 0.f;
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}glowEnm;

struct ClrRenderer {
	BYTE green, red, blue;
}ClrRender;

struct glowStructLocal {
	float red = 0.f;
	float green = 1.f;
	float blue = 0.f;
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}glowLocal;
#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif


struct view_matrix_t {
	float* operator[](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

struct Vector3 {
	float x, y, z;

	Vector3 operator+(Vector3 d) {
		return{ x + d.x, y + d.y, z + d.z };
	}
	Vector3 operator-(Vector3 d) {
		return{ x - d.x, y - d.y, z - d.z };
	}

	Vector3 operator*(float d) {
		return { x * d, y * d, z * d };
	}

	void Normalize() {
		while (y < -180) {
			y += 360;
		} while (y > 180) {
			y -= 360;
		}
		if (x > 89) {
			x = 89;
		}
		if (x < -89) {
			x = -89;
		}
	}
};