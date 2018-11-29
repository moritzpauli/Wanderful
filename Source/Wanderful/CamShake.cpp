// Fill out your copyright notice in the Description page of Project Settings.

#include "CamShake.h"


UCamShake::UCamShake() {
	OscillationDuration = 1.2f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2f;

	//rot oscillation
	RotOscillation.Pitch.Amplitude = 0.5f;
	RotOscillation.Pitch.Frequency = 10.0f;
	RotOscillation.Pitch.InitialOffset = EOO_OffsetRandom;
	RotOscillation.Yaw.Amplitude = 0.0f;
	RotOscillation.Yaw.Frequency = 5.0f;
	RotOscillation.Yaw.InitialOffset = EOO_OffsetRandom;
	RotOscillation.Roll.Amplitude = 1.0f;
	RotOscillation.Roll.Frequency = 9.0f;
	RotOscillation.Roll.InitialOffset = EOO_OffsetRandom;
	//loc oscillation
	LocOscillation.Z.Amplitude = 1.0f;
	LocOscillation.Z.Frequency = 10.0f;
	LocOscillation.Z.InitialOffset = EOO_OffsetRandom;
}

