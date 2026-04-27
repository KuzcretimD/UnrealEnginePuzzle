// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzles/PuzzleWall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AnimInstanceSandboxCharacter_CMC.h"
#include "Blueprint/UserWidget.h"
// Sets default values
APuzzleWall::APuzzleWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));
	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	RootComponent = WallMesh;

	InteractionPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractionPoint"));
	InteractionPoint->SetupAttachment(RootComponent);

	PuzzleCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Puzle Camera"));
	PuzzleCamera->SetupAttachment(RootComponent);

	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	InteractionZone->SetupAttachment(RootComponent);
	InteractionZone->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	WorldHandTargetLocation = FVector(0, 0, 0);

}

// Called when the game starts or when spawned
void APuzzleWall::BeginPlay()
{
	Super::BeginPlay();

	TInlineComponentArray<UActorComponent*> AllComponents;
	GetComponents<UActorComponent>(AllComponents);

	for (UActorComponent* Comp : AllComponents)
	{
		if (Comp->ComponentHasTag(FName("WallMesh"))) {
			WallMesh = Cast<UStaticMeshComponent>(Comp);
		}
		else if (Comp->ComponentHasTag(FName("InteractionPoint")))
		{
			InteractionPoint = Cast<UStaticMeshComponent>(Comp);
		}
		else if (Comp->ComponentHasTag(FName("InteractionCamera")))
		{
			PuzzleCamera = Cast<UCameraComponent>(Comp);
		}
	}

	bIsPuzzleSetupValid = true;

	if (!InteractionPoint->ComponentHasTag(FName("InteractionPoint"))) {
		bIsPuzzleSetupValid = false;
	}
	if (!PuzzleCamera->ComponentHasTag(FName("InteractionCamera"))) {
		bIsPuzzleSetupValid = false;
	}
	if (!WallMesh->ComponentHasTag(FName("WallMesh"))) {
		bIsPuzzleSetupValid = false;
	}
	if (bIsPuzzleSetupValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestPuzzle: enough components all good"));
		InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &APuzzleWall::OnOverlapBegin);
		InteractionZone->OnComponentEndOverlap.AddDynamic(this, &APuzzleWall::OnOverlapEnd);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("TestPuzzle: not enough components or no tags in them"));
	}

}

void APuzzleWall::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("TestPuzzle:Overlaped"));
	//if (ACharacter* PlayerCharater = Cast<ACharacter>(OtherActor)) {
	if (PlayerCharater = Cast<ACharacter>(OtherActor)) {
		UE_LOG(LogTemp, Warning, TEXT("TestPuzzle: PlayerCharater found"));
		//TODO Add collision box for the puzzle using hand socket
		//PlayerCharater->GetMesh()->GetSocketLocation(FName("hand_r")),
		SuggestInteraction();
		//TObjectPtr<APlayerController> PlayerControler = Cast<APlayerController>(PlayerCharater->GetController());
		if (PlayerControler = Cast<APlayerController>(PlayerCharater->GetController())) {
			UE_LOG(LogTemp, Warning, TEXT("TestPuzzle: PlayerControler found"))
		};
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControler->GetLocalPlayer()))
		{
			// 2. Dodajemy mapowanie dla myszki
			Subsystem->AddMappingContext(DefaultMappingContext, 100);
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerControler->InputComponent)) {

				//okej to jest crazy co ja tutaj robie musze to zmieniæ nie mogê tak poprostu w³¹czaæi wy³¹czaæ interakcji robi siê niestety nie zosta³o mi du¿o czasu na du¿e zmiany :(
				EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Triggered, this, &APuzzleWall::SetTryingToInteract);

			}
		}
	}
}

void APuzzleWall::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Clear the puzzle UI
	ClearPuzzleUI();
	return;
}

