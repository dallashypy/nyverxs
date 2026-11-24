#pragma once

#import "vinhtran.hpp"
#import "loading.hxx"
#include "OffsetsV7a.h"
#include <fstream>
#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include <chrono>
extern ImVec4 espv; // visível
extern ImVec4 espi; // invisível/dead
extern ImVec4 nameColor;
extern ImVec4 distanceColor;
extern bool istelekill;
extern bool isfly;



static void Transform_INTERNAL_SetPosition(void *transform, Vvector3 in) {
    void (*_SetPos)(void *, Vvector3) = (void (*)(void *, Vvector3))getRealOffset(oxo("0x105899B54"));
    _SetPos(transform, in);
}
typedef void* DamageInfo2_o;
inline float get_distance(Vector3 a, Vector3 b) {
float dx = a.x - b.x;
 float dy = a.y - b.y;
float dz = a.z - b.z;
 return sqrtf(dx * dx + dy * dy + dz * dz);
}

bool AimKill = false;
bool SowDamage = true;
bool autochangeweapon = false;
bool POFFNNMOOBM = false;
int GDKLMFLNNGM = 0;

namespace Save{
    void* DamageInfo;
    clock_t AimDelay;
    int AimFPS = (1000000 / 15);
}
struct PlayerID_MKFEKBKJCKE_o;

struct PlayerID {
    uint32_t NBPDJAAAFBH;
    uint32_t JEDDPHIHGKL;
    uint8_t IOICFFEKAIL;
    uint8_t PHAFNFOFFDB;
    uint64_t BNFAIDHEHOM;
};

struct COW_GamePlay_IHAAMHPPLMG_o {
 uint32_t NBPDJAAAFBH;
 uint32_t JEDDPHIHGKL;
 uint8_t IOICFFEKAIL;
 uint8_t PHAFNFOFFDB;
 uint64_t BNFAIDHEHOM;
};
int32_t Player_TakeDamage(void *Player, int32_t p_damage, COW_GamePlay_IHAAMHPPLMG_o PlayerID, DamageInfo2_o *DamageInfo, int32_t WeaponDataID, Vector3 FirePos, Vector3 TargetPos, monoList<float *> *CheckParams, void *p_idk1, int32_t p_idk2) {
    return ((int32_t (*)(void *, int32_t, COW_GamePlay_IHAAMHPPLMG_o, DamageInfo2_o *, int32_t, Vector3, Vector3, monoList<float *> *, void *, uint32_t))getRealOffset(0x104AC7E08))(Player, p_damage, PlayerID, DamageInfo, WeaponDataID, FirePos, TargetPos, CheckParams, p_idk1, p_idk2);
}
static void SwapWeapon(void *player, int POFFNNMOOBM, bool GDKLMFLNNGM) {
    void (*_SwapWeapon)(void *player, int POFFNNMOOBM, bool GDKLMFLNNGM) = (void (*)(void *, int, bool))getRealOffset(ENCRYPTOFFSET("0x104AF3484"));
    _SwapWeapon(player, POFFNNMOOBM, GDKLMFLNNGM);
}


static int32_t TakeDamage(void *_this, int32_t KOCMLPLOILD,COW_GamePlay_IHAAMHPPLMG_o HLJDHPGGODB, void* JIIJIFKKCCB, int32_t BOEIBGAABDL, Vector3 NJMFBKNHMBP, Vector3 DOBOBMFMKBJ, monoList<float *> *NBKBEBFNDBE, void* damagerWeaponDynamicInfo, uint32_t damagerVehicleID) {
    return ((int32_t (*)(void*, int32_t, COW_GamePlay_IHAAMHPPLMG_o, void*, int32_t, Vector3, Vector3, monoList<float *> *, void*, uint32_t))getRealOffset(0x104AC7E08))(_this, KOCMLPLOILD, HLJDHPGGODB, JIIJIFKKCCB, BOEIBGAABDL, NJMFBKNHMBP, DOBOBMFMKBJ, NBKBEBFNDBE, damagerWeaponDynamicInfo, damagerVehicleID);//public override 
}

monoList<float *> *LCLHHHKFCFP(void *Weapon,void *CAGCICACKCF,void *HFBDJJDICLN,bool LDGHPOPPPNL,void* DamageInfo)
{
    return ((monoList<float *> * (*)(void*,void*,void*,bool,void*))getRealOffset(0x1003C2E44))(Weapon,CAGCICACKCF,HFBDJJDICLN,LDGHPOPPPNL,DamageInfo);//
}
static int GetDamage(void *pthis)
{
    return ((int (*)(void *))getRealOffset(0x10039D4F8))(pthis);//pu
}

static void StartFiring(void *Player, void *WeaponOnHand) {
    void (*_StartFiring)(void *, void *) = (void (*)(void *, void *))getRealOffset(0x104AF289C);//public 
    return _StartFiring(Player, WeaponOnHand);
}
static void StopFire1(void* Player,void* WeaponOnHand){
    void(*_StopFire1)(void*,void*) = (void(*)(void*,void*))getRealOffset(0x104AC9ABC);//public overrid
}
static void *GetWeaponOnHand1(void *local) {
    void *(*_GetWeaponOnHand1)(void *local) = (void *(*)(void *))getRealOffset(0x10492FBA4);//public We
    return _GetWeaponOnHand1(local);
}

static int GetWeapon(void* enemy) {
    int (*GetWeapon)(void *player) = (int(*)(void *))getRealOffset(0x10302C5DC);// public u

    return GetWeapon(enemy);
}

COW_GamePlay_IHAAMHPPLMG_o GetplayerID(void *_this)
{
    return ((COW_GamePlay_IHAAMHPPLMG_o (*)(void *))getRealOffset(0x104914BB0))(_this);

}
void StartWholeBodyFiring(void* player,void* WeaponOnHand){

    void(*StartWholeBodyFiring)(void*,void*) = (void(*)(void*,void*))getRealOffset(ENCRYPTOFFSET("0x104AF2E18"));

    return StartWholeBodyFiring(player,WeaponOnHand);
}

void *get_HeadCollider(void *pthis)
{
    return ((void* (*)(void *))getRealOffset(0x104933544))(pthis);
}

void *get_gameObject(void *Pthis)
{
    return ((void* (*)(void *))getRealOffset(0x105850AF8))(Pthis);//public
}

void *GKHECDLGAJA(void *pthis, void* a1)
{
    return ((void* (*)(void *,void *))getRealOffset(0x10499485C))(pthis,a1);
}
struct Vars_t
{
    bool UpPlayerOne = false;
    bool Enable = {};
    bool AimbotEnable = {};
    bool Aimbot = {};
    bool ShowGhostButton = false;
    bool EnableGhost = false;
    float AimFov = {};
    int AimCheck = {};
    int AimType = {};
    int AimWhen = {};
    bool isAimFov = {};
    int AimHitbox = 0; // 0: Head, 1: Neck, 2: Body
    const char* aimHitboxes[3] = { " Cabeça", " Pescoço", " Corpo" };
    const char* dir[4] = { " Automático", " Disparo", " Escopo", " Disparo + Escopo" };
    bool lines = {};
    bool Box = {};
    bool Outline = {};
    bool Name = {};
    bool Health = {};
    bool Distance = {};
    bool fovaimglow = {};
    bool circlepos = {};
    bool skeleton = {};
    bool OOF = {};
    bool counts = {};
    ImVec4 boxColor = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    float AimSpeed = 9999.0f;
    bool VisibleCheck = false;
    bool IgnoreKnocked = false;
    float RenderDist = 200.0f;
    int LineStyle = 0; // 0 = Cima, 1 = Baixo
    int NameStyle = 0; // 0 = Com Sombra, 1 = Sem Sombra
    int BoxStyle = 0;  // 0 = Normal, 1 = Cornered, 2 = Redondo

