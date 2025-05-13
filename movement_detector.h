#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

class MovementDetector {
public:
    static MovementState DetectMovement() {
#ifdef _WIN32
        bool isWalking = (GetAsyncKeyState('W') & 0x8000) || 
                        (GetAsyncKeyState('A') & 0x8000) || 
                        (GetAsyncKeyState('S') & 0x8000) || 
                        (GetAsyncKeyState('D') & 0x8000);
        bool isCrouching = (GetAsyncKeyState(VK_CONTROL) & 0x8000) || 
                          (GetAsyncKeyState(VK_LCONTROL) & 0x8000);

        if (isCrouching && isWalking) {
            return MovementState::CrouchWalking;
        }
        else if (isCrouching) {
            return MovementState::Crouching;
        }
        else if (isWalking) {
            return MovementState::Walking;
        }
        return MovementState::Standing;
#else
        // Stub for non-Windows platforms
        return MovementState::Standing;
#endif
    }

    static bool IsInsertPressed() {
#ifdef _WIN32
        static bool wasPressed = false;
        bool isPressed = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
        bool result = isPressed && !wasPressed;
        wasPressed = isPressed;
        return result;
#else
        // Stub for non-Windows platforms
        return false;
#endif
    }
};
