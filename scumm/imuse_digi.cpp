/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001/2002 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#include "stdafx.h"
#include "scumm/scumm.h"
#include "scumm/imuse_digi.h"
#include "scumm/sound.h"

////////////////////////////////////////
//
// iMuse Digital Implementation
//   for SCUMM v7 and higher
//
////////////////////////////////////////

static void imus_digital_handler(void *engine) {
	// Avoid race condition
	if(engine && ((Scumm *)engine)->_imuseDigital)
		((Scumm *)engine)->_imuseDigital->handler();
}

IMuseDigital::IMuseDigital(Scumm *scumm) {
	memset(_channel, 0, sizeof(channel) * MAX_DIGITAL_CHANNELS);
	_scumm = scumm;
	for (int32 l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		_channel[l]._initialized = false;
	}
	_scumm->_mixer->beginSlots(MAX_DIGITAL_CHANNELS + 1);
	_scumm->_timer->installProcedure(imus_digital_handler, 200);
	_pause = false;
}

IMuseDigital::~IMuseDigital() {
	for (int32 l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		_scumm->_mixer->stop(l);
	}
	_scumm->_timer->releaseProcedure(imus_digital_handler);
}

struct imuse_music_table {
	int16 index;
	char name[30];
	char title[30];
	char filename[15];
	int8 unk1;
};

struct imuse_music_map {
	int16 room;
	int16 table_index;
	int16 unk1;
	int16 unk2;
	int16 unk3;
	int16 unk4;
};

static const imuse_music_map _digStateMusicMap[] = {
	{0,		0,	0,	0,	0,	0	},
	{1,		0,	0,	0,	0,	0	},
	{2,		2,	0,	0,	0,	0	},
	{3,		47,	0,	0,	0,	0	},
	{4,		3,	0,	0,	0,	0	},
	{5,		3,	0,	0,	0,	0	},
	{6,		3,	0,	0,	0,	0	},
	{7,		3,	0,	0,	0,	0	},
	{8,		4,	0,	0,	0,	0	},
	{9,		5,	0,	0,	0,	0	},
	{10,	4,	0,	0,	0,	0	},
	{11,	44,	0,	0,	0,	0	},
	{12,	5,	0,	0,	0,	0	},
	{13,	1,	0,	0,	0,	0	},
	{14,	5,	0,	0,	0,	0	},
	{15,	6,	29,	7,	0,	0	},
	{16,	8,	0,	0,	0,	0	},
	{17,	1,	0,	0,	0,	0	},
	{18,	9,	0,	0,	0,	0	},
	{19,	9,	0,	0,	0,	0	},
	{20,	6,	0,	0,	0,	0	},
	{21,	6,	0,	0,	0,	0	},
	{22,	44,	0,	0,	0,	0	},
	{23,	10,	7,	0,	0,	0	},
	{24,	26,	0,	0,	0,	0	},
	{25,	17,	0,	0,	0,	0	},
	{26,	17,	0,	0,	0,	0	},
	{27,	18,	0,	0,	0,	0	},
	{28,	1,	0,	0,	0,	0	},
	{29,	20,	0,	0,	0,	0	},
	{30,	22,	0,	0,	0,	0	},
	{31,	23,	0,	0,	0,	0	},
	{32,	22,	0,	0,	0,	0	},
	{33,	26,	0,	0,	0,	0	},
	{34,	24,	0,	0,	0,	0	},
	{35,	1,	0,	0,	0,	0	},
	{36,	1,	0,	0,	0,	0	},
	{37,	42,	0,	0,	0,	0	},
	{38,	43,	0,	0,	0,	0	},
	{39,	44,	0,	0,	0,	0	},
	{40,	1,	0,	0,	0,	0	},
	{41,	43,	0,	0,	0,	0	},
	{42,	44,	0,	0,	0,	0	},
	{43,	43,	0,	0,	0,	0	},
	{44,	45,	117,45,	114,26},
	{45,	1,	0,	0,	0,	0	},
	{46,	33,	6,	35,	5,	34},
	{47,	1,	0,	0,	0,	0	},
	{48,	43,	0,	0,	0,	0	},
	{49,	44,	0,	0,	0,	0	},
	{50,	1,	0,	0,	0,	0	},
	{51,	1,	0,	0,	0,	0	},
	{52,	0,	0,	0,	0,	0	},
	{53,	28,	0,	0,	0,	0	},
	{54,	28,	0,	0,	0,	0	},
	{55,	29,	0,	0,	0,	0	},
	{56,	29,	0,	0,	0,	0	},
	{57,	29,	0,	0,	0,	0	},
	{58,	31,	0,	0,	0,	0	},
	{59,	1,	0,	0,	0,	0	},
	{60,	37,	0,	0,	0,	0	},
	{61,	39,	0,	0,	0,	0	},
	{62,	38,	0,	0,	0,	0	},
	{63,	39,	0,	0,	0,	0	},
	{64,	39,	0,	0,	0,	0	},
	{65,	40,	0,	0,	0,	0	},
	{66,	1,	0,	0,	0,	0	},
	{67,	40,	0,	0,	0,	0	},
	{68,	39,	0,	0,	0,	0	},
	{69,	1,	0,	0,	0,	0	},
	{70,	49,	0,	0,	0,	0	},
	{71,	1,	0,	0,	0,	0	},
	{72,	1,	0,	0,	0,	0	},
	{73,	50,	0,	0,	0,	0	},
	{74,	1,	0,	0,	0,	0	},
	{75,	51,	0,	0,	0,	0	},
	{76,	1,	0,	0,	0,	0	},
	{77,	52,	7,	0,	0,	0	},
	{78,	63,	0,	0,	0,	0	},
	{79,	1,	0,	0,	0,	0	},
	{80,	41,	0,	0,	0,	0	},
	{81,	48,	0,	0,	0,	0	},
	{82,	21,	0,	0,	0,	0	},
	{83,	27,	0,	0,	0,	0	},
	{84,	1,	0,	0,	0,	0	},
	{85,	1,	0,	0,	0,	0	},
	{86,	0,	0,	0,	0,	0	},
	{87,	1,	0,	0,	0,	0	},
	{88,	32,	0,	0,	0,	0	},
	{89,	33,	6,	35,	5,	34},
	{90,	16,	0,	0,	0,	0	},
	{91,	57,	0,	0,	0,	0	},
	{92,	25,	0,	0,	0,	0	},
	{93,	0,	0,	0,	0,	0	},
	{94,	36,	0,	0,	0,	0	},
	{95,	19,	0,	0,	0,	0	},
	{96,	13,	0,	0,	0,	0	},
	{97,	14,	0,	0,	0,	0	},
	{98,	11,	0,	0,	0,	0	},
	{99,	15,	0,	0,	0,	0	},
	{100,	17,	0,	0,	0,	0	},
	{101,	38,	0,	0,	0,	0	},
	{102,	1,	0,	0,	0,	0	},
	{103,	0,	0,	0,	0,	0	},
	{104,	0,	0,	0,	0,	0	},
	{105,	30, 128,29,	0,	0	},
	{106,	0,	0,	0,	0,	0	},
	{107,	1,	0,	0,	0,	0	},
	{108,	1,	0,	0,	0,	0	},
	{109,	1,	0,	0,	0,	0	},
	{110,	2,	0,	0,	0,	0	},
	{111,	1,	0,	0,	0,	0	},
	{-1,	1,	0,	0,	0,	0	},
};

