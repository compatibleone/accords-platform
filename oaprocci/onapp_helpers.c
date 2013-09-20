/*!
 * @file onapp_helpers.c
 * @brief Helper functions for OnApp code.
 *
 *  Created on: 30 Jul 2013
 *      Author: me
 */

#include "onapp_helpers.h"

#include <stdio.h>
#include <string.h>
#include <allocate.h> // allocate(), liberate(), allocate_string()
#include <time.h> // time()

#include "oaconfig.h"
#include "onapp.h"
#include "onapp_extras.h"
#include "occiatb.h"
#include "occiclient.h"

/// String functions.

/// @brief Implementation for string prefix comparison.
/// @param target     - Target to check for prefix.
/// @param prefix     - Supposed prefix.
/// @param ignorecase - If non-zero, ignore case during comparison.
/// @return -1 if prefix larger than target, 1 if target larger than prefix, otherwise 0.
int _strprefixcaseimpl(char const *target, char const *prefix, int ignorecase)
{
  int result = -1;
  enum longer { prefix_longer = -1, same = 0, target_longer = 1 };
  char casebuffer_prefix[2];
  char casebuffer_target[2];

  if (prefix == NULL) {
    if (target == NULL) {
      result = same;
    }
    else {
      // Left "longer" than target.
      result = prefix_longer;
    }
  }
  else if (target == NULL) {
    // Right "longer" than prefix.
    result = target_longer;
  }
  else if (*prefix == '\0') {
    if (*target == '\0') {
      result = same;
    }
    else {
      // Right "longer" than prefix.
      result = target_longer;
    }
  }
  else {
    result = same;
    if (ignorecase == 0) {
      while (*prefix != '\0') {
        if (*target == '\0') {
          result = prefix_longer;
        }
        else if (*prefix < *target) {
          result = target_longer;
          break;
        }
        else if (*prefix > *target) {
          result = prefix_longer;
          break;
        }
        ++prefix, ++target;
      }
    }
    else {
      casebuffer_prefix[1] = '\0';
      casebuffer_target[1] = '\0';

      do {
        casebuffer_prefix[0] = *prefix;
        casebuffer_target[0] = *target;
        result = strncasecmp(casebuffer_prefix, casebuffer_target, 1);
        ++prefix, ++target;

      } while (result == 0 && *prefix != '\0');
    }
  }

  return result;
}

int strprefix(char const *target, char const *prefix)
{
  return _strprefixcaseimpl(target, prefix, 0);
}

int strprefixcase(char const *target, char const *prefix)
{
  return _strprefixcaseimpl(target, prefix, 1);
}

/// String normalisation.
struct NumberSuffixData
{
  char lower;
  char upper;
  int numzeroes;
};

/// @brief Lookup number suffix data for a suffix.
/// @param suffix Suffix to check for.
/// @return Number Suffix data for suffix, or default if not found.
static struct NumberSuffixData const * get_number_suffix_data(char suffix)
{
  static struct NumberSuffixData const NumberSuffixes[] =
  {
    { 'k', 'K', 3 }
    ,{ 'm', 'M', 6 }
    ,{ 'g', 'G', 9 }
    ,{ 't', 'T', 12 }
  };
  static struct NumberSuffixData const Default = { 0, 0, 0 };
  static size_t const CountNumberSuffixes = sizeof(NumberSuffixes) / sizeof(*NumberSuffixes);

  struct NumberSuffixData const *psuffixdata = &Default;

  struct NumberSuffixData const *psuffixdataIter;
  for (psuffixdataIter = NumberSuffixes; psuffixdataIter < NumberSuffixes + CountNumberSuffixes; ++psuffixdataIter) {
    if (psuffixdataIter->lower == suffix || psuffixdataIter->upper == suffix) {
      psuffixdata = psuffixdataIter;
      break;
    }
  }

  return psuffixdata;
}

