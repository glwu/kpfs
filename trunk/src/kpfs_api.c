/*
   (c) Copyright 2012  Tao Yu

   All rights reserved.

   Written by Tao Yu <yut616@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <oauth.h>

#include "kpfs.h"
#include "kpfs_oauth.h"
#include "kpfs_api.h"
#include "kpfs_conf.h"

static char *kpfs_api_common_request(char *url)
{
	char *t_key = kpfs_oauth_token_get();
	char *t_secret = kpfs_oauth_token_secret_get();
	char *req_url = NULL;
	char *reply = NULL;
	char *ret = NULL;

	KPFS_FILE_LOG("request url: %s ...\n", url);

	req_url =
	    oauth_sign_url2(url, NULL, OA_HMAC, NULL, (const char *)kpfs_conf_get_consumer_key(), (const char *)kpfs_conf_get_consumer_secret(), t_key,
			    t_secret);
	if (!req_url) {
		goto error_out;
	}

	KPFS_FILE_LOG("real url: %s.\n", req_url);

	reply = oauth_http_get(req_url, NULL);

	if (!reply) {
		goto error_out;
	}

	KPFS_FILE_LOG("response: %s\n", reply);
	ret = reply;

error_out:
	KPFS_SAFE_FREE(req_url);

	return ret;
}

char *kpfs_api_account_info()
{
	return kpfs_api_common_request(KPFS_API_ACCOUNT_INFO);
}

char *kpfs_api_metadata(const char *path)
{
	char fullpath[1024] = { 0 };

	snprintf(fullpath, sizeof(fullpath), "%s%s", KPFS_API_METADATA "/" KPFS_API_ROOT, path);
	KPFS_FILE_LOG("fullpath %s:\n", fullpath);
	return kpfs_api_common_request(fullpath);
}

char *kpfs_api_download_link_create(const char *path)
{
	char *t_key = kpfs_oauth_token_get();
	char *t_secret = kpfs_oauth_token_secret_get();
	char *url = KPFS_API_DOWNLOAD_FILE;
	char *req_url = NULL;
	char *reply = NULL;
	char *ret = NULL;

	if (NULL == path)
		return ret;

	KPFS_FILE_LOG("request url: %s ...\n", url);

	req_url =
	    oauth_sign_url2(url, NULL, OA_HMAC, NULL, (const char *)kpfs_conf_get_consumer_key(), (const char *)kpfs_conf_get_consumer_secret(), t_key,
			    t_secret);
	if (!req_url) {
		goto error_out;
	}

	KPFS_FILE_LOG("real url: %s.\n", req_url);

	reply = oauth_http_get(req_url, NULL);

	if (!reply) {
		goto error_out;
	}

	KPFS_FILE_LOG("response: %s\n", reply);
	ret = reply;

error_out:
	KPFS_SAFE_FREE(req_url);

	return ret;
}
