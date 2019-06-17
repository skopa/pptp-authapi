/*
 * $Id: options.h,v 1.1 2004/11/14 07:26:26 paulus Exp $
 *
 * Copyright (C) 1996 Lars Fenneberg
 *
 * See the file COPYRIGHT for the respective terms and conditions. 
 * If the file is missing contact me at lf@elemental.net 
 * and I'll send you a copy.
 *
 */

char *auth_api_url = NULL;
char *auth_api_secret = NULL;

static option_t options[] = {
		{ "authapi-url", o_string, &auth_api_url, "Set auth api url" },
		{ "authapi-secret", o_string, &auth_api_secret, "Set auth api secret key" },
		{ NULL }
};