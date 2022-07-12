#pragma once
#include "../../source-sdk/classes/c_usercmd.hpp"

class i_input {
public:
    std::byte p1[12];
    bool isTrackIRAvailable;
    bool isMouseInitialized;
    bool isMouseActive;
    std::byte p2[154];
    bool isCameraInThirdPerson;
    bool cameraMovingWithMouse;
    vec3_t cameraOffset;

	c_usercmd* get_user_cmd(int slot, int sequence_num) {
		using fn = c_usercmd * (__thiscall*)(void*, int, int);
		return (*(fn**)this)[8](this, slot, sequence_num);
	}
};