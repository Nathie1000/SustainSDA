/*
 * HttpClient.h
 *
 *  Created on: 20 sep. 2016
 *      Author: Nathan
 */

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include "Bearer.h"

class HttpClient : public Bearer{
private:
	int lastResponseStatus;

	bool httpAction(bool method, int &status, int &length);
	String send(const String &url, const String &body, bool method, const String &contentType);

public:
	/** Content type used for posting. */
	static const String CONTENT_TYPE_POST;
	/** Content type used for plain text/ */
	static const String CONTENT_TYPE_PLAIN_TEXT;
	/** Content type used for JSON */
	static const String CONTENT_TYPE_JSON;
	/** Content type used for Javascript. */
	static const String CONTENT_TYPE_JAVASCRIPT;

public:
	/**
	 * Create a new object.
	 * @param at the {@link AtClient} need for communication.
	 */
	explicit HttpClient(AtClient &at);


	/**
	 * Do a HTTP POST action.
	 * This method can not be used to send or receive binary data.
	 * This method only works if successfully connected.
	 * @param url the site name or IP.
	 * @param body the data to send.
	 * @param contentType the type of data. Important: this must be a valid type in order to for the POST to complete.
	 * @return the response parsed as a String.
	 */
	String post(const String &url, const String &body = "", const String &contentType = CONTENT_TYPE_POST);

	/**
	 * Do a HTTP GET action.
	 * GET request do not support writing.
	 * This method can not be used to send or receive binary data.
	 * This method only works if successfully connected.
	 * @param url the site name or IP.
	 * @return the response parsed as a String.
	 */
	String get(const String &url);

	int getStatus();
};



#endif /* HTTPCLIENT_H_ */