    // 🔥 Novo: posição da vida
    int HealthPos = 0; // 0 = Esquerda, 1 = Direita
    const char* healthPosOptions[2] = { " Esquerda", " Direita" };
} Vars;


class game_sdk_t
{
public:
    void init();
    int (*GetHp)(void* player);
    void* (*Curent_Match)();
    void* (*GetLocalPlayer)(void* Game);
    void* (*GetHeadPositions)(void* player);
    Vector3(*get_position)(void* player);
    void* (*Component_GetTransform)(void* player);
    void* (*get_camera)();
    Vector3(*WorldToScreenPoint)(void*, Vector3);
    bool (*get_isVisible)(void* player);
    bool (*get_isLocalTeam)(void* player);
    bool (*get_IsDieing)(void* player);
    int (*get_MaxHP)(void* player);
    Vector3(*GetForward)(void* player);
    void (*set_aim)(void* player, Quaternion look);
    bool (*get_IsSighting)(void* player);
    bool (*get_IsFiring)(void* player);
    monoString* (*name)(void* player);
    void* (*_GetHeadPositions)(void*);
    void* (*_newHipMods)(void*);
    void* (*_GetLeftAnkleTF)(void*);
    void* (*_GetRightAnkleTF)(void*);
    void* (*_GetLeftToeTF)(void*);
    void* (*_GetRightToeTF)(void*);
    void* (*_getLeftHandTF)(void*);
    void* (*_getRightHandTF)(void*);
    void* (*_getLeftForeArmTF)(void*);
    void* (*_getRightForeArmTF)(void*);
};

game_sdk_t* game_sdk = new game_sdk_t();

void game_sdk_t::init()
{
    this->GetHp = (int (*)(void*))getRealOffset(oxo("0x10499719C")); // public int get_CurHP() { }
    this->Curent_Match = (void* (*)())getRealOffset(oxo("0x100FE52DC"));   // public static NFJPHMKKEBF CurrentMatch() { }
    this->GetLocalPlayer = (void* (*)(void*))getRealOffset(oxo("0x103B0BA8C"));  // private static Player GetLocalPlayer() { }
    this->GetHeadPositions = (void* (*)(void*))getRealOffset(oxo("0x1049AF530"));  // public virtual Transform GetHeadTF() { }
    this->get_position = (Vector3(*)(void*))getRealOffset(oxo("0x105899A80"));  // public Vector3 get_position() { }
    this->Component_GetTransform = (void* (*)(void*))getRealOffset(oxo("0x105850A8C"));   //public Transform get_transform() { }
    this->get_camera = (void* (*)())getRealOffset(oxo("0x10584E620"));   //public static Camera get_main() { }
    this->WorldToScreenPoint = (Vector3(*)(void*, Vector3))getRealOffset(oxo("0x10584DF68"));    //public Vector3 WorldToViewportPoint(Vector3 position) { }
    this->get_isVisible = (bool (*)(void*))getRealOffset(oxo("0x104938678"));    //public override bool IsVisible() { }
    this->get_isLocalTeam = (bool (*)(void*))getRealOffset(oxo("0x10494E65C"));   //public virtual bool IsLocalTeammate(bool isCheckSocial3pEffect = False) { }
    this->get_IsDieing = (bool (*)(void*))getRealOffset(oxo("0x10491076C"));    //public bool get_IsDieing() { }
    this->get_MaxHP = (int (*)(void*))getRealOffset(oxo("0x104997244"));     //public int get_MaxHP() { }
    this->GetForward = (Vector3(*)(void*))getRealOffset(oxo("0x10589A430"));  //public Vector3 get_forward() { }
    this->set_aim = (void (*)(void*, Quaternion))getRealOffset(oxo("0x104934BE0"));   //public void SetAimRotation(Quaternion JGOGIAFGCFC) { }
    this->get_IsSighting = (bool (*)(void*))getRealOffset(oxo("0x104929520"));  //public bool get_IsSighting() { }
    this->get_IsFiring = (bool (*)(void*))getRealOffset(oxo("0x10492946C"));   //public bool IsFiring() { }
    this->name = (monoString * (*)(void* player)) getRealOffset(oxo("0x1049302E8"));   //public string get_NickName() { }
    // skeleton
    this->_GetHeadPositions = (void* (*)(void*))getRealOffset(oxo("0x1049AF530"));  // GetHeadTF
    this->_newHipMods = (void* (*)(void*))getRealOffset(oxo("0x1049AF680"));        // GetHipTF
    this->_GetLeftAnkleTF = (void* (*)(void*))getRealOffset(oxo("0x1049AF9B4"));    // GetLeftAnkleTF
    this->_GetRightAnkleTF = (void* (*)(void*))getRealOffset(oxo("0x1049AFA58"));   // GetRightAnkleTF
    this->_GetLeftToeTF = (void* (*)(void*))getRealOffset(oxo("0x1049AFAFC"));      // GetLeftToeTF
    this->_GetRightToeTF = (void* (*)(void*))getRealOffset(oxo("0x1049AFBA0"));     // GetRightToeTF
    this->_getLeftHandTF = (void* (*)(void*))getRealOffset(oxo("0x104933F3C"));     // get_LeftHandTF
    this->_getRightHandTF = (void* (*)(void*))getRealOffset(oxo("0x104933FE8"));    // get_RightHandTF
    this->_getLeftForeArmTF = (void* (*)(void*))getRealOffset(oxo("0x10493408C"));  // get_LeftForeArmTF
    this->_getRightForeArmTF = (void* (*)(void*))getRealOffset(oxo("0x104934130")); // get_RightForeArmTF
}


namespace Camera$$WorldToScreen {
    // v7a matrix, cached for 100 ms. The entity pass and the draw pass hit
    // this many times per frame; the cache keeps one read per frame group.
    static bool V7Matrix(float out[16]) {
        static float cached[16] = {};
        static uint64_t cachedAt = 0;
        static uint64_t failedAt = 0;

        uint64_t now = v7a::now_ms();
        if (cachedAt && now - cachedAt < 100) {
            memcpy(out, cached, sizeof(cached));
            return true;
        }
        // Negative cache: the draw pass calls this per bone. Without it a build
        // where the v7a chain never resolves would pay the probe on every call.
        if (failedAt && now - failedAt < 50)
            return false;

        uint64_t match = v7a::active_match();
        uint64_t local = v7a::local_player(match);
        if (!local) {
            failedAt = now;
            return false;
        }

        float matrix[16];
        if (!v7a::view_matrix(local, matrix)) {
            failedAt = now;
            return false;
        }

        memcpy(cached, matrix, sizeof(cached));
        cachedAt = now;
        failedAt = 0;
        memcpy(out, matrix, sizeof(matrix));
        return true;
    }