static const imuse_music_table _digStateMusicTable[] = {
	{0,		"STATE_NULL",						"",												"",							0},
	{1,		"stateNoChange",				"",												"",							0},
	{2,		"stateAstShip",					"Asteroid (amb-ship)",		"ASTERO~1.IMU",	3},
	{3,		"stateAstClose",				"Asteroid (amb-close)",		"ASTERO~2.IMU",	3},
	{4,		"stateAstInside",				"Asteroid (inside)",			"ASTERO~3.IMU",	3},
	{5,		"stateAstCore",					"Asteroid (core)",				"ASTERO~4.IMU",	3},
	{6,		"stateCanyonClose",			"Canyon (close)",					"CANYON~1.IMU",	3},
	{7,		"stateCanyonClose_m",		"Canyon (close-m)",				"CANYON~2.IMU",	3},
	{8,		"stateCanyonOver",			"Canyon (over)",					"CANYON~3.IMU",	3},
	{9,		"stateCanyonWreck",			"Canyon (wreck)",					"CANYON~4.IMU",	3},
	{10,	"stateNexusCanyon",			"Nexus (plan)",						"NEXUS(~1.IMU",	3},
	{11,	"stateNexusPlan",				"Nexus (plan)",						"NEXUS(~1.IMU",	3},
	{12,	"stateNexusRamp",				"Nexus (ramp)",						"NEXUS(~2.IMU",	3},
	{13,	"stateNexusMuseum",			"Nexus (museum)",					"NEXUS(~3.IMU",	3},
	{14,	"stateNexusMap",				"Nexus (map)",						"NEXUS(~4.IMU",	3},
	{15,	"stateNexusTomb",				"Nexus (tomb)",						"NE3706~5.IMU",	3},
	{16,	"stateNexusCath",				"Nexus (cath)",						"NE3305~5.IMU",	3},
	{17,	"stateNexusAirlock",		"Nexus (airlock)",				"NE2D3A~5.IMU",	3},
	{18,	"stateNexusPowerOff",		"Nexus (power)",					"NE8522~5.IMU",	3},
	{19,	"stateMuseumTramNear",	"Tram (mu-near)",					"TRAM(M~1.IMU",	3},
	{20,	"stateMuseumTramFar",		"Tram (mu-far)",					"TRAM(M~2.IMU",	3},
	{21,	"stateMuseumLockup",		"Museum (lockup)",				"MUSEUM~1.IMU",	3},
	{22,	"stateMuseumPool",			"Museum (amb-pool)",			"MUSEUM~2.IMU",	3},
	{23,	"stateMuseumSpire",			"Museum (amb-spire)",			"MUSEUM~3.IMU",	3},
	{24,	"stateMuseumMuseum",		"Museum (amb-mu)",				"MUSEUM~4.IMU",	3},
	{25,	"stateMuseumLibrary",		"Museum (library)",				"MUB575~5.IMU",	3},
	{26,	"stateMuseumCavern",		"Museum (cavern)",				"MUF9BE~5.IMU",	3},
	{27,	"stateTombTramNear",		"Tram (tomb-near)",				"TRAM(T~1.IMU",	3},
	{28,	"stateTombBase",				"Tomb (amb-base)",				"TOMB(A~1.IMU",	3},
	{29,	"stateTombSpire",				"Tomb (amb-spire)",				"TOMB(A~2.IMU",	3},
	{30,	"stateTombCave",				"Tomb (amb-cave)",				"TOMB(A~3.IMU",	3},
	{31,	"stateTombCrypt",				"Tomb (crypt)",						"TOMB(C~1.IMU",	3},
	{32,	"stateTombGuards",			"Tomb (crypt-guards)",		"TOMB(C~2.IMU",	3},
	{33,	"stateTombInner",				"Tomb (inner)",						"TOMB(I~1.IMU",	3},
	{34,	"stateTombCreator1",		"Tomb (creator 1)",				"TOMB(C~3.IMU",	3},
	{35,	"stateTombCreator2",		"Tomb (creator 2)",				"TOMB(C~4.IMU",	3},
	{36,	"statePlanTramNear",		"Tram (plan-near)",				"TRAM(P~1.IMU",	3},
	{37,	"statePlanTramFar",			"Tram (plan-far)",				"TRAM(P~2.IMU",	3},
	{38,	"statePlanBase",				"Plan (amb-base)",				"PLAN(A~1.IMU",	3},
	{39,	"statePlanSpire",				"Plan (amb-spire)",				"PLAN(A~2.IMU",	3},
	{40,	"statePlanDome",				"Plan (dome)",						"PLAN(D~1.IMU",	3},
	{41,	"stateMapTramNear",			"Tram (map-near)",				"TRAM(M~3.IMU",	3},
	{42,	"stateMapTramFar",			"Tram (map-far)",					"TRAM(M~4.IMU",	3},
	{43,	"stateMapCanyon",				"Map (amb-canyon)",				"MAP(AM~1.IMU",	3},
	{44,	"stateMapExposed",			"Map (amb-exposed)",			"MAP(AM~2.IMU",	3},
	{45,	"stateMapNestEmpty",		"Map (amb-nest)",					"MAP(AM~4.IMU",	3},
	{46,	"stateMapNestMonster",	"Map (monster)",					"MAP(MO~1.IMU",	3},
	{47,	"stateMapKlein",				"Map (klein)",						"MAP(KL~1.IMU",	3},
	{48,	"stateCathTramNear",		"Tram (cath-near)",				"TRAM(C~1.IMU",	3},
	{49,	"stateCathTramFar",			"Tram (cath-far)",				"TRAM(C~2.IMU",	3},
	{50,	"stateCathLab",					"Cath (amb-inside)",			"CATH(A~1.IMU",	3},
	{51,	"stateCathOutside",			"Cath (amb-outside)",			"CATH(A~2.IMU",	3},
	{52,	"stateWorldMuseum",			"World (museum)",					"WORLD(~1.IMU",	3},
	{53,	"stateWorldPlan",				"World (plan)",						"WORLD(~2.IMU",	3},
	{54,	"stateWorldTomb",				"World (tomb)",						"WORLD(~3.IMU",	3},
	{55,	"stateWorldMap",				"World (map)",						"WORLD(~4.IMU",	3},
	{56,	"stateWorldCath",				"World (cath)",						"WO3227~5.IMU",	3},
	{57,	"stateEye1",						"Eye 1",									"EYE1~1.IMU",		3},
	{58,	"stateEye2",						"Eye 2",									"EYE2~1.IMU",		3},
	{59,	"stateEye3",						"Eye 3",									"EYE3~1.IMU",		3},
	{60,	"stateEye4",						"Eye 4",									"EYE4~1.IMU",		3},
	{61,	"stateEye5",						"Eye 5",									"EYE5~1.IMU",		3},
	{62,	"stateEye6",						"Eye 6",									"EYE6~1.IMU",		3},
	{63,	"stateEye7",						"Eye 7",									"EYE7~1.IMU",		3},
	{-1,	"",											"",												"",							0},
};

