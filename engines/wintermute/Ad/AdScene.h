/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#ifndef WINTERMUTE_ADSCENE_H
#define WINTERMUTE_ADSCENE_H

#include "engines/wintermute/Base/BFader.h"

namespace WinterMute {

class CUIWindow;
class CAdObject;
class CAdRegion;
class CBViewport;
class CAdLayer;
class CBPoint;
class CAdWaypointGroup;
class CAdPath;
class CAdScaleLevel;
class CAdRotLevel;
class CAdPathPoint;
class CAdScene : public CBObject {
public:

	CBObject *getNextAccessObject(CBObject *CurrObject);
	CBObject *getPrevAccessObject(CBObject *CurrObject);
	HRESULT getSceneObjects(CBArray<CAdObject *, CAdObject *> &Objects, bool InteractiveOnly);
	HRESULT getRegionObjects(CAdRegion *Region, CBArray<CAdObject *, CAdObject *> &Objects, bool InteractiveOnly);

	HRESULT afterLoad();

	HRESULT getRegionsAt(int X, int Y, CAdRegion **RegionList, int NumRegions);
	HRESULT handleItemAssociations(const char *ItemName, bool Show);
	CUIWindow *_shieldWindow;
	float getRotationAt(int X, int Y);
	HRESULT loadState();
	HRESULT saveState();
	bool _persistentState;
	bool _persistentStateSprites;
	CBObject *getNodeByName(const char *name);
	void setOffset(int OffsetLeft, int OffsetTop);
	bool pointInViewport(int X, int Y);
	int getOffsetTop();
	int getOffsetLeft();
	HRESULT getViewportSize(int *Width = NULL, int *Height = NULL);
	HRESULT getViewportOffset(int *OffsetX = NULL, int *OffsetY = NULL);
	CBViewport *_viewport;
	CBFader *_fader;
	int _pfPointsNum;
	void pfPointsAdd(int X, int Y, int Distance);
	void pfPointsStart();
	bool _initialized;
	HRESULT correctTargetPoint(int StartX, int StartY, int *X, int *Y, bool CheckFreeObjects = false, CBObject *Requester = NULL);
	HRESULT correctTargetPoint2(int StartX, int StartY, int *TargetX, int *TargetY, bool CheckFreeObjects, CBObject *Requester);
	DECLARE_PERSISTENT(CAdScene, CBObject)
	HRESULT displayRegionContent(CAdRegion *Region = NULL, bool Display3DOnly = false);
	HRESULT displayRegionContentOld(CAdRegion *Region = NULL);
	static int compareObjs(const void *Obj1, const void *Obj2);

	HRESULT updateFreeObjects();
	HRESULT traverseNodes(bool Update = false);
	float getScaleAt(int Y);
	HRESULT sortScaleLevels();
	HRESULT sortRotLevels();
	virtual HRESULT saveAsText(CBDynBuffer *buffer, int indent);
	uint32 getAlphaAt(int X, int Y, bool ColorCheck = false);
	bool _paralaxScrolling;
	void skipTo(int OffsetX, int OffsetY);
	void setDefaults();
	void cleanup();
	void skipToObject(CBObject *Object);
	void scrollToObject(CBObject *Object);
	void scrollTo(int OffsetX, int OffsetY);
	virtual HRESULT update();
	bool _autoScroll;
	int _targetOffsetTop;
	int _targetOffsetLeft;

	int _scrollPixelsV;
	uint32 _scrollTimeV;
	uint32 _lastTimeV;

	int _scrollPixelsH;
	uint32 _scrollTimeH;
	uint32 _lastTimeH;

	virtual HRESULT display();
	uint32 _pfMaxTime;
	HRESULT initLoop();
	void pathFinderStep();
	bool isBlockedAt(int X, int Y, bool CheckFreeObjects = false, CBObject *Requester = NULL);
	bool isWalkableAt(int X, int Y, bool CheckFreeObjects = false, CBObject *Requester = NULL);
	CAdLayer *_mainLayer;
	float getZoomAt(int X, int Y);
	bool getPath(CBPoint source, CBPoint target, CAdPath *path, CBObject *requester = NULL);
	CAdScene(CBGame *inGame);
	virtual ~CAdScene();
	CBArray<CAdLayer *, CAdLayer *> _layers;
	CBArray<CAdObject *, CAdObject *> _objects;
	CBArray<CAdWaypointGroup *, CAdWaypointGroup *> _waypointGroups;
	HRESULT loadFile(const char *filename);
	HRESULT loadBuffer(byte *buffer, bool complete = true);
	int _width;
	int _height;
	HRESULT addObject(CAdObject *Object);
	HRESULT removeObject(CAdObject *Object);
	int _editorMarginH;
	int _editorMarginV;
	uint32 _editorColFrame;
	uint32 _editorColEntity;
	uint32 _editorColRegion;
	uint32 _editorColBlocked;
	uint32 _editorColWaypoints;
	uint32 _editorColEntitySel;
	uint32 _editorColRegionSel;
	uint32 _editorColBlockedSel;
	uint32 _editorColWaypointsSel;
	uint32 _editorColScale;
	uint32 _editorColDecor;
	uint32 _editorColDecorSel;

	bool _editorShowRegions;
	bool _editorShowBlocked;
	bool _editorShowDecor;
	bool _editorShowEntities;
	bool _editorShowScale;
	CBArray<CAdScaleLevel *, CAdScaleLevel *> _scaleLevels;
	CBArray<CAdRotLevel *, CAdRotLevel *> _rotLevels;

	virtual HRESULT restoreDeviceObjects();
	int getPointsDist(CBPoint p1, CBPoint p2, CBObject *requester = NULL);

	// scripting interface
	virtual CScValue *scGetProperty(const char *name);
	virtual HRESULT scSetProperty(const char *name, CScValue *value);
	virtual HRESULT scCallMethod(CScScript *script, CScStack *stack, CScStack *thisStack, const char *name);
	virtual const char *scToString();


private:
	HRESULT persistState(bool Saving = true);
	void pfAddWaypointGroup(CAdWaypointGroup *Wpt, CBObject *Requester = NULL);
	bool _pfReady;
	CBPoint *_pfTarget;
	CAdPath *_pfTargetPath;
	CBObject *_pfRequester;
	CBArray<CAdPathPoint *, CAdPathPoint *> _pfPath;

	int _offsetTop;
	int _offsetLeft;

};

} // end of namespace WinterMute

#endif
