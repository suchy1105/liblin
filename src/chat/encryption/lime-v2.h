/*
 * cpim-chat-message-modifier.h
 * Copyright (C) 2010-2018 Belledonne Communications SARL
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef _L_LIME_V2_H_
#define _L_LIME_V2_H_

#include "belle-sip/http-listener.h"
#include "encryption-engine-listener.h"
#include "lime/lime.hpp"

#include <belle-sip/belle-sip.h>
//#include "linphone/utils/general.h"
#include "carddav.h"

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

class BelleSipLimeManager : public lime::LimeManager {
public:
	BelleSipLimeManager(const std::string &db_access, belle_http_provider_t *prov);
	//void X3DHBelleSipServerPost(const std::string &url, const std::string &from, const std::vector<uint8_t> &message, const lime::limeX3DHServerResponseProcess &responseProcess);

private:
	static void processIoError (void *data, const belle_sip_io_error_event_t *event) noexcept;
	static void processResponse(void *data, const belle_http_response_event_t *event) noexcept;
	static void processAuthRequestedFromCarddavRequest(void *data, belle_sip_auth_event_t *event) noexcept;

	std::function<void(void *data, belle_sip_auth_event *event)> mProcessAuthRequest;
};

class LimeV2 : public EncryptionEngineListener {
public:
	LimeV2(const std::string &db_access, belle_http_provider_t *prov); // no manager without Database and http provider

	ChatMessageModifier::Result processIncomingMessage (const std::shared_ptr<ChatMessage> &message, int &errorCode) override;
	ChatMessageModifier::Result processOutgoingMessage (const std::shared_ptr<ChatMessage> &message, int &errorCode) override;
	bool encryptionEnabledForFileTransferCb (const std::shared_ptr<AbstractChatRoom> &ChatRoom) override;
	void generateFileTransferKeyCb (const std::shared_ptr<AbstractChatRoom> &ChatRoom, const std::shared_ptr<ChatMessage> &message) override;
	int downloadingFileCb (const std::shared_ptr<ChatMessage> &message, size_t offset, const uint8_t *buffer, size_t size, uint8_t *decrypted_buffer) override;
	int uploadingFileCb (const std::shared_ptr<ChatMessage> &message, size_t offset, const uint8_t *buffer, size_t size, uint8_t *encrypted_buffer) override;

private:
	std::unique_ptr<BelleSipLimeManager> belleSipLimeManager;
};

LINPHONE_END_NAMESPACE

#endif // _L_LIME_V2_H_