    ImVec2 Regular(Vector3 pos) {
        int ScreenWidth = ImGui::GetIO().DisplaySize.x;
        int ScreenHeight = ImGui::GetIO().DisplaySize.y;

        float matrix[16];
        if (V7Matrix(matrix)) {
            Vector3 projected;
            if (v7a::project(matrix, pos, ScreenWidth, ScreenHeight, projected))
                return { projected.x, projected.y };
        }

        auto cam = game_sdk->get_camera();
        if (!cam)
            return { 0, 0 };
        Vector3 worldPoint = game_sdk->WorldToScreenPoint(cam, pos);
        Vector3 location;
        location.x = ScreenWidth * worldPoint.x;
        location.y = ScreenHeight - worldPoint.y * ScreenHeight;
        location.z = worldPoint.z;
        return { location.x, location.y };
    }

    ImVec2 Checker(Vector3 pos, bool& checker) {
        int ScreenWidth = ImGui::GetIO().DisplaySize.x;
        int ScreenHeight = ImGui::GetIO().DisplaySize.y;

        float matrix[16];
        if (V7Matrix(matrix)) {
            Vector3 projected;
            if (v7a::project(matrix, pos, ScreenWidth, ScreenHeight, projected)) {
                checker = projected.z > 1;
                return { projected.x, projected.y };
            }
        }

        auto cam = game_sdk->get_camera();
        if (!cam)
            return { 0, 0 };
        Vector3 worldPoint = game_sdk->WorldToScreenPoint(cam, pos);
        Vector3 location;
        location.x = ScreenWidth * worldPoint.x;
        location.y = ScreenHeight - worldPoint.y * ScreenHeight;
        location.z = worldPoint.z;
        checker = location.z > 1;
        return { location.x, location.y };
    }
}

static uint32_t V7BoneOffset(void* (*transformGetter)(void*)) {
    if (transformGetter == game_sdk->_GetHeadPositions) return v7a::Off::Bones::Head;
    if (transformGetter == game_sdk->_newHipMods)       return v7a::Off::Bones::Hip;
    if (transformGetter == game_sdk->_GetLeftAnkleTF)    return v7a::Off::Bones::LeftAnkle;
    if (transformGetter == game_sdk->_GetRightAnkleTF)   return v7a::Off::Bones::RightAnkle;
    if (transformGetter == game_sdk->_GetLeftToeTF)      return v7a::Off::Bones::ToeLeft;
    if (transformGetter == game_sdk->_GetRightToeTF)     return v7a::Off::Bones::ToeRight;
    if (transformGetter == game_sdk->_getLeftHandTF)     return v7a::Off::Bones::LeftHand;
    if (transformGetter == game_sdk->_getRightHandTF)    return v7a::Off::Bones::RightHand;
    return 0;
}

Vector3 GetBonePosition(void* player, void* (*transformGetter)(void*)) {
    if (!player || !transformGetter)
        return Vector3();

    uint32_t offset = V7BoneOffset(transformGetter);
    if (offset && v7a::ready()) {
        Vector3 position;
        if (v7a::bone_position((uint64_t)player, offset, position) &&
            (position.x || position.y || position.z))
            return position;
    }

    void* transform = transformGetter(player);
    return transform ? game_sdk->get_position(game_sdk->Component_GetTransform(transform)) : Vector3();
}

Vector3 GetHitboxPosition(void* player, int hitbox) {
    if (!player) return Vector3::zero();

    switch (hitbox) {
    case 0: return GetBonePosition(player, game_sdk->_GetHeadPositions); // Head
    case 1: {
        Vector3 headPos = GetBonePosition(player, game_sdk->_GetHeadPositions);
        return headPos == Vector3::zero() ? headPos : Vector3(headPos.x, headPos.y - 0.05f, headPos.z); // Neck
    }
    case 2: {
        Vector3 headPos = GetBonePosition(player, game_sdk->_GetHeadPositions);
        return headPos == Vector3::zero() ? headPos : Vector3(headPos.x, headPos.y - 0.2f, headPos.z); // Body
    }
    default: return GetBonePosition(player, game_sdk->_GetHeadPositions);
    }
}

Vector3 getPosition(void* player) {
    return game_sdk->get_position(game_sdk->Component_GetTransform(player));
}

// ---------------------------------------------------------------- v7a bridge
// Native SDK stays authoritative wherever v7a has no equivalent (raycast,
// damage pipeline, setters). These wrappers prefer the v7a read and fall back
// to the native call when the pointer chain does not resolve.

static void* MatchV7() {
    uint64_t match = v7a::active_match();
    return match ? (void*)match : game_sdk->Curent_Match();
}

static void* LocalV7(void* match) {
    if (v7a::ready()) {
        uint64_t local = v7a::local_player((uint64_t)match);
        if (local) return (void*)local;
    }
    return game_sdk->GetLocalPlayer(match);
}

static bool IsDeadV7(void* player) {
    if (!player) return false;
    if (!v7a::ready())
        return game_sdk->get_IsDieing(player);

    bool dead = false;
    bool knocked = false;
    bool hasDead = v7a::player_dead((uint64_t)player, dead);
    bool hasKnocked = v7a::player_knocked((uint64_t)player, knocked);
    if (hasDead || hasKnocked)
        return dead || knocked;
    return game_sdk->get_IsDieing(player);
}

static bool IsLocalTeamV7(void* player, void* local) {
    uint8_t team = 0, localTeam = 0;
    if (player && local && v7a::ready() &&
        v7a::player_team((uint64_t)player, team) &&
        v7a::player_team((uint64_t)local, localTeam))
        return team == localTeam;
    return player && game_sdk->get_isLocalTeam(player);
}

static int HealthV7(void* player) {
    int health = 0;
    if (player && v7a::ready() && v7a::player_health((uint64_t)player, health))
        return health;
    return player ? game_sdk->GetHp(player) : 0;
}

static std::string NameV7(void* player) {
    std::string name;
    if (player && v7a::ready() && v7a::player_name((uint64_t)player, name) && !name.empty())
        return name;
    if (!player)
        return {};
    monoString* native = game_sdk->name(player);
    return native ? native->toCPPString() : std::string();
}

static Vector3 PositionV7(void* player) {
    Vector3 position;
    if (player && v7a::ready() && v7a::player_root((uint64_t)player, position) &&
        (position.x || position.y || position.z))
        return position;
    return player ? getPosition(player) : Vector3::zero();
}

