/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2012 Liliana Cabalantti (Bull SAS) <liliana.cabalantti@bull.net>*/
/*  (C) 2012 Vincent Renardias (CityPassenger) <vincent@renardias.com>  */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License");      */
/* you may not use this file except in compliance with the License.     */
/* You may obtain a copy of the License at                              */
/*                                                                      */
/*  http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                      */
/* Unless required by applicable law or agreed to in writing, software  */
/* distributed under the License is distributed on an "AS IS" BASIS,    */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      */
/* implied.                                                             */
/* See the License for the specific language governing permissions and  */
/* limitations under the License.                                       */
/* -------------------------------------------------------------------- */
#ifndef _testopenam_c      
#define _testopenam_c

/* 
   This command-line tool invokes the REST API of OpenAM/OpenSSO
   to execute Identity Management fonctions
   Original code by: Liliana Cabalantti / Bull SAS - 2012
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};


/* Write data callback fonction */
static size_t write_data_callback(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)stream;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) 
    {
      /* out of memory! */
      printf("not enough memory (realloc returned NULL)\n");
      exit(EXIT_FAILURE);
    }

  memcpy(&(mem->memory[mem->size]), ptr, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize; 
}


int main(int argc,char *argv[])
{
  CURL *curl;
  char *url;
  CURLcode res;
  FILE *fp;
  char *tokenid;
  char *user;
  struct MemoryStruct chunk;
  int xml_flag = 0;
  int http_code = 0;

  user = malloc(1024);
  tokenid = malloc(120);
  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  if (argc < 2) 
    {
      printf("Usage: %s [authenticate|istokenvalid|logout|search|attributes|read|create|update|delete]\n",argv[0]);
      return 1;
    }

  if (!strcmp(argv[1], "authenticate") && (argc < 5))
    {
      printf("Usage: %s authenticate username password xml|noxml\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "istokenvalid") && (argc < 4))
    {
      printf("Usage: %s istokenvalid tokenid xml|noxml\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "logout") && (argc < 3))
    {
      printf("Usage: %s logout tokenid\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "search") && (argc < 6))
    {
      printf("Usage: %s search attribute-name attribute-value admin-tokenid xml|noxml\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "attributes") && (argc < 4))
    {
      printf("Usage: %s attributes tokenid xml|noxml\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "read") && (argc < 5))
    {
      printf("Usage: %s read username admin-tokenid xml|noxml\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "create") && (argc < 6))
    {
      printf("Usage: %s create username password(min size = 8) usertype admin-tokenid\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "update") && (argc < 5))
    {
      printf("Usage: %s username password(min size = 8) admin-tokenid\n",argv[0]);
      return 1;
    }
  else if (!strcmp(argv[1], "delete") && (argc < 5))
    {
      printf("Usage: %s delete username usertype admin-tokenid\n",argv[0]);
      return 1;
    }

  /* Command treatment */
  if (!strcmp(argv[1], "authenticate"))
    {
      printf("%s received %s username=%s password=%s xml=%s\n",argv[0], argv[1], argv[2], argv[3], argv[4]);

      sprintf(user, "username=%s&password=%s", argv[2], argv[3]);

      if (!strcmp(argv[4], "xml"))
	xml_flag = 1;

      /* Make sure you set this appropriately */
      if (xml_flag)
	url="http://localhost:8081/opensso/identity/xml/authenticate";
      else
	url="http://localhost:8081/opensso/identity/authenticate";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, user);    

	  /* Send all data to this function  */
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);

	  /* we pass our 'chunk' struct to the callback function */
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(user);

	      if(chunk.memory)
		free(chunk.memory);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  /*
	   * Now, our chunk.memory points to a memory block that is chunk.size
	   * bytes big and contains the remote file.
	   *
	   */

	  printf("%lu bytes retrieved\n", (long)chunk.size);

	  /* Open file getting the response */
	  if (xml_flag)
	    fp = fopen("/home/ubuntu/rest_opensso_client/token_id.xml","w");
	  else
	    fp = fopen("/home/ubuntu/rest_opensso_client/token_id","w");
	  fwrite(chunk.memory, sizeof (char), chunk.size, fp);
	  fclose(fp);

	  free(user);

	  if(chunk.memory)
	    free(chunk.memory);
	}
    }
  else if (!strcmp(argv[1], "istokenvalid"))
    {
      printf("%s received %s %s xml=%s\n",argv[0], argv[1], argv[2], argv[3]);

      sprintf(tokenid, "tokenid=%s", argv[2]);
      printf("%s\n", tokenid);

      if (!strcmp(argv[3], "xml"))
	xml_flag = 1;

      /* Make sure you set this appropriately */
      if (xml_flag)
	url="http://localhost:8081/opensso/identity/xml/isTokenValid";
      else
	url="http://localhost:8081/opensso/identity/isTokenValid";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, tokenid);    

	  /* Send all data to this function  */
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);

	  /* we pass our 'chunk' struct to the callback function */
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      printf("\nThe token is no more valid\n");
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(tokenid);

	      if(chunk.memory)
		free(chunk.memory);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  /*
	   * Now, our chunk.memory points to a memory block that is chunk.size
	   * bytes big and contains the remote file.
	   *
	   */

	  printf("%lu bytes retrieved\n", (long)chunk.size);

	  /* Open file getting the response */
	  if (xml_flag)
	    fp = fopen("/home/ubuntu/rest_opensso_client/token_valid.xml","w");
	  else
	    fp = fopen("/home/ubuntu/rest_opensso_client/token_valid","w");
	  fwrite(chunk.memory, sizeof (char), chunk.size, fp);
	  fclose(fp);

	  free(tokenid);

	  if(chunk.memory)
	    free(chunk.memory);
	}
    }
  else if (!strcmp(argv[1], "logout"))
    {
      printf("%s received %s %s\n",argv[0], argv[1], argv[2]);

      sprintf(tokenid, "subjectid=%s", argv[2]);
      printf("%s\n", tokenid);

      /* Make sure you set this appropriately */
      url="http://localhost:8081/opensso/identity/logout";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     
	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, tokenid);    

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(tokenid);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	}
    }
  else if (!strcmp(argv[1], "search"))
    {
      printf("%s received %s attribute-name=%s attribute-value=%s admin-tokenid=%s xml=%s\n",argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);

      sprintf(user, "&filter=*&attributes_names=%s&attributes_values_%s=%s&admin=%s", argv[2], argv[2],argv[3], argv[4]);
      printf("POST parameters = %s\n", user);

      if (!strcmp(argv[5], "xml"))
	xml_flag = 1;

      /* Make sure you set this appropriately */
      if (xml_flag)
	url="http://localhost:8081/opensso/identity/xml/search";
      else
	url="http://localhost:8081/opensso/identity/search";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, user);    

	  /* Send all data to this function  */
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);

	  /* we pass our 'chunk' struct to the callback function */
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(user);

	      if(chunk.memory)
		free(chunk.memory);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  /*
	   * Now, our chunk.memory points to a memory block that is chunk.size
	   * bytes big and contains the remote file.
	   *
	   */

	  printf("%lu bytes retrieved\n", (long)chunk.size);

	  /* Open file getting the response */
	  if (xml_flag)
	    fp = fopen("/home/ubuntu/rest_opensso_client/search_res.xml","w");
	  else
	    fp = fopen("/home/ubuntu/rest_opensso_client/search_res","w");
	  fwrite(chunk.memory, sizeof (char), chunk.size, fp);
	  fclose(fp);

	  free(user);

	  if(chunk.memory)
	    free(chunk.memory);
	}
    }
  else if (!strcmp(argv[1], "attributes"))
    {
      printf("%s received %s %s xml=%s\n",argv[0], argv[1], argv[2], argv[3]);

      sprintf(tokenid, "subjectid=%s", argv[2]);
      printf("%s\n", tokenid);

      if (!strcmp(argv[3], "xml"))
	xml_flag = 1;

      /* Make sure you set this appropriately */
      if (xml_flag)
	url="http://localhost:8081/opensso/identity/xml/attributes";
      else
	url="http://localhost:8081/opensso/identity/attributes";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, tokenid);    

	  /* Send all data to this function  */
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);

	  /* we pass our 'chunk' struct to the callback function */
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      printf("\nThe token is no more valid\n");
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(tokenid);

	      if(chunk.memory)
		free(chunk.memory);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  /*
	   * Now, our chunk.memory points to a memory block that is chunk.size
	   * bytes big and contains the remote file.
	   *
	   */

	  printf("%lu bytes retrieved\n", (long)chunk.size);

	  /* Open file getting the response */
	  if (xml_flag)
	    fp = fopen("/home/ubuntu/rest_opensso_client/attributes_res.xml","w");
	  else
	    fp = fopen("/home/ubuntu/rest_opensso_client/attributes_res","w");
	  fwrite(chunk.memory, sizeof (char), chunk.size, fp);
	  fclose(fp);

	  free(tokenid);

	  if(chunk.memory)
	    free(chunk.memory);
	}
    }
  else if (!strcmp(argv[1], "read"))
    {
      printf("%s received %s username=%s admin-tokenid=%s xml=%s\n",argv[0], argv[1], argv[2], argv[3], argv[4]);

      sprintf(user, "name=%s&admin=%s", argv[2], argv[3]);
      printf("user name and admin-tokenid = %s\n", user);

      if (!strcmp(argv[4], "xml"))
	xml_flag = 1;

      /* Make sure you set this appropriately */
      if (xml_flag)
	url="http://localhost:8081/opensso/identity/xml/read";
      else
	url="http://localhost:8081/opensso/identity/read";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, user);    

	  /* Send all data to this function  */
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);

	  /* we pass our 'chunk' struct to the callback function */
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      printf("\nThe requested identity does not exist\n");
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(user);

	      if(chunk.memory)
		free(chunk.memory);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  /*
	   * Now, our chunk.memory points to a memory block that is chunk.size
	   * bytes big and contains the remote file.
	   *
	   */

	  printf("%lu bytes retrieved\n", (long)chunk.size);

	  /* Open file getting the response */
	  if (xml_flag)
	    fp = fopen("/home/ubuntu/rest_opensso_client/read_res.xml","w");
	  else
	    fp = fopen("/home/ubuntu/rest_opensso_client/read_res","w");
	  fwrite(chunk.memory, sizeof (char), chunk.size, fp);
	  fclose(fp);

	  free(user);

	  if(chunk.memory)
	    free(chunk.memory);
	}
    }
  else if (!strcmp(argv[1], "create"))
    {
      printf("%s received %s username=%s password=%s usertype=%s admin-tokenid=%s\n",argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);

      sprintf(user, "identity_name=%s&identity_attribute_names=userpassword&identity_attribute_values_userpassword=%s&identity_type=%s&admin=%s", argv[2], argv[3], argv[4], argv[5]);
      printf("Post parameters = %s\n", user);

      /* Make sure you set this appropriately */
      url="http://localhost:8081/opensso/identity/create";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, user);    

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(user);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  free(user);
	}
    }
  else if (!strcmp(argv[1], "update"))
    {
      printf("%s received %s username=%s password=%s admin-tokenid=%s\n",argv[0], argv[1], argv[2], argv[3], argv[4]);

      sprintf(user, "identity_name=%s&identity_attribute_names=userpassword&identity_attribute_values_userpassword=%s&admin=%s", argv[2], argv[3], argv[4]);
      printf("Post parameters = %s\n", user);

      /* Make sure you set this appropriately */
      url="http://localhost:8081/opensso/identity/update";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, user);    

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(user);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  free(user);
	}
    }
  else if (!strcmp(argv[1], "delete"))
    {
      printf("%s received %s username=%s usertype=%s admin-tokenid=%s\n",argv[0], argv[1], argv[2], argv[3], argv[4]);

      sprintf(user, "identity_name=%s&identity_type=%s&admin=%s", argv[2], argv[3], argv[4]);
      printf("Post parameters = %s\n", user);

      /* Make sure you set this appropriately */
      url="http://localhost:8081/opensso/identity/delete";
  
      curl = curl_easy_init();
      if(curl) 
	{
	  /* First set the URL that is about to receive our POST */
	  curl_easy_setopt(curl, CURLOPT_URL, url);
    
	  curl_easy_setopt(curl, CURLOPT_POST, 1);     

	  /* Now specify the POST data */ 
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, user);    

	  /* some servers don't like requests that are made without a user-agent
	     field, so we provide one */
	  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

	  /* Perform the request, res will get the return code */
	  res = curl_easy_perform(curl);
	  printf("\nResult of Operation: %d\n", res);
	  res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	  if (http_code && http_code != 200)
	    {
	      printf("\nHTTP return code: %d\n", http_code);
	      /* always cleanup */
	      curl_easy_cleanup(curl);
	      free(user);

	      return 0;
	    }

	  /* always cleanup */
	  curl_easy_cleanup(curl);

	  free(user);
	}
    }
  return 0;
}

	/* ------------- */
#endif 	/* _testopenam_c */
	/* ------------- */

