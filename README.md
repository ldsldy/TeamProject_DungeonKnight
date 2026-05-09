# Dungeon Knight

<p align="center">
  <img src="./Document/Images/Title.png" alt="Dungeon Knight 타이틀" width="100%" />
</p>

## 프로젝트 개요

| 항목 | 내용 |
|---|---|
| 개발 기간 및 인원 | 2025.12.11 ~ 2026.01.02, 4인 |
| 사용 엔진 버전 | Unreal Engine 5.5 |
| 대상 플랫폼 | PC |

## 프로젝트 소개

**Dungeon Knight**는 런타임에 생성되는 던전을 탐험하며 전투와 성장 선택을 반복하는 **3D 액션 로그라이트 프로젝트**입니다.  
플레이어는 무기와 Perk를 활용해 매 스테이지마다 달라지는 던전을 돌파하고, 전투 중 획득한 성장 선택지를 통해 캐릭터를 강화해 나갑니다.

## 폴더 구조

프로젝트의 C++ 헤더 기준 소스 구조는 별도 문서로 정리했습니다.  
전체 디렉토리 구성과 주요 시스템별 역할은 [헤더 기준 소스 구조](./Document/SourceStructure.md)에서 확인할 수 있습니다.

## 주요 기능 소개

### 1) 런타임 랜덤 던전 생성
- 매 플레이마다 달라지는 던전을 런타임에 생성해 반복 플레이의 변화를 제공
- 일반 방, 특수 방, 보스 방을 조합해 스테이지 진행 흐름을 구성
- 생성된 던전 정보를 미니맵과 로딩 시스템에 연결
- 관련 구현: [DungeonGanarator](./Source/TeamPotato/Logic/DungeonGanarator.cpp), [RoomBase](./Source/TeamPotato/Room/RoomBase.cpp), [Door](./Source/TeamPotato/Logic/Door.cpp), [ClosingWall](./Source/TeamPotato/Logic/ClosingWall.cpp)

### 2) 데이터 기반 레벨 이동과 Loading Gate
- 레벨 이동 정보를 데이터로 관리해 상태별 전환 대상을 유연하게 설정
- 로딩 화면을 통해 레벨 전환 중 플레이어에게 명확한 대기 피드백 제공
- 던전 생성과 렌더링 리소스 준비 상태를 고려해 로딩창 제거 타이밍 개선
- 관련 구현: [GameStateSubsystem](./Source/TeamPotato/Private/Subsystem/GameStateSubsystem.cpp), [LevelDataAsset](./Source/TeamPotato/Private/Data/LevelDataAsset.cpp), [MyGameSettings](./Source/TeamPotato/Public/Common/MyGameSettings.h)

### 3) SceneCapture 기반 런타임 미니맵
- 런타임에 생성되는 랜덤 던전 구조에 대응하는 미니맵 제공
- 던전 전체 구조와 플레이어 위치를 UI에서 직관적으로 확인할 수 있도록 구성
- 던전 생성 이벤트와 연동해 미니맵을 자동으로 갱신
- 관련 구현: [MinimapSubsystem](./Source/TeamPotato/Private/Subsystem/MinimapSubsystem.cpp), [MinimapSceneCapture2D](./Source/TeamPotato/Private/UI/Minimap/MinimapSceneCapture2D.cpp), [MinimapWidget](./Source/TeamPotato/Private/UI/Minimap/MinimapWidget.cpp)

### 4) Fog of War 미니맵 탐색 표현
- 탐험한 길과 아직 가보지 않은 영역을 구분해 던전 탐색 경험을 강화
- 플레이어 이동에 따라 미니맵 정보가 점진적으로 밝혀지도록 표현
- 불필요한 갱신을 줄여 미니맵 업데이트 비용을 관리
- 관련 구현: [MinimapSubsystem](./Source/TeamPotato/Private/Subsystem/MinimapSubsystem.cpp), [MinimapWidget](./Source/TeamPotato/Private/UI/Minimap/MinimapWidget.cpp)