static const imuse_music_table _digSeqMusicTable[] = {
	{2000,	"SEQ_NULL",							"",												"",							0},
	{2005,	"seqLogo",							"",												"",							0},
	{2010,	"seqIntro",							"",												"",							0},
	{2020,	"seqExplosion1b",				"",												"",							6},
	{2030,	"seqAstTunnel1a",				"Seq (ast tunnel 1a)",		"SEQ(AS~1.IMU",	3},
	{2031,	"seqAstTunnel2b",				"",												"",							6},
	{2032,	"seqAstTunnel3a",				"Seq (ast tunnel 3a)",		"SEQ(AS~2.IMU",	4},
	{2040,	"seqToPlanet1b",				"",												"",							5},
	{2045,	"seqArgBegin",					"Seq (arg begin)",				"SEQ(AR~1.IMU",	4},
	{2046,	"seqArgEnd",						"Seq (arg end)",					"SEQ(AR~2.IMU",	4},
	{2050,	"seqWreckGhost",				"Seq (ghost-wreck)",			"SEQ(GH~1.IMU",	4},
	{2060,	"seqCanyonGhost",				"Seq (ghost-canyon)",			"SEQ(GH~2.IMU",	4},
	{2070,	"seqBrinkFall",					"",												"",							0},
	{2080,	"seqPanUpCanyon",				"Seq (pan up canyon)",		"SEQ(PA~1.IMU",	4},
	{2091,	"seqAirlockTunnel1b",		"",												"",							6},
	{2100,	"seqTramToMu",					"",												"",							6},
	{2101,	"seqTramFromMu",				"",												"",							6},
	{2102,	"seqTramToTomb",				"",												"",							6},
	{2103,	"seqTramFromTomb",			"",												"",							6},
	{2104,	"seqTramToPlan",				"",												"",							6},
	{2105,	"seqTramFromPlan",			"",												"",							6},
	{2106,	"seqTramToMap",					"",												"",							6},
	{2107,	"seqTramFromMap",				"",												"",							6},
	{2108,	"seqTramToCath",				"",												"",							6},
	{2109,	"seqTramFromCath",			"",												"",							6},
	{2110,	"seqMuseumGhost",				"",												"",							0},
	{2120,	"seqSerpentAppears",		"",												"",							0},
	{2130,	"seqSerpentEats",				"",												"",							0},
	{2140,	"seqBrinkRes1b",				"",												"",							6},
	{2141,	"seqBrinkRes2a",				"Seq (brink's madness)",	"SEQ(BR~1.IMU",	4},
	{2150,	"seqLockupEntry",				"Seq (brink's madness)",	"SEQ(BR~1.IMU",	3},
	{2160,	"seqSerpentExplodes",		"",												"",							0},
	{2170,	"seqSwimUnderwater",		"Seq (descent)",					"SEQ(DE~1.IMU",	4},
	{2175,	"seqWavesPlunge",				"Seq (plunge)",						"SEQ(PL~1.IMU",	4},
	{2180,	"seqCryptOpens",				"",												"",							0},
	{2190,	"seqGuardsFight",				"",												"",							0},
	{2200,	"seqCreatorRes1.1a",		"Seq (creator res 1.1a)",	"SEQ(CR~1.IMU",	3},
	{2201,	"seqCreatorRes1.2b",		"",												"",							6},
	{2210,	"seqMaggieCapture1b",		"",												"",							6},
	{2220,	"seqStealCrystals",			"Seq (brink's madness)",	"SEQ(BR~1.IMU",	3},
	{2230,	"seqGetByMonster",			"",												"",							0},
	{2240,	"seqKillMonster1b",			"",												"",							6},
	{2250,	"seqCreatorRes2.1a",		"Seq (creator res 2.1a)",	"SEQ(CR~2.IMU",	3},
	{2251,	"seqCreatorRes2.2b",		"",												"",							6},
	{2252,	"seqCreatorRes2.3a",		"Seq (creator res 2.3a)",	"SEQ(CR~3.IMU",	4},
	{2260,	"seqMaggieInsists",			"",												"",							0},
	{2270,	"seqBrinkHelpCall",			"",												"",							0},
	{2280,	"seqBrinkCrevice1a",		"Seq (brink crevice 1a)",	"SEQ(BR~2.IMU",	3},
	{2281,	"seqBrinkCrevice2a",		"Seq (brink crevice 2a)",	"SEQ(BR~3.IMU",	3},
	{2290,	"seqCathAccess1b",			"",												"",							6},
	{2291,	"seqCathAccess2a",			"Seq (cath access 2a)",		"SEQ(CA~1.IMU",	4},
	{2300,	"seqBrinkAtGenerator",	"Seq (brink's madness)",	"SEQ(BR~1.IMU",	3},
	{2320,	"seqFightBrink1b",			"",												"",							6},
	{2340,	"seqMaggieDies1b",			"",												"",							6},
	{2346,	"seqMaggieRes1b",				"",												"",							6},
	{2347,	"seqMaggieRes2a",				"Seq (maggie res 2a)",		"SEQ(MA~1.IMU",	4},
	{2350,	"seqCreatureFalls",			"",												"",							0},
	{2360,	"seqFinale1b",					"",												"",							5},
	{2370,	"seqFinale2a",					"Seq (finale 2a)",				"SEQ(FI~1.IMU",	3},
	{2380,	"seqFinale3b1",					"",												"",							6},
	{2390,	"seqFinale3b2",					"",												"",							6},
	{2400,	"seqFinale4a",					"Seq (finale 4a)",				"SEQ(FI~2.IMU",	3},
	{2410,	"seqFinale5a",					"Seq (finale 5a)",				"SEQ(FI~3.IMU",	3},
	{2420,	"seqFinale6a",					"Seq (finale 6a)",				"SEQ(FI~4.IMU",	3},
	{2430,	"seqFinale7a",					"Seq (finale 7a)",				"SE3D2B~5.IMU",	3},
	{2440,	"seqFinale8b",					"",												"",							6},
	{2450,	"seqFinale9a",					"Seq (finale 9a)",				"SE313B~5.IMU",	4},
	{-1,		"",											"",												"",							0},
};

