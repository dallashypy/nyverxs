# NYVERXS iOS — extração e remote por Discord ID

Documento de referência do tweak Liên Quân Mobile iOS (`NYVERXS.dylib`) e do
remote que o controla pelo painel web sem loader `.exe`.

- Tweak: `C:\Users\dallashy\Downloads\src ios\src ios\nyverxs\nyverxs\nyverxs`
- Website: `C:\Users\dallashy\Downloads\NoLogs Projects\nologs ff external\Remote Abyssal\Remote Abyssal\website`
- Plataforma: iPhone arm64, jailbreak + MobileSubstrate/ElleKit (rootless)
- Deployment target: iOS 13.6
- Bundle filtrado: `com.garena.game.kgvn`

---

## 1. O que o tweak é

Uma dylib injetada no processo do jogo. Ela monta um overlay Metal (`MTKView`)
com Dear ImGui, desenha o ESP por cima do jogo e mexe na memória do processo via
`JRMemoryEngine` (`mach_task_self` + leitura/escrita de VM). As funções do jogo
são chamadas por offset fixo, resolvidos em `getRealOffset()`.

Fluxo de inicialização:

1. `NYVERXS.plist` injeta a dylib em `com.garena.game.kgvn`.
2. `Esp/LoadView.mm` escuta `UIApplicationDidFinishLaunchingNotification`.
3. Após 7 s, cria `MenuLoad` e instala a view ImGui + botão flutuante + gesto de
   3 dedos com 2 toques.
4. `lostwq.mm` inicializa o contexto ImGui, carrega fontes e abre o menu.
5. Um construtor separado (`initialize` em `lostwq.mm`) cria uma thread que
   espera 5 s e instala o hook anti-ban em `0x102D86DE4` via `hook/hook.c`
   (exception ports, não MSHook).

O menu tem quatro abas: **AIM**, **ESP**, **MISC** e **REMOTE**. A antiga aba
INFO, que consultava a licença do vendor (`APIClient`), foi removida; as pastas
`API/` e `APIKey/` saíram da árvore (só sobravam chamadas comentadas).

---

## 2. Mapa de arquivos

| Arquivo | Papel |
|---|---|
| `lostwq.mm` | Implementação do menu, toggles de memória, overlay e superfície remota |
| `Helper/Hooks.h` | Offsets, structs do jogo, aimbot, ESP, telekill, fly |
| `Helper/OffsetsV7a.h` | Camada `v7a` arm64: cadeia, entidades, ossos, câmera e W2S |
| `Esp/LoadView.mm` | Instalação do menu, botão flutuante, auto-start do remote |
| `Remote/RemoteClient.h` | API Objective-C/C do cliente remoto |
| `Remote/RemoteClient.mm` | Login, estado, WebSocket, comandos e reconexão |
| `Esp/ImGuiDrawView.h` | Interface do overlay |
| `Helper/Mem.h` | `getRealOffset`, leitura de base da imagem |
| `JRMemory.framework` | Scan/escrita de memória (`JRMemoryEngine`) |
| `hook/hook.c` | Hook por portas de exceção Mach |
| `NYVERXS.plist` | Filtro de injeção (`com.garena.game.kgvn`) |
| `control` | Metadados do pacote rootless (`com.nologs.nyverxs`) |
| `Makefile` | Theos, arm64, rootless, ARC em ObjC/ObjC++ |
| `.github/workflows/build.yml` | Build Theos em runner macOS; artifact e Release |
| `build-linux.sh` | Build equivalente em WSL2/Ubuntu |
| `Esp/CaptainHook.h` | Header auxiliar incluído pelo tweak |

---

## 3. Offsets do jogo

Todos os offsets são relativos à base da imagem principal. O jogo atualiza com
frequência; depois de cada update, re-derive a tabela antes de recompilar.

### 3.1 Funções principais

| Endereço | Símbolo interno | Uso |
|---|---|---|
| `0x100FE52DC` | `Curent_Match` | Retorna a partida atual |
| `0x103B0BA8C` | `GetLocalPlayer` | Jogador local da partida |
| `0x10499719C` | `GetHp` | HP atual |
| `0x104997244` | `get_MaxHP` | HP máximo |
| `0x10491076C` | `get_IsDieing` | Jogador derrubado/morto |
| `0x10494E65C` | `get_isLocalTeam` | Filtro de time |
| `0x104938678` | `get_isVisible` | Visibilidade pelo jogo |
| `0x10584E620` | `get_camera` | Câmera principal |
| `0x10584DF68` | `WorldToScreenPoint` | Mundo → viewport |
| `0x105899A80` | `get_position` | Posição de um Transform |
| `0x105850A8C` | `Component_GetTransform` | Componente → Transform |
| `0x10589A430` | `GetForward` | Vetor frente do Transform |
| `0x104934BE0` | `set_aim` | Aplica rotação de mira |
| `0x104929520` | `get_IsSighting` | Está na mira |
| `0x10492946C` | `get_IsFiring` | Está atirando |
| `0x1049302E8` | `get_NickName` | Nome do jogador (`monoString`) |
| `0x105899B54` | `Transform_SetPosition` | Teleporta um Transform |
| `0x105899AB0` | `Transform_GetPosition` | Posição via out-param |
| `0x104AC7E08` | `Player_TakeDamage` | Dano interno |
| `0x1048FA8E4` | `Physics_Raycast` | Visão real (parede) |

