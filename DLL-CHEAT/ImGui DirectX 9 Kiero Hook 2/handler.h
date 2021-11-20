#pragma once
#include <fstream>
#include "variables.h"
#pragma warning(disable:4996)

#define EnemyPen 0x000000FF
HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);

namespace Handler
{

	template<typename T> void WPM(SIZE_T address, T buffer) {
		WriteProcessMemory(hProc, (LPVOID)address, &buffer, sizeof(buffer), NULL);
	}

	template<typename T> T RPM(SIZE_T address) {
		T buffer;
		ReadProcessMemory(hProc, (LPCVOID)address, &buffer, sizeof(T), NULL);
		return buffer;
	}

	template <class dataType>
	void writeProcessMemory(dataType valToWrite, DWORD addressToWrite)
	{
		WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
	}

	template <class dataType>
	dataType readProcessMemory(DWORD addressToRead) {
		dataType rpmBuffer;
		ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);
		return rpmBuffer;
	}
	


	namespace Util
	{
		namespace Rendering
		{

			void DrawFilledRect(int x, int y, int w, int h)
			{
				RECT rect = { x, y, x + w, y + h };
				FillRect(hdc, &rect, EnemyBrush);
			}


			void DrawBorderBox(int x, int y, int w, int h, int thickness)
			{
				DrawFilledRect(x, y, w, thickness); //Top horiz line
				DrawFilledRect(x, y, thickness, h); //Left vertical line
				DrawFilledRect((x + w), y, thickness, h); //right vertical line
				DrawFilledRect(x, y + h, w + thickness, thickness); //bottom horiz line

			}

			void DrawLine(float StartX, float StartY, float EndX, float EndY)
			{
				int a, b = 0;
				HPEN hOPen;
				HPEN hNPen = CreatePen(PS_SOLID, 3, EnemyPen);// penstyle, width, color
				hOPen = (HPEN)SelectObject(hdc, hNPen);
				MoveToEx(hdc, StartX, StartY, NULL); //start
				a = LineTo(hdc, EndX, EndY); //end
				DeleteObject(SelectObject(hdc, hOPen));
			}


		}

		uintptr_t geteachplayer(int offset) {

			return Handler::readProcessMemory<int>(moduleBase + dwEntityList + offset * 0x10);
		}

		int getteam(uintptr_t player) {
			return  Handler::readProcessMemory<int>(player + m_iTeamNum);
		}

		int getcrosshairID(uintptr_t player) {
			return  Handler::readProcessMemory<int>(player + m_iCrosshairId);
		}
	}

	namespace Components
	{
		//Glow depreciated due to updates within CSGO
		struct updatedGlowStruct {
			int32_t m_nNextFreeSlot; //0x0000
			uint32_t pEntity; //0x0004
			float R; //0x0008
			float G; //0x000C
			float B; //0x0010
			float A; //0x0014
			char pad_0018[4]; //0x0018
			float flUnk; //0x001C
			float flBloom; //0x0020
			float localplayeriszeropoint3; //0x0024
			bool m_bRenderWhenOccluded; //0x0028
			bool m_bRenderWhenUnoccluded; //0x0029
			bool m_bFullBloomRender; //0x002A
			char pad_002B[1]; //0x002B
			int32_t m_nFullBloomStencilTestValue; //0x002C
			int32_t iUnk; //0x0030
			int32_t m_nSplitScreenSlot; //0x0034
		};


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

			Handler::Components::Vector3 WorldToScreen(const Handler::Components::Vector3 pos, Handler::Components::view_matrix_t matrix) {
				float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
				float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

				float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

				float inv_w = 1.f / w;
				_x *= inv_w;
				_y *= inv_w;

				float x = screenX * .5f;
				float y = screenY * .5f;

				x += 0.5f * _x * screenX + 0.5f;
				y -= 0.5f * _y * screenY + 0.5f;

				return { x,y,w };
			}

		};
	}
	


}