struct imuse_ft_music_table {
	int16 index;
	char audioname[15];
	int8 unk1;
	int8 volume;
	char name[30];
};

static const imuse_ft_music_table _ftStateMusicTable[] = {
	{0,		"",					0,	0,		"STATE_NULL"					},
	{1,		"",					4,	127,	"stateKstandOutside"	},
	{2,		"kinside",	2,	127,	"stateKstandInside"		},
	{3,		"moshop",		3,	64,		"stateMoesInside"			},
	{4,		"melcut",		2,	127,	"stateMoesOutside"		},
	{5,		"mellover",	2,	127,	"stateMellonAbove"		},
	{6,		"radloop",	3,	28,		"stateTrailerOutside"	},
	{7,		"radloop",	3,	58,		"stateTrailerInside"	},
	{8,		"radloop",	3,	127,	"stateTodShop"				},
	{9,		"junkgate",	2,	127,	"stateJunkGate"				},
	{10,	"junkover",	3,	127,	"stateJunkAbove"			},
	{11,	"gastower",	2,	127,	"stateGasTower"				},
	{12,	"",					4,	0,		"stateTowerAlarm"			},
	{13,	"melcut",		2,	127,	"stateCopsOnGround"		},
	{14,	"melcut",		2,	127,	"stateCopsAround"			},
	{15,	"melcut",		2,	127,	"stateMoesRuins"			},
	{16,	"melcut",		2,	127,	"stateKstandNight"		},
	{17,	"trukblu2",	2,	127,	"stateTruckerTalk"		},
	{18,	"stretch",	2,	127,	"stateMumblyPeg"			},
	{19,	"kstand",		2,	127,	"stateRanchOutside"		},
	{20,	"kinside",	2,	127,	"stateRanchInside"		},
	{21,	"desert",		2,	127,	"stateWreckedTruck"		},
	{22,	"opening",	2,	127,	"stateGorgeVista"			},
	{23,	"caveopen",	2,	127,	"stateCaveOpen"				},
	{24,	"cavecut1",	2,	127,	"stateCaveOuter"			},
	{25,	"cavecut1",	1,	127,	"stateCaveMiddle"			},
	{26,	"cave",			2,	127,	"stateCaveInner"			},
	{27,	"corville",	2,	127,	"stateCorvilleFront"	},
	{28,	"mines",		2,	127,	"stateMineField"			},
	{29,	"bunyman3",	2,	127,	"stateBunnyStore"			},
	{30,	"stretch",	2,	127,	"stateStretchBen"			},
	{31,	"saveme",		2,	127,	"stateBenPleas"				},
	{32,	"",					4,	0,		"stateBenConvinces"		},
	{33,	"derby",		3,	127,	"stateDemoDerby"			},
	{34,	"fire",			3,	127,	"stateLightMyFire"		},
	{35,	"derby",		3,	127,	"stateDerbyChase"			},
	{36,	"carparts",	2,	127,	"stateVultureCarParts"},
	{37,	"cavecut1",	2,	127,	"stateVulturesInside"	},
	{38,	"mines",		2,	127,	"stateFactoryRear"		},
	{39,	"croffice",	2,	127,	"stateCorleyOffice"		},
	{40,	"melcut",		2,	127,	"stateCorleyHall"			},
	{41,	"",					4,	0,		"stateProjRoom"				},
	{42,	"",					4,	0,		"stateMMRoom"					},
	{43,	"bumper",		2,	127,	"stateBenOnBumper"		},
	{44,	"benump",		2,	127,	"stateBenOnBack"			},
	{45,	"plane",		2,	127,	"stateInCargoPlane"		},
	{46,	"saveme",		2,	127,	"statePlaneControls"	},
	{47,	"",					4,	0,		"stateCliffHanger1"		},
	{48,	"",					4,	0,		"stateCliffHanger2"		},
	{-1,	"",					0,	0,		""										},
};