/// @brief Convert number to OnApp suitably formatted number allowing for size suffix.
/// @param source[in] String to convert.
/// @param StartType[in] Size type input string is treated as.
/// @param EndType[in] Size type input string should be converted to.
/// @param dropSuffix[in] Boolean - if true, do not include a size suffix in the result.
/// @param ppzeroed_string[out] Numeric string formatted for OnApp.
int onapp_normalise_number(char const *source, int StartType, int EndType, int dropSuffix, char **ppzeroed_string)
{
  char const *sourceiter;
  ssize_t zeroiter;
  ssize_t zeroes = 0;
  char * pvalue_with_zeroes = NULL;
  ssize_t count_source = 0;
  ssize_t count_source_zeroes = 0;
  ssize_t count_suffix = 0;
  int result = 0;

  struct NumberSuffixData const *startdata = get_number_suffix_data(StartType);
  struct NumberSuffixData const *enddata = get_number_suffix_data(EndType);

  zeroes = startdata->numzeroes - enddata->numzeroes;
  count_suffix = dropSuffix != 0 ? 0 : 1;
  sourceiter = source;

  while ( ( *sourceiter != '\0' ) && ( isdigit(*sourceiter) ) ) {
    ++sourceiter;
    if ( *sourceiter == '0' ) {
      ++count_source_zeroes;
    }
  }

  count_source = sourceiter - source;

  if (zeroes > 0) { // If adding zeroes
    // Add zeroes.
    if ( ! (pvalue_with_zeroes = (char *)allocate(count_source + zeroes + count_suffix + 1)) ) {
      result = 2;
    }
    else {
      strncpy(pvalue_with_zeroes, source, count_source);
      for (zeroiter = 0; zeroiter < zeroes; ++zeroiter) {
        *(pvalue_with_zeroes + count_source + zeroiter) = '0';
      }

      if (!dropSuffix) {
        *(pvalue_with_zeroes + count_source + zeroes) = enddata->upper;
      }

      *(pvalue_with_zeroes + count_source + count_suffix + zeroes) = '\0';
    }

  } // Ends if adding zeroes
  else if (zeroes < 0) { // Else dropping zeroes
    if (count_source_zeroes + zeroes < 0) { // If dropping more zeroes than in string
      // If we're dropping more zeroes than we have in the string, then we have an issue.
      // e.g. 900MB in GB. For now, just round to nearest GB (i.e. 1GB)
      if ( ! (pvalue_with_zeroes = pvalue_with_zeroes = (char *)allocate(1 + count_suffix + 1)) ) {
        result = 3;
      }
      else {
        *pvalue_with_zeroes = '1';

        if (!dropSuffix) {
          *(pvalue_with_zeroes + 1) = enddata->upper;
        }

        *(pvalue_with_zeroes + 1 + count_suffix) = '\0';
      }
    } // Ends if dropping more zeroes than in string
    else { // Else dropping less zeroes than in string

      if ( ! (pvalue_with_zeroes = (char *)allocate(count_source - (count_source_zeroes + zeroes) + 1)) ) {
        result = 4;
      }
      else {
        strncpy(pvalue_with_zeroes, source, count_source + zeroes);

        if (!dropSuffix) {
          *(pvalue_with_zeroes + count_source + zeroes) = enddata->upper;
        }

        *(pvalue_with_zeroes + count_source + zeroes + count_suffix) = '\0';
      }
    } // Ends else dropping less zeroes than in string
  } // Ends else dropping zeroes
  else { // Else no change in zeroes
    if ( ! (pvalue_with_zeroes = (char *)allocate(count_source + count_suffix + 1)) ) {
      result = 5;
    }
    else {
      strncpy(pvalue_with_zeroes, source, count_source);
      if (!dropSuffix) {
        *(pvalue_with_zeroes + count_source) = enddata->upper;
      }

      *(pvalue_with_zeroes + count_source + count_suffix) = '\0';
    }
  } // Ends else no change in zeroes

  if ( result == 0 ) {
    *ppzeroed_string = pvalue_with_zeroes;
  }

  return result;
}

