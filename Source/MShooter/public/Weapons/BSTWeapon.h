#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSTWeapon.generated.h"

class USphereComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EBSTWeaponState : uint8
{
	Initial,
	Equipped,
	Dropped
};

UCLASS()
class MSHOOTER_API ABSTWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABSTWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	
	
	void ShowPickupWidget(bool bShowWidget);

	void SetWeaponState(EBSTWeaponState NewWeaponState);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPickupBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPickupEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_WeaponState();

protected:
	UPROPERTY(VisibleAnywhere, Category= "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USphereComponent* PickupSphere;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_WeaponState, Category="Weapon")
	EBSTWeaponState WeaponState = EBSTWeaponState::Initial;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UWidgetComponent* PickupWidget;
};