static const imuse_ft_music_table _ftSeqMusicTable[] = {
	{0,		"",					2,	127,	"SEQ_NULL"						},
	{1,		"",					0,	0,		"seqLogo"							},
	{2,		"",					0,	0,		"seqOpenFlick"				},
	{3,		"",					0,	0,		"seqBartender"				},
	{4,		"opening", 	2,	127,	"seqBenWakes"					},
	{5,		"",					0,	0,		"seqPhotoScram"				},
	{6,		"",					0,	0,		"seqClimbChain"				},
	{7,		"",					0,	0,		"seqDogChase"					},
	{8,		"barbeat",	2,	127,	"seqDogSquish"				},
	{9,		"barwarn",	2,	127,	"seqDogHoist"					},
	{10,	"",					0,	0,		"seqCopsArrive"				},
	{11,	"",					0,	0,		"seqCopsLand"					},
	{12,	"benwakes",	2,	127,	"seqCopsLeave"				},
	{13,	"",					0,	0,		"seqCopterFlyby"			},
	{14,	"",					0,	0,		"seqCopterCrash"			},
	{15,	"",					0,	0,		"seqMoGetsParts"			},
	{16,	"barwarn",	2,	127,	"seqMoFixesBike"			},
	{17,	"",					0,	0,		"seqFirstGoodbye"			},
	{18,	"",					0,	0,		"seqCopRoadblock"			},
	{19,	"",					0,	0,		"seqDivertCops"				},
	{20,	"swatben",	2,	127,	"seqMurder"						},
	{21,	"",					0,	0,		"seqCorleyDies"				},
	{22,	"",					0,	0,		"seqTooLateAtMoes"		},
	{23,	"",					0,	0,		"seqPicture"					},
	{24,	"dogattak",	2,	127,	"seqNewsReel"					},
	{25,	"",					0,	0,		"seqCopsInspect"			},
	{26,	"",					0,	0,		"seqHijack"						},
	{27,	"",					0,	0,		"seqNestolusAtRanch"	},
	{28,	"",					4,	0,		"seqRipLimo"					},
	{29,	"",					0,	0,		"seqGorgeTurn"				},
	{30,	"",					0,	0,		"seqStealRamp"				},
	{31,	"",					0,	0,		"seqCavefishTalk"			},
	{32,	"",					4,	0,		"seqArriveCorville"		},
	{33,	"",					0,	0,		"seqSingleBunny"			},
	{34,	"",					0,	0,		"seqBunnyArmy"				},
	{35,	"",					0,	0,		"seqArriveAtMines"		},
	{36,	"cops2",		2,	127,	"seqArriveAtVultures"	},
	{37,	"",					0,	0,		"seqMakePlan"					},
	{38,	"",					0,	0,		"seqShowPlan"					},
	{39,	"",					0,	0,		"seqDerbyStart"				},
	{40,	"cops2",		2,	127,	"seqLightBales"				},
	{41,	"",					0,	0,		"seqNestolusBBQ"			},
	{42,	"",					0,	0,		"seqCallSecurity"			},
	{43,	"",					0,	0,		"seqFilmFail"					},
	{44,	"cops2",		2,	127,	"seqFilmBurn"					},
	{45,	"",					0,	0,		"seqRipSpeech"				},
	{46,	"",					0,	0,		"seqExposeRip"				},
	{47,	"",					0,	0,		"seqRipEscape"				},
	{48,	"",					0,	0,		"seqRareMoment"				},
	{49,	"",					0,	0,		"seqFanBunnies"				},
	{50,	"",					0,	0,		"seqRipDead"					},
	{51,	"bunymrch",	2,	127,	"seqFuneral"					},
	{52,	"",					0,	0,		"seqCredits"					},
	{-1,	"",					0,	0,		""										},
};

