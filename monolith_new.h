#pragma once

#include <glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "weapons.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

enum class MovementState {
    Standing,
    Walking,
    Crouching,
    CrouchWalking
};

// Recoil system class
class RecoilSystem {
private:
    float m_sensitivity;
    float m_ads;
    float m_smoothness;
    float m_randomizationX;
    float m_randomizationY;
    float m_recoilControlX;
    float m_recoilControlY;
    bool m_isActive;
    bool m_useOldRecoil;
    bool m_controlHipfire;
    bool m_isScoped;

    // Internal state
    float m_currentRecoilX;
    float m_currentRecoilY;
    float m_targetRecoilX;
    float m_targetRecoilY;

    // Weapon state
    size_t m_currentWeaponIndex;
    size_t m_currentPatternIndex;
    float m_lastShotTime;
    MovementState m_movementState;
    float m_userFov;

    float getMovementMultiplier() const {
        switch (m_movementState) {
        case MovementState::CrouchWalking: return 1.8f;
        case MovementState::Crouching: return 2.0f;
        case MovementState::Walking: return 0.8f;
        default: return 1.0f;
        }
    }

    float calculateRecoilMultiplier() const {
        float scopeData = 1.0f; // TODO: Implement scope multiplier
        float muzzleBrake = 1.0f; // TODO: Implement muzzle brake multiplier
        float movement = getMovementMultiplier();
        return scopeData * muzzleBrake / (-0.03f * ((m_sensitivity * m_ads) * movement) * 3.0f * (m_userFov / 100.0f));
    }

public:
    RecoilSystem() :
        m_sensitivity(0.5f),
        m_ads(0.5f),
        m_smoothness(0.5f),
        m_randomizationX(0.5f),
        m_randomizationY(0.5f),
        m_recoilControlX(100.0f),
        m_recoilControlY(100.0f),
        m_isActive(false),
        m_useOldRecoil(false),
        m_controlHipfire(false),
        m_isScoped(false),
        m_currentRecoilX(0.0f),
        m_currentRecoilY(0.0f),
        m_targetRecoilX(0.0f),
        m_targetRecoilY(0.0f),
        m_currentWeaponIndex(0),
        m_currentPatternIndex(0),
        m_lastShotTime(0.0f),
        m_movementState(MovementState::Standing),
        m_userFov(90.0f)
    {
    }

    // Setters for parameters
    void setSensitivity(float value) { m_sensitivity = value; }
    void setADS(float value) { m_ads = value; }
    void setSmoothness(float value) { m_smoothness = value; }
    void setRandomizationX(float value) { m_randomizationX = value; }
    void setRandomizationY(float value) { m_randomizationY = value; }
    void setRecoilControlX(float value) { m_recoilControlX = value; }
    void setRecoilControlY(float value) { m_recoilControlY = value; }
    void setActive(bool value) { m_isActive = value; }
    void setOldRecoil(bool value) { m_useOldRecoil = value; }
    void setControlHipfire(bool value) { m_controlHipfire = value; }
    void setScoped(bool value) { m_isScoped = value; }
    void setWeapon(size_t index) {
        if (index < Weapons::ALL_WEAPONS.size()) {
            m_currentWeaponIndex = index;
            m_currentPatternIndex = 0;
        }
    }
    void setMovementState(MovementState state) { m_movementState = state; }
    void setFOV(float fov) { m_userFov = fov; }

    // Main recoil calculation function
    void calculateRecoil(float deltaTime, float* outX, float* outY);

    // Reset recoil state
    void reset() {
        m_currentRecoilX = 0.0f;
        m_currentRecoilY = 0.0f;
        m_targetRecoilX = 0.0f;
        m_targetRecoilY = 0.0f;
        m_currentPatternIndex = 0;
    }

    const std::vector<Weapon>& getWeapons() const { return Weapons::ALL_WEAPONS; }
    size_t getCurrentWeaponIndex() const { return m_currentWeaponIndex; }
};

// Function declarations
void CustomStyle();
void renderGui(RecoilSystem& recoilSystem);