/// @brief Find the size suffix for the specified string.
/// @param String which may contain a size suffix.
/// @return Suffix found in string if found, otherwise 0.
char onapp_find_value_suffix(char *pvalue)
{
  char result = 0;
  char *pvalueiter;

  pvalueiter = pvalue;

  while ( *pvalueiter != '\0' && isdigit(*pvalueiter) ) {
    ++pvalueiter;
  }

  if (pvalueiter != '\0') {
    result = *pvalueiter;
  }

  return result;
}

int onapp_normalise_value( char * sptr, int normal, char **ppzeroed_string )
{
  int result = 0;

  char const suffix = onapp_find_value_suffix(sptr);

  if (suffix != 0) {
    result = onapp_normalise_number(sptr
      , suffix // String suffix
      , normal // Normal format
      , 1 // dropSuffix ?
      , ppzeroed_string); // Result
  }
  else {
    if (! (*ppzeroed_string = allocate_string(sptr) )) {
      result = 1;
    }
  }

  return result;
}

int onapp_normalise_value_liberate( char * sptr, int normal, char **ppzeroed_string )
{
  int const result = onapp_normalise_value( sptr, normal, ppzeroed_string );

  liberate(sptr);

  return result;
}

int onapp_atoi(char *p)
{
  int i = strtol(p, NULL, 10);
  liberate(p);
  return i;
}

/// OnApp Time functions.

struct tm * attempt_gmtime_r(time_t convert, struct tm *time)
{
	struct tm *result = NULL;
#ifdef HAVE_GMTIME_R
	struct tm time_temp;
	if (gmtime_r(&convert, &time_temp) != NULL)
	{
		*time = time_temp;
		result = time;
	}
#else
	struct tm *t = gmtime(&convert);
	if (t != NULL)
	{
		*time = *t;
		result = time;
	}
#endif
	return result;
}

int onapp_current_timestamp_int()
{
	return (int)time(0);
}

char * onapp_current_timestamp_string(char *default_value)
{
	static size_t const buffer_size = 64;

	char buffer[buffer_size];
	char *result = default_value;
	time_t const current = time(0);
	struct tm current_tm = { 0 };
	size_t const bytes = snprintf(buffer, buffer_size, "%lu", current);

	buffer[bytes] = '\0';

	// 20 for Time string. 4 for '( ' prefix and ')' and null-terminator.
	if (bytes + 20 + 4 < buffer_size)
	{
		if (attempt_gmtime_r(current, &current_tm) == &current_tm)
		{
			buffer[bytes] = '\0';
			strftime(buffer + bytes, 1024, "%FT%TZ)", &current_tm);
			result = allocate_string(buffer);

		}
		else
		{
			result = allocate_string(buffer);
		}
	}
	else if (bytes > 0)
	{
		result = allocate_string(buffer);
	}

	return result;
}

//#define OLD_GET_ZEROES
#ifdef OLD_GET_ZEROES
size_t onapp_get_normalize_zeroes(char const **ppzeroes, size_t *pcount_zeroes, int normal)
{
  size_t result = 0;

  if ( normal == 'T' || normal == 't' ) {
    static char const terrazeroes[] = "000000000000";
    *pcount_zeroes = (sizeof(terrazeroes) / sizeof(*terrazeroes) - 1);
    *ppzeroes = terrazeroes;
    result = *pcount_zeroes;
  }
  else if ( normal == 'G' || normal == 'g') {
    static char const gigazeroes[] = "000000000";
    *pcount_zeroes = (sizeof(gigazeroes) / sizeof(*gigazeroes)) - 1;
    *ppzeroes = gigazeroes;
    result = *pcount_zeroes;
  }
  else if ( normal == 'M'|| normal == 'm' ) {
    static char const megazeroes[] = "000000";
    *pcount_zeroes = (sizeof(megazeroes) / sizeof(*megazeroes)) - 1;
    *ppzeroes = megazeroes;
    result = *pcount_zeroes;
  }
  else if ( normal == 'K' || normal == 'k' ) {
    static char const kilazeroes[] =  "000";
    *pcount_zeroes = (sizeof(kilazeroes) / sizeof(*kilazeroes)) - 1;
    *ppzeroes = kilazeroes;
    result = *pcount_zeroes;
  }

  return result;
}

