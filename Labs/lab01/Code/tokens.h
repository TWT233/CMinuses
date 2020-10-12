/**
 * lexical token type naming rules:
 *
 *      [`group`_[`category`_]]<name>
 *
 * `group`:
 *      represents the functionality of tokens in this type.
 *      available options for this field:
 *          - DATA  :   raw data tokens
 *          - KWD   :   short for keyword
 *          - OP    :   short for operator
 *
 * `category`:
 *      smaller divide for a group, usually tells specific usage of this token.
 *
 * `name`:
 *      name of this type of tokens.
 *      easy to memory.
 *      easy to understand.
 *
 * e.g.
 *      LP
 *      KWD_INT
 *      OP_REL_GE
 *
 */

/**
 * lexical token type number assignment rules:
 *
 * typeno is a 32-bit unsigned int, which be divided into parts here:
 *
 *          0xFF|FF|FF|FF
 *            --|--|--|--
 *            P1|P2|P3|P4
 *
 * P1: the code of group:
 *      - DATA  : 01
 *      - OP    : 02
 *      - KWD   : 03
 *
 * P2: the code of category (optional):
 *     category code list: see below, start with `C_`
 *     set this field to zero for no-cate token types.
 *
 * P3 & P4: the code of name, P3 is necessary while
 *          P4 is enabled when this type of token is a ASCII char
 *          e.g. ">" "!" "=" "+"
 *
 */

#define G_DATA 01
#define G_OP 02
#define G_KWD 03

#define C_DATA_INT 0101

#define C_OP_REL 0201
#define C_OP_BOO 0202
#define C_OP_CAL 0203

#define C_KWD_TYPE 0301

#define _gen_token_code(p, n, c, g) (0x##g##c##n##00 + p)

#define gen_token_code(p, n, c, g) _gen_token_code(p, n, c, g)

//***********************************************

#define T_DATA_INT_DEC gen_token_code(00, 01, C_DATA_INT, )
#define T_DATA_INT_OCT gen_token_code(00, 02, C_DATA_INT, )
#define T_DATA_INT_HEX gen_token_code(00, 03, C_DATA_INT, )
//-----------------------------------------------
#define T_DATA_FLOAT gen_token_code(00, 01, 00, G_DATA)

//***********************************************

#define T_OP_REL_GE gen_token_code(00, 01, C_OP_REL, )
#define T_OP_REL_LE gen_token_code(00, 02, C_OP_REL, )
#define T_OP_REL_G gen_token_code('>', 03, C_OP_REL, )
#define T_OP_REL_L gen_token_code('<', 04, C_OP_REL, )
#define T_OP_REL_EQ gen_token_code(00, 05, C_OP_REL, )
#define T_OP_REL_NE gen_token_code(00, 06, C_OP_REL, )
//-----------------------------------------------
#define T_OP_BOO_AND gen_token_code(00, 01, C_OP_BOO, )
#define T_OP_BOO_OR gen_token_code(00, 02, C_OP_BOO, )
#define T_OP_BOO_NOT gen_token_code('!', 03, C_OP_BOO, )
//-----------------------------------------------
#define T_OP_CAL_PLUS gen_token_code('+', 01, C_OP_CAL, )
#define T_OP_CAL_MINUS gen_token_code('-', 02, C_OP_CAL, )
#define T_OP_CAL_STAR gen_token_code('*', 03, C_OP_CAL, )
#define T_OP_CAL_DIV gen_token_code('/', 04, C_OP_CAL, )
//-----------------------------------------------
#define T_OP_ASSIGN gen_token_code('=', 01, 00, G_OP)
#define T_OP_DOT gen_token_code('.', 02, 00, G_OP)

//***********************************************

#define T_KWD_TYPE_INT gen_token_code(00, 01, C_KWD_TYPE, )
#define T_KWD_TYPE_FLOAT gen_token_code(00, 02, C_KWD_TYPE, )
//-----------------------------------------------
#define T_KWD_STRUCT gen_token_code(00, 03, 00, G_KWD)
#define T_KWD_RETURN gen_token_code(00, 04, 00, G_KWD)
#define T_KWD_IF gen_token_code(00, 05, 00, G_KWD)
#define T_KWD_ELSE gen_token_code(00, 06, 00, G_KWD)
#define T_KWD_WHILE gen_token_code(00, 07, 00, G_KWD)

//***********************************************

#define T_ID gen_token_code(00, 01, 00, 00)
#define T_SEMI gen_token_code(';', 02, 00, 00)
#define T_COMMA gen_token_code(',', 03, 00, 00)
#define T_LP gen_token_code('(', 04, 00, 00)
#define T_RP gen_token_code(')', 05, 00, 00)
#define T_LB gen_token_code('[', 06, 00, 00)
#define T_RB gen_token_code(']', 07, 00, 00)
#define T_LC gen_token_code('{', 08, 00, 00)
#define T_RC gen_token_code('}', 09, 00, 00)

//***********************************************

// error handling types

#define T_UNKNOWN 0x11111111

//***********************************************

// helpers macros

const static char* G_TABLE[][4] = {{"", "", "", ""},
                                   {"DATA", "INT", "", ""},
                                   {"OP", "REL", "BOO", "CAL"},
                                   {"KWD", "TYPE", "", ""}};

#define get_g(x) (G_TABLE[(x >> 24) & 0xFF][0])

#define get_c(x) \
  (((x >> 16) & 0xFF) ? G_TABLE[(x >> 24) & 0xFF][(x >> 16) & 0xFF] : "")
