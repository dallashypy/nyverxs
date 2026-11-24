// ---------------------------------------------------------------------------
// v7a offsets - iOS arm64 layout
//
// The supplied v7a struct was written against the 32-bit desktop dump. The
// iOS build is arm64, so every Il2Cpp object/array/string pointer is 8 bytes.
// This layer keeps the v7a field offsets but reads them through mach VM with
// 64-bit container layouts, and it probes several image bases because
// InitBase is a module-relative address whose slide differs between the app
// binary and UnityFramework.
//
// Every reader returns false instead of touching a bad pointer, so callers can
// fall back to the native SDK functions when the chain does not resolve.
// ---------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <cstring>
#include <cmath>
#include <string>
#include <chrono>

#include <mach/mach.h>
#include <mach/mach_traps.h>

#include "Vector3.h"
#include "Quaternion.h"

// Defined in Mem.h. Declared here so this header does not drag Mem.h's
// non-inline helpers into every translation unit.
extern uintptr_t getAbsoluteAddress(const char* fileName, uintptr_t address);

namespace v7a {

// ------------------------------------------------------------------ offsets
struct Off {
    static constexpr uint64_t InitBase            = 0xA342EFC;
    static constexpr uint32_t StaticClass         = 0x5C;
    static constexpr uint32_t CurrentMatch        = 0x50;
    static constexpr uint32_t MatchStatus         = 0x8C;
    static constexpr uint32_t LocalPlayer         = 0x94;
    static constexpr uint32_t DictionaryEntities  = 0x68;

    static constexpr uint32_t Player_IsDead       = 0x50;
    static constexpr uint32_t Player_Name         = 0x31C;
    static constexpr uint32_t Player_Data         = 0x48;
    static constexpr uint32_t ShadowState         = 0x1A60;
    static constexpr uint32_t HeadCollider        = 0x360;

    static constexpr uint32_t AvatarManager       = 0x504;
    static constexpr uint32_t Avatar              = 0xA8;
    static constexpr uint32_t Avatar_Data_IsTeam  = 0x59;
    static constexpr uint32_t Avatar_Data_IsBot   = 0x324;
    static constexpr uint32_t IsClientBot         = 0x324;
    static constexpr uint32_t Avatar_IsVisible    = 0x95;
    static constexpr uint32_t Avatar_Data         = 0x14;

    static constexpr uint32_t FollowCamera        = 0x494;
    static constexpr uint32_t MainCameraTransform = 0x28C;
    static constexpr uint32_t ViewMatrix          = 0xE8;

    static constexpr uint32_t AimRotation         = 0x440;
    static constexpr uint32_t IsFiring            = 0x58C;
    static constexpr uint32_t LocalPlayerAttributes = 0x500;
    static constexpr uint32_t Weapon              = 0x434;
    static constexpr uint32_t FireInterval        = 0x1C8;

    static constexpr uint32_t ListTransform       = 0x7B4;
    static constexpr uint32_t IsGirl              = 0x830;