int onapp_normalise_value( char * sptr, int normal, char **ppzeroed_string )
{
  char *siterptr = sptr;
  char const *pzeroes = "";
  size_t count_zeroes = 0, count_string = 0;
  char * pvalue_with_zeroes = NULL;

  if ( sptr == 0 )
    return 0;

  onapp_get_normalize_zeroes(&pzeroes, &count_zeroes, normal);

  while ( ( (*siterptr != '\0') && ( isdigit(*siterptr) ) )) {
    ++siterptr;
    ++count_string;
  }

  if ( *siterptr != '\0' ) {
    onapp_get_normalize_zeroes(&pzeroes, &count_zeroes, *siterptr);
  }

  if ( ! (pvalue_with_zeroes = allocate(count_string + count_zeroes + 1) ) )
    return 2;
  else {
    strncpy(pvalue_with_zeroes, sptr, count_string);
    if (count_zeroes > 0) {
      strncpy(pvalue_with_zeroes + count_string, pzeroes, count_zeroes);
    }
    *(pvalue_with_zeroes + count_zeroes + count_string) = '\0';
    *ppzeroed_string = pvalue_with_zeroes;
  }

  return 0;
}
#endif

/// HTTP functions.
/// HTTP functions.
char * get_http_body_length_string(char *buffer, size_t buffer_size, int type, char const *body)
{
 char * body_length = NULL;

 if (buffer_size > 0 && body != NULL)
 {
   if (type == _FILE_BODY)
   {
     struct  stat info;
     struct  rest_header * hptr;
     if ( stat( body,&info ) >= 0 )
     {
       if (snprintf(buffer, buffer_size, "%lu", info.st_size) < buffer_size)
       {
         body_length = buffer;
       }
     }
   }
   else if (type == _TEXT_BODY)
   {
     // Take strlen() of body as Content-Length.
     if (body == NULL)
     {
       buffer[0] = '0';

       if (buffer_size > 1)
       {
         buffer[1] = '\0';
       }

       body_length = buffer;
     }
     else
     {
       buffer[0] = '\0';
       if (snprintf(buffer, buffer_size, "%lu", (long unsigned)strlen(body)) < buffer_size)
       {
         body_length = buffer;

       }
     }
   }
 }

 return body_length;
}

/// File handling.
size_t write_filecontent_to_filehandle(char const * const filename, FILE* h, char const * const prefix, char const * const suffix)
{
  FILE* hbody;
  const size_t buffersize = 4095;
  char buffer[buffersize + 1];
  buffer[0] = '\0';
  buffer[buffersize] = '\0';
  size_t bytes = 0;
  size_t readbytes = 0;
  size_t totalreadbytes = 0;
  size_t writebytes = 0;

  if (prefix != NULL) {
    fwrite(prefix, 1, strlen(prefix), h);
  }

  if (filename != NULL)
  {
    if ((hbody = fopen(filename, "rb"))) { // If opened input file OK
      while (1) {
        bytes =  fread(buffer + readbytes, 1, (buffersize - readbytes), hbody);
        if (bytes == 0) { // If end of read
          fwrite(buffer, 1, readbytes, h);
          break;

        } // Ends if end of read
        else { // Else read bytes
          readbytes += bytes;
          totalreadbytes += bytes;
          if (readbytes >= buffersize) { // If filled the buffer
            fwrite(buffer, 1, readbytes, h);
            // Reset the buffer.
            buffer[0] = '\0';
            buffer[buffersize] = '\0';
            readbytes = 0;

          } // Ends if filled the buffer
        } // Ends else read bytes
      } // Ends while reading bytes
      buffer[readbytes] = '\0';
      fclose(hbody);

    } // Ends if opened input file OK

  }

  if (suffix != NULL) {
    fwrite(suffix, 1, strlen(suffix), h);
  }

  fflush(h);

  return totalreadbytes;
}