### 3.2 Ossos / skeleton

| Endereço | Osso |
|---|---|
| `0x1049AF530` | Cabeça (`GetHeadTF`) |
| `0x1049AF680` | Quadril (`GetHipTF`) |
| `0x1049AF9B4` | Tornozelo esquerdo |
| `0x1049AFA58` | Tornozelo direito |
| `0x1049AFAFC` | Dedo do pé esquerdo |
| `0x1049AFBA0` | Dedo do pé direito |
| `0x104933F3C` | Mão esquerda |
| `0x104933FE8` | Mão direita |
| `0x10493408C` | Antebraço esquerdo |
| `0x104934130` | Antebraço direito |

### 3.3 Combate / aimkill

| Endereço | Função |
|---|---|
| `0x104AF289C` | `StartFiring` |
| `0x104AC9ABC` | `StopFire1` |
| `0x104AF2E18` | `StartWholeBodyFiring` |
| `0x10492FBA4` | `GetWeaponOnHand1` |
| `0x10302C5DC` | `GetWeapon` |
| `0x1003C2E44` | `LCLHHHKFCFP` (monta lista de checagem) |
| `0x1003DFA84` | Construtor de `WeaponDynamicInfo` (usado como ponteiro) |
| `0x104933544` | `get_HeadCollider` |
| `0x105850AF8` | `get_gameObject` |
| `0x10499485C` | `GKHECDLGAJA` (resolver alvo do pool) |
| `0x104AF3484` | `SwapWeapon` |

### 3.4 Offsets de struct

| Caminho | Significado |
|---|---|
| `match + 0xC8` | Dicionário legado de jogadores (fallback do `ForEachPlayer`) |
| `LocalPlayer + 0x2D0` | `PlayerID` (16 bytes: dois uint32, dois uint8, uint64) |
| `LocalPlayer + 0xC58` | `ObjectPool` |
| `LocalPlayer + 0x500` | `PlayerAttributes` no `v7a` (`0x618` é o fallback legado) |
| `WeaponHand + 0xA0` | Dano base da arma |
| `PlayerAttributes + 0x2A0` | `DamageContextData` |
| `DamageContextData + 0x10` | `DamageInfo` |
| `DamageInfo + 0x10` | Dano base (write) |
| `DamageInfo + 0x14` | Flag `1` (write) |
| `DamageInfo + 0x28` | `PlayerID` (write) |
| `DamageInfo + 0x40` | `WeaponHand` (write) |
| `ObjectPool + 0x18` | GameObject do hit |
| `ObjectPool + 0x20` | Collider da cabeça |
| `ObjectPool + 0x64` | Flag `1` |
| `LocalPlayer + 0x2B0` | Câmera usada no `set_aim` |

### 3.5 Hook anti-ban

| Endereço | Ação |
|---|---|
| `0x102D86DE4` | Substituído por `antiban()` que retorna `false` |

O hook é instalado 5 s depois do construtor, em thread separada, por
`hook(address, function, 1)`.

### 3.6 Camada `v7a` (arm64)

`Helper/OffsetsV7a.h` aplica o struct `v7a` (dump 32-bit) ao runtime arm64 do
iOS. Toda leitura passa por `vm_read_overwrite` e devolve `false` em qualquer elo
inválido; o `Hooks.h` então cai na função nativa correspondente. As leituras
diretas de campo (morto, derrubado, vida, time, nome, osso, firing, atributos)
só são usadas quando `v7a::ready()` confirma a cadeia base; sem ela, o nativo
responde. As funções nativas continuam sendo a autoridade onde o `v7a` não tem
equivalente: raycast, pipeline de dano, `set_aim`, `Transform_SetPosition`,
`baseDamage`, `ObjectPool` e `PlayerID`.

Cadeia resolvida em runtime:

```
base + InitBase (0xA342EFC)
  -> facade
  -> +0x00 klass
  -> +0x5C statics
  -> +0x00 game
  -> +0x50 match
  -> +0x94 local player
```

Entidades ficam em `game+0x68`; `u32(match+0x8C) == 1` marca partida ativa. A base
é sondada em quatro candidatos (binário do app/UnityFramework × offset puro /
`0x100000000 + InitBase`), a cadeia é validada e o resultado é memorizado por 2 s.

Containers Il2Cpp em 64 bits:

| Container | Offset |
|---|---|
| `Dictionary::values` | `0x28` |
| `monoArray::length` | `0x18` |
| `monoArray::data` | `0x20` |
| `monoString::length` | `0x10` |
| `monoString::chars` | `0x14` |

Fallback V31 do dicionário (entradas embutidas): entries `+0x18`, count `+0x20`,
dados `+0x20`, stride `0x18`, valor `+0x10`.

| Leitura | Caminho |
|---|---|
| Morto | `player+0x50` (byte) |
| Derrubado | `player+0x1A60` → `+0x78` (`XPose == 8`) |
| Time | `player+0x504` → `+0xA8` → `+0x14` → `+0x59` |
| Visível | `player+0x504` → `+0xA8` → `+0x95` |
| Bot | `data+0x324` ou `player+0x324` |
| Vida | `player+0x48` → `+0x8` → `+0x10` → `+0x10` (`int16`) |
| Nome | `player+0x31C` (monoString UTF-16) |
| Atributos | `player+0x500` (fallback `0x618`) |
| Atirando | `player+0x58C` |
| Matriz de visão | `local+0x494` → `+0x18` → `+0x8` → `+0xE8` |
| Câmera (posição) | `local+0x28C` |

