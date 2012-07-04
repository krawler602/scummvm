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

#include "engines/wintermute/dcgf.h"
#include "engines/wintermute/Base/BGame.h"
#include "engines/wintermute/Ad/AdNodeState.h"
#include "engines/wintermute/Ad/AdEntity.h"
#include "engines/wintermute/Base/BStringTable.h"
#include "engines/wintermute/Base/BSprite.h"
#include "engines/wintermute/utils/utils.h"
#include "engines/wintermute/PlatformSDL.h"
#include "common/str.h"

namespace WinterMute {

IMPLEMENT_PERSISTENT(CAdNodeState, false)


//////////////////////////////////////////////////////////////////////////
CAdNodeState::CAdNodeState(CBGame *inGame): CBBase(inGame) {
	_name = NULL;
	_active = false;
	for (int i = 0; i < 7; i++) _caption[i] = NULL;
	_alphaColor = 0;
	_filename = NULL;
	_cursor = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdNodeState::~CAdNodeState() {
	delete[] _name;
	delete[] _filename;
	delete[] _cursor;
	_name = NULL;
	_filename = NULL;
	_cursor = NULL;
	for (int i = 0; i < 7; i++) {
		delete[] _caption[i];
		_caption[i] = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
void CAdNodeState::setName(const char *name) {
	delete[] _name;
	_name = NULL;
	CBUtils::setString(&_name, name);
}


//////////////////////////////////////////////////////////////////////////
void CAdNodeState::setFilename(const char *filename) {
	delete[] _filename;
	_filename = NULL;
	CBUtils::setString(&_filename, filename);
}


//////////////////////////////////////////////////////////////////////////
void CAdNodeState::setCursor(const char *filename) {
	delete[] _cursor;
	_cursor = NULL;
	CBUtils::setString(&_cursor, filename);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdNodeState::persist(CBPersistMgr *persistMgr) {
	persistMgr->transfer(TMEMBER(Game));

	persistMgr->transfer(TMEMBER(_active));
	persistMgr->transfer(TMEMBER(_name));
	persistMgr->transfer(TMEMBER(_filename));
	persistMgr->transfer(TMEMBER(_cursor));
	persistMgr->transfer(TMEMBER(_alphaColor));
	for (int i = 0; i < 7; i++) persistMgr->transfer(TMEMBER(_caption[i]));

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CAdNodeState::setCaption(const char *caption, int caseVal) {
	if (caseVal== 0) caseVal= 1;
	if (caseVal< 1 || caseVal> 7) return;

	delete[] _caption[caseVal- 1];
	_caption[caseVal- 1] = new char[strlen(caption) + 1];
	if (_caption[caseVal- 1]) {
		strcpy(_caption[caseVal- 1], caption);
		Game->_stringTable->Expand(&_caption[caseVal- 1]);
	}
}


//////////////////////////////////////////////////////////////////////////
char *CAdNodeState::getCaption(int caseVal) {
	if (caseVal== 0) caseVal= 1;
	if (caseVal< 1 || caseVal> 7 || _caption[caseVal- 1] == NULL) return "";
	else return _caption[caseVal- 1];
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdNodeState::transferEntity(CAdEntity *entity, bool includingSprites, bool saving) {
	if (!entity) return E_FAIL;

	// hack!
	if (this->Game != entity->Game) this->Game = entity->Game;

	if (saving) {
		for (int i = 0; i < 7; i++) {
			if (entity->_caption[i]) setCaption(entity->_caption[i], i);
		}
		if (!entity->_region && entity->_sprite && entity->_sprite->_filename) {
			if (includingSprites) setFilename(entity->_sprite->_filename);
			else setFilename("");
		}
		if (entity->_cursor && entity->_cursor->_filename) setCursor(entity->_cursor->_filename);
		_alphaColor = entity->_alphaColor;
		_active = entity->_active;
	} else {
		for (int i = 0; i < 7; i++) {
			if (_caption[i]) entity->setCaption(_caption[i], i);
		}
		if (_filename && !entity->_region && includingSprites && strcmp(_filename, "") != 0) {
			if (!entity->_sprite || !entity->_sprite->_filename || scumm_stricmp(entity->_sprite->_filename, _filename) != 0)
				entity->setSprite(_filename);
		}
		if (_cursor) {
			if (!entity->_cursor || !entity->_cursor->_filename || scumm_stricmp(entity->_cursor->_filename, _cursor) != 0)
				entity->setCursor(_cursor);
		}

		entity->_active = _active;
		entity->_alphaColor = _alphaColor;
	}

	return S_OK;
}

} // end of namespace WinterMute
