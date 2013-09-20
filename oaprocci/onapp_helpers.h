/*!
 * @file onapp_helpers.h
 * @brief Helper functions for OnApp code.
 *  Created on: 30 Jul 2013
 *      Author: mE
 */

#ifndef ONAPP_HELPERS_H_
#define ONAPP_HELPERS_H_

#include <stddef.h>  // NULL
#include <stdio.h> // FILE

struct oa_config;
struct onapp;
struct onapp_extras;
struct url;
struct occi_attribute;
struct occi_category;
struct rest_response;
struct occi_response;
struct occi_client;
struct cords_onapp_extras;

/// @brief Boolean used by OnApp code.
typedef enum OnAppBool_
{
	OABOOL_FALSE = 0,
	OABOOL_TRUE = 1
} OnAppBool;

/// @defgroup oa_string_handling OnApp String handling
/// @brief String handling functions used inside OnApp code.
/// @{
/// @brief Check for NULL or empty (just NULL-terminator) string.
/// @param s  String to examine.
/// @return OABOOL_TRUE if string is NULL or empty, otherwise OABOOL_FALSE.
static inline OnAppBool string_is_null_or_empty(char const * const s) { return (s == NULL || *s == '\0') ? OABOOL_TRUE : OABOOL_FALSE; }

/// @brief Examine string for prefix.
/// @param target     Target to check for prefix.
/// @param prefix     Supposed prefix.
/// @param ignorecase If non-zero, ignore case during comparison.
/// @return -1 if prefix larger than target, 1 if target larger than prefix, otherwise 0.
extern int strprefix(char const *target, char const *prefix);

/// @brief Examine string for prefix, ignoring case.
/// @param target     Target to check for prefix.
/// @param prefix     Supposed prefix.
/// @param ignorecase If non-zero, ignore case during comparison.
/// @return -1 if prefix larger than target, 1 if target larger than prefix, otherwise 0.
extern int strprefixcase(char const *target, char const *prefix);

/// @brief Convert a numeric string with byte-size suffix into appropriate string for OnApp.
/// @param sptr[in] Numeric string with size suffix e.g. 1G for one gigabyte.
/// @param normal[in] Suffix expected, used if not specified in sptr.
/// @param ppzeroed_string[out] Numeric string expanded with appropriate number of zeroes and suffix for OnApp processing.
/// @return 0 if successful.
extern int onapp_normalise_value( char * sptr, int normal, char **ppzeroed_string );

/// @brief Convert a numeric string with byte-size suffix into appropriate string for OnApp, liberating the string on completion.
/// @param sptr[in] Numeric string with size suffix e.g. 1G for one gigabyte, liberated at end of execution.
/// @param normal[in] Suffix expected, used if not specified in sptr.
/// @param ppzeroed_string[out] Numeric string expanded with appropriate number of zeroes and suffix for OnApp processing.
/// @return 0 if successful.
extern int onapp_normalise_value_liberate( char * sptr, int normal, char **ppzeroed_string );

/// @brief Convert a numeric string to an integer, *and* liberate the string on completion.
/// @return Numeric string value.
extern int onapp_atoi(char *p);

/// @}

/// @defgroup oa_time_handling OnApp Time handling
/// @brief Time handling functions used inside OnApp code.
/// @{
/// @brief Get the current time as an integer.
/// @return Time stamp.
extern int onapp_current_timestamp_int();

/// @brief Get the current time in a string format.
/// @return Newly allocated string containing current time.
extern char * onapp_current_timestamp_string(char *default_value);

/// @}

/// @defgroup oa_http_handling OnApp HTTP handling
/// @brief HTTP functions used inside OnApp code.
/// @{
/// @brief Populate a buffer with a string representation of the HTTP body length.
/// @param buffer[out]	Buffer to populate.
/// @param buffer_size[in]	Buffer size.
/// @param type	HTTP body type e.g. _FILE_BODY, _TEXT_BODY.
/// @param body HTTP body.
/// @return NULL if fail to discover buffer length, otherwise pointer to buffer.
extern char * get_http_body_length_string(char *buffer, size_t buffer_size, int type, char const *body);
/// @}