// One entity walk for every feature. v7a's dictionary is preferred; the old
// match+0xC8 dictionary is the fallback for builds where the chain fails.
template <typename Fn>
static int ForEachPlayer(void* match, Fn fn) {
    int visited = 0;
    uint64_t game = v7a::game();
    if (game) {
        int count = v7a::for_each_entity(game, [&](uint64_t player) {
            visited++;
            return fn((void*)player);
        });
        // An empty dictionary read is not proof the match has no players; let
        // the legacy dictionary answer before trusting a zero walk.
        if (count > 0)
            return visited;
    }

    Dictionary<uint8_t*, void**>* players = *(Dictionary<uint8_t*, void**> **)((long)match + oxo("0xC8"));
    if (!players || !players->getValues())
        return 0;
    for (int i = 0; i < players->getNumValues(); i++) {
        void* player = players->getValues()[i];
        if (!player)
            continue;
        visited++;
        if (!fn(player))
            break;
    }
    return visited;
}

static Vector3 GetHeadPosition(void* player) {
    return game_sdk->get_position(game_sdk->GetHeadPositions(player));
}

static Vector3 CameraMain(void* player) {
    return game_sdk->get_position(*(void**)((uint64_t)player + oxo("0x2B0")));
}

Quaternion GetRotationToTheLocation(Vector3 Target, float Height, Vector3 MyEnemy) {
    Vector3 direction = (Target + Vector3(0, Height, 0)) - MyEnemy;
    return Quaternion::LookRotation(direction, Vector3(0, 1, 0));
}

Quaternion GetCurrentRotation(void* player) {
    void* transform = game_sdk->Component_GetTransform(player);
    if (!transform) return Quaternion();
    return Quaternion::LookRotation(game_sdk->GetForward(transform), Vector3(0, 1, 0));
}

#include "Helper/Ext.h"

class tanghinh {
public:
    static Vector3 Transform_GetPosition(void* player) {
        Vector3 out = Vector3::zero();
        void (*_Transform_GetPosition)(void* transform, Vector3 * out) = (void (*)(void*, Vector3*))getRealOffset(oxo("0x105899AB0"));
        _Transform_GetPosition(player, &out);
        return out;
    }

    static void* Player_GetHeadCollider(void* player) {
        void* (*_Player_GetHeadCollider)(void* players) = (void* (*)(void*))getRealOffset(oxo("0x104933544"));
        return _Player_GetHeadCollider(player);
    }

    static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) {
        bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) = (bool (*)(Vector3, Vector3, unsigned int, void*))getRealOffset(oxo("0x1048FA8E4"));
        return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
    }

    static bool isVisible(void* enemy) {
        if (enemy != NULL) {
            void* hitObj = NULL;
            auto Camera = Transform_GetPosition(game_sdk->Component_GetTransform(game_sdk->get_camera()));
            auto Target = Transform_GetPosition(game_sdk->Component_GetTransform(Player_GetHeadCollider(enemy)));
            return !Physics_Raycast(Camera, Target, 12, &hitObj);
        }
        return false;
    }
};

void DrawLine(ImDrawList* drawList, ImVec2 start, ImVec2 end, float thickness, bool isDead = false, bool isVisible = false) {
    if (!drawList) return;
    ImColor color = isDead ? ImColor(espi) : isVisible ? ImColor(espv) : ImColor(espi);
    drawList->AddLine(start, end, color, thickness);
}

void DrawHealthBar(ImDrawList* drawList, ImVec2 start, ImVec2 end, float healthMultiplier, float thickness, bool isDead = false) {
    if (!drawList) return;

    float totalHeight = end.y - start.y;
    float healthHeight = totalHeight * healthMultiplier;

    drawList->AddRectFilled(
        ImVec2(start.x - thickness / 2, start.y),
        ImVec2(start.x + thickness / 2, end.y),
        ImColor(50, 50, 50, 200)
    );

    if (healthMultiplier > 0) {
        ImColor color = isDead ? ImColor(255, 0, 0) : ImColor(0, 255, 0);
        drawList->AddRectFilled(
            ImVec2(start.x - thickness / 2, end.y - healthHeight),
            ImVec2(start.x + thickness / 2, end.y),
            color
        );
    }

    if (Vars.Outline) {
        drawList->AddRect(
            ImVec2(start.x - thickness / 2 - 1, start.y - 1),
            ImVec2(start.x + thickness / 2 + 1, end.y + 1),
            ImColor(0, 255, 0)
        );
    }
}

void DrawSkeleton(void* player, ImDrawList* drawList) {
    if (!player || !drawList)
        return;
    bool isPlayerVisible = tanghinh::isVisible(player);
    bool isPlayerDead = IsDeadV7(player);

    Vector3 headPos = GetBonePosition(player, game_sdk->_GetHeadPositions);
    Vector3 hipPos = GetBonePosition(player, game_sdk->_newHipMods);
    Vector3 leftAnklePos = GetBonePosition(player, game_sdk->_GetLeftAnkleTF);
    Vector3 rightAnklePos = GetBonePosition(player, game_sdk->_GetRightAnkleTF);
    Vector3 leftToePos = GetBonePosition(player, game_sdk->_GetLeftToeTF);
    Vector3 rightToePos = GetBonePosition(player, game_sdk->_GetRightToeTF);
    Vector3 leftHandPos = GetBonePosition(player, game_sdk->_getLeftHandTF);
    Vector3 rightHandPos = GetBonePosition(player, game_sdk->_getRightHandTF);
    Vector3 leftForeArmPos = GetBonePosition(player, game_sdk->_getLeftForeArmTF);
    Vector3 rightForeArmPos = GetBonePosition(player, game_sdk->_getRightForeArmTF);
    bool visible;
    ImVec2 headScreen = Camera$$WorldToScreen::Checker(headPos, visible);
    if (!visible)
        return;
    ImVec2 hipScreen = Camera$$WorldToScreen::Regular(hipPos);
    ImVec2 leftAnkleScreen = Camera$$WorldToScreen::Regular(leftAnklePos);
    ImVec2 rightAnkleScreen = Camera$$WorldToScreen::Regular(rightAnklePos);
    ImVec2 leftToeScreen = Camera$$WorldToScreen::Regular(leftToePos);
    ImVec2 rightToeScreen = Camera$$WorldToScreen::Regular(rightToePos);
    ImVec2 leftHandScreen = Camera$$WorldToScreen::Regular(leftHandPos);
    ImVec2 rightHandScreen = Camera$$WorldToScreen::Regular(rightHandPos);
    ImVec2 leftForeArmScreen = Camera$$WorldToScreen::Regular(leftForeArmPos);
    ImVec2 rightForeArmScreen = Camera$$WorldToScreen::Regular(rightForeArmPos);
    float thickness = 1.0f;
    ImColor color = isPlayerDead ? ImColor(espi) : isPlayerVisible ? ImColor(espv) : ImColor(espi);

    drawList->AddCircle(headScreen, 2.0f, color, 12, thickness);
    drawList->AddLine(headScreen, hipScreen, color, thickness);
    drawList->AddLine(headScreen, leftForeArmScreen, color, thickness);
    drawList->AddLine(headScreen, rightForeArmScreen, color, thickness);
    drawList->AddLine(leftForeArmScreen, leftHandScreen, color, thickness);
    drawList->AddLine(rightForeArmScreen, rightHandScreen, color, thickness);
    drawList->AddLine(hipScreen, leftAnkleScreen, color, thickness);
    drawList->AddLine(hipScreen, rightAnkleScreen, color, thickness);
    drawList->AddLine(leftAnkleScreen, leftToeScreen, color, thickness);
    drawList->AddLine(rightAnkleScreen, rightToeScreen, color, thickness);
}