void IMuseDigital::handler() {
	uint32 l = 0, i = 0;

	if (_pause == true)
		return;

	for (l = 0; l < MAX_DIGITAL_CHANNELS;l ++) {
		if (_channel[l]._used) {
			if (_channel[l]._toBeRemoved == true) {
				_scumm->_mixer->stop(l);
				if (_scumm->_mixer->_channels[l] == NULL) {
					free(_channel[l]._data);
					_channel[l]._used = false;
					_channel[l]._initialized = false;
				}
				continue;
			}

			if (_channel[l]._delay > 0) {
				_channel[l]._delay--;
				continue;
			}

			if (_channel[l]._volumeFade != -1) {
				if (_channel[l]._volumeFadeStep < 0) {
					if (_channel[l]._volume > _channel[l]._volumeFade) {
						_channel[l]._volume += _channel[l]._volumeFadeStep;
						_channel[l]._volumeRight += _channel[l]._volumeFadeStep;
						if (_channel[l]._volume < _channel[l]._volumeFade) {
							_channel[l]._volume = _channel[l]._volumeFade;
						}
						if (_channel[l]._volumeRight < _channel[l]._volumeFade) {
							_channel[l]._volumeRight = _channel[l]._volumeFade;
						}
						if ((_channel[l]._volume == 0) && (_channel[l]._volumeRight == 0)) {
							_channel[l]._toBeRemoved = true;
						}
					}
				} else if (_channel[l]._volumeFadeStep > 0) {
					if (_channel[l]._volume < _channel[l]._volumeFade) {
						_channel[l]._volume += _channel[l]._volumeFadeStep;
						_channel[l]._volumeRight += _channel[l]._volumeFadeStep;
						if (_channel[l]._volume > _channel[l]._volumeFade) {
							_channel[l]._volume = _channel[l]._volumeFade;
						}
						if (_channel[l]._volumeRight > _channel[l]._volumeFade) {
							_channel[l]._volumeRight = _channel[l]._volumeFade;
						}
					}
				}
			}

			if ((_channel[l]._jump[0]._numLoops == 0) && (_channel[l]._isJump == true)) {
				_channel[l]._isJump = false;
			}

			uint32 new_size = _channel[l]._mixerSize;
			uint32 mixer_size = new_size;

			if (_channel[l]._initialized == false) {
				mixer_size *= 2;
				new_size *= 2;
			}

			if (_channel[l]._isJump == false) {
				if (_channel[l]._offset + mixer_size > _channel[l]._size) {
					new_size = _channel[l]._size - _channel[l]._offset;
					if (_channel[l]._numLoops == 0) {
						_channel[l]._toBeRemoved = true;
						mixer_size = new_size;
					}
				}
			} else if (_channel[l]._isJump == true) {
				if (_channel[l]._jump[0]._numLoops != 500) {
					_channel[l]._jump[0]._numLoops--;
				}
				if (_channel[l]._offset + mixer_size >= _channel[l]._jump[0]._offset) {
					new_size = _channel[l]._jump[0]._offset - _channel[l]._offset;
				}
			}

			byte *buf = (byte*)malloc(mixer_size);

			memcpy(buf, _channel[l]._data + _channel[l]._offset, new_size);
			if ((new_size != mixer_size) && (_channel[l]._isJump == true)) {
				memcpy(buf + new_size, _channel[l]._data + _channel[l]._jump[0]._dest, mixer_size - new_size);
				_channel[l]._offset = _channel[l]._jump[0]._dest + (mixer_size - new_size);
			} else if ((_channel[l]._numLoops > 0) && (new_size != mixer_size)) {
				memcpy(buf + new_size, _channel[l]._data, mixer_size - new_size);
				_channel[l]._offset = mixer_size - new_size;
			} else {
				_channel[l]._offset += mixer_size;
			}

			if (_channel[l]._bits == 12) {
				for(i = 0; i < (mixer_size / 4); i++) {
					byte sample1 = buf[i * 4 + 0];
					byte sample2 = buf[i * 4 + 1];
					byte sample3 = buf[i * 4 + 2];
					byte sample4 = buf[i * 4 + 3];
					uint16 sample_a = (uint16)(((int16)((sample1 << 8) | sample2) * _channel[l]._volumeRight) >> 8);
					uint16 sample_b = (uint16)(((int16)((sample3 << 8) | sample4) * _channel[l]._volume) >> 8);
					buf[i * 4 + 0] = (byte)(sample_a >> 8);
					buf[i * 4 + 1] = (byte)(sample_a & 0xff);
					buf[i * 4 + 2] = (byte)(sample_b >> 8);
					buf[i * 4 + 3] = (byte)(sample_b & 0xff);
				}
			} else if (_channel[l]._bits == 8) {
				for(i = 0; i < (mixer_size / 2); i++) {
					buf[i * 2 + 0] = (byte)(((int8)(buf[i * 2 + 0] ^ 0x80) * _channel[l]._volumeRight) >> 8) ^ 0x80;
					buf[i * 2 + 1] = (byte)(((int8)(buf[i * 2 + 1] ^ 0x80) * _channel[l]._volume) >> 8) ^ 0x80;
				}
			}

			if (_scumm->_silentDigitalImuse == false) {
				if (_channel[l]._initialized == false) {
					_scumm->_mixer->playStream(NULL, l, buf, mixer_size,
					                           _channel[l]._freq, _channel[l]._mixerFlags, 3, 2000000);
					_channel[l]._initialized = true;
				} else {
					_scumm->_mixer->append(l, buf, mixer_size, _channel[l]._freq, _channel[l]._mixerFlags);
				}
			}
		}
	}
}