### 5) 데이터 드리븐 Perk 선택 시스템
- Perk 정보를 데이터로 분리해 새로운 성장 선택지를 쉽게 추가할 수 있도록 구성
- 가중치 기반 랜덤 선택으로 매번 다른 Perk 후보를 제공
- Perk 선택 UI와 실제 장착/효과 적용 흐름을 분리해 확장성 확보
- 관련 구현: [PerkSelectionScreenWidget](./Source/TeamPotato/Private/UI/Perk/PerkSelectionScreenWidget.cpp), [PerkCardWidget](./Source/TeamPotato/Private/UI/Perk/PerkCardWidget.cpp), [PerkDataTableRow](./Source/TeamPotato/Public/Data/PerkDataTableRow.h), [PerkDataAsset](./Source/TeamPotato/Public/Data/PerkDataAsset.h)

### 6) 오버라이딩 기반 PerkEffect 구조
- Perk별 효과 로직을 독립적으로 확장할 수 있는 구조 설계
- Component는 공통 호출 흐름만 담당하고, 세부 효과는 Effect 클래스에서 처리
- 새로운 성장 효과를 추가할 때 기존 장착 로직의 변경을 최소화
- 관련 구현: [PerkComponent](./Source/TeamPotato/Private/Component/PerkComponent.cpp), [PerkEffectBase](./Source/TeamPotato/Public/Player/Perk/PerkEffectBase.h), [BuffPerkEffect](./Source/TeamPotato/Private/Player/Perk/BuffPerkEffect.cpp), [PlayerResource](./Source/TeamPotato/Private/Component/PlayerResource.cpp)

### 7) 오디오 옵션과 SaveGame 설정 저장
- 음악과 효과음 볼륨을 분리해 플레이어가 원하는 사운드 환경을 설정 가능
- 오디오 리소스 참조와 실제 볼륨 적용 책임을 분리해 관리
- 변경된 옵션 값을 저장해 재실행 이후에도 설정 유지
- 관련 구현: [AudioSubsystem](./Source/TeamPotato/Private/Subsystem/AudioSubsystem.cpp), [SaveGameSubsystem](./Source/TeamPotato/Private/Subsystem/SaveGameSubsystem.cpp), [SaveSettings](./Source/TeamPotato/Public/Subsystem/Save/SaveSettings.h), [MainMenuSoundOptionWidget](./Source/TeamPotato/Private/UI/MainMenu/MainMenuSoundOptionWidget.cpp)

## 수행 업무

### 런타임 던전 연동 시스템 구현
- 던전 생성 완료 시점의 이벤트를 미니맵과 로딩 시스템에 연결
- 랜덤 던전 구조 변화에 대응하도록 SceneCapture 기반 미니맵 캡처와 던전 크기 계산 흐름 구성

### 미니맵과 Fog of War 구현
- 플레이어 위치를 미니맵 UV 좌표로 변환하고, 타이머 기반 갱신과 이동 거리 검증으로 업데이트 비용 절감
- 탐험 영역을 텍스처에 누적 기록하여 플레이어가 지나온 길과 미지의 영역을 구분

### Perk 시스템 구현
- DataTable 기반 가중치 랜덤 선택으로 매번 다른 Perk 후보 제공
- Perk 장착, Effect 적용, ViewModel을 통한 UI 갱신 흐름 구성
- `OnEquip` 오버라이딩 기반 Effect 구조로 새로운 Perk 효과를 확장할 수 있도록 설계

### 레벨 이동과 로딩 화면 개선
- DeveloperSettings와 LevelDataAsset을 활용한 데이터 기반 레벨 이동 구조 구현
- 던전 생성 완료 이후 스트리밍/PSO 준비 상태까지 확인하는 Loading Gate로 첫 진입 품질 개선

### 오디오 옵션과 설정 저장 구현
- SoundMix와 SoundClass를 DeveloperSettings로 관리하고, AudioSubsystem에서 전역 볼륨 적용
- SaveGameSubsystem을 통해 사운드 옵션 값을 디스크에 저장 및 로드
