#include "monolith.h"
#include "weapons.h"
#include <cmath>
#include <random>
#include <chrono>

// Linear interpolation function
inline float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

RecoilSystem::RecoilSystem() :
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
{}

void RecoilSystem::setWeapon(size_t index) {
    if (index < Weapons::ALL_WEAPONS.size()) {
        m_currentWeaponIndex = index;
        m_currentPatternIndex = 0;
    }
}

const std::vector<Weapon>& RecoilSystem::getWeapons() const {
    return Weapons::ALL_WEAPONS;
}

void RecoilSystem::calculateRecoil(float deltaTime, float* outX, float* outY) {
    if (!m_isActive) {
        *outX = 0.0f;
        *outY = 0.0f;
        return;
    }

    const Weapon& currentWeapon = Weapons::ALL_WEAPONS[m_currentWeaponIndex];
    const std::vector<Vec2>& pattern = currentWeapon.getPattern();
    
    // Check if enough time has passed for next shot
    auto now = std::chrono::steady_clock::now();
    float currentTime = std::chrono::duration<float>(now.time_since_epoch()).count();
    float timeSinceLastShot = currentTime - m_lastShotTime;

    if (timeSinceLastShot >= currentWeapon.getDelay() / 1000.0f) {
        // Get current pattern point
        if (m_currentPatternIndex < pattern.size()) {
            Vec2 recoilPoint = pattern[m_currentPatternIndex];
            
            // Calculate recoil multiplier
            float multiplier = calculateRecoilMultiplier();
            
            // Apply recoil control
            float controlledX = recoilPoint.x * (m_recoilControlX * 0.01f);
            float controlledY = recoilPoint.y * (m_recoilControlY * 0.01f);

            // Add randomization
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

            float randX = dis(gen) * m_randomizationX;
            float randY = dis(gen) * m_randomizationY;

            // Calculate target recoil values
            m_targetRecoilX = (controlledX + randX) * multiplier;
            m_targetRecoilY = (controlledY + randY) * multiplier;

            // Apply smoothing
            m_currentRecoilX = lerp(m_currentRecoilX, m_targetRecoilX, 1.0f - m_smoothness);
            m_currentRecoilY = lerp(m_currentRecoilY, m_targetRecoilY, 1.0f - m_smoothness);

            // Apply ADS modifier
            if (!m_controlHipfire && m_isScoped) {
                m_currentRecoilX *= m_ads;
                m_currentRecoilY *= m_ads;
            }

            // Apply old recoil modifier
            if (m_useOldRecoil) {
                m_currentRecoilX *= 1.5f;
                m_currentRecoilY *= 1.5f;
            }

            // Update pattern index
            m_currentPatternIndex = (m_currentPatternIndex + 1) % pattern.size();
            m_lastShotTime = currentTime;
        }
    }

    *outX = m_currentRecoilX;
    *outY = m_currentRecoilY;
}