Ossos usam `v7a::Off::Bones` (cabeça `0x49C`, quadril `0x4A0`, pescoço `0x4A4`,
pelve `0x4AC`, root `0x4B0`, tornozelos `0x4B8/0x4BC`, pés `0x4C0/0x4C4`, mãos
`0x498/0x4C8`, ombros `0x4D0/0x4D4`). O transform é `node+0x8` → transform,
`transform+0x8` → object, índice `+0x24`, matriz `+0x20`, lista `+0x18`, índices
`+0x1C`, stride `0x30`, com composição pai-filho por quaternion.

`ForEachPlayer()` centraliza a caminhada de entidades: prefere o dicionário
`v7a` e cai em `match+0xC8` quando a cadeia não resolve. O W2S usa a matriz
`v7a` com cache de 100 ms; se a matriz não resolve, usa
`WorldToScreenPoint` nativo.

---

## 4. Funcionalidades e estado

### 4.1 `Vars_t` (`Helper/Hooks.h`)

- **AIM**: `Aimbot`, `AimFov`, `AimHitbox` (0 cabeça, 1 pescoço, 2 corpo),
  `AimWhen` (0 sempre, 1 ao atirar, 2 na mira, 3 mira+tiro),
  `isAimFov`, `fovaimglow`, `VisibleCheck`, `IgnoreKnocked`.
- **ESP**: `Enable`, `lines`, `Box`, `Outline`, `Name`, `Health`, `Distance`,
  `skeleton`, `counts`, `OOF`, `BoxStyle`, `LineStyle`, `HealthPos`,
  `RenderDist` (novo, default `200.0f`).
- **Outros**: `UpPlayerOne`, `AimSpeed`.

### 4.2 Aimbot

`ProcessAimbot()`:

1. Exige `Vars.Aimbot` **ou** `AimKill`.
2. Pega partida, jogador local, alvo mais próximo por distância dentro do FOV.
3. Respeita `VisibleCheck` (raycast) e `IgnoreKnocked`.
4. Aplica `set_aim` com `GetRotationToTheLocation`.
5. Se `AimKill`, chama `PlayerTakeDamage`, que monta `DamageInfo`, resolve o
   collider da cabeça e chama `TakeDamage` + `StartWholeBodyFiring`.

Correções aplicadas nesta versão:

- `get_players()` processa mira **antes** do gate `Vars.Enable`; o ESP pode
  estar desligado e a mira continuar.
- `AimKill` sozinho também passa pelo `GetClosestEnemy` e pelo `ProcessAimbot`.
- A chamada duplicada de `ProcessAimbot()` dentro de `aimbot()` foi removida.
- `aimbot()` não é mais chamado no draw loop; o FOV é desenhado uma vez.

### 4.3 ESP

`get_players()` percorre as entidades por `ForEachPlayer()`, projeta posições com
a matriz `v7a` (fallback `WorldToScreenPoint`) e desenha linha, caixa, vida,
nome, distância, skeleton e contador. O limite de desenho usa `Vars.RenderDist`,
ajustável pelo painel (`iosRenderDist`).

### 4.4 Telekill / Fly / Rage

- `RunTelekill()`: se `istelekill`, teleporta inimigos a até 8 m para 1,2 m à
  frente do jogador local, no chão.
- `RunUpPlayer()`: se `isfly`, sobe os inimigos 5,7 m em passos de 0,9 m.
- `UpOneEnemy()`: variante antiga controlada por `Vars.UpPlayerOne`; continua
  disponível pela chave `upEnemy`, mas o menu usa `isfly`.

As duas funções agora são chamadas a cada frame no draw loop, depois de
`get_players()`.

### 4.5 Toggles de memória (`lostwq.mm`)

Escaneiam a faixa `0x100000000–0x200000000` (Speed) ou
`0x100000000–0x160000000` (demais) com `JRMemoryEngine`:

| Toggle | Scan | Escrita |
|---|---|---|
| Speed | `4397530849764387586` (uint64) | `4366458311853765201` |
| No Recoil | `1016018816` (uint64) | `0` |
| Wall Glow | `1.22f` | `965.0f` |
| Wall Fly | `1.5f` | `900.0f` |
| Scope | `0.03f` | `10.0f` |
| Wall Hack | cadeia `2` → `0.10000000149` nearby 0x20 → `3` → `4.2038954e-45` → nearby | `-99.0f` |

`dispatch_once` guarda os resultados. Os checkboxes do menu agora **chamam** os
métodos de instância (`toggleSpeedX2:`, `toggleNoRecoil:`, etc.). Antes eles só
mudavam o booleano e nenhum scan era feito.

`WallHack` não tem restauração dos valores originais; desligar apenas zera a
lista de resultados. Está documentado no painel.

---

## 5. Remote sem `.exe` — arquitetura