size_t write_filecontent_to_filename(char const * const filename, char const * const output_filename, char const * const prefix, char const * const suffix) {
  FILE *h;
  size_t totalreadbytes = 0;
  if ( (h = fopen(output_filename, "wb"))) { // If opened output file OK
    totalreadbytes = write_filecontent_to_filehandle(filename, h, prefix, suffix);
    fclose(h);

  } // Ends if opened output file OK

  return totalreadbytes;
}

/// Dump code.

int dump_oa_config_data( FILE *h,
  struct oa_config const * pptr)
{
  if ( h == NULL ) {
    h = stdout;
  }

  fprintf(h, "===oa_config===\n");
  fprintf(h, "  id = %s\n"
    "  total_memory = %s\n"
    "  cpus = %d\n"
    "  agent = %s\n"
    "  local_id = %s\n"
    "  failover_disabled = %d\n"
    "  memory_guarantee = %d\n"
    "  location_city = %s\n"
    "  store_size = %s\n"
    "  user_id = %s\n"
    "  cloud_id = %s\n"
    "  location_country = %s\n"
    "  location_region = %s\n"
    "  label = %s\n"
    "  current = %d\n"
    "  authentication = %s\n"
    "  version = %s\n"
    "  description = %s\n"
    "  deleted = %d\n"
    "  is_active = %d\n"
    "  net_zone_id = %d\n"
    "  host = %s\n"
    "  base = %s\n"
    "  user = %s\n"
    "  credentials = %s\n"
    "  password = %s\n"
    "  tls = %s\n"
    "  name = %s\n"
    "  hypervisor_type = %s\n"
    "  created_at = %d\n"
    "  location_name = %s\n"
    "  store_zone_id = %d\n"
    "  net_zone_label = %s\n"
    "  store_label = %s\n"
    , pptr->id
    , pptr->total_memory
    , pptr->cpus
    , pptr->agent
    , pptr->local_id
    , pptr->failover_disabled
    , pptr->memory_guarantee
    , pptr->location_city
    , pptr->store_size
    , pptr->user_id
    , pptr->cloud_id
    , pptr->location_country
    , pptr->location_region
    , pptr->label
    , pptr->current
    , pptr->authentication
    , pptr->version
    , pptr->description
    , pptr->deleted
    , pptr->is_active
    , pptr->net_zone_id
    , pptr->host
    , pptr->base
    , pptr->user
    , pptr->credentials
    , pptr->password
    , pptr->tls
    , pptr->name
    , pptr->hypervisor_type
    , pptr->created_at
    , pptr->location_name
    , pptr->store_zone_id
    , pptr->net_zone_label
    , pptr->store_label
  );
  fprintf(h, "\n");

  return 0;
}

int dump_oa_config_data_stdout( struct oa_config const * poa_config )
{
	return dump_oa_config_data(stdout, poa_config);
}

int dump_onapp_data( FILE *h,
  struct onapp *ponapp)
{
  if ( h == NULL ) {
    h = stdout;
  }

