#ifndef FLAG_H
# define FLAG_H
# include <stdarg.h>

struct s_flag;
struct s_params;
typedef void (*t_flag_apply)(struct s_flag *, struct s_params *);

enum flag_type {
    FLAG_INTEGER,	//Needs an argument type integer
    FLAG_RANGE_INTEGER,	//Needs a range as type integer
    FLAG_STRING,     //Needs an argument type string
	FLAG_CHAR,		//Needs an argument type char
	FLAG_BOOLEAN,	//Needs an argument type boolean
	FLAG_NONE,		//Doesnt needs any argument, just the flag presence
    FLAG_HELP       //This flag shows a print about the other flags
};

typedef struct s_flag
{
    char            *name;
    enum flag_type  type;
    int             active;
    int             error;
    void (*apply)(struct s_flag *flag, struct s_params *params);
    void (*not_apply)(struct s_flag *flag, struct s_params *params);
    
    union
    {
        int     int_value;
        struct {
            int min;
            int max;
        } range;
        char    *str_value;
        char    char_value;
        int     bool_value;
    } value;
}   t_flag;

t_list *set_up_flag(t_list *flag, char *name,enum flag_type type, t_flag_apply apply,t_flag_apply not_apply,...);
int process_flags(int argc, char **argv, t_list *flag,struct s_params *params);
void print_flag(void *content);

void free_flags(t_list **flags);
void free_flag(void *content);

# endif