```
iPhone (tweak)                         Website (Express)                Navegador
     |                                       |                              |
     | POST /api/device/auth                 |                              |
     | {discord_id, sid} ------------------> | POST /remote/id-login        |
     |                                       | (API Remote IOS)             |
     |                                       |<--- ok + token remoto        |
     | <------------------ {ok, token, user} | emite JWT loader local       |
     |                                       |                              |
     | GET /api/state (Bearer) ------------->| devolve estado salvo         |
     | <------------------ {state, rev}      |                              |
     |                                       |                              |
     | WS /?role=loader (Bearer) ----------->| registra loader              |
     |                                       |<--- POST /api/auth/discord --|
     |                                       |     (mesmo Discord ID)       |
     |                                       |<--- POST /api/state ---------|
     | <--- {action:"set",param,value} ------|     patch persistido         |
     | --- {type:"status", data:{...}} ----->|----------------------------->| painel
     | --- {type:"toggle", param, value} --->|----------------------------->|
```

### 5.1 Login do dispositivo

`POST /api/device/auth`

```json
{ "discord_id": "123456789012345678", "sid": "ios-<UUID>" }
```

Antes de emitir qualquer token, o servidor chama a API Remote IOS:

```
POST ${NOLOGS_API_URL}/remote/id-login
{ "discordId": "<17-20 digitos>", "product": "remote_ios" }

200 -> { "token": "<sessao remota>" }
400 -> corpo sem produto
4xx -> { "error": "..." }
```

Resposta `200` do site:

```json
{ "ok": true, "token": "<jwt>", "user": { "id": 1, "username": "ios_345678", "...": "..." } }
```

Regras:

- Discord ID: 17 a 20 dígitos. SID: 8 a 128 caracteres, persistido em
  `NSUserDefaults` (`nyv_remote_device_sid`).
- A API é a autoridade: ID fora do formato → `400`; ID não cadastrado, pausado
  ou expirado → `401`; excesso de tentativas → `429`; API fora do ar/timeout →
  `503`.
- O token remoto é apenas prova de que o ID está ativo. Ele não é persistido,
  não vai para o navegador e não aparece em log; o site emite o JWT local
  (`channel: "loader"`, `product: "remote_ios"`).
- O `product` é obrigatório no corpo: a API responde `400` sem ele e `401`
  quando o ID não tem key daquele produto. O site usa `NOLOGS_PRODUCT`, default
  `remote_ios`.
- Conta aprovada que ainda não existe é criada com `nologs_product = remote_ios`
  e `DEVICE_LOGIN` ligado. Conta existente é reutilizada; `machine_id` acompanha
  o SID mais recente e `remote_verified_at` é atualizado a cada login.
- Se a cópia local estiver vencida mas a API aprovar, a expiração local é
  estendida por `DEVICE_TRIAL_DAYS` (default 30). A cópia local nunca contradiz
  a autoridade remota.
- `DEVICE_LOGIN=0` desliga a criação de contas novas; contas existentes aprovadas
  pela API continuam entrando.
- `/api/auth/discord` usa o mesmo caminho: valida na API, provisiona a conta
  local se necessário e só então devolve a sessão do navegador. Assim o operador
  pode entrar no site e abrir o app depois, com a mesma identidade.
- O JWT carrega `channel: "loader"` e `product: "remote_ios"`, exatamente o que
  o WebSocket e `/api/state` esperam.

### 5.2 WebSocket

- URL: `wss://<host>/?role=loader`
- Header: `Authorization: Bearer <jwt>`
- O servidor fecha com `1008` se o token não for de loader, se o produto não
  bater, se a assinatura estiver vencida ou se o JTI estiver revogado.

Mensagens servidor → telefone:

| JSON | Ação no tweak |
|---|---|
| `{"action":"set","param":"iosAimbot","value":"true"}` | `nyv_remote_apply` |
| `{"action":"ping"}` | responde `{"type":"pong"}` |
| `{"action":"get_status"}` | envia `status` |
| `{"action":"get_userdata"}` | envia `userdata` |
| `{"action":"get_skins"}` | responde catálogo vazio (`ready:false`) |
| `{"action":"get_chams_state"}` | responde `unloaded` |
| `{"action":"load_memory" ...}` | `nyv_remote_action` |

Mensagens telefone → servidor:

| JSON | Efeito no painel |
|---|---|
| `{"type":"status","data":{...}}` | `infoMem`, `iosStatus`, `iosMemory`, rage/legit |
| `{"type":"toggle","param":"...","value":"..."}` | confirmação do `set` |
| `{"type":"response","data":"..."}` | toast/resposta de ação |
| `{"type":"userdata",...}` | dados da conta no canal |
| `{"type":"pong"}` | keepalive |

Heartbeat: o servidor manda `ping` a cada 25 s; o cliente também envia `pong`
a cada 20 s. Reconexão com backoff exponencial (2 s → 30 s). `1008` força novo
device auth.

### 5.3 Status enviado pelo telefone

```json
{
  "platform": "ios",
  "client": "nyverxs",
  "state": "online",
  "aimbot": true,
  "esp": false,
  "aim_kill": false,
  "fly": false,
  "teleport": false,
  "speed": false,
  "no_recoil": false,
  "wall_glow": false,
  "wall_fly": false,
  "wall_hack": false,
  "scope": false,
  "stream_mode": true,
  "memory_loaded": true,
  "memory_status": "pronta",
  "rage_state": 0,
  "legit_state": 3,
  "chams_loaded": false,
  "render_dist": 200.0,
  "fov": 150.0
}
```

---

## 6. Chaves do painel

O painel mantém o estado compartilhado por usuário (`db.state[userId]`). Cada
chave vira um `set` para o loader. A seção **iOS / LQ** usa chaves próprias e as
chaves antigas continuam funcionando como alias.

### 6.1 iOS