    struct Bones {
        static constexpr uint32_t Head          = 0x49C;
        static constexpr uint32_t Hip           = 0x4A0;
        static constexpr uint32_t Neck          = 0x4A4;
        static constexpr uint32_t Pelvis        = 0x4AC;
        static constexpr uint32_t Root          = 0x4B0;
        static constexpr uint32_t RightShoulder = 0x4D4;
        static constexpr uint32_t LeftShoulder  = 0x4D0;
        static constexpr uint32_t RightElbow    = 0x4E0;
        static constexpr uint32_t LeftElbow     = 0x4E4;
        static constexpr uint32_t RightWrist    = 0x4D8;
        static constexpr uint32_t LeftWrist     = 0x4DC;
        static constexpr uint32_t RightAnkle    = 0x4BC;
        static constexpr uint32_t LeftAnkle     = 0x4B8;
        static constexpr uint32_t ToeRight      = 0x4C4;
        static constexpr uint32_t ToeLeft       = 0x4C0;
        static constexpr uint32_t RightHand     = 0x498;
        static constexpr uint32_t LeftHand      = 0x4C8;
    };
};

namespace layout {
// 64-bit Il2Cpp containers.
static constexpr uint32_t DictionaryValues        = 0x28; // Dictionary::values
static constexpr uint32_t MonoArrayLength         = 0x18; // monoArray::max_length
static constexpr uint32_t MonoArrayData           = 0x20; // monoArray::vector
static constexpr uint32_t MonoStringLength        = 0x10; // monoString::length
static constexpr uint32_t MonoStringData          = 0x14; // monoString::chars

// V31 fallback: hash entries embedded in the dictionary object.
static constexpr uint32_t V31Entries              = 0x18;
static constexpr uint32_t V31Count                = 0x20;
static constexpr uint32_t V31Data                 = 0x20;
static constexpr uint32_t V31Stride               = 0x18;
static constexpr uint32_t V31Value                = 0x10;

// Transform chain, identical to the reference implementation.
static constexpr uint32_t TransformObject         = 0x8;
static constexpr uint32_t TransformIndex          = 0x24;
static constexpr uint32_t TransformMatrix         = 0x20;
static constexpr uint32_t MatrixList              = 0x18;
static constexpr uint32_t MatrixIndices           = 0x1C;
static constexpr uint32_t MatrixStride            = 0x30;

static constexpr uint32_t PoolObj                 = 0x8;
static constexpr uint32_t PoolHealth              = 0x10;
static constexpr uint32_t XPose                   = 0x78; // shadow state pose
} // namespace layout

// ------------------------------------------------------------------- memory
inline bool valid_ptr(uint64_t p) {
    return p >= 0x100000000ULL && p < 0x00007FFFFFFFFFFFULL && (p & 7) == 0;
}

inline bool read(uint64_t address, void* out, size_t size) {
    if (!address || !out || size == 0 || size > 4096) return false;
    vm_size_t got = 0;
    kern_return_t kr = vm_read_overwrite(mach_task_self(),
                                         (vm_address_t)address,
                                         (vm_size_t)size,
                                         (vm_address_t)out,
                                         &got);
    return kr == KERN_SUCCESS && got == size;
}

template <typename T>
inline bool get(uint64_t address, T& out) {
    return read(address, &out, sizeof(T));
}

inline uint64_t ptr(uint64_t address) {
    uint64_t value = 0;
    return get(address, value) && valid_ptr(value) ? value : 0;
}

inline uint32_t u32(uint64_t address) {
    uint32_t value = 0;
    return get(address, value) ? value : 0;
}

inline int32_t i32(uint64_t address) {
    int32_t value = 0;
    return get(address, value) ? value : 0;
}

inline uint8_t u8(uint64_t address) {
    uint8_t value = 0;
    return get(address, value) ? value : 0;
}

inline float f32(uint64_t address) {
    float value = 0;
    return get(address, value) ? value : 0;
}

inline bool write(uint64_t address, const void* data, size_t size) {
    if (!valid_ptr(address) || !data || size == 0 || size > 256) return false;
    mach_port_t task = mach_task_self();
    kern_return_t kr = vm_protect(task, (vm_address_t)address, (vm_size_t)size,
                                  false, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
    if (kr != KERN_SUCCESS) return false;
    kr = vm_write(task, (vm_address_t)address,
                  (vm_offset_t)reinterpret_cast<uintptr_t>(data),
                  (mach_msg_type_number_t)size);
    // Targets here are heap objects (aim rotation, transforms): restore RW.
    // Leaving the page RX would make the next write fault.
    vm_protect(task, (vm_address_t)address, (vm_size_t)size,
               false, VM_PROT_READ | VM_PROT_WRITE);
    return kr == KERN_SUCCESS;
}

// --------------------------------------------------------------- base chain
struct Chain {
    uint64_t base = 0;
    uint64_t game = 0;
    uint64_t facade = 0;
    uint64_t klass = 0;
    uint64_t statics = 0;
};

inline uint64_t now_ms() {
    using namespace std::chrono;
    return (uint64_t)duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

inline void push_candidate(uint64_t* list, int& count, uint64_t value) {
    if (!value || count >= 8) return;
    for (int i = 0; i < count; i++)
        if (list[i] == value) return;
    list[count++] = value;
}

inline bool probe(uint64_t candidate, Chain& out) {
    if (!valid_ptr(candidate)) return false;

    uint64_t facade = ptr(candidate);
    if (!facade) return false;
    uint64_t klass = ptr(facade);
    if (!klass) return false;
    uint64_t statics = ptr(klass + Off::StaticClass);
    if (!statics) return false;
    uint64_t game = ptr(statics);
    if (!game) return false;

    uint64_t match = ptr(game + Off::CurrentMatch);
    if (match) {
        uint32_t status = u32(match + Off::MatchStatus);
        if (status > 5) return false;
        // A running match must expose a local player; anything else is a
        // lucky-looking garbage read, not the game singleton.
        if (status == 1 && !ptr(match + Off::LocalPlayer)) return false;
    }

    out.base = candidate;
    out.facade = facade;
    out.klass = klass;
    out.statics = statics;
    out.game = game;
    return true;
}

inline bool resolve(Chain& out) {
    uint64_t candidates[8];
    int count = 0;

    push_candidate(candidates, count, (uint64_t)getAbsoluteAddress(nullptr, Off::InitBase));
    push_candidate(candidates, count, (uint64_t)getAbsoluteAddress("UnityFramework", Off::InitBase));
    push_candidate(candidates, count, (uint64_t)getAbsoluteAddress(nullptr, 0x100000000ULL + Off::InitBase));
    push_candidate(candidates, count, (uint64_t)getAbsoluteAddress("UnityFramework", 0x100000000ULL + Off::InitBase));

    for (int i = 0; i < count; i++)
        if (probe(candidates[i], out)) return true;
    return false;
}

inline uint64_t game(uint64_t max_age_ms = 2000) {
    static Chain cached;
    static uint64_t resolved_at = 0;

    uint64_t now = now_ms();
    // Cache both outcomes: a failed probe must not run four vm_read chains on
    // every ESP/W2S call for the next two seconds.
    if (now - resolved_at < max_age_ms)
        return valid_ptr(cached.game) ? cached.game : 0;

    Chain fresh;
    if (!resolve(fresh)) {
        cached = Chain{};
        resolved_at = now;
        return 0;
    }
    cached = fresh;
    resolved_at = now;
    return cached.game;
}

// True when the v7a base chain resolved. Direct field reads (dead, health,
// firing, bones) are only trustworthy on a build where the chain matches;
// without this gate an offset that moved would silently override the native
// function with garbage.
inline bool ready() {
    return game() != 0;
}

inline uint64_t current_match() {
    uint64_t g = game();
    if (!g) return 0;
    uint64_t match = ptr(g + Off::CurrentMatch);
    if (!match) return 0;
    return u32(match + Off::MatchStatus) <= 5 ? match : 0;
}

inline uint64_t active_match() {
    uint64_t match = current_match();
    if (!match) return 0;
    return u32(match + Off::MatchStatus) == 1 ? match : 0;
}

inline uint64_t local_player(uint64_t match) {
    if (!valid_ptr(match)) return 0;
    return ptr(match + Off::LocalPlayer);
}

// ------------------------------------------------------------ entities loop
// Calls fn(player) for every entity. fn returns false to stop. Returns the
// number of entities visited, or -1 when no dictionary layout resolved.
template <typename Fn>
inline int for_each_entity(uint64_t g, Fn fn) {
    if (!valid_ptr(g)) return -1;
    uint64_t dict = ptr(g + Off::DictionaryEntities);
    if (!dict) return -1;

    // 64-bit Dictionary: values is a monoArray<void*>.
    uint64_t values = ptr(dict + layout::DictionaryValues);
    if (values) {
        int count = i32(values + layout::MonoArrayLength);
        if (count > 0 && count <= 1024) {
            int visited = 0;
            for (int i = 0; i < count; i++) {
                uint64_t player = ptr(values + layout::MonoArrayData + (uint64_t)i * 8);
                if (!player) continue;
                visited++;
                if (!fn(player)) break;
            }
            return visited;
        }
    }

    // V31 fallback: embedded hash entries.
    uint64_t entries = ptr(dict + layout::V31Entries);
    if (!entries) return -1;
    int count = i32(entries + layout::V31Count);
    if (count < 0 || count > 1024) return -1;

    int visited = 0;
    for (int i = 0; i < count; i++) {
        uint64_t slot = entries + layout::V31Data + (uint64_t)i * layout::V31Stride;
        int32_t hash = i32(slot);
        if (hash < 0) continue;
        uint64_t player = ptr(slot + layout::V31Value);
        if (!player) continue;
        visited++;
        if (!fn(player)) break;
    }
    return visited;
}

// -------------------------------------------------------------- player data
inline bool player_dead(uint64_t player, bool& out) {
    if (!valid_ptr(player)) return false;
    uint8_t value = 0;
    if (!get(player + Off::Player_IsDead, value)) return false;
    out = value != 0;
    return true;
}

// Knocked / bleeding out: ShadowState is a pointer, XPose lives inside it.
inline bool player_knocked(uint64_t player, bool& out) {
    if (!valid_ptr(player)) return false;
    uint64_t shadow = ptr(player + Off::ShadowState);
    if (!shadow) return false;
    out = i32(shadow + layout::XPose) == 8;
    return true;
}

inline bool player_team(uint64_t player, uint8_t& out) {
    if (!valid_ptr(player)) return false;
    uint64_t manager = ptr(player + Off::AvatarManager);
    if (!manager) return false;
    uint64_t avatar = ptr(manager + Off::Avatar);
    if (!avatar) return false;
    uint64_t data = ptr(avatar + Off::Avatar_Data);
    if (!data) return false;
    return get(data + Off::Avatar_Data_IsTeam, out);
}

inline bool player_visible(uint64_t player, bool& out) {
    if (!valid_ptr(player)) return false;
    uint64_t manager = ptr(player + Off::AvatarManager);
    if (!manager) return false;
    uint64_t avatar = ptr(manager + Off::Avatar);
    if (!avatar) return false;
    uint8_t value = 0;
    if (!get(avatar + Off::Avatar_IsVisible, value)) return false;
    out = value != 0;
    return true;
}

inline bool player_bot(uint64_t player, bool& out) {
    if (!valid_ptr(player)) return false;
    uint64_t manager = ptr(player + Off::AvatarManager);
    if (!manager) return false;
    uint64_t avatar = ptr(manager + Off::Avatar);
    if (!avatar) return false;
    uint64_t data = ptr(avatar + Off::Avatar_Data);
    if (!data) return false;
    uint8_t from_data = u8(data + Off::Avatar_Data_IsBot);
    uint8_t from_entity = u8(player + Off::IsClientBot);
    out = from_data != 0 || from_entity != 0;
    return true;
}

inline bool player_health(uint64_t player, int& out) {
    if (!valid_ptr(player)) return false;
    uint64_t pool = ptr(player + Off::Player_Data);
    if (!pool) return false;
    uint64_t object = ptr(pool + layout::PoolObj);
    if (!object) return false;
    uint64_t health = ptr(object + layout::PoolHealth);
    if (!health) return false;
    int16_t value = 0;
    if (!get(health + layout::PoolHealth, value)) return false;
    out = value;
    return true;
}

inline bool player_attributes(uint64_t player, uint64_t& out) {
    if (!valid_ptr(player)) return false;
    out = ptr(player + Off::LocalPlayerAttributes);
    return out != 0;
}

inline bool player_fire_interval(uint64_t player, float& out) {
    uint64_t attributes = 0;
    if (!player_attributes(player, attributes)) return false;
    return get(attributes + Off::FireInterval, out);
}

inline bool player_firing(uint64_t player, bool& out) {
    if (!valid_ptr(player)) return false;
    uint8_t value = 0;
    if (!get(player + Off::IsFiring, value)) return false;
    out = value != 0;
    return true;
}

inline bool player_aim_rotation(uint64_t player, Quaternion& out) {
    if (!valid_ptr(player)) return false;
    return get(player + Off::AimRotation, out);
}

inline bool set_player_aim_rotation(uint64_t player, const Quaternion& value) {
    if (!valid_ptr(player)) return false;
    return write(player + Off::AimRotation, &value, sizeof(value));
}

// --------------------------------------------------------------------- name
inline bool decode_name(uint64_t address, int length, std::string& out) {
    if (length <= 0 || length > 128) return false;
    uint16_t buffer[128];
    if (!read(address, buffer, (size_t)length * 2)) return false;

    out.clear();
    out.reserve((size_t)length);
    for (int i = 0; i < length; i++) {
        uint32_t c = buffer[i];
        if (c == 0) break;
        if (c < 0x80) {
            out.push_back((char)c);
        } else if (c < 0x800) {
            out.push_back((char)(0xC0 | (c >> 6)));
            out.push_back((char)(0x80 | (c & 0x3F)));
        } else {
            out.push_back((char)(0xE0 | (c >> 12)));
            out.push_back((char)(0x80 | ((c >> 6) & 0x3F)));
            out.push_back((char)(0x80 | (c & 0x3F)));
        }
    }
    return !out.empty();
}

inline bool player_name(uint64_t player, std::string& out) {
    if (!valid_ptr(player)) return false;
    uint64_t name = ptr(player + Off::Player_Name);
    if (!name) return false;

    // arm64 Il2CppString: length 0x10, UTF-16 0x14.
    int32_t length = i32(name + layout::MonoStringLength);
    if (decode_name(name + layout::MonoStringData, length, out)) return true;

    // 32-bit layout fallback (length 0x8, data 0xC).
    length = i32(name + 0x8);
    return decode_name(name + 0xC, length, out);
}

// ------------------------------------------------------------------- bones
inline bool transform_position(uint64_t transform, Vector3& out) {
    if (!valid_ptr(transform)) return false;

    uint64_t object = ptr(transform + layout::TransformObject);
    if (!object) return false;

    uint32_t index = u32(object + layout::TransformIndex);
    if (index > 65535) return false;

    uint64_t matrix = ptr(object + layout::TransformMatrix);
    if (!matrix) return false;

    uint64_t list = ptr(matrix + layout::MatrixList);
    uint64_t indices = ptr(matrix + layout::MatrixIndices);
    if (!list || !indices) return false;

    Vector3 position;
    if (!get(list + (uint64_t)index * layout::MatrixStride, position)) return false;
    if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z))
        return false;

    int32_t parent = -1;
    if (!get(indices + (uint64_t)index * 4, parent)) return false;

    struct Node { float p[4], q[4], s[4]; };
    static_assert(sizeof(Node) == 0x30, "transform node must be 0x30");

    for (int depth = 0; parent >= 0; depth++) {
        if (depth >= 128 || parent > 65535) return false;

        Node node{};
        if (!get(list + (uint32_t)parent * layout::MatrixStride, node)) return false;
        for (float f : node.q)
            if (!std::isfinite(f)) return false;

        float x = position.x * node.s[0];
        float y = position.y * node.s[1];
        float z = position.z * node.s[2];
        float qx = node.q[0], qy = node.q[1], qz = node.q[2], qw = node.q[3];

        position.x = node.p[0] + x + x * (-2 * qy * qy - 2 * qz * qz)
                     + y * (-2 * qw * qz + 2 * qy * qx) + z * (2 * qz * qx + 2 * qw * qy);
        position.y = node.p[1] + y + x * (2 * qx * qy + 2 * qw * qz)
                     + y * (-2 * qz * qz - 2 * qx * qx) + z * (-2 * qw * qx + 2 * qz * qy);
        position.z = node.p[2] + z + x * (-2 * qw * qy + 2 * qx * qz)
                     + y * (2 * qz * qy + 2 * qw * qx) + z * (-2 * qx * qx - 2 * qy * qy);

        if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z))
            return false;
        if (!get(indices + (uint32_t)parent * 4, parent)) return false;
    }

    out = position;
    return true;
}

