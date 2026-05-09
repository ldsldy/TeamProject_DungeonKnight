# 헤더 기준 소스 구조

이 문서는 Dungeon Knight의 C++ 소스 구조를 헤더 파일 기준으로 정리한 문서입니다.
각 시스템의 인터페이스, 데이터 타입, 기능 경계를 확인하기 쉽도록 `Public` 디렉토리를 중심으로 구성했습니다.

## 전체 구조

```txt
Source
+-- TeamPotato
|   +-- TeamPotato.h
|   +-- PotatoGameplayTags.h
|
|   +-- Logic
|   |   +-- DungeonGanarator.h
|   |   +-- Door.h
|   |   +-- ClosingWall.h
|
|   +-- Room
|   |   +-- RoomBase.h
|   |   +-- DungeonRoom*.h
|
|   +-- Public
|       +-- AI
|       |   +-- Decorator
|       |   +-- Survice
|       |   +-- Task
|       |
|       +-- Animation
|       |   +-- Notify
|       |
|       +-- Common
|       +-- Component
|       |
|       +-- Data
|       |   +-- Object
|       |
|       +-- Enemy
|       |   +-- Bullet
|       |   +-- DamagePopup
|       |
|       +-- IntetFace
|       |
|       +-- Item
|       |   +-- Weapon
|       |
|       +-- Player
|       |   +-- Perk
|       |
|       +-- Subsystem
|       |   +-- Save
|       |   +-- ViewModel
|       |       +-- Fields
|       |
|       +-- UI
|           +-- CharacterSelect
|           +-- Enemy
|           +-- InGameMenu
|           +-- MainMenu
|           +-- Minimap
|           +-- Perk
|           +-- Player
```

## 디렉토리 역할

| 디렉토리 | 역할 |
|---|---|
| `Logic` | 런타임 던전 생성, 문, 닫힌 벽처럼 레벨 진행에 직접 연결되는 게임플레이 로직을 관리합니다. |
| `Room` | 던전 생성에 사용되는 방 Actor와 방 기반 클래스를 관리합니다. |
| `AI` | 적의 행동 판단을 위한 AI Controller, Behavior Tree Task, Decorator, Service를 관리합니다. |
| `Animation` | 플레이어 애니메이션 인스턴스와 공격, 콤보, 무적, 입력 제한 등 AnimNotify를 관리합니다. |
| `Common` | Developer Settings, 게임 상태 타입처럼 여러 시스템이 공유하는 공통 설정과 타입을 관리합니다. |
| `Component` | 플레이어 자원, 무기, Perk, Wave, 탄막처럼 Actor에 부착되는 기능 단위를 관리합니다. |
| `Data` | 캐릭터, 무기, Perk, 레벨 이동 등 데이터 기반 콘텐츠 구성을 위한 DataAsset과 DataTable Row를 관리합니다. |
| `Enemy` | 일반 적, 엘리트, 보스, 투사체, 데미지 팝업 등 적 전투 객체를 관리합니다. |
| `IntetFace` | 적, 상호작용, 픽업처럼 시스템 간 연결에 사용되는 인터페이스를 관리합니다. |
| `Item` | 픽업 아이템, 상점, 무기 Actor, 무기 상자 등 아이템 관련 런타임 객체를 관리합니다. |
| `Player` | 플레이어 캐릭터, 컨트롤러, 애니메이션, PerkEffect 등 플레이어 중심 기능을 관리합니다. |
| `Subsystem` | 게임 상태, 미니맵, 오디오, 저장, 캐릭터, MVVM, 풀링 등 전역 시스템을 관리합니다. |
| `UI` | HUD, 미니맵, Perk 선택, 메인 메뉴, 인게임 메뉴, 캐릭터 선택 등 UMG 위젯을 관리합니다. |

## 기능별 구조

| 기능 | 주요 헤더 위치 |
|---|---|
| 런타임 랜덤 던전 생성 | `Logic/DungeonGanarator.h`, `Room/RoomBase.h`, `Room/DungeonRoom*.h` |
| 레벨 이동과 로딩 화면 | `Subsystem/GameStateSubsystem.h`, `Data/LevelDataAsset.h`, `Common/MyGameSettings.h` |
| 미니맵과 Fog of War | `Subsystem/MinimapSubsystem.h`, `UI/Minimap` |
| Perk 선택과 장착 | `Component/PerkComponent.h`, `UI/Perk`, `Subsystem/ViewModel/PerkViewModel.h` |
| PerkEffect 확장 | `Player/Perk/PerkEffectBase.h`, `Player/Perk/BuffPerkEffect.h`, `Data/PerkDataAsset.h` |
| 오디오 옵션 | `Subsystem/AudioSubsystem.h`, `UI/MainMenu/MainMenuSoundOptionWidget.h`, `Common/MyGameSettings.h` |
| SaveGame 설정 저장 | `Subsystem/SaveGameSubsystem.h`, `Subsystem/Save/SaveSettings.h` |
| 캐릭터 선택과 플레이어 상태 | `Subsystem/CharacterSubsystem.h`, `Subsystem/Save/PlayerSaveData.h`, `Component/PlayerResource.h` |
| 적 AI와 전투 | `AI`, `Enemy`, `Component/WeaponComponent.h`, `Component/PlayerResource.h` |

## 대표 헤더

| 영역 | 대표 헤더 |
|---|---|
| 던전 생성 | `DungeonGanarator.h`, `RoomBase.h`, `Door.h`, `ClosingWall.h` |
| 게임 상태 / 레벨 이동 | `GameStateSubsystem.h`, `GameStateTypes.h`, `LevelDataAsset.h`, `MyGameSettings.h` |
| 미니맵 | `MinimapSubsystem.h`, `MinimapSceneCapture2D.h`, `MinimapWidget.h` |
| Perk | `PerkComponent.h`, `PerkDataAsset.h`, `PerkDataTableRow.h`, `PerkCardWidget.h`, `PerkSelectionScreenWidget.h` |
| PerkEffect | `PerkEffectBase.h`, `BuffPerkEffect.h` |
| 오디오 / 저장 | `AudioSubsystem.h`, `SaveGameSubsystem.h`, `SaveSettings.h`, `MainMenuSoundOptionWidget.h` |
| 플레이어 | `TestCharacter.h`, `TestPlayerController.h`, `PlayerResource.h`, `WeaponComponent.h` |
| 적 / AI | `EnemyCharacter.h`, `BossBase.h`, `MyAIController.h`, `BTT_DefaultAttack.h`, `RangeCheck.h` |
| UI / MVVM | `MainHUD.h`, `MainHUDWidget.h`, `PGViewModelBase.h`, `PlayerResourceViewModel.h`, `PerkViewModel.h`, `WeaponViewModel.h` |
