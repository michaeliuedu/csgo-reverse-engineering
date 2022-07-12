#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class glow_object_definition_t {
public:
	void set(float r, float g, float b, float a) {
		R = r; G = g; B = b; A = a;
		m_bRenderWhenOccluded = true;
		m_bRenderWhenUnoccluded = false;
		flBloom = .5f;
	}
	bool unused() {
		return m_nNextFreeSlot != -2;
	}

	int32_t m_nNextFreeSlot; //0x0000
	void* pEntity; //0x0004
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

	//void* entity;
	//vec3_t color;
	//float alpha;
	//char unknown0[8];
	//float bloom_amount;
	//char unknown1[4];
	//bool render_when_occluded;
	//bool render_when_unoccluded;
	//bool full_bloom_render;
	//char unknown2[13];
	//int next_free_slot;
};

class glow_manager_t {
public:
	glow_object_definition_t* objects;
	char pad[8];
	int size;
};