void DrawCornerBox(ImDrawList* drawList, ImVec2 topLeft, ImVec2 bottomRight, ImColor color, float thickness = 1.0f) {
    float w = bottomRight.x - topLeft.x;
    float h = bottomRight.y - topLeft.y;
    float lineW = w / 4;
    float lineH = h / 4;

    drawList->AddLine(topLeft, ImVec2(topLeft.x + lineW, topLeft.y), color, thickness);
    drawList->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + lineH), color, thickness);

    drawList->AddLine(ImVec2(bottomRight.x - lineW, topLeft.y), ImVec2(bottomRight.x, topLeft.y), color, thickness);
    drawList->AddLine(ImVec2(bottomRight.x, topLeft.y), ImVec2(bottomRight.x, topLeft.y + lineH), color, thickness);

    drawList->AddLine(ImVec2(topLeft.x, bottomRight.y - lineH), ImVec2(topLeft.x, bottomRight.y), color, thickness);
    drawList->AddLine(ImVec2(topLeft.x, bottomRight.y), ImVec2(topLeft.x + lineW, bottomRight.y), color, thickness);

    drawList->AddLine(ImVec2(bottomRight.x - lineW, bottomRight.y), ImVec2(bottomRight.x, bottomRight.y), color, thickness);
    drawList->AddLine(ImVec2(bottomRight.x, bottomRight.y - lineH), ImVec2(bottomRight.x, bottomRight.y), color, thickness);
}

void DrawRoundBox(ImDrawList* drawList, ImVec2 topLeft, ImVec2 bottomRight, ImColor color, float thickness = 1.0f) {
    drawList->AddRect(topLeft, bottomRight, color, 8.0f, 0, thickness);
}

bool isFov(Vector3 vec1, Vector3 vec2, int radius) {
    float dx = vec1.x - vec2.x;
    float dy = vec1.y - vec2.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

void* GetClosestEnemy() {
    try {
        float shortestDistance = 250.0f;
        void* closestEnemy = NULL;
        void* get_MatchGame = MatchV7();
        if (!get_MatchGame)
            return NULL;
        void* LocalPlayer = LocalV7(get_MatchGame);
        if (!LocalPlayer || !game_sdk->Component_GetTransform(LocalPlayer))
            return NULL;
        if (!Vars.Aimbot && !Vars.Enable && !AimKill)
            return NULL;

        Vector3 LocalPlayerPos = PositionV7(LocalPlayer);
        ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);

        ForEachPlayer(get_MatchGame, [&](void* Player) {
            if (!Player || Player == LocalPlayer || !game_sdk->get_MaxHP(Player) || IsLocalTeamV7(Player, LocalPlayer))
                return true;

            if (Vars.IgnoreKnocked && IsDeadV7(Player))
                return true;
            if (Vars.VisibleCheck && !tanghinh::isVisible(Player))
                return true;

            Vector3 PlayerPos = GetHitboxPosition(Player, Vars.AimHitbox);
            float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
            if (distance >= 300)
                return true;

            ImVec2 enemyScreenPos = Camera$$WorldToScreen::Regular(PlayerPos);
            bool isValidTarget = isFov(Vector3(enemyScreenPos.x, enemyScreenPos.y, 0), Vector3(center.x, center.y, 0), Vars.AimFov);

            if (isValidTarget && distance < shortestDistance) {
                shortestDistance = distance;
                closestEnemy = Player;
            }
            return true;
        });
        return closestEnemy;
    }
    catch (...) {
        return NULL;
    }
}

void UpOneEnemy() {
    if (!Vars.Enable || !Vars.UpPlayerOne)
        return;

    void *match = MatchV7();
    if (!match) return;

    void *local = LocalV7(match);
    if (!local || !game_sdk->Component_GetTransform(local)) return;

    ForEachPlayer(match, [&](void *enemy) {
        if (!enemy || enemy == local) return true;
        if (!game_sdk->Component_GetTransform(enemy)) return true;
        if (!game_sdk->get_MaxHP(enemy)) return true;
        if (IsDeadV7(enemy)) return true;
        if (HealthV7(enemy) <= 0) return true;
        if (IsLocalTeamV7(enemy, local)) return true;

        void *enemyTF = game_sdk->Component_GetTransform(enemy);
        void *localTF = game_sdk->Component_GetTransform(local);
        if (!enemyTF || !localTF) return true;

        Vector3 enemyPos = game_sdk->get_position(enemyTF);
        Vector3 localPos = game_sdk->get_position(localTF);
        float distance = Vector3::Distance(localPos, enemyPos);
        if (distance <= 10.0f) return true;

        float groundY = enemyPos.y;
        float targetY = groundY + 5.7f;

        float step = 0.35f;

        if (enemyPos.y < targetY - 0.1f)
            enemyPos.y += step;
        else if (enemyPos.y > targetY + 0.1f)
            enemyPos.y -= step;

        Transform_INTERNAL_SetPosition(enemyTF, Vvector3(enemyPos.x, enemyPos.y, enemyPos.z));
        return true;
    });
}