| Chave | Efeito |
|---|---|
| `iosAimbot` | `Vars.Aimbot` |
| `iosEsp` | `Vars.Enable` |
| `iosAimKill` | `AimKill` |
| `iosVisible` | `Vars.VisibleCheck` |
| `iosIgnoreKnocked` | `Vars.IgnoreKnocked` |
| `iosAimFov` | `Vars.AimFov` (0–1000) |
| `iosShowFov` | `Vars.isAimFov` |
| `iosHitbox` | `Vars.AimHitbox` (0–2) |
| `iosAimWhen` | `Vars.AimWhen` (0–3) |
| `iosBox` | `Vars.Box` |
| `iosName` | `Vars.Name` |
| `iosHealth` | `Vars.Health` |
| `iosDistance` | `Vars.Distance` |
| `iosSkeleton` | `Vars.skeleton` |
| `iosLines` | `Vars.lines` |
| `iosRenderDist` | `Vars.RenderDist` (0–600) |
| `iosStream` | `StreamerMode` |
| `iosFly` | `isfly` |
| `iosTele` | `istelekill` |
| `iosSpeed` | `SpeeeX2Enabled` + scan |
| `iosNoRecoil` | `NoRecoilEnabled` + scan |
| `iosWallGlow` | `WallGlowEnabled` + scan |
| `iosWallFly` | `WallFlyEnabled` + scan |
| `iosWallHack` | `WallHackEnabled` + scan |
| `iosScope` | `ScopeEnabled` + scan |

### 6.2 Aliases do painel antigo

| Chave antiga | Efeito iOS |
|---|---|
| `memEnabled`, `aimbot`, `aimEnabled`, `precisionAim`, `aimLegit` | `Vars.Aimbot` |
| `aimKill`, `silentEnabled` | `AimKill` |
| `aimAwm`, `visibleCheck` | `Vars.VisibleCheck` |
| `aimScope` | `Vars.AimWhen` 2/0 |
| `aimNeck` | `Vars.AimHitbox` 1/0 |
| `aimFov`, `fov`, `silentFov` | `Vars.AimFov` |
| `showFov`, `silentShowFov` | `Vars.isAimFov` |
| `espBox`, `boxType` | caixa e estilo |
| `espName` | nome |
| `espHealthBar`, `espHealthText` | vida |
| `espDistance` | distância |
| `espSkeleton` | skeleton |
| `espSnapLine`, `linePos` | linha e estilo |
| `espBoxFilled` | `Vars.Outline` |
| `hpPos` | vida + posição |
| `renderDist` | `Vars.RenderDist` |
| `noRecoil` | scan No Recoil |
| `speed`, `speedLite`, `speedHack` | scan Speed |
| `streamMode` | `StreamerMode` |
| `upPlayer`, `fly` | `isfly` |
| `teleport`, `telekill`, `teleport8m` | `istelekill` |
| `wallGlow`, `wallFly`, `wallHack`, `scope` | scans correspondentes |

Chaves sem equivalente no iOS (`spinBot`, `w*`, `fireDelay`, `autoRevive`,
`memStrength`, `key*`, `silentMaxDistance`, `silentIgnoreBots`) são aceitas e
ignoradas para o estado compartilhado não dessincronizar.

### 6.3 Ações

| Ação | Efeito no iOS |
|---|---|
| `load_memory` | confirma memória pronta; scans rodam sob demanda |
| `reload` | `game_sdk->init()` |
| `load_rage` | `Vars.Aimbot = true`, `AimKill = true` |
| `load_legit` | `Vars.Aimbot = true`, `AimKill = false`, `AimWhen = 1` |
| `load_chams` / `unload_chams` | responde indisponível |
| `bypass_ss` | alterna `StreamerMode` |
| `unload` | desliga tudo, responde e derruba o WebSocket |
| `destruct` | igual a `unload` |

---

## 7. Build do tweak

Requisitos: macOS ou Linux com Theos, SDK iOS, toolchain clang arm64.

```bash
export THEOS=$HOME/theos     # o Makefile usa THEOS ?= $(HOME)/theos
cd "src ios/nyverxs/nyverxs/nyverxs"
make clean package
```

O `Makefile` atual:

- `ARCHS = arm64`, `TARGET = iphone:clang:latest:13.6`
- `THEOS_PACKAGE_SCHEME = rootless` → instala em `/var/jb/...`
- `-fobjc-arc` em `CFLAGS` **e** `CCFLAGS` (o remote usa `__weak`/blocks)
- `Remote/*.mm` incluído em `_FILES`
- `API/` e `APIKey/` removidos da árvore; sem `LDFLAGS` de vendor

Instalação: seção 12 (GitHub Actions ou WSL2 geram o `.deb`; Sileo/Zebra ou
TrollFools instalam no aparelho).

O filtro atual é `com.garena.game.kgvn` (Liên Quân VN). Para outra região,
ajuste `NYVERXS.plist` para o bundle ID correspondente (`kgth`, `kgid`, `kgtw`).

---

## 8. Deploy e configuração do website

```env
PORT=80
HOST=0.0.0.0
BASE_URL=https://nologsontop.squareweb.app
NOLOGS_API_URL=https://camisaextragrandesnl.squareweb.app
NOLOGS_PRODUCT=remote_ios
DEVICE_LOGIN=1
DEVICE_TRIAL_DAYS=30
```

