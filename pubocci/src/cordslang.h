/* ---------------------------------------------------------------------------- */
/* Advanced Capabilities for Compatible One Resources Delivery System - ACCORDS	*/
/* (C) 2011 by Iain James Marshall <ijm667@hotmail.com>				*/
/* ---------------------------------------------------------------------------- */
/*										*/
/* This is free software; you can redistribute it and/or modify it		*/
/* under the terms of the GNU Lesser General Public License as			*/
/* published by the Free Software Foundation; either version 2.1 of		*/
/* the License, or (at your option) any later version.				*/
/*										*/
/* This software is distributed in the hope that it will be useful,		*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of		*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU		*/
/* Lesser General Public License for more details.				*/
/*										*/
/* You should have received a copy of the GNU Lesser General Public		*/
/* License along with this software; if not, write to the Free			*/
/* Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA		*/
/* 02110-1301 USA, or see the FSF site: http://www.fsf.org.			*/
/*										*/
/* ---------------------------------------------------------------------------- */
#ifndef	_cords_language_h
#define	_cords_language_h

#define	_CORDS_NS	"http://www.compatibleone.fr/schemes/cords.xsd"

/*	------------------------	*/
/*	 cords attribute values		*/
/*	------------------------	*/
#define	_CORDS_NULL	"(null)"
#define	_CORDS_ANY	"any"
#define	_CORDS_NONE	"none"
#define	_CORDS_SIMPLE	"simple"
#define	_CORDS_PUBLIC	"public"
#define	_CORDS_PRIVATE	"private"
#define	_CORDS_COMMON	"common"
#define	_CORDS_NORMAL	"normal"
#define	_CORDS_GLOBAL	"global"

/*	------------------------	*/
/*	cords action identifiers	*/
/*	------------------------	*/
#define	_CORDS_START	"start"
#define	_CORDS_SUSPEND	"suspend"
#define	_CORDS_SAVE	"save"
#define	_CORDS_SNAPSHOT "snapshot"
#define	_CORDS_RESTART	"restart"
#define	_CORDS_STOP	"stop"
#define	_CORDS_INSTANCE	"instance"
#define	_CORDS_BUILD	"build"
#define	_CORDS_CHOOSE	"choose"

/*	-------------------------	*/
/*	cords atribut identifiers	*/
/*	-------------------------	*/
#define	_CORDS_ID		"id"
#define	_CORDS_NAME		"name"
#define	_CORDS_MEMORY		"memory"
#define	_CORDS_SPEED		"speed"
#define	_CORDS_CORES		"cores"
#define	_CORDS_SIZE		"size"
#define	_CORDS_TYPE		"type"
#define	_CORDS_ACCESS		"access"
#define	_CORDS_SCOPE		"scope"
#define	_CORDS_CREATED		"created"
#define	_CORDS_VALIDATION	"validation"

/*	-------------------------	*/
/*	cords element identifiers	*/
/*	-------------------------	*/
#define	_CORDS_PUBLICATION	"publication"

#define	_CORDS_MANIFEST		"manifest"
#define	_CORDS_DESCRIPTION	"description"
#define	_CORDS_SECURITY		"security"
#define	_CORDS_ACCOUNT		"account"
#define	_CORDS_USER		"user"

/*	------------------------	*/
/*	cords node atributs		*/
/*	------------------------	*/
#define	_CORDS_NODE		"node"
#define	_CORDS_PROVIDER		"provider"
#define	_CORDS_PROFILE 		"profile"
#define	_CORDS_INFRASTRUCTURE	"infrastructure"
#define	_CORDS_COMPUTE		"compute"
#define	_CORDS_STORAGE		"storage"
#define	_CORDS_NETWORK		"network"
#define	_CORDS_IMAGE		"image"
#define	_CORDS_SYSTEM		"system"
#define	_CORDS_PACKAGE		"package"
#define	_CORDS_PORT		"port"

#define	_CORDS_CONFIGURATION	"configuration"
#define	_CORDS_INTERFACE	"interface"
#define	_CORDS_ACTION		"action"
#define	_CORDS_PARAMETER	"parameter"
#define	_CORDS_INSTRUCTION	"instruction"
#define	_CORDS_VALUE		"value"
#define	_CORDS_EXPRESSION	"expression"

#define	_CORDS_PLACEMENT	"placement"
#define	_CORDS_SOLUTION 	"solution"
#define	_CORDS_PLAN		"plan"
#define	_CORDS_SERVICE		"service"

#define	_CORDS_OPENSTACK	"openstack"
#define	_CORDS_OPENNEBULA	"opennebula"
#define	_CORDS_WINDOWSAZURE	"windowsazure"
#define	_CORDS_SLAPOS		"slapos"
#define	_CORDS_PROACTIVE	"proactive"

#define	_CORDS_EVENT		"event"
#define	_CORDS_MONITOR		"monitor"
#define	_CORDS_REPORT		"report"
#define	_CORDS_SESSION		"session"
#define	_CORDS_CONSUMER		"consumer"
#define	_CORDS_CONNECTION	"connection"
#define	_CORDS_STREAM		"stream"
#define	_CORDS_PROBE		"probe"
#define	_CORDS_METRIC		"metric"
#define	_CORDS_ALERT		"alert"

#define	_CORDS_TRANSACTION	"transaction"
#define	_CORDS_APPLICATION	"application"
#define	_CORDS_SCHEDULE		"schedule"

#define	_CORDS_METADATA		"metadata"
#define	_CORDS_FILE		"file"
#define	_CORDS_SCRIPT		"script"
#define	_CORDS_VM		"vm"
#define	_CORDS_IPADDRESS	"ipaddress"

/*	------------------------	*/
/*	cords  parser  feed back	*/
/*	------------------------	*/
#define	_CORDS_ERROR		"error"
#define	_CORDS_WARNING		"warning"
#define	_CORDS_CHOICE		"choice"

/*	------------------------	*/
/*	cords machine attributes	*/
/*	------------------------	*/
#define	_CORDS_CONTRACT		"contract"
#define	_CORDS_TARIFICATION	"tarification"
#define	_CORDS_PRICE		"price"
#define	_CORDS_AUTHORIZATION	"authorization"
#define	_CORDS_HOSTNAME		"hostname"
#define	_CORDS_ROOTPASS		"rootpass"
#define	_CORDS_REFERENCE	"reference"
#define	_CORDS_CATEGORY		"category"

	/* ----------------- */
#endif	/* _cords_language_h */
	/* ----------------- */