inline bool node_position(uint64_t node, Vector3& out) {
    uint64_t transform = ptr(node + layout::TransformObject);
    return transform && transform_position(transform, out);
}

inline bool bone_position(uint64_t player, uint32_t offset, Vector3& out) {
    if (!valid_ptr(player)) return false;
    uint64_t node = ptr(player + offset);
    return node && node_position(node, out);
}

inline bool player_root(uint64_t player, Vector3& out) {
    return bone_position(player, Off::Bones::Root, out);
}

// -------------------------------------------------------------- camera/W2S
inline bool usable_matrix(const float* m) {
    bool any = false;
    for (int i = 0; i < 16; i++) {
        if (!std::isfinite(m[i])) return false;
        if (m[i] != 0) any = true;
    }
    return any && (std::fabs(m[3]) + std::fabs(m[7]) + std::fabs(m[11]) + std::fabs(m[15]) > 1e-4f);
}

inline bool view_matrix(uint64_t holder, float out[16]) {
    if (!valid_ptr(holder)) return false;
    uint64_t follow = ptr(holder + Off::FollowCamera);
    if (!follow) return false;
    uint64_t camera = ptr(follow + 0x18);
    if (!camera) return false;
    uint64_t base = ptr(camera + 0x8);
    if (!base) return false;
    if (!read(base + Off::ViewMatrix, out, sizeof(float) * 16)) return false;
    return usable_matrix(out);
}

inline bool camera_position(uint64_t holder, Vector3& out) {
    if (!valid_ptr(holder)) return false;
    uint64_t transform = ptr(holder + Off::MainCameraTransform);
    return transform && transform_position(transform, out);
}

inline bool project(const float* m, const Vector3& point, int width, int height, Vector3& out) {
    if (width <= 0 || height <= 0) return false;
    if (!std::isfinite(point.x) || !std::isfinite(point.y) || !std::isfinite(point.z))
        return false;

    float clipX = point.x * m[0] + point.y * m[4] + point.z * m[8] + m[12];
    float clipY = point.x * m[1] + point.y * m[5] + point.z * m[9] + m[13];
    float clipW = point.x * m[3] + point.y * m[7] + point.z * m[11] + m[15];

    if (!std::isfinite(clipW) || clipW < 0.01f) return false;

    out.x = width * 0.5f * (1.0f + clipX / clipW);
    out.y = height * 0.5f * (1.0f - clipY / clipW);
    out.z = clipW;
    return std::isfinite(out.x) && std::isfinite(out.y) &&
           std::fabs(out.x) < width * 100.0f && std::fabs(out.y) < height * 100.0f;
}

} // namespace v7a