- `BASE_URL`: domínio público do site no Square Cloud (`SUBDOMAIN=nologsontop`
  no `squarecloud.config`). Controla o cookie `secure` e o log de boot.
- `NOLOGS_API_URL`: base da API Remote IOS. O login é
  `${NOLOGS_API_URL}/remote/id-login`; sem `NOLOGS_API_URL`, o default acima é
  usado.
- `NOLOGS_PRODUCT=remote_ios`: produto exigido pela API e gravado no claim do
  JWT. É o mesmo valor do produto **Remote iOS** no bot do Discord e o mesmo que
  o WebSocket do loader valida.
- `DEVICE_LOGIN=1`: cria conta local no primeiro login aprovado pela API.
- `DEVICE_TRIAL_DAYS=30`: janela local espelhada; se a API aprovar um ID com
  cópia local vencida, a expiração é estendida por esse valor.
- Não existe mais `DEVICE_API_CHECK`: a API Remote IOS não é opcional, é a
  autoridade de login. Se ela não responde, o site responde `503`.
- `painel.html` é lido para memória no boot; reinicie o processo após editar.
- `/painel.html` exige cookie de sessão **e** loader conectado. O iPhone entra
  como loader assim que o WebSocket abre.

Passo a passo do operador:

1. Instala o tweak e abre o jogo.
2. No menu **REMOTE**, digita o Discord ID e o servidor (`https://...`),
   toca em **Conectar**. O SID é gerado e salvo sozinho.
3. Abre o site no navegador, entra com o **mesmo Discord ID**.
4. O painel mostra `loader online`; a seção **iOS / LQ** controla o aparelho.
5. O botão **Desconectar** desliga os módulos e encerra o vínculo; não fecha o
   jogo.

---

## 9. Teste executado

Ambiente local (`Node 24`, porta 8099, banco temporário, mock da API Remote IOS
na porta 8100 respondendo `/remote/id-login`):

1. `POST /api/device/auth` com Discord ID de 16 dígitos → `400`.
2. ID remoto desconhecido ou pausado → `401` (erro da API repassado como recusa).
3. API remota com `5xx`/timeout → `503`.
4. `POST /api/device/auth` com ID aprovado + SID → `200`, token de loader.
5. WebSocket `/?role=loader` com esse token → aberto.
6. `POST /api/auth/discord` com o mesmo ID → token de navegador.
7. `POST /api/state` `{iosAimbot:true}` → loader recebe
   `{"action":"set","param":"iosAimbot","value":"true"}`.
8. `POST /api/state` `{iosAimFov:175}` → relay correto.
9. Loader envia `status` → `GET /api/cheat/info` devolve `platform=ios`,
   `memory_status=pronta`.
10. `GET /api/state` devolve o patch persistido e `loader=true`.
11. Segundo `device/auth` reutiliza a mesma conta (sem duplicar usuário).
12. Login somente pelo navegador provisiona a conta local antes de abrir o app.
13. Nova checagem na API remota a cada login: ID revogado deixa de entrar mesmo
    com JWT local ainda válido.

Resultado: `ALL CHECKS PASSED`.

Não foi possível compilar a dylib neste ambiente Windows (sem Theos/Xcode). A
validação do lado iOS é estática: parser de chaves com estados (`final depth 0`),
revisão de assinaturas, includes, ARC e fluxo de chamadas. Compile no macOS/Linux
com Theos antes de instalar.

---

## 10. Limitações conhecidas

- Offsets de função e de struct são de um build específico do jogo; update do
  Liên Quân quebra a tabela. Re-derive com IDA/Ghidra e o dump
  `Helper/dump.cpp`.
- A camada `v7a` foi montada a partir do dump 32-bit fornecido, adaptada para
  arm64. Se o binário iOS atual não bater, a cadeia não valida e cada leitura cai
  na função nativa antiga; nenhuma feature fica pior que antes, mas o ganho do
  `v7a` só aparece quando a base e os offsets conferem.
- `WallHack` não restaura os valores originais ao desligar.
- Chams, skins, spinbot, weapon mods, fire delay e auto-revive existem no painel
  Windows, mas não têm implementação neste tweak iOS. As chaves são aceitas e
  ignoradas.
- `get_skins` responde catálogo vazio; a página de skins do painel não tem
  backend iOS.
- O remote não fecha o jogo: `unload` desliga módulos e derruba o WebSocket.
- O cliente usa `NSURLSessionWebSocketTask` (iOS 13+), sem dependência de
  CydiaSubstrate além do que o Theos já injeta.
- A conta local é por Discord ID. Vários iPhones com o mesmo ID disputam o
  mesmo loader; o último WebSocket registrado recebe os comandos.

---

## 11. Resumo das alterações