  fprintf(h, "\n===onapp info===\n");
  fprintf(h, "  id = %s\n"
    "  node = %s\n"
    "  disk_type_of_format = %s\n"
    "  firewall = %s\n"
    "  image = %s\n"
    "  cpus = %d\n"
    "  agent = %s\n"
    "  swap_size = %s\n"
    "  required_virtual_machine_build = %d\n"
    "  user_id = %s\n"
    "  zone = %s\n"
    "  hostname = %s\n"
    "  label = %s\n"
    "  access = %s\n"
    "  state = %d\n"
    "  location = %s\n"
    "  provider = %s\n"
    "  created_at = %d\n"
    "  required_ip_address_assignment = %d\n"
    "  required_virtual_machine_startup = %d\n"
    "  profile = %s\n"
    "  description = %s\n"
    "  initial_root_password_encryption_key = %s\n"
    "  price = %s\n"
    "  memory = %s\n"
    "  initial_root_password = %s\n"
    "  password = %s\n"
    "  ipaddress = %s\n"
    "  account = %s\n"
    "  disk_size = %s\n"
    "  name = %s\n"
    "  region = %s\n"
    "  username = %s\n"
    "  required_automatic_backup = %d\n"
    "  local_id = %s\n"
    "  status = %s\n"
    "  host_name = %s\n"
    "  original = %s\n"
    "  cpu_shares = %s\n"
    , ponapp->id
    , ponapp->node
    , ponapp->disk_type_of_format
    , ponapp->firewall
    , ponapp->image
    , ponapp->cpus
    , ponapp->agent
    , ponapp->swap_size
    , ponapp->required_virtual_machine_build
    , ponapp->user_id
    , ponapp->zone
    , ponapp->hostname
    , ponapp->label
    , ponapp->access
    , ponapp->state
    , ponapp->location
    , ponapp->provider
    , ponapp->created_at
    , ponapp->required_ip_address_assignment
    , ponapp->required_virtual_machine_startup
    , ponapp->profile
    , ponapp->description
    , ponapp->initial_root_password_encryption_key
    , ponapp->price
    , ponapp->memory
    , ponapp->initial_root_password
    , ponapp->password
    , ponapp->ipaddress
    , ponapp->account
    , ponapp->disk_size
    , ponapp->name
    , ponapp->region
    , ponapp->username
    , ponapp->required_automatic_backup
    , ponapp->local_id
    , ponapp->status
    , ponapp->host_name
    , ponapp->original
    , ponapp->cpu_shares
  );

  fprintf(h, "\n\n");

  return 0;
}

int dump_onapp_data_stdout(struct onapp *ponapp
  )
{
  return dump_onapp_data(stdout, ponapp);
}

void dump_url(FILE *hOutput, struct url const *purl)
{
  fwrite("Url: '", 1, 6, hOutput);
  fputs(purl->host, hOutput);

  if (purl->port != 0)
  {
    fprintf(hOutput, ":%d", purl->port);
  }

  if (purl->object != NULL)
  {
    fputs(purl->object, hOutput);
  }

  fputc('\'', hOutput);

  if (purl->service != NULL)
  {
    fwrite(". Service: '", 1, 12, hOutput);
    fputs(purl->service, hOutput);
    fwrite("'.", 1, 2, hOutput);
  }
}

/*
struct  occi_attribute
{
  struct  occi_attribute  * previous;
  struct  occi_attribute  * next;
  struct  occi_category   * parent;
  char          * name;
  char          * type;
  char          * units;
  char          * validation;
  char          * defaultvalue;
  int       mandatory;
  int       indexed;
  int       immutable;
};
*/
void dump_occi_attribute(FILE *hOutput, struct occi_attribute const *pocci_attribute)
{
  if (pocci_attribute == NULL)
  {
    fwrite("occi_attribute (null).", 1, 22, hOutput);
  }
  else
  {
    fprintf(hOutput,
        "occi_attribute name='%s' "
        "type='%s' "
        "units='%s' "
        "validation='%s' "
        "defaultvalue='%s' "
        "mandatory='%d' "
        "indexed='%d' "
        "immutable='%d'."
      , pocci_attribute->name
      , pocci_attribute->type
      , pocci_attribute->units
      , pocci_attribute->validation
      , pocci_attribute->defaultvalue
      , pocci_attribute->mandatory
      , pocci_attribute->indexed
      , pocci_attribute->immutable
    );
  }
}
/*
struct  occi_category
{
  struct  occi_category   * previous;
  struct  occi_category   * next;
  struct  occi_attribute  * first;
  struct  occi_attribute  * last;
  struct  occi_action * firstact;
  struct  occi_action * lastact;
  char      * domain;
  char          * id;
  char      * scheme;
  char      * class;
  char      * title;
  char      * rel;
  char      * version;
  char      * location;
  char      * price;
  char      * rating;
  int       access;
  void      * interface;
  void      * callback;
  void      * payload;
  char *  (*html_rendering)(struct occi_category * cptr,struct rest_header  * hptr );

};
 */