void PlayerTakeDamage(void* ClosestEnemy) {
    //DebugLog("Step 1: Start PlayerTakeDamage");

//void* Camera = get_camera();
  //  if (!Camera) return;


    if (ClosestEnemy != nullptr && game_sdk->get_isVisible(ClosestEnemy) && clock() > Save::AimDelay) {
        Save::AimDelay = clock() + Save::AimFPS;


    //void* LocalPlayer = Current_Local_Player();

void* match = MatchV7();

   if (!match) return; // Kiểm tra match NULL

    void* LocalPlayer = LocalV7(match);



    if (LocalPlayer != NULL) {


//auto CreateDamageInfo = (void* (*)(void*, void*))getRealOffset(0x103B88BE4);


void* WeaponHand = WeaponHand = GetWeaponOnHand1(LocalPlayer);

if (WeaponHand == nullptr) return;

//void* DamageInfo = CreateDamageInfo(LocalPlayer, WeaponHand);-> work

void *ObjectPool = *(void**)((uint64_t)LocalPlayer + 0xC58);//private MADMMIICBNN GEGFCFDGGGP;-> player

    if (!ObjectPool) return;

//void* weaponData = *(void**)((uint64_t)WeaponHand + 0x48);
        //if (!weaponData)
            //return;

auto PlayerID2 = *(COW_GamePlay_IHAAMHPPLMG_o*)((uintptr_t)LocalPlayer + 0x2D0);

auto baseDamage = *(int*)((uintptr_t)WeaponHand + 0xA0);
//protected FKPFNILEOHE EFGDILOKKDP;-> class GPBDEDFKJNA , protected WeaponParams m_WeaponParams; // 0xA0 -> class Weapon

int WeaponID = GetWeapon(WeaponHand);
//int baseDamage = GetDamage(WeaponHand);


     Vector3 localLocation = GetHeadPosition(LocalPlayer);
    Vector3 enemyLocation = GetHeadPosition(ClosestEnemy);

void* damagerWeaponDynamicInfo = reinterpret_cast<void*>(getRealOffset(0x1003DFA84));//public Void .ctor() { }->class MKFEKBKJCKE, class WeaponDynamicInfo

 void *damagerDynamicInfo;
    if (!SowDamage) {
    damagerDynamicInfo = damagerWeaponDynamicInfo;
    }else{
    damagerDynamicInfo = nullptr;
    }




if (WeaponID == -1 || baseDamage == 0) return;



    //auto PlayerID2Ptr = (uintptr_t)LocalPlayer + 0x2B0;
 //   auto PlayerID2 = *(PlayerID*)PlayerID2Ptr;


 

//oid* match = Curent_Match();
//int WeaponID;
///int baseDamage;
//if (match){


//}else{
//WeaponID = 0;
//baseDamage = 0;

//}

// LocalPlayerAttributes moved to 0x500 in v7a. The 0x618 read stays as a
// fallback for the old layout; the damage context offsets below are unchanged.
auto damage_module = [](void* attributes) -> void* {
    return attributes ? *(void**)((uint64_t)attributes + 0x2A0) : nullptr;
};
uint64_t attributes = 0;
void* PlayerAttributes = nullptr;
if (v7a::ready() && v7a::player_attributes((uint64_t)LocalPlayer, attributes)) {
    PlayerAttributes = (void*)attributes;
    if (!damage_module(PlayerAttributes))
        PlayerAttributes = nullptr;
}
if (!PlayerAttributes)
    PlayerAttributes = *(void**)((uint64_t)LocalPlayer + 0x618);
        if (!PlayerAttributes)
            return;

        void* DamageModule = damage_module(PlayerAttributes);
        if (!DamageModule)
            return;

        void* DamageInfo = *(void**)((uint64_t)DamageModule + 0x10);//private DamageInfo damageInfo; // 0x10:: FDamageContextData

        if (!DamageInfo)
            return;
    
    //DebugLog("Step 18: WeaponID = %d, BaseDamage = %d", WeaponID, baseDamage);

    *(int*)((char*)DamageInfo + 0x14) = 1;
    *(void**)((char*)DamageInfo + 0x40) = WeaponHand;
     *(int*)((char*)DamageInfo + 0x10) = baseDamage;
      *(COW_GamePlay_IHAAMHPPLMG_o*)((char*)DamageInfo + 0x28) = PlayerID2;
//   *(COW_GamePlay_IHAAMHPPLMG_o*)((uintptr_t)Save::DamageInfo + 0x28) = PlayerID2;
   


        //uintptr_t damageInfoAddr = (uintptr_t) DamageInfo;
       // uintptr_t localPlayerAddr =(uintptr_t) LocalPlayer;
        

//if (damageInfoAddr + oxoranyoffset("0x58") < damageInfoAddr || localPlayerAddr + oxoranyoffset("0x14") < localPlayerAddr) return;
                

        //*(void**)((uintptr_t) DamageInfo + oxoranyoffset("0x28")) = *(void**)(localPlayerAddr + oxoranyoffset("0x14"));
       // *(void**)((uintptr_t) DamageInfo + oxoranyoffset("0x40")) = WeaponHand;
        //*(Vector3*)((uintptr_t) DamageInfo + oxoranyoffset("0x4C")) = CameraMain(LocalPlayer);
       // *(Vector3*)((uintptr_t) DamageInfo + oxoranyoffset("0x58")) = GetHeadPosition(ClosestEnemy);
      // *(int*)((uintptr_t) DamageInfo + 0x28) = baseDamage;



    //DebugLog("Step 19: DamageInfo prepared");

    //void* HitInfo = *(void**)((uintptr_t)LocalPlayer + 0x940);
    //DebugLog("Step 20: HitInfo = %p", HitInfo);
    //if (HitInfo == nullptr) return;

    void* headCollider = get_HeadCollider(ClosestEnemy);
    //DebugLog("Step 21: headCollider = %p", headCollider);
    if (headCollider == nullptr) return;

    void* hitGameObject = get_gameObject(headCollider);

    if (hitGameObject == nullptr) return;

    *(void**)((char*)ObjectPool + 0x18) = hitGameObject;
    *(void**)((char*)ObjectPool + 0x20) = headCollider;
    *(int*)((char*)ObjectPool + 0x64) = 1;


    auto targetPosition = GKHECDLGAJA(LocalPlayer, ObjectPool);

    if (targetPosition == nullptr) return;

    monoList<float*>* CheckParametros = LCLHHHKFCFP(WeaponHand, targetPosition, headCollider,false, DamageInfo);
    if (CheckParametros == nullptr) return;

          StartWholeBodyFiring(LocalPlayer, WeaponHand);
                TakeDamage(ClosestEnemy, baseDamage, PlayerID2, DamageInfo,
                           WeaponID, localLocation, enemyLocation, CheckParametros, damagerDynamicInfo, 0);
                StartFiring(LocalPlayer, WeaponHand);
            
        
        StopFire1(LocalPlayer, WeaponHand);
    

    GDKLMFLNNGM++;
   

   if (GDKLMFLNNGM > 1 && autochangeweapon) { POFFNNMOOBM = !POFFNNMOOBM; SwapWeapon(LocalPlayer, POFFNNMOOBM, false); GDKLMFLNNGM = 0; }

 
}
//}
}
}

void ProcessAimbot() {
    if (!Vars.Aimbot && !AimKill)
        return;
    void* CurrentMatch = MatchV7();
    if (!CurrentMatch)
        return;
    void* LocalPlayer = LocalV7(CurrentMatch);
    if (!LocalPlayer || !game_sdk->Component_GetTransform(LocalPlayer))
        return;
    void* closestEnemy = GetClosestEnemy();
    if (!closestEnemy || !game_sdk->Component_GetTransform(closestEnemy))
        return;

    Vector3 EnemyLocation = GetHitboxPosition(closestEnemy, Vars.AimHitbox);
    if (EnemyLocation == Vector3::zero())
        return;
    Vector3 PlayerLocation = CameraMain(LocalPlayer);
    if (PlayerLocation == Vector3::zero())
        return;

    bool IsScopeOn = game_sdk->get_IsSighting(LocalPlayer);
    bool IsFiring = game_sdk->get_IsFiring(LocalPlayer);
    bool v7Firing = false;
    if (v7a::ready() && v7a::player_firing((uint64_t)LocalPlayer, v7Firing))
        IsFiring = v7Firing;
    bool shouldAim =
        (Vars.AimWhen == 0) ||                          // Always
        (Vars.AimWhen == 1 && IsFiring) ||              // Firing
        (Vars.AimWhen == 2 && IsScopeOn) ||             // Scoping
        (Vars.AimWhen == 3 && (IsFiring || IsScopeOn)); // Fire & Scope

    if (shouldAim && (!Vars.VisibleCheck || tanghinh::isVisible(closestEnemy))) {
        if (IsDeadV7(closestEnemy) && Vars.IgnoreKnocked) {
            float shortestDistance = 9999.0f;
            void* newTarget = NULL;
            ForEachPlayer(CurrentMatch, [&](void* Player) {
                if (!Player || Player == LocalPlayer || !game_sdk->get_MaxHP(Player) || IsLocalTeamV7(Player, LocalPlayer) || Player == closestEnemy)
                    return true;

                if (Vars.IgnoreKnocked && IsDeadV7(Player))
                    return true;
                if (Vars.VisibleCheck && !tanghinh::isVisible(Player))
                    return true;

                Vector3 PlayerPos = GetHitboxPosition(Player, Vars.AimHitbox);
                float distance = Vector3::Distance(PlayerLocation, PlayerPos);
                if (distance < 300 && distance < shortestDistance) {
                    shortestDistance = distance;
                    newTarget = Player;
                }
                return true;
            });

            if (newTarget) {
                EnemyLocation = GetHitboxPosition(newTarget, Vars.AimHitbox);
                closestEnemy = newTarget;
            }
            else {
                return;
            }
        }

        Quaternion TargetLook = GetRotationToTheLocation(EnemyLocation, 0.05f, PlayerLocation);
        game_sdk->set_aim(LocalPlayer, TargetLook);
    }
        
    if(AimKill){
             PlayerTakeDamage(closestEnemy);
            }
}