| Arquivo | Alteração |
|---|---|
| `lostwq.mm` | Aba REMOTE; `AimKill`/`isfly`/`istelekill` no menu; toggles de memória chamados; `RunTelekill`/`RunUpPlayer` no loop; superfície `nyv_remote_*` |
| `Helper/Hooks.h` | `RenderDist`; mira antes do gate de ESP; `AimKill` no alvo; sem `ProcessAimbot` duplicado; wrappers `MatchV7`/`LocalV7`/`IsDeadV7`/`HealthV7`/`NameV7`/`PositionV7`; `ForEachPlayer`; W2S por matriz `v7a` |
| `Helper/OffsetsV7a.h` | Struct `v7a`, containers 64-bit, cadeia com sondagem de base, entidades, ossos, matriz/câmera, escrita segura |
| `Remote/RemoteClient.mm` | Validação do Discord ID em 17–20 dígitos, alinhada ao servidor |
| `Esp/LoadView.mm` | Gate `APIClient paid:` removido; menu direto; auto-start do remote |
| `Makefile` | `THEOS ?= $(HOME)/theos`; `TWEAK_NAME = NYVERXS`; `Remote/*.mm`, ARC em ObjC++, lib do vendor removida |
| `NYVERXS.plist` | Renomeado de `@NYVERXS.plist`; filtro só `com.garena.game.kgvn` |
| `control` | Pacote `com.nologs.nyverxs`, versão 1.0.0 |
| `.github/workflows/build.yml` | Build do `.deb` em runner macOS (sem MacBook) |
| `build-linux.sh` | Build do `.deb` em WSL2/Ubuntu |
| `API/`, `APIKey/`, `packages/`, `.theos/` | Removidos da árvore (~231 MB de peso morto) |
| `website/services/nologs_api.js` | `loginRemote` → `POST /remote/id-login` com `product: NOLOGS_PRODUCT` |
| `website/services/remote_account.js` | `authorizeRemoteAccount`: valida na API, provisiona/atualiza a conta local |
| `website/routes/device.js` | Login por Discord ID + SID; autoridade na API Remote IOS; 400/401/429/503 |
| `website/routes/auth.js` | `/api/auth/discord` valida na API e provisiona a conta local |
| `website/config.js` | `NOLOGS_API_URL`, `NOLOGS_PRODUCT=remote_ios`, `DEVICE_LOGIN`, `DEVICE_TRIAL_DAYS`; sem `DEVICE_API_CHECK` |
| `website/server.js` | Mount de `/api/device` |
| `website/public/script.js` | Validação `^\d{17,20}$` no login |
| `website/public/index.html` | `maxlength=20` no campo de Discord ID |
| `website/public/painel.html` | Seção **iOS / LQ**, status iOS, confirmação de unload |
| `website/.env.example` | Variáveis do device login |

---

## 12. Build e teste sem MacBook

O Theos precisa de macOS ou Linux. Windows puro não compila tweak iOS; o que
funciona é usar um runner macOS (GitHub Actions) ou um Ubuntu dentro do WSL2.
Os dois caminhos produzem o mesmo pacote:

```
packages/com.nologs.nyverxs_1.0.0_iphoneos-arm64.deb
```

### 12.1 GitHub Actions (recomendado)

O workflow está em `.github/workflows/build.yml`. Ele roda em `macos-14`,
instala `ldid`, `xz` e `dpkg`, clona o Theos com submódulos, baixa **um** SDK
do `theos/sdks` via sparse checkout (o repositório completo tem ~2 GB) e roda
`make clean package FINALPACKAGE=1`. Sobe como artifact:

- `packages/*.deb` — instalação via Sileo/Zebra (rootless)
- `.theos/obj/arm64/NYVERXS.dylib` — injeção via TrollFools

Primeiro envio do projeto:

```powershell
cd "C:\Users\<voce>\Downloads\src ios\src ios\nyverxs\nyverxs\nyverxs"
git init
git add .
git commit -m "nyverxs"
git branch -M main
git remote add origin https://github.com/<usuario>/nyverxs.git
git push -u origin main
```

A aba **Actions** roda o build no push. Para gerar um Release (o iPhone baixa
o `.deb` direto pelo Safari, sem passar pelo PC):

```powershell
git tag v1.0.0
git push origin v1.0.0
```

O Release fica em `https://github.com/<usuario>/nyverxs/releases`. Tempo típico
do primeiro build: 6 a 10 min (o SDK é o gargalo); os seguintes caem para ~3 min.

A pasta do tweak está com ~25 MB depois da limpeza (`.theos` e `packages` são
cache de build e voltam sozinhos).

### 12.2 WSL2 + Ubuntu

```powershell
wsl --install -d Ubuntu-22.04
```

Depois de reiniciar e criar o usuário Linux:

```bash
cd "/mnt/c/Users/<voce>/Downloads/src ios/src ios/nyverxs/nyverxs/nyverxs"
bash build-linux.sh
```

`build-linux.sh` instala as dependências (build-essential, fakeroot, libssl,
libplist, libtool, autoconf, automake, pkg-config), baixa o `ldid` do
ProcursusTeam, clona o Theos com submódulos, baixa um SDK e chama
`make package`. O primeiro run baixa ~300 MB entre Theos e SDK.

Detalhes que costumam travar:

- Compilar em `/mnt/c` funciona, mas é lento (DrvFs). Se o build demorar demais,
  copie o projeto para `~/nyverxs` e rode de lá.
- `THEOS` agora é `?= $(HOME)/theos` no Makefile. Para apontar outro caminho:
  `make THEOS=/opt/theos package`.
- Se `ldid` não existir, o Theos falha na assinatura ad-hoc do dylib.

### 12.3 Instalar no iPhone

O tweak é injetado por MobileSubstrate; sem jailbreak ou TrollStore não há
caminho — o app da App Store é assinado e não carrega dylib de fora.

**Jailbreak rootless (Dopamine/palera1n rootless):**

1. No PC, dentro de `packages/`, suba um servidor HTTP:
   `python -m http.server 8080`