void APuzzleWall::MoveHand(const FInputActionValue &Value)
{
	FVector2D Input2D = Value.Get<FVector2D>();

	WorldHandTargetLocation.Y += Input2D.X * MouseSensitivity * WallMesh->GetRightVector().Y;
	WorldHandTargetLocation.X += Input2D.X * MouseSensitivity * WallMesh->GetRightVector().X;
	WorldHandTargetLocation.Z -= Input2D.Y * MouseSensitivity * WallMesh->GetUpVector().Z;

	DrawDebugSphere(GetWorld(), InteractionPoint->GetComponentLocation(), GrabRadius, 12, FColor::Emerald, false, -1.f, 0, 1.f);
	if (bTryGrabing) {
		DrawDebugSphere(GetWorld(),PlayerCharater->GetMesh()->GetBoneLocation("hand_r"), 5, 12, FColor::Emerald, false, -1.f, 0, 1.f);
		if (FVector::Distance(PlayerCharater->GetMesh()->GetBoneLocation("hand_r"), InteractionPoint->GetComponentLocation()) <= GrabRadius) {
			UE_LOG(LogTemp, Warning, TEXT("mozesz lapac"));
			
			InteractionPoint->AddWorldOffset(
				FVector(Input2D.X * MouseSensitivity * WallMesh->GetRightVector().X,
						Input2D.X * MouseSensitivity * WallMesh->GetRightVector().Y,
						-Input2D.Y * MouseSensitivity * WallMesh->GetUpVector().Z)
				);
			//dodaj coœ w tym stylu ¿e coœ siê dzieje z punktem interakcji kiedy gracz zaczyne go przesówaæ mo¿e wciskanie go nie ma sensu ale jakaœ rotacja 
			//chce przekazaæ graczowi dodatkokwy feedback ¿e coœ siê faktycznie dzieje z kostk¹
			//if (!presed) {
			//	InteractionPoint->SetRelativeLocation(FVector(InteractionPoint->GetRelativeLocation().X + 5, InteractionPoint->GetRelativeLocation().Y, InteractionPoint->GetRelativeLocation().Z));
			//	pressed = true;
			//}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("nie mozesz lapac"));
		}
	}
	
	//WorldHandTargetLocation = WallMesh->GetComponentLocation() + WallMesh->GetComponentRotation().RotateVector(WorldHandTargetLocation);
	//FVector WorldHandPosition = WallMesh->GetComponentTransform().TransformPosition(WorldHandTargetLocation);
	DrawDebugSphere(GetWorld(), WorldHandTargetLocation, 15.f, 12, FColor::Red, false, -1.f, 0, 2.f);
	DrawDebugLine(GetWorld(), PlayerCharater->GetMesh()->GetSocketLocation(FName("hand_r")), WorldHandTargetLocation, FColor::Green, false, -1.f, 0, 2.f);

	//const FVector WorldHandLocation = WallMesh->GetComponentTransform().TransformPosition(WorldHandTargetLocation);
	const FVector MeshSpaceHandLocation = WallMesh->GetComponentTransform().InverseTransformPosition(WorldHandTargetLocation);
	
	FRotator WallMeshRotation = WallMesh->GetComponentRotation();
	FQuat PlayerHandLocalQuat = PlayerCharater->GetMesh()->GetBoneTransform(FName("hand_r")).InverseTransformRotation(WallMeshRotation.Quaternion());

	FRotator PlayerHandLocalRoation = PlayerHandLocalQuat.Rotator();

	//DrawDebugSphere(GetWorld(), WorldHandLocation, 15.f, 12, FColor::Yellow, false, -1.f, 0, 2.f);
	//DrawDebugLine(GetWorld(), PlayerCharater->GetMesh()->GetSocketLocation(FName("hand_r")), WorldHandLocation, FColor::Yellow, false, -1.f, 0, 2.f);
	

	DrawDebugSphere(GetWorld(), MeshSpaceHandLocation, 15.f, 12, FColor::Magenta, false, -1.f, 0, 2.f);
	DrawDebugLine(GetWorld(), PlayerCharater->GetMesh()->GetSocketLocation(FName("hand_r")), MeshSpaceHandLocation, FColor::Magenta, false, -1.f, 0, 2.f);


	//DrawDebugSphere(GetWorld(), WorldHandPosition, 15.f, 12, FColor::Purple, false, -1.f, 0, 2.f);
	DrawDebugLine(GetWorld(), PlayerCharater->GetMesh()->GetSocketLocation(FName("hand_r")), WorldHandTargetLocation, FColor::Cyan, false, -1.f, 0, 2.f);
	//Draw a debug sphere that i can see in editor for  local hand location 
	UE_LOG(LogTemp, Warning, TEXT("Myszka ruszyla sie o: X = %f, Y = %f"), Input2D.X, Input2D.Y);
	UE_LOG(LogTemp, Warning, TEXT("Pozycja dla WorldHandTargetLocation: X = %f, Y = %f"), WorldHandTargetLocation.Y, WorldHandTargetLocation.Z);
	//UpdateAnimation();
	
	
	if (TObjectPtr<UAnimInstanceSandboxCharacter_CMC> PlayerAnimInstance = Cast<UAnimInstanceSandboxCharacter_CMC>(PlayerCharater->GetMesh()->GetAnimInstance())) {
		PlayerAnimInstance->SetPuzzleMode(true, WorldHandTargetLocation, PlayerHandLocalRoation);
	}

	if (!bTryingToInteract) {
		EndPuzzle();
	}
	
}