void test_dump_occi_category(FILE *hOutput, struct occi_category const *pocci_category)
{
  struct occi_attribute *pocci_attribute_iter = NULL;

  if (pocci_category == NULL)
  {
    fwrite("occi_category (null).", 1, 21, hOutput);
  }
  else
  {
    fprintf(hOutput,
        "occi_category domain='%s' "
        "id='%s' "
        "scheme='%s' "
        "class='%s' "
        "title='%s' "
        "rel='%s' "
        "version='%s' "
        "location='%s' "
        "price='%s' "
        "rating='%s' "
        "access='%d' "
        "interface='0x%p' "
        "callback='0x%p' "
        "payload='0x%p'."
      , pocci_category->domain
      , pocci_category->id
      , pocci_category->scheme
      , pocci_category->class
      , pocci_category->title
      , pocci_category->rel
      , pocci_category->version
      , pocci_category->location
      , pocci_category->price
      , pocci_category->rating
      , pocci_category->access
      , pocci_category->interface
      , pocci_category->callback
      , pocci_category->payload
    );
  }
  if (pocci_category->first == NULL)
  {

  }
  else
  {
    fputc('\n', hOutput);

    for (pocci_attribute_iter = pocci_category->first; pocci_attribute_iter != NULL; pocci_attribute_iter=pocci_attribute_iter->next)
    {
      dump_occi_attribute(hOutput, pocci_attribute_iter);
      fputc('\n', hOutput);
    }
  }
}
/*
struct  rest_response
{
  struct  rest_header * first;
  struct  rest_header * last;
  int     status;
  char      * version;
  char      * message;
  int     type;
  char      * body;
};
*/
void dump_rest_response(FILE *hOutput, struct rest_response const *prest_response)
{
  struct rest_header *prest_header;
  fprintf(hOutput, "rest_response status=%d version='%s' message='%s' type=%d body='%s'\n"
    , prest_response->status
    , prest_response->version
    , prest_response->message
    , prest_response->type
    , prest_response->body
  );
  for (prest_header = prest_response->first; prest_header != NULL; prest_header = prest_header->next)
  {
    fprintf(hOutput, " %s:%s\n", prest_header->name, prest_header->value);
  }

  write_filecontent_to_filehandle(prest_response->body, hOutput, "Response Body:\n", "\n");
}
/*
struct  occi_response
{
  struct  occi_element * first;
  struct  occi_element * last;
  int type;
  char *  name;
  char *  host;
  struct  occi_category * category;
  struct  rest_response * response;
};
 */
void dump_occi_response(FILE* hOutput, struct occi_response const * pocci_response)
{
  struct occi_element* pocci_element_iter = (struct occi_element *) NULL;
  char* psz = NULL;
  unsigned int counter = 0;

  fprintf(hOutput, "occi_response type='%d' name='%s' host='%s'\n", pocci_response->type, pocci_response->name, pocci_response->host);
  for (pocci_element_iter = pocci_response->first;
      pocci_element_iter != (struct occi_element*) 0;
      pocci_element_iter = pocci_element_iter->next)
  {
    if (pocci_element_iter->name == NULL )
    {
      fprintf(hOutput, "%u] occi_response element missing name\n", counter);
      continue;
    }
    else if (pocci_element_iter->value == NULL )
    {
      fprintf(hOutput, "%u] occi_response element missing value\n", counter);
      continue;
    }
    else
    {
      psz = occi_category_id(pocci_element_iter->value);
      fprintf(hOutput,
        "%u] occi_response element name='%s', value='%s'. category id='%s'\n",
        counter, pocci_element_iter->name, pocci_element_iter->value, psz
      );
    }
    //else  sprintf(url,"%s%s",buffer,vptr);
  }

  test_dump_occi_category(hOutput, pocci_response->category);
  fputc('\n', hOutput);
  dump_rest_response(hOutput, pocci_response->response);
  fputc('\n', hOutput);
}

