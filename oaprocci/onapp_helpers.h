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

/// @defgroup oa_file_handling OnApp File handling
/// @brief File handling functions used inside OnApp code.
/// @{
/// @brief Write content of specified file to file handle, with prefixed strings if specified.
/// @param filename Name of file whose contents are being written from.
/// @param h Handle to file being written to.
/// @param prefix Prefix string to write to file handle [optional].
/// @param suffix Suffix string to write to file handle [optional].
extern size_t write_filecontent_to_filehandle(char const * const filename, FILE* h, char const * const prefix, char const * const suffix);

/// @brief Write content of specified file to specified target file, with prefixed strings if specified.
/// @param filename Name of file whose contents are being written from.
/// @param output_filename Name of file being written to. Existing content is truncated.
/// @param prefix Prefix string to write to output file [optional].
/// @param suffix Suffix string to write to output file [optional].
extern size_t write_filecontent_to_filename(char const * const filename, char const * const output_filename, char const * const prefix, char const * const suffix);
/// @}

/// @defgroup oa_dump OnApp Variable Dumping
/// @brief Variable dumping functions used inside OnApp code.
/// @{
/// @brief Dump oa_config and onapp to FILE.
/// @param pptr oa_config to dump.
/// @param ponapp onapp to dump.
/// @param h File to dump to.
/// @return 0 if successful.
extern int dump_onapp_data( struct oa_config * pptr, struct onapp *ponapp, FILE *h);

/// @brief Dump oa_config and onapp to stdout.
/// @param pptr oa_config to dump.
/// @param ponapp onapp to dump.
/// @param h File to dump to.
/// @return 0 if successful.
extern int dump_onapp_data_stdout(const struct oa_config * pptr, struct onapp *ponapp);

/// @brief Dump url to FILE.
/// @param purl url to dump.
/// @param h File to dump to.
extern void dump_url(struct url *purl, FILE *hOutput);

/// @brief Dump occi_attribute to FILE.
/// @param pocci_attribute occi_attribute to dump.
/// @param h File to dump to.
extern void dump_occi_attribute(struct occi_attribute *pocci_attribute, FILE *hOutput);

extern void test_dump_occi_category(struct occi_category *pocci_category, FILE *hOutput);

/// @brief Dump rest_response to FILE.
/// @param prest_response rest_response to dump.
/// @param h File to dump to.
extern void dump_rest_response(struct rest_response *prest_response, FILE *hOutput);

/// @brief Dump occi_response to FILE.
/// @param pocci_response occi_response to dump.
/// @param h File to dump to.
extern void dump_occi_response(struct occi_response* pocci_response, FILE* hOutput);

/// @brief Dump occi_response to FILE, and liberate it.
/// @param pocci_response occi_response to dump and liberate.
/// @param h File to dump to.
/// @return Result of liberating occi_response.
extern struct occi_response * dump_occi_response_liberate(struct occi_response * pocci_response, FILE* hOutput);

/// @brief Dump occi_client to FILE.
/// @param pocci_client occi_clientto dump.
/// @param h File to dump to.
extern void dump_occi_client(struct occi_client *pocci_client, FILE *hOutput);

/// @brief Dump onapp_extras data from occi_response to FILE.
/// @param pocci_response onapp_extras data to dump.
/// @param h File to dump to.
extern void dump_onapp_extras_occi(struct occi_response *pocci_response, FILE *hOutput);

/// @brief Dump onapp_extras to FILE.
/// @param ponapp_extras onapp_extras data to dump.
/// @param h File to dump to.
extern void dump_onapp_extras(struct cords_onapp_extras *ponapp_extras, FILE *hOutput);

#endif /* ONAPP_HELPERS_H_ */
