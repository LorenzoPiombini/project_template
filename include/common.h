#ifndef COMMON_H
#define COMMON_H

/** DO NOT MODIFY **/
#define STATUS_ERROR -1
#define SCHEMA_ERR 2
#define SCHEMA_EQ 3
#define SCHEMA_NW 4
#define SCHEMA_CT 5
#define UPDATE_OLD 6 // you can overwrite the old record with no worries!
#define UPDATE_OLDN 8
#define ALREADY_KEY 9
/*>----------------------------------------<*/
/* DEFINE YOUR CUSTOM MACROS AFTER THIS LINE */

/* instruction included in the json object */
typedef enum
{
	/* PUT HERE THE INSTRUCTIONS TO PERFORM ON YOUR DATABASE */
	YOU_CAN_ERASE_THIS
} Instructions;

#endif
