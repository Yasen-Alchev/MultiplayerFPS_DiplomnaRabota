#include "MultiplayerFPSHealthSystem.h"
#include "../Default/MultiplayerFPSCharacter.h"
#include "../Default/MultiplayerFPSPlayerController.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

UMultiplayerFPSHealthSystem::UMultiplayerFPSHealthSystem()
{
	PrimaryComponentTick.bCanEverTick = false;

	this->MaxHealth = 0.0f;
	this->CurrentHealth = this->MaxHealth;

	this->MaxShield = 0.0f;
	this->CurrentShield = this->MaxShield;

	this->ShieldRechargeRate = 0.0f;
	this->ShieldRechargeCooldownAfterDamage = 0.0f;

	this->ShieldRateCounter = 0;

	SetIsReplicatedByDefault(true);
}

void UMultiplayerFPSHealthSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMultiplayerFPSHealthSystem, CurrentHealth);
	DOREPLIFETIME(UMultiplayerFPSHealthSystem, CurrentShield);
}

void UMultiplayerFPSHealthSystem::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSHealthSystem::BeginPlay !IsValid(OwnerActor)"));
		return;
	}

	this->CurrentHealth = this->MaxHealth;
	this->CurrentShield = this->MaxShield;

	OwnerActor->OnTakeAnyDamage.AddDynamic(this, &UMultiplayerFPSHealthSystem::TakeDamage);
}

void UMultiplayerFPSHealthSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMultiplayerFPSHealthSystem::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSHealthSystem::TakeDamage !IsValid(OwnerActor)"));
		return;
	}

	FTimerDelegate ShieldRechargeDelegate;
	ShieldRechargeDelegate.BindUFunction(this, FName("StartShieldRecharge"));

	if (OwnerActor->GetWorldTimerManager().IsTimerActive(this->ShieldRechargeStartTimer))
	{
		OwnerActor->GetWorldTimerManager().ClearTimer(this->ShieldRechargeStartTimer);
		OwnerActor->GetWorldTimerManager().SetTimer(this->ShieldRechargeStartTimer, ShieldRechargeDelegate, this->ShieldRechargeCooldownAfterDamage, false);
	}
	else
	{
		OwnerActor->GetWorldTimerManager().SetTimer(this->ShieldRechargeStartTimer, ShieldRechargeDelegate, this->ShieldRechargeCooldownAfterDamage, false);
	}

	if (OwnerActor->GetWorldTimerManager().IsTimerActive(this->ShieldRechargeTimer))
	{
		OwnerActor->GetWorldTimerManager().ClearTimer(this->ShieldRechargeTimer);
	}

	if (this->CurrentShield == 0.0f)
	{
		this->CurrentHealth = FMath::Clamp(this->CurrentHealth - Damage, 0.0f, this->MaxHealth);

		UE_LOG(LogTemp, Warning, TEXT("Health Damage Taken - %.2f"), Damage);
	}
	else
	{
		this->CurrentShield = FMath::Clamp(this->CurrentShield - Damage, 0.0f, this->MaxShield);

		UE_LOG(LogTemp, Warning, TEXT("Shield Damage Taken - %.2f"), Damage);
	}

	OnHealthChangedEvent.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UMultiplayerFPSHealthSystem::StartShieldRecharge()
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSHealthSystem::StartShieldRecharge !IsValid(OwnerActor)"));
		return;
	}

	FTimerDelegate ShieldRechargeDelegate;
	ShieldRechargeDelegate.BindUFunction(this, FName("RechargeShield"));

	OwnerActor->GetWorldTimerManager().ClearTimer(this->ShieldRechargeStartTimer);

	OwnerActor->GetWorldTimerManager().SetTimer(this->ShieldRechargeTimer, ShieldRechargeDelegate, this->ShieldRechargeInterval, true, 0.0f);
}

void UMultiplayerFPSHealthSystem::RechargeShield()
{
	AActor* OwnerActor = GetOwner();
	if (!IsValid(OwnerActor))
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiplayerFPSHealthSystem::StartShieldRecharge !IsValid(OwnerActor)"));
		return;
	}

	ShieldRateCounter++;

	if (ShieldRateCounter % 10 == 0)
	{
		this->ShieldRechargeRate += this->ShieldRechargeRateIncreaseValue;
		ShieldRateCounter = 0;
	}

	this->CurrentShield = FMath::Clamp(this->CurrentShield + this->ShieldRechargeRate, 0.0f, this->MaxShield);
	UE_LOG(LogTemp, Warning, TEXT("Shield is at: %.2f"), this->CurrentShield);

	if (this->CurrentShield == this->MaxShield)
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(this->ShieldRechargeTimer);
		
		return;
	}
}
	
void UMultiplayerFPSHealthSystem::Heal(float Value)	
{
	if (Value == 100)
	{
		this->CurrentHealth = this->MaxHealth;

		UE_LOG(LogTemp, Warning, TEXT("Player Health Restored!"));
	}
	else
	{
		this->CurrentHealth = FMath::Clamp(this->CurrentHealth + Value, 0.0f, this->MaxHealth);

		UE_LOG(LogTemp, Warning, TEXT("Player Healed for: %.2f"), Value);
		UE_LOG(LogTemp, Warning, TEXT("Current Player Health: %.2f"), this->CurrentHealth);
	}
}