void get_players() {
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    int numberOfPlayersAround = 0;
    if (!draw_list)
        return;

    try {
        // Aim runs even with the ESP off: Vars.Enable only gates drawing.
        if (Vars.Aimbot || AimKill) {
            ProcessAimbot();
        }
        if (Vars.UpPlayerOne) {
            UpOneEnemy();
        }
        if (!Vars.Enable)
            return;
        void* current_Match = MatchV7();
        if (!current_Match)
            return;

        void* local_player = LocalV7(current_Match);
        if (!local_player)
            return;

        void* camera = game_sdk->get_camera();
        if (!camera)
            return;

        ForEachPlayer(current_Match, [&](void* closestEnemy) {
            if (!closestEnemy ||
                !game_sdk->Component_GetTransform(closestEnemy) ||
                closestEnemy == local_player ||
                !game_sdk->get_MaxHP(closestEnemy) ||
                IsLocalTeamV7(closestEnemy, local_player)) {
                return true;
            }
            numberOfPlayersAround++;

            Vector3 pos = PositionV7(closestEnemy);
            Vector3 pos2 = PositionV7(local_player);
            float distance = Vector3::Distance(pos, pos2);
            if (distance > Vars.RenderDist)
                return true;

            bool isEnemyDead = IsDeadV7(closestEnemy);
            bool isEnemyVisible = tanghinh::isVisible(closestEnemy);

            bool w2sc;
            ImVec2 top_pos = Camera$$WorldToScreen::Regular(pos + Vector3(0, 1.6, 0));
            ImVec2 bot_pos = Camera$$WorldToScreen::Regular(pos);
            ImVec2 pos_3 = Camera$$WorldToScreen::Checker(pos, w2sc);
            auto pmtXtop = top_pos.x;
            auto pmtXbottom = bot_pos.x;
            if (top_pos.x > bot_pos.x) {
                pmtXtop = bot_pos.x;
                pmtXbottom = top_pos.x;
            }
            Camera$$WorldToScreen::Checker(pos + Vector3(0, 0.75f, 0), w2sc);
            float calculatedPosition = fabs((top_pos.y - bot_pos.y) * (0.0092f / 0.019f) / 2);

            ImRect rect(
                ImVec2(pmtXtop - calculatedPosition, top_pos.y),
                ImVec2(pmtXbottom + calculatedPosition, bot_pos.y));
            Vector3 viewpos;
            if (!v7a::ready() || !v7a::camera_position((uint64_t)local_player, viewpos))
                viewpos = game_sdk->get_position(game_sdk->Component_GetTransform(game_sdk->get_camera()));

            if (w2sc) {
                if (Vars.lines) {
                    // calcula o ponto inicial da linha baseado no estilo escolhido
                    float centerX = ImGui::GetIO().DisplaySize.x / 2.0f;
                    ImVec2 start;
                    if (Vars.LineStyle == 0) {
                        // Cima (topo) - 15 px do topo para não colar na borda
                        start = ImVec2(centerX, 15.0f);
                    }
                    else {
                        // Baixo (rodapé) - 15 px acima do bottom
                        start = ImVec2(centerX, ImGui::GetIO().DisplaySize.y - 15.0f);
                    }
                    ImVec2 end = ImVec2(rect.GetCenter().x, rect.Min.y);
                    DrawLine(draw_list, start, end, 1.0f, isEnemyDead, isEnemyVisible);
                }

                if (Vars.Box) {
                    ImColor boxColor = isEnemyDead ? ImColor(espi) : isEnemyVisible ? ImColor(espv) : ImColor(espi);

                    if (Vars.BoxStyle == 0) {
                        draw_list->AddRect(rect.Min, rect.Max, boxColor, 0.0f, 0, 1.0f);
                    }
                    else if (Vars.BoxStyle == 1) {
                        DrawCornerBox(draw_list, rect.Min, rect.Max, boxColor, 1.0f);
                    }
                    else if (Vars.BoxStyle == 2) {
                        DrawRoundBox(draw_list, rect.Min, rect.Max, boxColor, 1.0f);
                    }

                    if (Vars.Outline) {
                        draw_list->AddRect(rect.Min - ImVec2(1, 1), rect.Max + ImVec2(1, 1), ImColor(0, 0, 0));
                        draw_list->AddRect(rect.Min + ImVec2(1, 1), rect.Max - ImVec2(1, 1), ImColor(0, 0, 0));
                    }
                }

                if (Vars.Health) {
                    auto health = HealthV7(closestEnemy);
                    auto maxhealth = game_sdk->get_MaxHP(closestEnemy);

                    float health_multiplier = (float)health / (float)maxhealth;
                    if (health_multiplier < 0) health_multiplier = 0;
                    if (health_multiplier > 1) health_multiplier = 1;

                    float health_bar_pos;

                    if (Vars.HealthPos == 0) { // Esquerda
                        health_bar_pos = rect.Min.x - 4;
                    }
                    else { // Direita
                        health_bar_pos = rect.Max.x + 4;
                    }

                    DrawHealthBar(
                        draw_list,
                        ImVec2(health_bar_pos, rect.Min.y - 1),
                        ImVec2(health_bar_pos, rect.Max.y),
                        health_multiplier,
                        3.0f,
                        isEnemyDead
                    );
                }

                
                // Defina os estilos de nome
                enum NameStyle {
                    SEM_SOMBRA = 0,
                    COM_SOMBRA = 1
                };
                if (Vars.Name)
                {
                    std::string names = NameV7(closestEnemy);
                    if (names.empty())
                        names = "null";

                    std::transform(names.begin(), names.end(), names.begin(), ::tolower);
                    std::string name = names;

                    ImVec2 text_size = verdana_smol->CalcTextSizeA(8, FLT_MAX, 0, names.c_str());
                    ImVec2 name_pos = {
                        rect.Min.x + (rect.GetWidth() / 2) - text_size.x / 2,
                        rect.Min.y - 2 - text_size.y
                    };

                    ImDrawList* draw = ImGui::GetForegroundDrawList();

                    // calcula retângulo de fundo
                    ImVec2 bg_min = name_pos - ImVec2(2, 2);
                    ImVec2 bg_max = name_pos + text_size + ImVec2(2, 2);

                    // só desenha retângulo quando for "Com Sombra"
                    if (Vars.NameStyle == COM_SOMBRA)
                    {
                        draw->AddRectFilled(bg_min, bg_max, ImColor(0, 0, 0, 150), 3.0f);
                    }

                    // o texto é sempre desenhado
                    draw->AddText(
                        verdana_smol,
                        8,
                        name_pos,
                        ImColor(nameColor),
                        name.c_str()
                    );
                }








                if (Vars.Distance) {
                    std::string distancestr = fmt::format(oxorany("{}M"), static_cast<int>(distance));
                    ImVec2 distance_pos = {
                        rect.Max.x + 4,
                        rect.Min.y };
                    AddText(verdana_smol, 8, false, false, distance_pos, ImColor(distanceColor), distancestr.c_str());
                }



                if (Vars.skeleton) {
                    DrawSkeleton(closestEnemy, draw_list);
                }
            }

            if (Vars.OOF) {
                if ((pos_3.x < 0 || pos_3.x > disp.width) || (pos_3.y < 0 || pos_3.y > disp.height) || !w2sc) {
                    constexpr int maxpixels = 150;
                    int pixels = maxpixels;
                    if (w2sc) {
                        if (pos_3.x < 0)
                            pixels = clamp((int)-pos_3.x, 0, (int)maxpixels);
                        if (pos_3.y < 0)
                            pixels = clamp((int)-pos_3.y, 0, (int)maxpixels);

                        if (pos_3.x > disp.width)
                            pixels = clamp((int)pos_3.x - (int)disp.width, 0, (int)maxpixels);
                        if (pos_3.y > disp.height)
                            pixels = clamp((int)pos_3.y - (int)disp.height, 0, (int)maxpixels);
                    }

                    float opacity = (float)pixels / (float)maxpixels;

                    float size = 3.5f;
                    Vector3 viewdir = game_sdk->GetForward(game_sdk->Component_GetTransform(game_sdk->get_camera()));
                    Vector3 targetdir = Vector3::Normalized(pos - viewpos);

                    float viewangle = atan2(viewdir.z, viewdir.x) * Rad2Deg;
                    float targetangle = atan2(targetdir.z, targetdir.x) * Rad2Deg;

                    if (viewangle < 0)
                        viewangle += 360;
                    if (targetangle < 0)
                        targetangle += 360;

                    float angle = targetangle - viewangle;

                    while (angle < 0)
                        angle += 360;
                    while (angle > 360)
                        angle -= 360;

                    angle = 360 - angle;
                    angle -= 90;
                    OtFovV1(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2, 90 + distance * 2,
                        angle - size,
                        angle + size,
                        ImColor(1.f, 1.f, 1.f, 1.f * opacity), 1);
                }
            }

            return true;
        });


        if (Vars.counts) {
            ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
            ImVec2 numberPosition(ImGui::GetIO().DisplaySize.x / 2.0f, 12);
            std::string countText = "Inimigos: " + std::to_string(numberOfPlayersAround);
            ImVec2 textSize = verdana_smol->CalcTextSizeA(25.0f, FLT_MAX, 0, countText.c_str());
            ImVec2 textPos(
                numberPosition.x - textSize.x / 2.0f,
                numberPosition.y
            );

            draw_list->AddText(verdana_smol, 25.0f, textPos, ImColor(255, 255, 255, 255), countText.c_str());
        }

    }
    catch (...) {
        return;
    }
}