void APuzzleWall::HandleGrab(const FInputActionValue& Value)
{
	bTryGrabing = Value.Get<bool>();
}

void APuzzleWall::SetTryingToInteract(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("trying to interact= %d , %d"),Value.Get<bool>(), bTryingToInteract);
	if (bTryingToInteract) {
		StartPuzzle();
		SuggestInteractionEnd();
	}
	bTryingToInteract = !bTryingToInteract;

}

//void APuzzleWall::StartPuzzle(APlayerController* PlayerControler, ACharacter* PlayerCharater)
void APuzzleWall::StartPuzzle()
{
	bIsInteracting = true;
	
	if (!bIsPuzzleSetupValid || !PlayerControler) { return; UE_LOG(LogTemp, Warning, TEXT("TestPuzzle: Puzle failed to start"));}
	
	PlayerCharater->DisableInput(PlayerControler);
	PlayerControler->SetViewTargetWithBlend(this, 1.f);

	//DEBUGING it's prety messy but leave it for now might help
	//WorldHandTargetLocation = WallMesh->GetComponentLocation() + WallMesh->GetComponentRotation().RotateVector(WorldHandTargetLocation);
	//WorldHandTargetLocation = FVector(WallMesh->GetForwardVector() + WallMesh->GetRightVector()+ WallMesh->GetUpVector());
	//UE_LOG(LogTemp, Warning, TEXT("Pozycja dla WorldHandTargetLocation GetVector: X = %f, Y = %f, Z = %f"), WorldHandTargetLocation.X, WorldHandTargetLocation.Y, WorldHandTargetLocation.Y);	
	//WorldHandTargetLocation = FVector(WallMesh->GetRelativeLocation());
	//UE_LOG(LogTemp, Warning, TEXT("Pozycja dla WorldHandTargetLocation GetRelativeLocation: X = %f, Y = %f, Z = %f"), WorldHandTargetLocation.X, WorldHandTargetLocation.Y, WorldHandTargetLocation.Y);
	//WorldHandTargetLocation = FVector(WallMesh->GetComponentLocation() + (WallMesh->GetForwardVector() * (-20)));
	//UE_LOG(LogTemp, Warning, TEXT("Pozycja dla WorldHandTargetLocation GetComponentLocation: X = %f, Y = %f, Z = %f"), WorldHandTargetLocation.X, WorldHandTargetLocation.Y, WorldHandTargetLocation.Y);
	//WorldHandTargetLocation = FVector(WallMesh->GetComponentLocation() + (WallMesh->GetForwardVector() * (-20)) + WallMesh->GetComponentRotation().RotateVector(WorldHandTargetLocation));
	//UE_LOG(LogTemp, Warning, TEXT("Pozycja dla WorldHandTargetLocation GetComponentLocation after RotateVector(WorldHandTargetLocation): X = %f, Y = %f, Z = %f"), WorldHandTargetLocation.X, WorldHandTargetLocation.Y, WorldHandTargetLocation.Y);

	WorldHandTargetLocation = WallMesh->GetComponentLocation() + (WallMesh->GetForwardVector() *(-25) + (InteractionPoint->GetRelativeLocation() - WallMesh->GetRelativeLocation())) ;
	UE_LOG(LogTemp, Warning, TEXT("Pozycja dla WallMesh WorldHandTaegetRotator after RotateVector(WorldHandTargetLocation): %s"), *WorldHandTargetLocation.ToOrientationRotator().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Pozycja relatywna dla interaction point w stosunku do wallmesh: x:%f, y:%f, z:%f"), InteractionPoint->GetRelativeLocation().X, InteractionPoint->GetRelativeLocation().Y, InteractionPoint->GetRelativeLocation().Z);
	FRotator WallMeshRotation = WallMesh->GetComponentRotation();
	UE_LOG(LogTemp, Warning, TEXT("Pozycja dla WallMesh component roation: %s"), *WallMeshRotation.ToString());

	FVector DesiredCharaterRotation = WallMesh->GetComponentLocation() - PlayerCharater->GetActorLocation();
	DesiredCharaterRotation.Z = 0;
	PlayerCharater->SetActorRotation(DesiredCharaterRotation.Rotation());


	DrawDebugSphere(GetWorld(), WallMesh->GetComponentLocation(), 200.f, 12, FColor::Silver, true, -1.f, 0, 5.f);
	DrawDebugSphere(GetWorld(), RootComponent->GetComponentLocation(), 240.f, 12, FColor::Black, true, -1.f, 0, 5.f);
	DrawDebugSphere(GetWorld(), WorldHandTargetLocation, 210.f, 12, FColor::Blue, true, -1.f, 0, 5.f);
	DrawDebugSphere(GetWorld(), RootComponent->GetRelativeLocation(), 240.f, 12, FColor::Green, true, -1.f, 0, 5.f);

	//DrawDebugSphere(GetWorld(), WallMesh->GetComponentLocation().RightVector, 200.f, 12, FColor::Purple, true, -1.f, 0, 5.f);
	//DrawDebugSphere(GetWorld(), WallMesh->GetComponentLocation().UpVector, 240.f, 12, FColor::Yellow, true, -1.f, 0, 5.f);
	UE_LOG(LogTemp, Warning, TEXT("Vektory relatywne do Wallmesh: Y = %f, Z = %f"), WallMesh->GetRightVector().Y, WallMesh->GetRightVector().Z);

	UE_LOG(LogTemp, Warning, TEXT("TestPuzzle: Camera changed"));


	// To jest bardzo Ÿle powinien byæ jeden subsystem a nie ¿e ja sobie dodaje tysi¹c ró¿nych bindów to jest tragiczne musze to zmieniæ. szkoda ¿e nie zauwazy³em tego wczeœniej 
	// powwinienm to wszystko robiæ w Player controler i mieæ jakieœ delegaty pomyœl o tym.
	// TODO FIX THIS 
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControler->GetLocalPlayer()))
	{
		// 2. Dodajemy mapowanie dla myszki
		Subsystem->AddMappingContext(DefaultMappingContext, 100);
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerControler->InputComponent)) {
			EnhancedInputComponent->BindAction(MouseMove, ETriggerEvent::Triggered, this, &APuzzleWall::MoveHand);
			EnhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Triggered, this, &APuzzleWall::HandleGrab);
			//let's see if the actions work this ETriggerEvent works this way 
			EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Triggered, this, &APuzzleWall::SetTryingToInteract);
			EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Completed, this, &APuzzleWall::SetTryingToInteract);

		}
	}
}