void IMuseDigital::startSound(int sound) {
	debug(2, "IMuseDigital::startSound(%d)", sound);
	int32 l;

	for(l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if(_channel[l]._used == false) {
			byte *ptr = _scumm->getResourceAddress(rtSound, sound);
			byte *s_ptr = ptr;
			if(ptr == NULL) {
				warning("IMuseDigital::startSound(%d) NULL resource pointer", sound);
				return;
			}
			_channel[l]._idSound = sound;
			_channel[l]._offset = 0;
			_channel[l]._numRegions = 0;
			_channel[l]._numJumps = 0;
			_channel[l]._volumeRight = 127;
			_channel[l]._volume = 127;
			_channel[l]._volumeFade = -1;
			_channel[l]._volumeFadeParam = 0;
			_channel[l]._delay = 1;

			uint32 tag, size = 0, r, t;

			if (READ_UINT32_UNALIGNED(ptr) == MKID('Crea')) {
				_channel[l]._bits = 8;
				_channel[l]._channels = 2;
				_channel[l]._mixerSize = (22050 / 5) * 2;
				_channel[l]._mixerFlags = SoundMixer::FLAG_AUTOFREE | SoundMixer::FLAG_STEREO | SoundMixer::FLAG_REVERSE_STEREO | SoundMixer::FLAG_UNSIGNED;
				byte * t_ptr= _scumm->_sound->readCreativeVocFile(ptr, size, _channel[l]._freq, _channel[l]._numLoops);

				if (_channel[l]._freq == 22222) {
					_channel[l]._freq = 22050;
				} else if (_channel[l]._freq == 10989) {
					_channel[l]._freq = 11025;
				}
				
				if (_channel[l]._freq == 11025) {
					_channel[l]._mixerSize /= 2;
				}
				size *= 2;
				_channel[l]._data = (byte *)malloc(size);
				for (t = 0; t < size / 2; t++) {
					*(_channel[l]._data + t * 2 + 0) = *(t_ptr + t);
					*(_channel[l]._data + t * 2 + 1) = *(t_ptr + t);
				}
				free(t_ptr);
				_channel[l]._size = size;
			} else if (READ_UINT32_UNALIGNED(ptr) == MKID('iMUS')) {
				ptr += 16;
				for (;;) {
					tag = READ_BE_UINT32(ptr); ptr += 4;
					switch(tag) {
						case MKID_BE('FRMT'):
							ptr += 12;
							_channel[l]._bits = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._freq = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._channels = READ_BE_UINT32(ptr); ptr += 4;
						break;
						case MKID_BE('TEXT'):
							size = READ_BE_UINT32(ptr); ptr += size + 4;
						break;
						case MKID_BE('REGN'):
							ptr += 4;
							if (_channel[l]._numRegions >= MAX_IMUSE_REGIONS) {
								warning("IMuseDigital::startSound(%d) Not enough space for Region");
								ptr += 8;
								break;
							}
							_channel[l]._region[_channel[l]._numRegions]._offset = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._region[_channel[l]._numRegions]._length = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._numRegions++;
						break;
						case MKID_BE('STOP'):
							ptr += 4;
							_channel[l]._offsetStop = READ_BE_UINT32(ptr); ptr += 4;
						break;
						case MKID_BE('JUMP'):
							ptr += 4;
							if (_channel[l]._numJumps >= MAX_IMUSE_JUMPS) {
								warning("IMuseDigital::startSound(%d) Not enough space for Jump");
								ptr += 16;
								break;
							}
							_channel[l]._jump[_channel[l]._numJumps]._offset = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._jump[_channel[l]._numJumps]._dest = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._jump[_channel[l]._numJumps]._id = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._jump[_channel[l]._numJumps]._numLoops = READ_BE_UINT32(ptr); ptr += 4;
							_channel[l]._isJump = true;
							_channel[l]._numJumps++;
						break;
						case MKID_BE('DATA'):
							size = READ_BE_UINT32(ptr); ptr += 4;
						break;
						default:
							error("IMuseDigital::startSound(%d) Unknown sfx header %c%c%c%c", tag>>24, tag>>16, tag>>8, tag);
					}
					if (tag == MKID_BE('DATA')) break;
				}

//				if ((sound == 131) || (sound == 123) || (sound == 122)) {
					_channel[l]._isJump = false;
					_channel[l]._numJumps = 0;
//				}

				uint32 header_size = ptr - s_ptr;
				_channel[l]._offsetStop -= header_size;
				if (_channel[l]._bits == 12) {
					_channel[l]._offsetStop = (_channel[l]._offsetStop / 3) * 4;
				}
				for (r = 0; r < _channel[l]._numRegions; r++) {
					_channel[l]._region[r]._offset -= header_size;
					if (_channel[l]._bits == 12) {
						_channel[l]._region[r]._offset = (_channel[l]._region[r]._offset / 3) * 4;
						_channel[l]._region[r]._length = (_channel[l]._region[r]._length / 3) * 4;
					}
				}
				if (_channel[l]._numJumps > 0) {
					for (r = 0; r < _channel[l]._numJumps; r++) {
						_channel[l]._jump[r]._offset -= header_size;
						_channel[l]._jump[r]._dest -= header_size;
						if (_channel[l]._bits == 12) {
							_channel[l]._jump[r]._offset = (_channel[l]._jump[r]._offset / 3) * 4;
							_channel[l]._jump[r]._dest = (_channel[l]._jump[r]._dest / 3) * 4;
						}
					}
				}
				_channel[l]._mixerSize = (22050 / 5) * 2;
				_channel[l]._mixerFlags = SoundMixer::FLAG_AUTOFREE | SoundMixer::FLAG_STEREO | SoundMixer::FLAG_REVERSE_STEREO;
				if (_channel[l]._bits == 12) {
					_channel[l]._mixerSize *= 2;
					_channel[l]._mixerFlags |= SoundMixer::FLAG_16BITS;
					_channel[l]._size = _scumm->_sound->decode12BitsSample(ptr, &_channel[l]._data, size, (_channel[l]._channels == 2) ? false : true);
				}
				if (_channel[l]._bits == 8) {
					_channel[l]._mixerFlags |= SoundMixer::FLAG_UNSIGNED;
					if (_channel[l]._channels == 1) {
						size *= 2;
						_channel[l]._channels = 2;
						_channel[l]._data = (byte *)malloc(size);
						for (t = 0; t < size / 2; t++) {
							*(_channel[l]._data + t * 2 + 0) = *(ptr + t);
							*(_channel[l]._data + t * 2 + 1) = *(ptr + t);
						}
					} else {
						_channel[l]._data = (byte *)malloc(size);
						memcpy(_channel[l]._data, ptr, size);
					}
					_channel[l]._size = size;
				}
				if (_channel[l]._freq == 11025) {
					_channel[l]._mixerSize /= 2;
				}
			}
			_channel[l]._toBeRemoved = false;
			_channel[l]._used = true;
			break;
		}
	}
}

void IMuseDigital::stopSound(int sound) {
	debug(2, "IMuseDigital::stopSound(%d)", sound);
	for (int32 l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if ((_channel[l]._idSound == sound) && (_channel[l]._used == true)) {
			_channel[l]._toBeRemoved = true;
		}
	}
}

void IMuseDigital::stopAll() {
	for (int32 l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if (_channel[l]._used == true) {
			_channel[l]._toBeRemoved = true;
		}
	}
}

void IMuseDigital::pause(bool p) {
	_pause = p;
}

