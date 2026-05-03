#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyCharacter.h"
#include "Component/BulletHellComponent.h"
#include "BossBase.generated.h"
//보스의 패턴이 끝날 시 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPatternFinished);
// 보스 스폰 시 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossSpawn);
//보스 사망 시 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDie);
// 보스의 체력이 바뀌면 호출되는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossHealthChanged, float, CurrentHealth, float, MaxHealth);

UCLASS()
class TEAMPOTATO_API ABossBase : public AEnemyCharacter
{
    GENERATED_BODY()

public:
    ABossBase();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //데미지와 사망 처리
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    virtual void OnDie() override;

    //이속 변경
    virtual float SetMovementSpeed_Implementation(EEnemySpeed State) override;

public:
    //현재 페이즈(지금은 사용 안함)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|Status")
    int32 CurrentPhase = 1;

    //패턴 실행하는 함수
    UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
    virtual void ExecutePattern(int32 PatternIndex);

    UFUNCTION(BlueprintPure, Category = "Boss|Status")
    float GetCurrentHealthValue() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "Boss|Status")
    float GetMaxHealthValue() const { return MaxHealth; }

    //탄막(Bullet Hell) 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UBulletHellComponent* BulletPatternComp;

    //패턴 종료시 델리게이트 호출하는 함수(블루프린트에서 씀)
    UFUNCTION(BlueprintCallable, Category = "Boss|Combat")
    void FinishCurrentPattern();

    //패턴 종료 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Boss|Event")
    FOnPatternFinished OnPatternFinished;

    // 보스가 스폰되었을 때 호출되는 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Boss|Event")
    FOnBossSpawn OnBossSpawn;

    // 보스가 사망했을 때 호출되는 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Boss|Event")
    FOnBossDie OnBossDie;

    //보스 체력 변경시 나오는 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Boss|Event")
    FOnBossHealthChanged OnBossHealthChanged;

protected:
    //보스 패턴들 실제 구현은 블루프린트에서 함
    UFUNCTION(BlueprintNativeEvent, Category = "Boss|Combat")
    void PatternOne();

    UFUNCTION(BlueprintNativeEvent, Category = "Boss|Combat")
    void PatternTwo();

    UFUNCTION(BlueprintNativeEvent, Category = "Boss|Combat")
    void PatternThree();

    UFUNCTION(BlueprintNativeEvent, Category = "Boss|Combat")
    void PatternFour();

    UFUNCTION(BlueprintNativeEvent, Category = "Boss|Combat")
    void PatternFive();
};