2. No iPhone, mesma rede Wi-Fi: `http://<ip-do-pc>:8080/` → toque no `.deb` →
   Compartilhar → Sileo/Zebra → Instalar.
   Alternativa: Filza abre o `.deb` e instala; ou NewTerm como root:
   `dpkg -i /var/mobile/com.nologs.nyverxs_1.0.0_iphoneos-arm64.deb`.
3. Confira os arquivos instalados:
   `/var/jb/Library/MobileSubstrate/DynamicLibraries/NYVERXS.dylib` e
   `NYVERXS.plist`. Um respring (`sbreload`) garante o relê do filtro.
4. `dpkg -l | grep nyverxs` deve listar `1.0.0`.

**TrollStore + TrollFools (sem jailbreak):**

1. Baixe o `NYVERXS.dylib` no iPhone (Release do GitHub ou servidor local).
2. TrollFools → escolha o Liên Quân Mobile → **Inject** → selecione o dylib.
3. Reabra o jogo. O TrollFools assina o bundle de novo; o TrollStore precisa
   estar com o CoreTrust ativo (iOS 14.0–17.0, conforme o aparelho).

**Filtro de bundle.** `NYVERXS.plist` lista `com.garena.game.kgvn` (Liên Quân
VN). Se o jogo instalado for de outra região, o dylib carrega mas nunca injeta.
Para conferir o ID real:

```bash
python -m pymobiledevice3 apps list --type user | findstr -i kg
```

Ou Filza → app → `Info.plist` → `CFBundleIdentifier`. Troque o ID no plist e
reinstale.

### 12.4 Logs do iPhone no Windows

`pymobiledevice3` fala com o dispositivo pelo usbmuxd do iTunes/Apple Devices.
Instale uma vez:

```powershell
pip install -U pymobiledevice3
```

Stream ao vivo (deixe rodando enquanto abre o jogo):

```powershell
python -m pymobiledevice3 syslog live
```

O stream mostra todos os processos. O executável do jogo é `kgvn`; para cortar
o resto:

```powershell
python -m pymobiledevice3 syslog live | Select-String kgvn
```

Crash logs (se o jogo fechar sozinho na injeção):

```powershell
python -m pymobiledevice3 crash pull
```

O 3uTools faz o mesmo por interface: **Toolbox → Real-time Log** e
**Crash Analysis**. O log do Metal (`imgui_impl_metal.mm`) aparece como
`Error: failed to create Metal ...` quando o overlay não sobe.

O que procurar:

- Menu não aparece: linha de crash no `SpringBoard`/jogo, ou filtro de bundle
  errado.
- Menu aparece e o remote não conecta: falha de `POST /api/device/auth` no log
  do servidor (`[remote-account] recusado ...`).
- Conecta e o ESP não desenha: cadeia `v7a` não resolveu (ver 12.5).

### 12.5 Roteiro do primeiro teste

1. **Servidor no ar.** `node server.js` com `NOLOGS_API_URL` apontando para a
   API Remote IOS. O Discord ID precisa estar ativo lá; o site só espelha a
   decisão da API.
2. **Jogo aberto.** O menu sobe 7 s depois do `didFinishLaunching`. Abre de
   duas formas: toque no botão invisível de 50×50 no canto superior esquerdo
   (arrastável) ou **três dedos, toque duplo** em qualquer ponto da tela.
3. **Login.** Aba REMOTE → Discord ID (17 a 20 dígitos) → conectar. No servidor
   deve aparecer `[ws] loader conectado user=N`. O ID fica salvo; nas próximas
   aberturas o remote sobe sozinho.
4. **Veredito dos offsets.** Ligue o ESP. Caixa/linha/nome em cima dos
   jogadores = a cadeia `v7a` (`InitBase 0xA342EFC`) bateu no binário atual.
   Menu funciona, remote conecta, mas ESP vazio → a sonda rejeitou a base e
   tudo caiu no caminho nativo. Nesse caso os offsets precisam ser re-derivados
   do binário instalado (seção 3.6), não é bug de build.
5. **Aimbot/telekill.** Mira usa `set_aim` nativo; telekill usa
   `Transform_SetPosition` + `PlayerID`. Se o ESP está certo e a mira erra, o
   problema é de osso/raycast, não da camada `v7a`.
6. **Painel.** Com o loader conectado, os toggles vão por
   `POST /api/cheat/commands` e chegam no WebSocket do iPhone. Confirme no log
   do servidor e no estado da aba iOS.
7. **Conta de teste.** Use uma conta descartável: o jogo pode marcar a conta no
   primeiro voo com ESP/aimbot, e isso invalida o resto do teste.

### 12.6 Falhas comuns de build

| Sintoma | Causa | Correção |
|---|---|---|
| `make: *** No rule to make target .../common.mk` | `THEOS` errado | `make THEOS=$HOME/theos package` |
| `unable to find SDK` | `$THEOS/sdks` vazio | rodar de novo o passo de SDK |
| `ldid: command not found` | assinatura ad-hoc ausente | `brew install ldid` / script Linux |
| `fatal error: '...' file not found` em C++ | include relativo fora do lugar | includes do projeto são relativos à raiz (`Helper/...`) |
| Build passa, nada no jogo | filtro de bundle | conferir `NYVERXS.plist` (12.3) |
| Build passa, jogo fecha ao abrir | dylib carregou e crashou | `pymobiledevice3 crash pull` |