struct occi_response * dump_occi_response_liberate(FILE* hOutput, struct occi_response * pocci_response)
{
  if (pocci_response == NULL)
  {
    fputs("No response\n", hOutput);
  }
  else
  {
    dump_occi_response(hOutput, pocci_response);
  }

  return occi_remove_response(pocci_response);
}

void dump_occi_client(FILE *hOutput, struct occi_client const *pocci_client)
{
  fwrite("===Occi Client===\n", 1, 18, hOutput);
  fwrite("Target ", 1, 7, hOutput);
  dump_url(hOutput, pocci_client->target);
  fputc('\n', hOutput);

  fprintf(hOutput, "Host: '%s'\nAgent: '%s'\nMimetype: '%s'\nCategories: %d\nStarted: %d\nRequests: %d\nFailures: %d\n"
     , pocci_client->host
     , pocci_client->agent
     , pocci_client->mimetype
     , pocci_client->categories
     , pocci_client->started
     , pocci_client->requests
     , pocci_client->failures
  );
  fputc('\n', hOutput);
}

void dump_onapp_extras_occi(FILE *hOutput, struct occi_response *pocci_response)
{
  struct printf_field
  {
    char * const name;
    char const * const field;
  };

  static struct printf_field const onapp_printf_fields[] =
  {
      { "required_automatic_backup" }
    , { "initial_root_password" }
    , { "name" }
    , { "initial_root_password_encryption_key" }
    , { "required_ip_address_assignment" }
    , { "disk_type_of_format" }
    , { "label" }
    , { "required_automatic_backup" }
    , { "swap_size" }
    , { "required_virtual_machine_build" }
    , { "required_virtual_machine_startup" }
    , { "cpu_shares" }
  };
  static size_t const onapp_printf_fields_length = sizeof(onapp_printf_fields) / sizeof(*onapp_printf_fields);

  struct printf_field const *pfield_iter;
  char *pattribute_iter;

  fputs("===Get Onapp occi_response===\n", hOutput);
  dump_occi_response(hOutput, pocci_response);
  fputc('\n', hOutput);

  fputs("===Parsing out onapp_extras===\n", hOutput);
  for (pfield_iter = onapp_printf_fields; pfield_iter < onapp_printf_fields + onapp_printf_fields_length; ++pfield_iter)
  {
    pattribute_iter = occi_extract_atribut(pocci_response, "occi", "onapp_extras", pfield_iter->name);
    fprintf(hOutput, " %s=", pfield_iter->name);
    fprintf(hOutput, pfield_iter->field != NULL ? pfield_iter->field : "%s", pattribute_iter);
    fputc('\n', hOutput);
  }
  fputc('\n', hOutput);
}

void dump_onapp_extras(FILE *hOutput, struct cords_onapp_extras const *ponapp_extras)
{
  fprintf(hOutput, "  id = %s\n"
    "  initial_root_password = %s\n"
    "  name = %s\n"
    "  initial_root_password_encryption_key = %s\n"
    "  required_ip_address_assignment = %d\n"
    "  disk_type_of_format = %s\n"
    "  label = %s\n"
    "  required_automatic_backup = %d\n"
    "  swap_size = %s\n"
    "  required_virtual_machine_build = %d\n"
    "  required_virtual_machine_startup = %d\n"
    "  cpu_shares = %s\n"
    , ponapp_extras->id
    , ponapp_extras->initial_root_password
    , ponapp_extras->name
    , ponapp_extras->initial_root_password_encryption_key
    , ponapp_extras->required_ip_address_assignment
    , ponapp_extras->disk_type_of_format
    , ponapp_extras->label
    , ponapp_extras->required_automatic_backup
    , ponapp_extras->swap_size
    , ponapp_extras->required_virtual_machine_build
    , ponapp_extras->required_virtual_machine_startup
    , ponapp_extras->cpu_shares
  );
}