void APuzzleWall::EndPuzzle()
{
	UE_LOG(LogTemp, Warning, TEXT("Puzzle Ended return"));
	//zmien to spowrotem 
	//PlayerCharater->DisableInput(PlayerControler);
	//PlayerControler->SetViewTargetWithBlend(this, 1.f);
	
	PlayerControler->SetViewTargetWithBlend(PlayerCharater, 1.f);
	PlayerCharater->EnableInput(PlayerControler);
	if (TObjectPtr<UAnimInstanceSandboxCharacter_CMC> PlayerAnimInstance = Cast<UAnimInstanceSandboxCharacter_CMC>(PlayerCharater->GetMesh()->GetAnimInstance())) {
		PlayerAnimInstance->SetPuzzleMode(false, FVector().ZeroVector, FRotator().ZeroRotator);
	}

	//prawdopodobnie nadal stoi w boxCollision wiêc 
	SuggestInteraction();

}

void APuzzleWall::SuggestInteraction()
{
	// Sprawdzamy, czy w Blueprincie przypisano klasê UI
	if (!PuzzleStartUI) {
		UE_LOG(LogTemp, Warning, TEXT("Brak przypisanego UI"));
		return;
	}
	// Jeœli widget jeszcze nie istnieje, tworzymy go
	if (!PuzzleUIInstance)
	{
		PuzzleUIInstance = CreateWidget<UUserWidget>(GetWorld(), PuzzleStartUI);
	}

	// Jeœli uda³o siê stworzyæ i jeszcze go nie ma na ekranie, dodajemy
	if (PuzzleUIInstance && !PuzzleUIInstance->IsInViewport())
	{
		PuzzleUIInstance->AddToViewport();
	}

	//if(btryingtointeract)
	//bIsInteracting = true;
}

void APuzzleWall::SuggestInteractionEnd()
{

}

void APuzzleWall::ClearPuzzleUI()
{
	if (PuzzleUIInstance && PuzzleUIInstance->IsInViewport())
	{
		PuzzleUIInstance->RemoveFromViewport();
	}
}
