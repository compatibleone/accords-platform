#ifndef	_xmlparse_h
#define	_xmlparse_h

/*	----------------------------------------	*/
/*	XML Parsing machine constants and errors	*/
/*	----------------------------------------	*/


#define	_XML_MAX_TOKEN_SIZE		8192
#define	_XML_MAX_NESTING		64
#define	_XML_MAX_PAYLOAD		64

#define	_XML_SUCCESS			0
#define	_XML_UNEXPECTED_TOKEN		1
#define	_XML_INCORRECT_PUNCTUATION	2
#define	_XML_UNEXPECTED_EOF		3
#define	_XML_FILE_ERROR			4
#define	_XML_INCORRECT_TOKEN		5

/*	-----------------------------	*/
/*	XML Application Context Stack	*/
/*	-----------------------------	*/
struct	xml_parser;

struct xml_application {
	int		  nested;
	int		  heap[_XML_MAX_NESTING];
	char		  *buffer;
	int		  buflen;
	int		  echo;
	int		  offset;
	int		  strict;
	void		  *payload[_XML_MAX_PAYLOAD];
	struct xml_parser *parent;
	};

/*	---------------------------------------------------------------------------	*/
/*	This structure defines the functions to be called by the XML parsing engine	*/
/*	during the various stages of its operational cycle				*/ 
/*	---------------------------------------------------------------------------	*/

struct	xml_relay	{
	int	(*open_tag)(struct xml_application *,char *);
	int	(*atb_name)(struct xml_application *,char *);
	int	(*atb_value)(struct xml_application *,char *);
	int	(*white_space)(struct xml_application *,int);
	int	(*token)(struct xml_application *,char *);
	int	(*punctuation)(struct xml_application *,int);
	int	(*close_tag)(struct xml_application *);
	int	(*failure)(char *,int,int);
	};

/*	------------------------------------------------------------------		*/
/*	This structures describes the XML parsing engine source controller		*/
/*	------------------------------------------------------------------		*/

struct	xml_parser	{

	struct xml_relay 	* user;
	struct xml_application 	* work;
	FILE			* handle;
	char			* name;
	int			lines;
	int			column;
	int			ungotch;
	char			token[_XML_MAX_TOKEN_SIZE+1];
	int			punctuation;
	int			length;
	int			state;
	int			nesting;
	int			echo;
	};

/*	--------------------------------------		*/
/*	The various state definition constants		*/
/*	--------------------------------------		*/

#define	_TAG_IDLE		 0
#define	_TAG_NAME		 1
#define	_TAG_FINAL		 2
#define	_TAG_OPEN		 3
#define	_TAG_COMMENT		 4
#define	_TAG_ATBNAME 		 5
#define	_TAG_ATBEQUAL		 6
#define	_TAG_ATBVALUE		 7
#define	_TAG_CLOSE		 8
#define	_TAG_QUESTION		 9
#define	_TAG_ENDQUESTION	10
#define	_TAG_EXCLAIM		12

/*	--------------------------	*/
/*	Public Function Prototypes	*/
/*	--------------------------	*/
int	xml_parse( char * filename, struct xml_relay * interfaceitem, void * application );

/*	--------------------------------------	*/
/*	Call back functions for data retrieval	*/
/*	--------------------------------------	*/
int	xml_mnemonic( char * sptr, char * mptr );
void	xml_get_word(struct xml_application * xptr, int * iptr );
void	xml_get_xy(struct xml_application * xptr, int * x, int * y );
void	xml_get_string(struct xml_application * xptr, char ** target );
void	xml_get_text(struct xml_application * xptr, char ** rptr );

#endif	/*  _xmlparse_h  */
	/* ------------- */