/// @defgroup oa_file_handling OnApp File handling
/// @brief File handling functions used inside OnApp code.
/// @{
/// @brief Write content of specified file to file handle, with prefix and suffix strings if specified.
/// @param filename Name of file whose contents are being written from.
/// @param h Handle to file being written to.
/// @param prefix Prefix string to write to file handle [optional].
/// @param suffix Suffix string to write to file handle [optional].
extern size_t write_filecontent_to_filehandle(char const * const filename, FILE* h, char const * const prefix, char const * const suffix);

/// @brief Write content of specified file to specified target file, with prefix and suffix strings if specified.
/// @param filename Name of file whose contents are being written from.
/// @param output_filename Name of file being written to. Existing content is truncated.
/// @param prefix Prefix string to write to output file [optional].
/// @param suffix Suffix string to write to output file [optional].
extern size_t write_filecontent_to_filename(char const * const filename, char const * const output_filename, char const * const prefix, char const * const suffix);
/// @}

/// @defgroup oa_dump OnApp Variable Dumping
/// @brief Variable dumping functions used inside OnApp code.
/// @{
/// @brief Dump oa_config to FILE.
/// @param poa_config oa_config to dump.
/// @param h File to dump to.
/// @return 0 if successful.
extern int dump_oa_config_data(FILE *h, struct oa_config const * poa_config);

/// @brief Dump oa_config and onapp to stdout.
/// @param poa_config oa_config to dump.
/// @param h File to dump to.
/// @return 0 if successful.
extern int dump_oa_config_data_stdout(struct oa_config const * poa_config);

/// @brief Dump onapp to FILE.
/// @param ponapp onapp to dump.
/// @param h File to dump to.
/// @return 0 if successful.
extern int dump_onapp_data(FILE *h, struct onapp *ponapp);

/// @brief Dump oa_config and onapp to stdout.
/// @param pptr oa_config to dump.
/// @param ponapp onapp to dump.
/// @param h File to dump to.
/// @return 0 if successful.
extern int dump_onapp_data_stdout(struct onapp *ponapp);

/// @brief Dump url to FILE.
/// @param purl url to dump.
/// @param h File to dump to.
extern void dump_url(FILE *hOutput, struct url const *purl);

/// @brief Dump occi_attribute to FILE.
/// @param pocci_attribute occi_attribute to dump.
/// @param h File to dump to.
extern void dump_occi_attribute(FILE *hOutput, struct occi_attribute const *pocci_attribute);

extern void test_dump_occi_category(FILE *hOutput, struct occi_category const *pocci_category);

/// @brief Dump rest_response to FILE.
/// @param prest_response rest_response to dump.
/// @param h File to dump to.
extern void dump_rest_response(FILE *hOutput, struct rest_response const *prest_response);

/// @brief Dump occi_response to FILE.
/// @param pocci_response occi_response to dump.
/// @param h File to dump to.
extern void dump_occi_response(FILE* hOutput, struct occi_response const * pocci_response);

/// @brief Dump occi_response to FILE, and liberate it.
/// @param pocci_response occi_response to dump and liberate.
/// @param h File to dump to.
/// @return Result of liberating occi_response.
extern struct occi_response * dump_occi_response_liberate(FILE* hOutput, struct occi_response * pocci_response);

/// @brief Dump occi_client to FILE.
/// @param pocci_client occi_clientto dump.
/// @param h File to dump to.
extern void dump_occi_client(FILE *hOutput, struct occi_client const *pocci_client);

/// @brief Dump onapp_extras data from occi_response to FILE.
/// @param pocci_response onapp_extras data to dump.
/// @param h File to dump to.
extern void dump_onapp_extras_occi(FILE *hOutput, struct occi_response *pocci_response);

/// @brief Dump onapp_extras to FILE.
/// @param ponapp_extras onapp_extras data to dump.
/// @param h File to dump to.
extern void dump_onapp_extras(FILE *hOutput, struct cords_onapp_extras const *ponapp_extras);

#endif /* ONAPP_HELPERS_H_ */