//@lostx.wq

void RunTelekill() {
    if (!istelekill)
        return;

    void* match = MatchV7();
    if (!match) return;

    void* local = LocalV7(match);
    if (!local || !game_sdk->Component_GetTransform(local)) return;

    void* localTF = game_sdk->Component_GetTransform(local);
    Vector3 localPos = game_sdk->get_position(localTF);
    Vector3 forward = game_sdk->GetForward(localTF);

    ForEachPlayer(match, [&](void* enemy) {
        if (!enemy || enemy == local) return true;
        if (!game_sdk->Component_GetTransform(enemy)) return true;
        if (!game_sdk->get_MaxHP(enemy)) return true;
        if (IsDeadV7(enemy)) return true;
        if (HealthV7(enemy) <= 0) return true;
        if (IsLocalTeamV7(enemy, local)) return true;

        void* enemyTF = game_sdk->Component_GetTransform(enemy);
        Vector3 enemyPos = game_sdk->get_position(enemyTF);
        float distance = Vector3::Distance(localPos, enemyPos);

        if (distance > 8.0f)
            return true;

        Vector3 stableFront = localPos + forward * 1.2f;
        stableFront.y = localPos.y;

        Transform_INTERNAL_SetPosition(enemyTF, Vvector3(stableFront.x, stableFront.y, stableFront.z));
        return true;
    });
}

void RunUpPlayer() {
    if (!isfly)
        return;

    void* match = MatchV7();
    if (!match) return;

    void* local = LocalV7(match);
    if (!local || !game_sdk->Component_GetTransform(local)) return;

    ForEachPlayer(match, [&](void* enemy) {
        if (!enemy || enemy == local) return true;
        if (!game_sdk->Component_GetTransform(enemy)) return true;
        if (!game_sdk->get_MaxHP(enemy)) return true;
        if (IsDeadV7(enemy)) return true;
        if (HealthV7(enemy) <= 0) return true;
        if (IsLocalTeamV7(enemy, local)) return true;

        void* enemyTF = game_sdk->Component_GetTransform(enemy);
        void* localTF = game_sdk->Component_GetTransform(local);
        if (!enemyTF || !localTF) return true;

        Vector3 enemyPos = game_sdk->get_position(enemyTF);
        Vector3 localPos = game_sdk->get_position(localTF);
        float distance = Vector3::Distance(localPos, enemyPos);

        float groundY = enemyPos.y;
        float targetY = groundY + 5.7f;

        float step = 0.90f;

        if (enemyPos.y < targetY - 0.1f)
            enemyPos.y += step;

        Transform_INTERNAL_SetPosition(enemyTF, Vvector3(enemyPos.x, enemyPos.y, enemyPos.z));
        return true;
    });
}

void aimbot() {
    ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
    if (!Vars.Aimbot)
        return;
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    if (!draw_list)
        return;
    void* Match = MatchV7();
    if (!Match)
        return;

    if (Vars.Aimbot && Vars.isAimFov) {
        if (Vars.fovaimglow)
            drawcircleglow(draw_list, center, Vars.AimFov, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 999, 1, 12);
        else
            draw_list->AddCircle(center, Vars.AimFov, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 100);
    }

    void* LocalPlayer = LocalV7(Match);
    if (!LocalPlayer || !game_sdk->Component_GetTransform(LocalPlayer))
        return;

    void* playertarget = GetClosestEnemy();
    if (!playertarget)
        return;

    ImVec2 EnemyLocation = Camera$$WorldToScreen::Regular(GetHitboxPosition(playertarget, Vars.AimHitbox));
    ImColor aimbotColor = ImColor(255, 255, 255);
    (void)EnemyLocation;
    (void)aimbotColor;
}