int32 IMuseDigital::doCommand(int a, int b, int c, int d, int e, int f, int g, int h) {
	byte cmd = a & 0xFF;
	byte param = a >> 8;
	int32 sample = b, r;
	byte sub_cmd = c >> 8;
	int8 chan = -1, l;
	int8 tmp;

	if (!(cmd || param))
		return 1;

	if (param == 0) {
		switch (cmd) {
		case 12:
			switch (sub_cmd) {
			case 5:
				debug(2, "IMuseDigital::doCommand 12,5 sample(%d), param(%d)", sample, d);
				return 0;
			case 6: // volume control (0-127)
				debug(2, "IMuseDigital::doCommand setting volume sample(%d), volume(%d)", sample, d);
				for (l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
					if ((_channel[l]._idSound == sample) && (_channel[l]._used == true)) {
						chan = l;
						break;
					}
				}
				if (chan == -1) {
					debug(2, "IMuseDigital::doCommand 12,6 sample(%d) not exist in channels", sample);
					return 1;
				}
				_channel[chan]._volume = d;
				_channel[chan]._volumeRight = d;
				if (_channel[chan]._volumeFade != -1) {
					tmp = ((_channel[chan]._volumeFade - _channel[chan]._volume) * 2) / _channel[chan]._volumeFadeParam;
					if ((tmp < 0) && (tmp > -2)) {
						tmp = -1;
					} else if ((tmp > 0) && (tmp < 2)) {
						tmp = 1;
					} else {
						tmp /= 2;
					}
				_channel[chan]._volumeFadeStep = tmp;
				}
				return 0;
			case 7: // right volume control (0-127)
				debug(2, "IMuseDigital::doCommand setting right volume sample(%d),volume(%d)", sample, d);
				for (l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
					if ((_channel[l]._idSound == sample) && (_channel[l]._used == true)) {
						chan = l;
						break;
					}
				}
				if (chan == -1) {
					debug(2, "IMuseDigital::doCommand 12,7 sample(%d) not exist in channels", sample);
					return 1;
				}
				_channel[chan]._volumeRight = d;
				return 0;
			default:
				warning("IMuseDigital::doCommand 12 DEFAULT sub command %d", sub_cmd);
				return 1;
			}
		case 14:
			switch (sub_cmd) {
			case 6: // fade volume control
				debug(2, "IMuseDigital::doCommand fading volume sample(%d),fade(%d, %d)", sample, d, e);
				for (l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
					if ((_channel[l]._idSound == sample) && (_channel[l]._used == true)) {
						chan = l;
						break;
					}
				}
				if (chan == -1) {
						debug(2, "IMuseDigital::doCommand 14,6 sample %d not exist in channels", sample);
					return 1;
				}
				_channel[chan]._volumeFade = d;
				_channel[chan]._volumeFadeParam = e;
				tmp = ((_channel[chan]._volumeFade - _channel[chan]._volume) * 2) / _channel[chan]._volumeFadeParam;
				if ((tmp < 0) && (tmp > -2)) {
					tmp = -1;
				} else if ((tmp > 0) && (tmp < 2)) {
					tmp = 1;
				} else {
					tmp /= 2;
				}
				_channel[chan]._volumeFadeStep = tmp;
				return 0;
			default:
				warning("IMuseDigital::doCommand 14 DEFAULT sub command %d", sub_cmd);
				return 1;
			}
		default:
			warning("IMuseDigital::doCommand DEFAULT command %d", cmd);
			return 1;
		}
	} else if (param == 16) {
		switch (cmd) {
		case 0: // play music (state)
			debug(2, "IMuseDigital::doCommand 0x1000 (%d)", b);
			if (_scumm->_gameId == GID_DIG) {
				for(l = 0;; l++) {
					if (_digStateMusicMap[l].room == -1) {
						return 1;
					}
					if (_digStateMusicMap[l].room == b) {
						int16 music = _digStateMusicMap[l].table_index;
						debug(2, "Play imuse music: %s, %s, %s", _digStateMusicTable[music].name, _digStateMusicTable[music].title, _digStateMusicTable[music].filename);
						if (_digStateMusicTable[music].filename[0] != 0) {
							_scumm->_sound->playBundleMusic((char*)&_digStateMusicTable[music].filename);
						}
						return 0;
					}
				}
			} else if (_scumm->_gameId == GID_FT) {
				for(l = 0;; l++) {
					if (_ftStateMusicTable[l].index == -1) {
						return 1;
					}
					if (_ftStateMusicTable[l].index == b) {
						debug(2, "Play imuse music: %s, %s", _ftStateMusicTable[l].name, _ftStateMusicTable[l].audioname);
						if (_ftStateMusicTable[l].audioname[0] != 0) {
							for(r = 0; r < _scumm->_numAudioNames; r++) {
								if (strcmp(_ftStateMusicTable[l].audioname, &_scumm->_audioNames[r * 9]) == 0) {
									startSound(r);
									doCommand(12, r, 1536, _ftStateMusicTable[l].volume, 0, 0, 0, 0);
								}
							}
						}
					}
				}
			}
			return 0;
		case 1: // play music (seq)
			debug(2, "IMuseDigital::doCommand 0x1001 (%d)", b);
			if (_scumm->_gameId == GID_DIG) {
				for(l = 0;; l++) {
					if (_digSeqMusicTable[l].index == -1) {
						return 1;
					}
					if ((_digSeqMusicTable[l].index == b)) {
						debug(2, "Play imuse music: %s, %s, %s", _digSeqMusicTable[l].name, _digSeqMusicTable[l].title, _digSeqMusicTable[l].filename);
						if (_digSeqMusicTable[l].filename[0] != 0) {
							_scumm->_sound->playBundleMusic((char*)&_digSeqMusicTable[l].filename);
						}
						return 0;
					}
				}
			} else if (_scumm->_gameId == GID_FT) {
				for(l = 0;; l++) {
					if (_ftSeqMusicTable[l].index == -1) {
						return 1;
					}
					if (_ftSeqMusicTable[l].index == b) {
						debug(2, "Play imuse music: %s, %s", _ftSeqMusicTable[l].name, _ftSeqMusicTable[l].audioname);
						if (_ftSeqMusicTable[l].audioname[0] != 0) {
							for(r = 0; r < _scumm->_numAudioNames; r++) {
								if (strcmp(_ftSeqMusicTable[l].audioname, &_scumm->_audioNames[r * 9]) == 0) {
									startSound(r);
									doCommand(12, r, 1536, _ftSeqMusicTable[l].volume, 0, 0, 0, 0);
								}
							}
						}
					}
				}
			}
			return 0;
		case 2: // dummy in DIG and CMI
			debug(2, "IMuseDigital::doCommand 0x1002 (%d)", b);
			return 0;
		case 3: // ??? (stream related)
			debug(2, "IMuseDigital::doCommand 0x1003 (%d,%d)", b, c);
			return 0;
		default:
			warning("IMuseDigital::doCommand (0x1xxx) DEFAULT command %d", cmd);
			return 1;
		}
	}

	return 1;
}

int IMuseDigital::getSoundStatus(int sound) {
	debug(2, "IMuseDigital::getSoundStatus(%d)", sound);
	for (int32 l = 0; l < MAX_DIGITAL_CHANNELS; l++) {
		if ((_channel[l]._idSound == sound) && (_channel[l]._used == true)) {
			return 1;
		}
	}

	return 0;
}
